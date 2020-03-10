#pragma once

#include<stdio.h>
#include<string.h>
#include<winsock2.h>
#include<windows.h>
#include<sstream>

#include<iostream>
#include<conio.h>




#pragma comment(lib,"ws2_32.lib") 

#pragma warning (disable: 4996) //_CRT_SECURE_NO_WARNINGS
class pipeSock
{
	void SendData(std::string str, SOCKET sock);
	void SendData(const wchar_t* wbuffer, SOCKET sock);

public:
	void SysToServer(SOCKET sock);
};
