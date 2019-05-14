# include<stdio.h>
# include<stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include <unistd.h>

#define MAX_LIMIT 1024

int main(int argc, char *argv[])
{
	int sock, cli;
	struct sockaddr_in server, client;
	unsigned int len;
	char mesg[] = "Hello server";
	int sent;
	if((sock = socket(AF_INET, SOCK_STREAM, 0))== -1)
	{
		perror("Error in client socket");
		exit(-1);
	}
	
	if(argc <= 1){
	    printf("Please Enter the Port number to use\n");
	    return 0;
	}
	
	int PORT_NO = atoi(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NO);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);
	
	len =sizeof(struct sockaddr_in);
	
	 if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
	    { 
		printf("\nConnection Failed \n"); 
		return -1; 
	    } 
	    else{
	   	 printf("\nConnection build \n"); 
         char sendingMSG[MAX_LIMIT] = {0};
         printf("Enter Message to be Sent : ");
         scanf("%[^\n]%*c", sendingMSG);
		
		 /*Sending message from Client to Server*/
		 sent = send(sock, sendingMSG, 1024, 0);
		 printf("Message Sent from Client\n");  
		 
		 int valread = read(sock, sendingMSG, 1024);
		 printf("Message Echoed from Server : %s\n",sendingMSG);
		 
		}
    
	return 0;

}
