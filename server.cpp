//Look into the README for a detailed description
//Created By: Bryce Golamco
//This is the server, you must run this first before launching the client
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>


using namespace std;

int DH_start(int n, int q,int private_n){
	return ((int)pow(n,private_n) % q);
}

int DH_key(int x, int private_x, int q){
	return ((int)pow(x,private_x) % q);
}

char * encrypt(int key, char * plaintext,  int length){
	srand(key);
	int x = rand();
	for (int i =0; i < length; i++){
		plaintext[i] =  plaintext[i]^x;
		x = rand();
	}
	return plaintext;
}

int main(){
	long long int public_n = 0;
	long long int public_q = 0;
	long long int private_n = 0;
	long long int public_x = 0;
	char * recv_buf;
	long long int start[1];
	long long int key = 0;

	cout << "Warning, please use 2-digit numbers or less as you may risk overflow." << endl;
	cout << "Please input private number: ";
	cin >> private_n;


//	private_n = 3; //Value used for testing

	struct sockaddr_in server;
	int x = 0;
	int recv_return = -1;
	int serverport = 5010;
	char buffer[256]= {0};
	int listen_socket = 0;
	int serv_len = sizeof(server);
	memset(&server, '0', sizeof(server));


	int sock = socket(AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	server.sin_port = htons(serverport);
	server.sin_addr.s_addr = INADDR_ANY;

	bind(sock, (struct sockaddr *)&server, sizeof(server));
	listen(sock, 1);
	listen_socket = accept(sock, (struct sockaddr *)&server, (socklen_t *)&serv_len);
	
	//Diffie Hellman Key Exchange
	while(recv(listen_socket, buffer, 256,0) == -1);
	public_n = (int)buffer[0];
	while(recv(listen_socket, buffer, 256,0) == -1);
	public_q = (int)buffer[0];
	while(recv(listen_socket, buffer, 256,0) == -1);
	public_x = (int) buffer[0];

	key = DH_key(public_x, private_n, public_q);
	key = abs(key);

	start[0] = DH_start(public_n, public_q, private_n);
	send(listen_socket, (char *)start,1,0);
	cout << "Key Obtained..." << endl;
	cout << "The key was : " << key << endl;
	

	//Ready to decrypt
	while(true){
		while((recv_return = recv(listen_socket, buffer, 256,0)) == -1);
		if (recv_return == 0) break;	
		recv_buf = encrypt(key, (char *)buffer, 256);
		cout << buffer << endl;
	}
	close(sock);
	return 0;
}

