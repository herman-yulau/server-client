#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <signal.h>

 
int main(int argc , char *argv[])
{
    int read_size, socket_serv, client_socket, c, data_sent;
    struct sockaddr_in server , client;
    char *message, client_message[2000], buffer[1024];
     
    socket_serv = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (socket_serv < 0) {
        printf("Could not create socket");
    }
     
    server.sin_family = AF_INET;			// Prepare the sockaddr_in structure
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(29008);
     
    if (bind(socket_serv, (struct sockaddr *)&server, sizeof(server)) < 0) {  // binding server to the socket
        puts("Binding failed");
        return 1;
    }
    puts("Bind done");
     
    listen(socket_serv , 3);				// listening the socket
	signal(SIGCHLD, SIG_IGN);
     
    puts("Waiting for incoming connections...");
	
    c = sizeof(struct sockaddr_in);
    while ((client_socket = accept(socket_serv, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepted");
		if (fork() == 0) {
        	read_size = recv(client_socket , client_message , sizeof(client_message), 0); // receiving path
			if(read_size < 0) {
				perror("Reading failed");
				continue;
			}
			printf("Path is received: %s\n", client_message);
		
			int bytes_read;
			FILE *f = fopen(client_message, "rb");
			if(f == 0) { 
				puts("File wasn't opened");
				continue;
			}
			while (!feof(f)) {									// reading the file
        		bytes_read = fread(buffer, 1, sizeof(buffer), f);
				int size = ftell(f);
				printf("Bytes read: %d, size: %d \n", bytes_read, size);
        		data_sent = send(client_socket, buffer, bytes_read, 0);		// sending packet
				if (data_sent >= 0)
					printf("Data is sent, size: %d\n\n", data_sent);
				else
					perror("Sending failed");
			}
			sleep(1);
		send(client_socket, "endfile", strlen("endfile"), 0);		// sending end of file
		puts("Sending finished");
		fclose(f);
    	}
		close(client_socket);
	}
	
    if (client_socket < 0) {
        perror("accept failed");
        return 1;
    }
	
	close(socket_serv);   
    return 0;
}
