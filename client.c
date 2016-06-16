#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
 
int main(int argc, char *argv[])
{
    int client_socket, data_sent_size, received_size;
	FILE *f;
    struct sockaddr_in server;
    char *path = argv[2], *id = argv[1], buffer[1024], *filename;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0); // Create socket
    if (client_socket < 0) {
        printf("Could not create socket");
    }
         
    server.sin_addr.s_addr = inet_addr(id);
    server.sin_family = AF_INET;
    server.sin_port = htons(29008);
 
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) { // Connect to remote server
    	puts("connect error");
        return 1;
    }
     
    puts("Connected");
   
	data_sent_size = send(client_socket, path, strlen(path), 0); // sending path to the server
    if (data_sent_size < 0) {
        puts("Sending failed");
        return 1;
    }
    printf("Path was sent, size: %d\n", data_sent_size);
	
	filename = strrchr(path, '/') + 1;							// retrieving file name from path
	f = fopen(filename, "wb");
	
	while (1) {
		received_size = recv(client_socket, buffer, sizeof(buffer), 0); // receiving data from the server
    	if (received_size < 0) {
        	puts("receiving failed");
			return 1;
		}
		if (received_size == 0)
			break;
		printf("Reply is received, size: %d\n", received_size);
		fwrite(buffer, 1, received_size, f);							// write in file
		memset(buffer, 0, strlen(buffer));								//buffer cleaning
	}
	
    fclose(f);
     
    return 0;
}
