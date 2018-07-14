//Look into the README for a detailed description
//Created By: Bryce Golamco
//This is the client, you must first launch the server before launching this.
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
#include <math.h>


using namespace std;

int DH_start(long int n, long int q, long int private_n){
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
	struct sockaddr_in server;
	int count =0;
	char *server_host_name = new char[256];
	int server_port_number = 0;
	long long int private_n = 0;
	long long int public_n = 0;
	long long int public_q = 0;
	long long int public_x = 0;
	long long int key = 0;
	char buffer[256];
	long long int start[3];
	char * send_buf;


	cout << "Warning, please use 2-digit numbers or less as you may risk overflow." << endl;
	cout << "Please input the server name/ip address: ";
	cin >> server_host_name;
	cout << "Please input the private number: ";
	cin >> private_n;
	cout << "Please input public number n: ";
	cin >> public_n;
	cout << "Please input public number q: ";
	cin >> public_q;

// Values used for testing
//	server_host_name = "127.0.0.1";
//	private_n = 3;
//	public_n = 4;
//	public_q = 5;


	server_port_number = 5010;	

	memset(&server, '0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port_number);


	int sock = socket(AF_INET,SOCK_STREAM,0);
	inet_pton(AF_INET, server_host_name, &server.sin_addr);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
		cout << "Can't find connection";
		return 1;
	}


	//Diffie Hellman key exchange client-side
	start[0] = public_n;
	send(sock,(char *)start,3,0);
	start[0] =public_q;
	usleep(1000000);
	send(sock,(char*)start,3,0);
	usleep(1000000);
	start[0] = DH_start(public_n,public_q,private_n);
	send(sock,(char*)start,3,0);

	while(recv(sock,buffer,256,0) == -1);
	public_x = (int)buffer[0];
	
	key = DH_key(public_x,private_n, public_q);
	key = abs(key);

	cout << "Key Obtained..." <<endl;	
	cout << "The key was : " << key << endl;
	cout << "To quit, type in \"quit\" and press enter\n";

	//Ready to encrypt and send
	while(true){
		cout << "Output reply: \n";
		cin >> buffer;
		if(buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't'){
			break;		
		}
		send_buf = encrypt(key,(char *)buffer,256);
		send(sock,buffer,256,0);
	}

	cout << endl << "Done";
	close(sock);
	return 0;
}


