//TCP ������
#include"tcpServer.h"

#define MY_PORT 777 // server will listen that port

using namespace std;

int nclients = 0;  //amount of active users




int main(int argc, char* argv[])
{
	char buff[1024];
	printf("TCP server demo\n");
	// ��� 1  ������������� ���������� �������
	// �. �. ������������ �������� ���������� �� ������������
	// �� ���������� ��������� �� ������� �����, ������������� � ���������
	// �� ��������� WSADATA.
	// ����� ����� ��������� ���������� ���� ����������, ������, �����
	// ������ ���� �� ����� ������������ �������� (��������� WSADATA
	// �������� 400 ����)

	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		printf("Error WSAStartup %d\n", WSAGetLastError());
		return 1;
	}

	SOCKET mysocket; // ��� 2  �������� ������
					 // AF_INET  ����� ���������
					 // SOCK_STREAM  ��������� ����� (� ���������� ����������)
					 // 0  �� ��������� ���������� TCP ��������
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		printf("Error socket %d\n", WSAGetLastError());
		WSACleanup(); // �������������� ���������� Winsock
		return 1;
	}

	sockaddr_in local_addr; // ��� 3 ���������� ������ � ��������� �������
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT); // �� �������� � ������� �������!!!
	local_addr.sin_addr.s_addr = 0; // ������ ��������� ����������� �� ��� ���� IP ������ �������� bind ��� ����������
	if (bind(mysocket, (sockaddr *)&local_addr, sizeof(local_addr)))
	{
		printf("Error bind %d\n", WSAGetLastError());
		closesocket(mysocket); // ��������� �����!
		WSACleanup();
		return 1;
	}

	if (listen(mysocket, 0x100)) // ��� 4 �������� ����������� ������ ������� � 0x100
	{
		printf("Error listen %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return 1;
	}
	printf("Waiting for connection... \n");	// ��� 5 ��������� ��������� �� �������
	SOCKET client_socket; // ����� ��� �������
	sockaddr_in client_addr; // ����� ������� (����������� ��������)


	int client_addr_size = sizeof(client_addr);	// ���� ���������� �������� �� ����������� �� �������
	while ((client_socket = accept(mysocket, (sockaddr *)&client_addr, &client_addr_size)))  // ������� accept ���������� �������� ������ ���������
	{
		nclients++; // ����������� ������� �������������� �������� 
		HOSTENT *hst; // �������� �������� ��� �����

		hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET); //hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);


																			   // ����� �������� � �������
		cout << endl << endl << "-------------------------------------------------------------------------------" << endl;
		printf("+%s [%s]         ", (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr)); //printf("+%s [%s] new connect!\n", (hst) ? hst->h_name:"", inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS
			// ����� ������ ������ ��� ����������� �������
			// ��, ��� ����� ������������� ������������ _beginthreadex
			// ��, ��������� ������� ����� ������� ����������� �� ����������
			// ����� �� ������, ����� �������� � CreateThread
			DWORD thID;
		CreateThread(NULL, NULL, forClient, &client_socket, NULL, &thID);
	}
	return 0;
}