#include"tcpServer.h"

extern int nclients;

DWORD WINAPI forClient(LPVOID client_socket)  // ��� ������� ��������� � ��������� ������ � ���������� ���������� ��������������� ������� ���������� �� ���������
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[20 * 1024];
#define sHELLO "Hello\r\n"


	send(my_sock, sHELLO, sizeof(sHELLO), 0); // ���������� ������� �����������

	int bytes_recv;
	while ((bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR)
	{
		//send(my_sock, &buff[0], bytes_recv, 0);

		std::cout << std::endl << buff;
	}

	// ���� �� �����, �� ��������� ����� �� ����� �� ������� ���������� �������� recv ������ � ���������� � �������� ���������

	nclients--; // ��������� ������� �������� ��������
	printf("-disconnect\n"); PRINTNUSERS

		closesocket(my_sock);
	return 0;
}

