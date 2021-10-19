#ifndef UI_H
#define UI_H

#include <iostream>
#include <sstream>
#include <strstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include"global.h"
#include"system.h"

class userInterface
{
private:
	unsigned int userType = 0;
	bool ifLogin = false;
	string username_t;
	int token;

public:
	void show(const string& ip, const string& port);

	userInterface() = default;
	~userInterface() = default;
};


#endif
