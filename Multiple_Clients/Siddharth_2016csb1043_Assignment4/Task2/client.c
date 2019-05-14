/*

Name : Siddharth Nahar
Entry : 2016csb1043
Date : 2/2/19
Purpose : 

    1. Contains Client side code.
    2. Get string from command line , send the string and recieve it back in reverse format from server.
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
                if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                
                        printf("Creating Socket Failed......Please Try Again\n");
                        return 0;
                        
                }
                
                
                /*Assign 0 to server structure and assign some quantites*/
                bzero(&server, sizeof(server));
                server.sin_family = AF_INET;
	            server.sin_port = htons(PORT_NO);
	            server.sin_addr.s_addr = INADDR_ANY;
	            
                /*

                Connect to Server specified by addr.

                1. Get fie descriptor in sock_fd
                2. Connect client to server specified by address in sockaddr_in

                */
                if(connect(sock_fd, (struct sockaddr *)&server, (socklen_t)sizeof(struct sockaddr_in)) == -1){

                        printf("Failed to Connect to Server.........\n");
                        return 0;
                }
                
                printf("Connection Successfull with Server..........\n");
                
                /*Created two buffers to send and recieve data from client*/
                char recvBuffer[1024] = {0};
                char sendBuffer[1024] = {0};
                
                ssize_t flag;
      
                /*Continue Client to run till Bye String is Entered*/          
                while(1){
                
                        //printf("\n-------------------------------------------------------------------------------------------\n");
                        printf("\nEnter File name to be sent to Server or Enter Bye to close connection : ");
                        scanf("%[^\n]%*c", sendBuffer);
                        
                        /*Terminating Client Condition String passed is "BYE"*/
                        if(strcmp(sendBuffer, "Bye") == 0){
                        
                                printf("Closing Client......\n");
                                break;
                        }
                        
                        //printf("\nThe Client sends File name \"%s\"\n",sendBuffer);
                        /*Sending message from Client to Server*/
		                flag = send(sock_fd, sendBuffer, 1024, 0);
                
                        flag = recv(sock_fd, recvBuffer, 1024, 0);
                        
                        printf("Content Recieved From Server : \n");
                        printf("\n%s", recvBuffer);
                        
                        /*Reset Strings to 0*/
                        memset(sendBuffer, 0, 1024);
                        memset(recvBuffer, 0, 1024);
                                
                }
                
                close(sock_fd);
        
        }
        
        
        return 0;
        
}
