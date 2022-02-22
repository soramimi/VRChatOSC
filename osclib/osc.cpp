
#include "osc.h"
#include <cstring>
#include <thread>

#ifdef _WIN32
#include <WinSock2.h>

#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define closesocket ::close
#define ioctlsocket ::ioctl
#define IN_ADDR in_addr
#endif

namespace {

bool get_host_by_name(const char *name, IN_ADDR *out)
{
	struct hostent *he = nullptr;
#if defined(_WIN32) || defined(__APPLE__)
	he = ::gethostbyname(name);
#else
	int err = 0;
	char buf[2048];
	struct hostent tmp;
	gethostbyname_r(name, &tmp, buf, sizeof(buf), &he, &err);
#endif
	if (he) {
		memcpy(&out->s_addr, he->h_addr, sizeof(out->s_addr));
		return true;
	}
	return false;
}

} // namespace

//// Transmitter ////

struct osc::Transmitter::Private {
	struct sockaddr_in addr;
	int sock;
};

osc::Transmitter::Transmitter()
	: m(new Private)
{
}

osc::Transmitter::~Transmitter()
{
	close();
	delete m;
}

void osc::Transmitter::open(char const *hostname)
{
	close();

	m->sock = socket(AF_INET, SOCK_DGRAM, 0);

	m->addr.sin_family = AF_INET;
	m->addr.sin_port = htons(9000);
	get_host_by_name(hostname, &m->addr.sin_addr);
}

void osc::Transmitter::close()
{
	if (m->sock != -1) {
		closesocket(m->sock);
		m->sock = -1;
	}
}

void osc::Transmitter::send_bool(const std::string &addr, bool val)
{
	size_t n;

	char tmp[100];

	size_t i = addr.size();
	memcpy(tmp, addr.c_str(), i);
	tmp[i++] = 0;
	for (int j = 0; j < 3 && (i & 3); j++) tmp[i++] = 0;

	tmp[i++] = ',';
	tmp[i++] = val ? 'T' : 'F';
	tmp[i++] = 0;
	tmp[i++] = 0;

	sendto(m->sock, tmp, i, 0, (struct sockaddr *)&m->addr, sizeof(m->addr));
}

void osc::Transmitter::send_int(const std::string &addr, int32_t val)
{
	size_t n;

	char tmp[100];

	size_t i = addr.size();
	memcpy(tmp, addr.c_str(), i);
	tmp[i++] = 0;
	for (int j = 0; j < 3 && (i & 3); j++) tmp[i++] = 0;

	tmp[i++] = ',';
	tmp[i++] = 'i';
	tmp[i++] = 0;
	tmp[i++] = 0;

	tmp[i++] = uint8_t(val >> 24);
	tmp[i++] = uint8_t(val >> 16);
	tmp[i++] = uint8_t(val >> 8);
	tmp[i++] = uint8_t(val);

	sendto(m->sock, tmp, i, 0, (struct sockaddr *)&m->addr, sizeof(m->addr));
}

void osc::Transmitter::send_float(const std::string &addr, float val)
{
	size_t n;

	char tmp[100];

	size_t i = addr.size();
	memcpy(tmp, addr.c_str(), i);
	tmp[i++] = 0;
	for (int j = 0; j < 3 && (i & 3); j++) tmp[i++] = 0;

	tmp[i++] = ',';
	tmp[i++] = 'f';
	tmp[i++] = 0;
	tmp[i++] = 0;

	uint8_t const *s = (uint8_t const *)&val;
	uint8_t *d = (uint8_t *)tmp + i;
	for (int i = 0; i < 4; i++) {
		d[i] = s[3 - i];
	}

	sendto(m->sock, tmp, i, 0, (struct sockaddr *)&m->addr, sizeof(m->addr));
}

//// Receiver ////

struct osc::Receiver::Private {
	struct sockaddr_in addr;
	int sock = -1;
	std::thread thread;
	bool interrupted = false;

	osc::Listener *listener = nullptr;
};

osc::Receiver::Receiver()
	: m(new Private)
{
}

osc::Receiver::~Receiver()
{
	stop();
	close();
	delete m;
}

osc::Listener *osc::Receiver::set_listener(Listener *listener)
{
	auto old = m->listener;
	m->listener = listener;
	return old;
}

bool osc::Receiver::isInterruptionRequested() const
{
	return m->interrupted;
}

void osc::Receiver::run()
{
	while (1) {
		char buffer[2048];
		if (isInterruptionRequested()) break;
		int n = recv(m->sock, buffer, sizeof(buffer), 0);
		if (n < 1) {
			if (errno == EAGAIN) {
				// not yet
			} else {
				if (isInterruptionRequested()) break;
			}
		} else {
			if (m->listener && m->listener->received) {
				m->listener->received(buffer, n);
			}

			Value value;

			int pos = 0;
			int end = sizeof(buffer);
			while (pos < end && buffer[pos]) pos++;
			std::string addr((char const *)buffer, pos);
			pos = pos + 4;
			pos -= pos & 3;
			if (pos + 2 < end && buffer[pos] == ',') {
				pos++;
				char c = buffer[pos];
				pos+= 3;
				switch (c) {
				case 'i':
					value.type_ = Value::Type::Int;
					if (pos + sizeof(int32_t) <= end) {
						uint8_t const *s = (uint8_t *)buffer + pos;
						uint8_t *d = (uint8_t *)&value.v.i;
						for (int i = 0; i < sizeof(int32_t); i++) {
							d[i] = s[sizeof(int32_t) - 1 - i];
						}
					}
					break;
				case 'f':
					value.type_ = Value::Type::Float;
					if (pos + sizeof(float) <= end) {
						uint8_t const *s = (uint8_t *)buffer + pos;
						uint8_t *d = (uint8_t *)&value.v.f;
						for (int i = 0; i < sizeof(float); i++) {
							d[i] = s[sizeof(float) - 1 - i];
						}
					}
					break;
				case 'T':
					value.type_ = Value::Type::Bool;
					value.v.b = true;
					break;
				case 'F':
					value.type_ = Value::Type::Bool;
					value.v.b = false;
					break;
				}

				if (m->listener && m->listener->value) {
					m->listener->value(addr, value);
				}
			}
		}
	}
}

void osc::Receiver::open(char const *hostname)
{
	close();

	m->sock = socket(AF_INET, SOCK_DGRAM, 0);

	m->addr.sin_family = AF_INET;
	m->addr.sin_port = htons(9001);
	get_host_by_name(hostname, &m->addr.sin_addr);

	start();
}

void osc::Receiver::close()
{
	if (m->sock != -1) {
		closesocket(m->sock);
		m->sock = -1;
	}
}

void osc::Receiver::start()
{
	bind(m->sock, (struct sockaddr *)&m->addr, sizeof(m->addr));
	u_long val = 1;
	ioctlsocket(m->sock, FIONBIO, &val);

	m->thread = std::thread([&](){
		run();
	});
}

void osc::Receiver::stop()
{
	m->interrupted = true;
	if (m->thread.joinable()) {
		m->thread.join();
	}
	m->thread = {};
}

//
