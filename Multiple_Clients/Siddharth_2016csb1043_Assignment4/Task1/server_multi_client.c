/*

Name :- Siddharth Nahar
Entry No :- 2016csb1043
Date :- 12/2/19
Purpose :-

        1. Contains Server side code for handling multiple clients.
        2. Use select() function for same instead of multithreading.
        3. Function is more efficient than multiThreading beacuse it works on signaling systems

*/

/* Include important libraries */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>  
#include <sys/types.h>
#include <sys/unistd.h>
#include <errno.h>

/*

    1. For select function use this lib.
    2. According to POSIX.1-2001, POSIX.1-2008
    3. select, FD_SET,FD_ZERO and other macros

*/
#include <sys/select.h>

/*Define number of maximum clients to handle*/
#define MAX_CLIENTS 10
#define TRUE 1

/*Function returns the max_fd found to use in select function*/
int getMaxFD(int sock_fd, int clients_fd[], fd_set *read_fds){

    int max_fd = sock_fd;
    
    /*Iterate through each client fd see if any one is greater*/
    for(int i = 0;i < MAX_CLIENTS;i++){
    
        int sd = clients_fd[i];
        
        /*If client is active add it to FD_SET*/
        if(sd > 0)
            FD_SET(sd, read_fds);
            
        if(sd > max_fd)
            max_fd = sd;
    
    }
    
    return max_fd;


}

/*This function creates connection and send message to client about connection*/
void createConnection(int sock_fd, struct sockaddr_in *client,int clients_fd[]){

    int client_new_fd;
    socklen_t length = sizeof(struct sockaddr_in);
    
    /*Check if client can connect to server*/
    if((client_new_fd = accept(sock_fd, (struct sockaddr *)client, &length)) == -1){
                        
            printf("Failed to Connect to Client.........\n");
            return;
    }
    
    /*Update the clients_fd by new client*/
    for(int i = 0;i < MAX_CLIENTS;i++){
    
        if(clients_fd[i] == 0){
        
            clients_fd[i] = client_new_fd;
            
            break;
        }
    
    }
    
    /*Print connected message*/
    printf("Successfully Connected with Client %d, ip is %s\n",client_new_fd, inet_ntoa((*client).sin_addr));
    
    /*Send message to client*/
    
    /*
    char* message = "Hello from Server";
    int flag = send(client_new_fd, message, strlen(message), 0);
    */
    
}


/* Dirver Code for Socket Creation and handling multiple clients */
int main(int argc, char* argv[]){

    /*Check if PORT_NO is passed as command line argument*/
    if(argc <= 1){

            /*Print the message of incorrect run*/
            printf("Please Pass PORT_NO as command line argument.\n");

    }else{

            /*Convert char* to integer*/
            int PORT_NO = atoi(argv[1]);
            
            /*Both represent server and client file descripters respectively*/
            int sock_fd, clients_fd[MAX_CLIENTS] = {0};
            
            /*Server struct for address,port*/
            struct sockaddr_in server, client;
            
            /*
            
             Create Socket with socket function
             
             1. AF_INET for IPv4,IPv6 address.
             2. SOCK_STREAM for TCP type client
             3. 0 represents generic value for protocol
             
             Return value -1 for failure
            
            */
            if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            
                    printf("Creating Socket Failed......Please Try Again\n");
                    return 0;
                    
            }
            
            printf("Socket Created Successfully .....\n");
            
            /*  
                Allowing Server for handling multiple clients
                
                1. Socket descriptor file.
                2. SOL_SOCKET for socket api level changes.
                3. SO_REUSEADDR for option of multi clients
                4. void* True for allowing clients.
                
            */
            
            int opt = TRUE;
            
            if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, (socklen_t)sizeof(opt)) == -1){

                printf("Failed to update server options for handling multiple clients.\n");
                return 0;

            }
            
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
            
            /*
            
             Make server to listening state
             
             1. sock_fd Socket file descriptor
             2. Number of maximum backlog creations
            
            */
            if(listen(sock_fd, 3) == -1){
            
                    printf("Socket failed to Listen ......\n");
                    return 0;
            }
            
            printf("Socket successfully Created and went to Listen Connections.......\n");
            unsigned int length = sizeof(struct sockaddr_in);
            
            /*Read fds :- Set of socket descriptiors use to kepp information about all sockets*/
            fd_set read_fds;
            /*Run server indefinitely*/           
            while(TRUE){
            
                    /*Clear all the set Sockets and reinitialize*/
                    FD_ZERO(&read_fds);
                    
                    /*Set server socket in FD_SET*/
                    FD_SET(sock_fd, &read_fds);
                    
                    /*Call MaxFD to get maximum value of file descriptor*/
                    int max_sd = getMaxFD(sock_fd, clients_fd, &read_fds);
                    
                    /*
                    
                        Select function updates the set value of socket descriptors for any activity
                        
                        1. Max value to search for
                        2. Set of file descriptors
                        
                    */
                    
                    int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
                    
                    /*If returns -1 , but signal is caught leave*/
                    if(activity == -1 && errno != EINTR){
                    
                        printf("Select Throws ERROR!!\n");
                    
                    }
                    
                    /*
                    
                        There are two possible activities :-
                        
                            1. Either there is an incoming request in this case sock_fd willbe set
                            2. Or there is IO on any of the client
                            
                    */
                    
                    if(FD_ISSET(sock_fd, &read_fds)){
                    
                        /*There is incoming connection request accept it and send acceptance message to client*/
                        createConnection(sock_fd, &client, clients_fd);
                    
                    }
                    
                    /*Check if there are any i/o pending on clients*/
                    for(int i = 0;i < MAX_CLIENTS;i++){
            
                        int sd = clients_fd[i];
                        
                        /*If there is signal on any client sd check*/
                        if(FD_ISSET(sd, &read_fds)){
                        
                            char buffer[1024] = {0};
                            int flag  = recv(sd, buffer, 1024, 0);
                            
                            /*Close the client connection*/
                            if(flag <= 0){
                            
                                printf("Client %d Disconnected..........\n",sd);
                                
                                close(sd);
                                
                                /*Reinitialize the index*/
                                clients_fd[i] = 0;
                            
                            }else{
                            
                                printf("Message recieved from Client %d : %s\n",sd,buffer);
                                /*Echo back the same message to client*/
                                flag = send(sd, buffer, strlen(buffer), 0);
                                printf("Message Succesfully Echoed Back!!!!\n");
                            
                            }
                        
                        
                        }
            
                    }
            }
            
    }
}
