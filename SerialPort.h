#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>

using namespace std;

class SerialPort {

private:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	string syntax_name_;
	char front_delimiter_;
	char end_delimiter_;

public:
	void CustomSyntax(string syntax_type);

	SerialPort(char *com_port, DWORD COM_BAUD_RATE);

	string ReadSerialPort(int reply_wait_time, string syntax_type);

	bool WriteSerialPort(char* data_sent);

	bool CloseSerialPort();

	~SerialPort();

	bool connected_;
};