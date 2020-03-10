//TCP client, namedPipe client 
#include"tcpClient.h"

#define PORT 777

//#define SERVERADDR "192.168.0.100"
#define SERVERADDR "127.0.0.1"



using namespace std;



int main(int argc, char* argv[])
{
	pipeSock firstConnect;
	char buff[1024];
	printf("TCP demo client\n");

	if (WSAStartup(0x202, (WSADATA *)&buff[0]))// Шаг 1  инициализация библиотеки Winsock
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		_getch();
		return 1;
	}

	SOCKET my_sock;// Шаг 2  создание сокета
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock<0)
	{
		printf("socket() error %d\n", WSAGetLastError());
		_getch();
		return 1;
	}
	// Шаг 3  установка соединения
	// заполнение структуры sockaddr_in – указание адреса и порта сервера
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;
	// преобразование IP адреса из символьного в сетевой формат
	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);


	else
	{
		printf("Invalid address %s\n", SERVERADDR);
		closesocket(my_sock);
		WSACleanup();
		_getch();
		return 1;
	}

	// адрес сервера получен – пытаемся установить соединение
	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("connect() error %d\n", WSAGetLastError());
		_getch();
		return 1;
	}
	printf("Connection with %s was succesfully established \n", SERVERADDR);
	// Шаг 4  чтение и передача сообщений
	int nsize;
	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{

		buff[nsize] = 0;// ставим завершающий ноль в конце строки
		cout << buff;
		firstConnect.SysToServer(my_sock);//SendSystemInfo(my_sock);
		Sleep(5000);
	}
	printf("recv() error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	_getch();
	return 1;
}

//*/