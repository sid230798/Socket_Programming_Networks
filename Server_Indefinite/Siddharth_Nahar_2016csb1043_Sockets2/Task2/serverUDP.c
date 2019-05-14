/*

Name : Siddharth Nahar
Entry : 2016csb1043
Date : 2/2/19
Purpose : 

    1. Contains Server side code.
    2. Get string host from client ,  return the IP address and send it back to client.
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


#define SIZE 6
/*LookUP Table*/
char *domainName[] = {"google.com", "youtube.com", "iitrpr.ac.in", "bing.com", "netflix.com", "github.com"};
char *IP[] = {"10.1.217.43", "110.92.21.80", "5.5.5.5", "6.6.6.6", "72.72.12.13", "8.8.8.8"};

/*Print the lookup table for reference*/
void printTable(){

    int i;
    
    printf("\nLOOKUP TABLE IS AS FOLLOWS ------------------------------>\n");
    for(i = 1;i <= SIZE;i++){
    
        printf("%d. %s -> %s\n",i,domainName[i-1], IP[i-1]);
    
    }

}

/*Check of string is present in lookup*/
void tableLookup(char* host, char* ipAddress){

        int i;
        
        /*Loop thorugh the table*/
        for(i = 0;i < SIZE;i++){
        
                /*Check if host name is correct*/
                if(strcmp(host, domainName[i]) == 0){
                
                        strcpy(ipAddress, IP[i]);
                        return;
                }
        }
        
        /*That domain name doesnt exist*/
        strcpy(ipAddress, "Domain does not exist.");
}

/*

    Main Difference between TCP and UDP :-
    
        1. UDP is connectionless protocol.
        2. We don't have to listen and accept for server.
        3. No feedback from client side.

*/

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
                struct sockaddr_in server, client;
                
                /*
                
                 Create Socket with socket function
                 
                 1. AF_INET for IPv4,IPv6 address.
                 2. SOCK_DGRAM for UDP type client
                 3. 0 represents generic value for protocol
                 
                 Return value -1 for failure
                
                */
                if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
                
                        printf("Creating Socket Failed......Please Try Again\n");
                        return 0;
                        
                }
                
                printf("Socket Created Successfully .....\n");
                
                /*Assign 0 to server structure and assign some quantites*/
                bzero(&server, sizeof(server));
                server.sin_family = AF_INET;
                server.sin_port = htons(PORT_NO);
                server.sin_addr.s_addr = INADDR_ANY;
                
                /*
                 
                 Bind server to address of struct server
                 1. sock_fd Socket descriptor
                 2. server address to which socket is to binded.
                 3. Length of address structure pointed    
                
                */
                if(bind(sock_fd, (struct sockaddr *)&server, (socklen_t)sizeof(struct sockaddr_in)) ==  -1){
                
                        printf("Binding of Server to address failed.....\n");
                        return 0;
                        
                }
                
                printf("Socket Successfully Binded .....\n");
                
                /*Print the Look Up Table*/
                printTable();
                /*Keep on accepting Datagrams and sending Reply*/
                while(1){
        
                        /*Created two buffers to send and recieve data from client*/
                        char recvBuffer[1024] = {0};
                        char sendBuffer[1024] = {0};
                        
                        ssize_t flag = 0;
                        /*Recvfrom uses MSG_WAITALL wait till it recieves datagram*/
                        unsigned int length = sizeof(struct sockaddr_in);
                        flag = recvfrom(sock_fd, recvBuffer, 1024, MSG_WAITALL, (struct sockaddr *)&client, &length);
                        
                        printf("\nRecieved from Client : %s\n",recvBuffer);
                        
                        if(strcmp(recvBuffer,"Bye") == 0){
                            
                                printf("Client getting Closed......\n");
                                continue;
                        }
                        
                        tableLookup(recvBuffer, sendBuffer);
                        printf("IP address of asked Domain : %s\n", sendBuffer);
                        
                        /*Sending data to client which sent the data*/
                        flag = sendto(sock_fd, (const char *)sendBuffer, strlen(sendBuffer), MSG_CONFIRM, (struct sockaddr *)&client, length);
                               
                }
        
        }

        return 0;
    
}
