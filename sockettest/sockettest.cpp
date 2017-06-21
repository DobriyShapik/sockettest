// sockettest.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"



//  при входе в приложение будет подключение к БД которое будет хранить айпи сервера-клиента, который онлайн и его индификатор, 
// через єту БД приложение клиента и будут узнавать адресса получателя  
// будет происходит коннект детерменировано 
// если клиент, который отправил смс выщел а получатель не зашел в сеть - то смс ходит междду клиентами

// тут будет функция сервера для 2 - потока который будет сервером и будет общаться с другими клиентами
bool socktestServ()
{
	cout << "СЕРВЕР\n";
	SOCKET s;
	SOCKADDR_IN servAdr;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_port = htons(4000);
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	//INADDR_ANY;
	//    inet_addr("65.55.21.250");
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		cerr << "ERROR: socket unsuccessful" << endl;
		system("pause");
		return 0;
	}
	if (bind(s, (SOCKADDR*)&servAdr, sizeof(servAdr)) == 0)
	{
		cerr << "ERROR: bind unsuccessful\n";
		system("pause");
		return 0;
	}
	listen(s, 10); // 10 - кол-во клиентов что могут подсоеденится 
				   //получаем в буфер индификатор
	SOCKADDR_IN from_sin;
	int s_new, from_len;
	char massage[1024];
	int bytes_read;
	while (1)
	{
		from_len = sizeof(from_sin);
		s_new = accept(s, (SOCKADDR*)&from_sin, &from_len);
		if (s_new < 0)
			cerr << "ERROR. Acept with client field\n";
		if (inet_ntoa(from_sin.sin_addr) == inet_ntoa(servAdr.sin_addr))
			cout << "Мы та машина которая получает смс\n"; // дальнейшие действия с смс
		while (1)
		{
			bytes_read = recv(s_new, massage, sizeof(massage), 0);
			if (bytes_read <= 0) break;
			cout << "SMS of client: " << massage;
			send(s, massage, bytes_read, 0);
		}
		shutdown(s_new, 0);
		closesocket(s);
	}
	return 0;
}


bool client(char* message) {  // SOCKADDR_IN reciever - аргумент кому передовать
	cout << "CLIENT\n";
	int s;
	int from_len;
	//  struct hostent *hp;
	struct sockaddr_in clnt_sin, srv_sin;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		cerr << "ERROR: socket unsuccessful" << endl;
		system("pause");
		return 0;
	}
	memset((char *)&clnt_sin, '\0', sizeof(clnt_sin));
	clnt_sin.sin_family = AF_INET;
	clnt_sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	// INADDR_ANY;
	clnt_sin.sin_port = htons(0);
	bind(s, (sockaddr *)&clnt_sin, sizeof(clnt_sin));
	memset((char *)&srv_sin, '\0', sizeof(srv_sin));
	//hp = gethostbyname(SRV_HOST);
	srv_sin.sin_family = AF_INET;
	srv_sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	// memcpy((char*)&srv_sin.sin_addr, hp->h_addr, hp->h_length);
	srv_sin.sin_port = htons(4000);

	if (connect(s, (SOCKADDR*)&srv_sin, sizeof(srv_sin)) < 0)
	{
		perror("connect");
		exit(2);
	}

	//from_len =	send(s, message.c_str, sizeof(message), 0);
	from_len = send(s, message, sizeof(message), 0);
	cout << "SMS send. Byte to send = " << from_len << '\n';
	message = {};
	recv(s, message, sizeof(message), 0);
	cout << "Сообщение принято от сервера\n" << message << "\n";

	closesocket(s);
	exit(0);

}

int main()
{
    return 0;
}

