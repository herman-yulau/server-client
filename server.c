#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

 
int main(int argc , char *argv[])
{
    int read_size, socket_desc , new_socket , c, data_sent;
    struct sockaddr_in server , client;
    char *message, client_message[2000], buffer[1024];
     
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); // create socket
    if (socket_desc < 0) {
        printf("Could not create socket");
    }
     
    server.sin_family = AF_INET;			// Prepare the sockaddr_in structure
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(29008);
     
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {  // binding server to the socket
        puts("Binding failed");
        return 1;
    }
    puts("Bind done");
     
    listen(socket_desc , 3);				// listening the socket
     
    puts("Waiting for incoming connections...");
	
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepted");
		
        read_size = recv(new_socket , client_message , sizeof(client_message), 0); // receiving path
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
        	data_sent = send(new_socket, buffer, bytes_read, 0);		// sending packet
			if (data_sent >= 0)
				printf("Data is sent, size: %d\n", data_sent);
			else
				perror("Sending failed");
		}
		sleep(1);
	send(new_socket, "endfile", strlen("endfile"), 0);		// sending end of file
	puts("Sending finished");
	fclose(f);
    }
    
    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
