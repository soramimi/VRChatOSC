
#include "../osclib/osc.h"
#include "../osclib/sock.h"
#include <cstring>
#include <signal.h>
#include <thread>

bool interrupted = false;

void onSIGINT(int)
{
	interrupted = true;
}

void msleep(int n)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

void dump(char const *ptr, int len)
{
	int offset = 0;
	while (offset < len) {
		char line[100];
		memset(line, ' ', sizeof(line));
		sprintf(line, "%04X ", offset);
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				sprintf(line + 5 + 3 * i, "%02X ", c);
			} else {
				strcpy(line + 5 + 3 * i, "   ");
			}
		}
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				if (c < 0x20 || c > 0x7f) {
					c = '.';
				}
				line[5 + 3 * 16 + i] = c;
			} else {
				line[5 + 3 * 16 + i] = ' ';
			}
		}
		line[5 + 3 * 16 + 16] = 0;
		offset += 16;

		fprintf(stderr, "%s\n", line);
	}
}

int main()
{
	sock::startup();

	signal(SIGINT, onSIGINT);

	osc::Listener listener;

	listener.received = [](char const *ptr, int len){
		dump(ptr, len);
	};

	listener.value = [](std::string const &addr, osc::Value const &value){
		switch (value.type()) {
		case osc::Value::Type::Void:
			fprintf(stderr, "void\n");
			break;
		case osc::Value::Type::Bool:
			fprintf(stderr, "%s bool %d\n", addr.c_str(), (int)value.bool_value());
			break;
		case osc::Value::Type::Int:
			fprintf(stderr, "%s int %d\n", addr.c_str(), (int)value.int_value());
			break;
		case osc::Value::Type::Float:
			fprintf(stderr, "%s float %f\n", addr.c_str(), value.float_value());
			break;
		}
	};

	osc::Receiver rx;
	rx.set_listener(&listener);
	rx.open("127.0.0.1");

	while (1) {
		if (interrupted) break;
		msleep(10);
	}

	rx.close();

	sock::cleanup();
	return 0;
}
