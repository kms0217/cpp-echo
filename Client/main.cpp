#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include<winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << '\n';
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;				//소켓 생성
	SOCKADDR_IN serverAddress;		

	int serverPort = 9999;
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//Winsock초기화
		ShowErrorMessage("WSAStartup()");
		
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);		//TCP소켓 생성

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");//문자열 ip를 네트워크 바이트 형식으로 변경 이 pc에 서버클라 다 동작시킬예정이므로 루프백 주소
	serverAddress.sin_port = htons(serverPort);//2바이트 정수 네트워크 바이트 형식으로
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connect()\n";

	while (1) {			//반복적으로 메시지 전송 및 전달 받기 
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "")continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}