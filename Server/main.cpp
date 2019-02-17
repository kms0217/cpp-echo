#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]:" << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;     
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9999;				//서버 포트 설정
	char received[256];				//문자열 버퍼 크기 256으로 설정

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)		//MAKEWORD 구조체를 사용해 2.2버전임을 명시, winsock 초기화
		ShowErrorMessage("WSAStartup()");				//초기화 실패시 1반환-> 에러표시

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);		//TCP(SOCK_STREAM) 소켓 생성
	
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));		
	serverAddress.sin_family = AF_INET;			//ipv4사용
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	//4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort);		//2바이트 정수 네트워크 바이트 형식으로
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)  //바인딩 처리
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)	//동시요청 5개까지 리스닝
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";
	
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {	//클라이언트의 메시지 그대로 다시 전달
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //마지막문저 NULL로 바꾸기
		cout << "[클라이언트 메시지]:" << received << '\n';
		cout << "[메시지 전송]:" << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0); //엔터를 쳐서 보내기때문에 마지막 \n은 무시하기위해 -1
			cout << "[서버 종료]\n";
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