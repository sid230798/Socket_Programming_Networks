/*

Name : Siddharth Nahar
Entry : 2016csb1043
Date : 2/2/19
Purpose : 

    1. Contains Client side code.
    2. Get string host from command , send it to Server and recieve its IP.
    3. Terminate by Bye message.

*/

/*Include all networks and useful libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

/*Driver code for Socket Creation and sending and reversing message*/
int main(int argc, char* argv[]){

        /*Check if port number is passed or not*/
        if(argc <= 1)
                printf("Please pass the port number to use\n"); 
        else{
        
                /*Convert string to int*/
                int PORT_NO = atoi(argv[1]);
                
                /*Both represent server and client file descripters respectively*/
                int sock_fd;
                
                /*Server struct for address,port*/
                struct sockaddr_in server;
                
                /*Create socket_fd and then use it to connect to server*/
                if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
                
                        printf("Creating Socket Failed......Please Try Again\n");
                        return 0;
                        
                }
                
                /*Assign 0 to server structure and assign some quantites*/
                bzero(&server, sizeof(server));
                server.sin_family = AF_INET;
	            server.sin_port = htons(PORT_NO);
	            server.sin_addr.s_addr = INADDR_ANY;
	            
	            /*Continue to send and recieve strings from commandline*/
	            while(1){
	            
	                    /*Created two buffers to send and recieve data from client*/
                        char recvBuffer[1024] = {0};
                        char sendBuffer[1024] = {0};
                        
                        ssize_t flag;
                        unsigned int length = sizeof(struct sockaddr_in);
                        
                        printf("Enter domain name to sent to Server or Enter Bye to close connection : ");
                        scanf("%[^\n]%*c", sendBuffer);
                        
                        /*Terminating Client Condition String passed is "BYE"*/
                        if(strcmp(sendBuffer, "Bye") == 0){
                        
                                printf("Closing Client......\n");
                                break;
                        }
                        
                        printf("\nThe Client sends the string \"%s\"\n",sendBuffer);
                        /*Sending data to client which sent the data*/
                        flag = sendto(sock_fd, (const char *)sendBuffer, strlen(sendBuffer), MSG_CONFIRM, (struct sockaddr *)&server, length);
	            
	                    /*Recieve the data back from Server*/
                        flag = recvfrom(sock_fd, recvBuffer, 1024, MSG_WAITALL, (struct sockaddr *)&server, &length);
                        
                        printf("The IP Address recieved from Server \"%s\"\n", recvBuffer); 	                    
	            }
	            
	            close(sock_fd);
        }
        
        return 0;
        
}
