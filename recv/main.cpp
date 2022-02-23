
#include "../misc/print.h"
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

int main()
{
	sock::startup();

	signal(SIGINT, onSIGINT);

	osc::Listener listener;

	listener.received = [](char const *ptr, int len){
		print_hex_dump(ptr, len);
	};

	listener.value = [](std::string const &addr, osc::Value const &value){
		print_osc_value(addr, value);
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
