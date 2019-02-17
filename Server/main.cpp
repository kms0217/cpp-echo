#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[���� �߻�]:" << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;     
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9999;				//���� ��Ʈ ����
	char received[256];				//���ڿ� ���� ũ�� 256���� ����

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//MAKEWORD ����ü�� ����� 2.2�������� ���, winsock �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");				//�ʱ�ȭ ���н� 1��ȯ-> ����ǥ��

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);		//TCP(SOCK_STREAM) ���� ����
	
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));		
	serverAddress.sin_family = AF_INET;			//ipv4���
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	//4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort);		//2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)  //���ε� ó��
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)	//���ÿ�û 5������ ������
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";
	
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {	//Ŭ���̾�Ʈ�� �޽��� �״�� �ٽ� ����
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //���������� NULL�� �ٲٱ�
		cout << "[Ŭ���̾�Ʈ �޽���]:" << received << '\n';
		cout << "[�޽��� ����]:" << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0); //���͸� �ļ� �����⶧���� ������ \n�� �����ϱ����� -1
			cout << "[���� ����]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}