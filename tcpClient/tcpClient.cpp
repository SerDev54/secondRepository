#include"tcpClient.h"

void pipeSock::SendData(std::string str, SOCKET sock)
{
	const char *ch = strdup(str.c_str());
	send(sock, ch, str.length(), 0);
}

void pipeSock::SendData(const wchar_t* wbuffer, SOCKET sock)
{

	size_t buffer_size;
	wcstombs_s(&buffer_size, NULL, 0, wbuffer, _TRUNCATE);                              // determine the required buffer size


	char *buffer = (char*)malloc(buffer_size);
	wcstombs_s(&buffer_size, buffer, buffer_size, wbuffer, _TRUNCATE);                  // do the actual conversion


	size_t buffer_sent = 0;
	while (buffer_sent < buffer_size)                                                   // send the data
	{
		int sent_size = send(sock, buffer + buffer_sent, buffer_size - buffer_sent, 0);
		buffer_sent += sent_size;
	}
	free(buffer);
}

void pipeSock::SysToServer(SOCKET sock)
{
	HANDLE hPipe = NULL;
	DWORD iCountIO = 0;
	DWORD dwMode = 0;
	BOOL isSuccess = FALSE;

	const int strSize = 2048;
	LPWSTR str = new WCHAR[strSize];
	memset(str, 0, (strSize - 1) * 2);

	std::wstring server = L".";  	                                //std::wcout << L"Print server name or .  for localHost: " << std::endl; //std::getline(std::wcin, server);

	std::wstring pipe = L"testPipe";	                            //std::wcout << L"Print name of pipe: " << std::endl;	//std::getline(std::wcin, pipe);

	std::wstring namedPipe = L"\\\\";
	namedPipe.append(server);
	namedPipe.append(L"\\pipe\\");
	namedPipe.append(pipe);
	std::wcout << L"namedPipe= " << namedPipe << std::endl;

	std::wstring message = L"Hello from tcpClient  ";             	//std::wcout << L"Print message to server: " << std::endl;	//std::getline(std::wcin, message);


	hPipe = CreateFileW(
		namedPipe.c_str(),
		GENERIC_ALL,
		0, NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		std::wcout << L"Unable to open pipe" << std::endl;
	}

	WaitNamedPipeW(namedPipe.c_str(), INFINITE);

	dwMode = PIPE_READMODE_MESSAGE;

	isSuccess = SetNamedPipeHandleState(
		hPipe,
		&dwMode,
		NULL,
		NULL);

	if (!isSuccess)
	{
		std::wcout << L"Unable to edit mode of pipe" << std::endl;
		EXIT_FAILURE;
	}

	std::wcout << L"Sending message to server" << std::endl;

	isSuccess = WriteFile(
		hPipe,
		message.c_str(),
		(message.size() + 1) * sizeof(WCHAR),
		&iCountIO,
		NULL);

	if (!isSuccess)
	{
		std::wcout << L"Unable to send message" << std::endl;
		EXIT_FAILURE;
	}
	else
		std::wcout << L"message pushed to server" << std::endl;

	//Soooooooooooooooooooooooooooooooooooooooooooooooccccccccccckkkkkkkkk

	int msgCount = 0;
	do
	{
		isSuccess = ReadFile(
			hPipe,
			str,
			(strSize + 1) * sizeof(WCHAR),
			&iCountIO,
			NULL);
		if (isSuccess && iCountIO > 0)
		{

			std::wcout << L"message from systemPart: " << std::endl << str << std::endl;
			SendData(str, sock);

			msgCount++;
		}
	} while (msgCount < 2);//while (!isSuccess);

	CloseHandle(hPipe);
	delete[] str;
}