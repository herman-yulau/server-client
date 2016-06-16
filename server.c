#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <signal.h>

int client_handler(int *client_socket);
 
int main(int argc , char *argv[])
{
    int read_size, serv_socket, client_socket, c, data_sent;
    struct sockaddr_in server , client;
    
     
    serv_socket = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (serv_socket < 0) {
        printf("Could not create socket");
    }
     
    server.sin_family = AF_INET;			// Prepare the sockaddr_in structure
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(29008);
     
    if (bind(serv_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {  // binding server to the socket
        puts("Binding failed");
        return 1;
    }
    puts("Bind done");
     
    listen(serv_socket , 3);				// listening the socket
	signal(SIGCHLD, SIG_IGN);
     
    puts("Waiting for incoming connections...");
	
    c = sizeof(struct sockaddr_in);
    while ((client_socket = accept(serv_socket, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepted");
		if (fork() == 0) {
        	client_handler(&client_socket);	
		}
		else
			close(client_socket);
	}
	
    if (client_socket < 0) {
        perror("accept failed");
        return 1;
    }
	
	close(serv_socket);   
    return 0;
}

int client_handler(int *client_socket)
{
	char buffer[1024];
	int data_sent, bytes_read, size, read_size;
	
	read_size = recv(*client_socket , buffer , sizeof(buffer), 0); // receiving path
	buffer[read_size] = '\0';
	if(read_size < 0) {
		perror("Reading failed");
		return 1;
	}
	printf("Path is received: %s\n", buffer);
		
	FILE *f = fopen(buffer, "rb");
	if(f == 0) { 
		puts("File wasn't opened");
		return 1;
	}
	while (!feof(f)) {									// reading the file
        bytes_read = fread(buffer, 1, sizeof(buffer), f);
		size = ftell(f);
		printf("Bytes read: %d, size: %d \n", bytes_read, size);
        data_sent = send(*client_socket, buffer, bytes_read, 0);		// sending packet
		if (data_sent >= 0)
			printf("Data is sent, size: %d\n\n", data_sent);
		else
			perror("Sending failed");
	}
	fclose(f);
	close(*client_socket);
	puts("Sending finished");
	
	return 0;
}
