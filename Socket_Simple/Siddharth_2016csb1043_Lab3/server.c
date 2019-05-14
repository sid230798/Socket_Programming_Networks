# include<stdio.h>
# include<stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int sock, cli;
	struct sockaddr_in server, client;
	unsigned int len;
	char mesg[] = "Hello client";
	int sent;
	
	if(argc <= 1){
	    printf("Please Enter the Port number to use\n");
	    return 0;
	}
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0))== -1)
	{
		perror("Error in socket");
		exit(-1);
	}
	
	int PORT_NO = atoi(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NO);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);
	
	len =sizeof(struct sockaddr_in);
	
	if(bind(sock, (struct sockaddr *)&server, len) == -1)
	{
		perror("Error in bind");
		exit(-1);
	}
	
	if((listen(sock, 3) == -1))
	{
		perror("Error in listen");
		exit(-1);
	}
	
	while(1){				// Blocking call
		if((cli =accept(sock, (struct sockaddr *)&client, &len)) == -1)
		{
			perror("Error in accept");
			exit(-1);
		}	
		
		printf("Connection Build.....\n");
        char buffer[1024] = {0};
	    int valread;
		valread = read(cli, buffer, 1024);
		//printf("%d\n",valread);
		printf("Message Recieved from Client : %s\n",buffer);
		int sent = send(cli, buffer, 1024, 0);
		printf("Message Sent from Server : %s\n",buffer);
		close(cli);
	}
	return 0;

}
