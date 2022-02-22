#include "sock.h"

#ifdef _WIN32
#include <WinSock2.h>
void sock::startup()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
}
void sock::cleanup()
{
	WSACleanup();
}
#else
void sock::startup()
{
}
void sock::cleanup()
{
}
#endif
