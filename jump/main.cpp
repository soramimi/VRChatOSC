
#include "../osclib/osc.h"
#include "../osclib/sock.h"
#include <thread>

void msleep(int n)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

int main()
{
	sock::startup();

	osc::Transmitter tx;
	tx.open("127.0.0.1");

	tx.send_int("/input/Jump", 1);
	msleep(30);
	tx.send_int("/input/Jump", 0);

	tx.close();

	sock::cleanup();
	return 0;
}
