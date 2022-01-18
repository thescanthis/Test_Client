#include "Client.h"

//ZeroMemory sizeof name 만큼 0으로 값을 초기화하겠다 라는 매크로임.
//send(): 클라이언트가 보낸 데이터가 서버쪽으로 잘 전달됐는지 확인하는용도이다.
// recv 보낼 소켓의주소,  그 값,크기, (여러옵션을 설정하는 것인데) 아무것도없다면 0으로 설정.
void recvData(SOCKET &s)
{
	char buffer[PACKET_PSIZE];
	while (TRUE){

		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0);
		std::cout <<"Server : "<< buffer << std::endl;
	}

}

int main()
{
	char IP[PACKET_PSIZE] = { 0 },
		name[PACKET_PSIZE] = { 0 };
	std::cout << "이름을 입력하세요 : " << std::endl;
	std::cin >> name;
	std::cout << "아이피를 입력하세요 : ";
	std::cin >> IP;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	while (connect(server, (SOCKADDR*)&addr, sizeof addr));
	send(server, name, sizeof name, 0); // 이름 보내기

	std::thread(recvData, std::ref(server)).detach();

	while (TRUE)
	{
		ZeroMemory(name, sizeof name);
		std::cout << "메세지를 입력하세요";
		std::cin >> name;
		send(server, name, sizeof name, 0);
	}
}
