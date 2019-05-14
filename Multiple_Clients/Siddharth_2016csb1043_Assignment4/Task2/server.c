/*

Name : Siddharth Nahar
Entry : 2016csb1043
Date : 2/2/19
Purpose : 

    1. Contains Server side code.
    2. Get string from client , reverse the string and send it back to client.
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

/*Library for directory search*/
#include <dirent.h>

/*--------------------------------------------------------------------------------------------------------*/

/*Function reads file contents from file name and store it in file_content array*/
void readFileContents(int n_files, const char** listFiles, char file_name[], char file_content[]){

    /*Check if file name exists*/
    for(int i = 0;i < n_files;i++){
    
        /*If file found open file and search*/
        if(strcmp(listFiles[i], file_name) == 0){
        
            /*Create file name as Webpages/web1.html*/
            char complete_file_name[100] = {0};
            strcat(complete_file_name, "Webpages/");
            strcat(complete_file_name, listFiles[i]);
        
            /*Open file and assign it to pointer*/
            FILE *fptr = fopen(complete_file_name, "r");
            
            int index = 0;
            char c;
            
            /*Write content of file to file_content character by character*/
            while((c = fgetc(fptr)) != EOF){
            
                file_content[index++] = c;
            }
            
            return;
        }
    
    }
    
    /*If file is not found print 404 Error file not found*/
    strcpy(file_content, "ERROR 404: Page not found\n");

}

/*--------------------------------------------------------------------------------------------------------*/

/*Counts number of webpages with HTML extension*/
int getCountOfWebpages(){

    /*First Search for how many files with html extension are present.*/
    DIR* directory;
    
    /*Structure to store directory information*/
    struct dirent *dir; 

    directory = opendir("./Webpages");
    
    if(directory == NULL){
    
        printf("Unable to open directory!!\n");
        return 0;
    
    }
    
    int count = 0;
    /*Read all directory contents*/
    while((dir = readdir(directory)) != NULL){
    
        char* nameFile = dir->d_name;
        
        /*See if file is an HTML file*/
        char* index = strstr(nameFile, ".html");
        
        if(index == NULL)
            continue;
    
        count++;
        //printf("%d. %s\n",count, nameFile);
    }

    closedir(directory);
    

    return count;
}

/*--------------------------------------------------------------------------------------------------------*/

/*Search file in directory and create 2-D array of html file present.*/
const char** createListOfWebpages(int count){

    /*Structure to store directory information*/
    struct dirent *dir;
    /*Allocate size to double pointer*/
    char** listHTMLFiles = (char**)malloc(count*sizeof(char *));
    
    DIR* directory = opendir("./Webpages");
    
    //printf("Size allocate dcorrectly\n");
    count = 0;
    while((dir = readdir(directory)) != NULL){
    
        char* nameFile = dir->d_name;
        
        /*See if file is an HTML file*/
        char* index = strstr(nameFile, ".html");
        
        if(index == NULL)
            continue;
    
        /*Allocate size to each indivisual pointer*/
        listHTMLFiles[count] = (char*)malloc((strlen(nameFile) + 1)*sizeof(char));
        
        /*Copy the content and increment the pointer*/
        strcpy(listHTMLFiles[count], nameFile);
        
        //printf("%d. %s\n",count, listHTMLFiles[count]);
        
        count++;
    }
    
    return (const char**)listHTMLFiles;
    
}

/*--------------------------------------------------------------------------------------------------------*/

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
                 2. SOCK_STREAM for TCP type client
                 3. 0 represents generic value for protocol
                 
                 Return value -1 for failure
                
                */
                if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                
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
                /*No forking So only 1 client at time but can accept as many clients as possible*/
                
                int n_files = getCountOfWebpages();
                
                const char** listHTMLFiles = createListOfWebpages(n_files);
                
                printf("\nThis are files Present in Directory :- \n");
                
                for(int i = 0;i < n_files;i++){
                
                    printf("%d. %s\n",i, listHTMLFiles[i]);
                
                }
                
                while(1){
                   
                        int client_fd;
                                
                        printf("\n\nWaiting to accept Connections...............................\n");
                        if((client_fd = accept(sock_fd, (struct sockaddr *)&client, &length)) == -1){
                        
                                printf("Failed to Connect to Client.........\n");
                                continue;
                        }
                        
                        printf("Client Successfully Connected .......\n");
                        
                        /*Created two buffers to send and recieve data from client*/
                        char recvBuffer[1024] = {0};
                        char sendBuffer[1024] = {0};
                        
                        ssize_t flag = 0;
                        
                        
                        /*check variable to see if client has closed connection or not*/
                        int check = 1;
                        while(check == 1){
                         
                                /*
                        
                                 Recv function to read from client.
                                 
                                 1. Client id file descriptor of client.
                                 2. char array to recieve string from client
                                 3. Length of array
                                 4. flag descriptor
                                
                                */
                                
                                
                                flag = recv(client_fd, recvBuffer, 1024, 0);
                                
                                
                                if(flag <= 0){
                                
                                    printf("Closing Connection of Client...........\n");
                                    close(client_fd);
                                    break;
                                
                                }
                                
                                /*Terminating Client Condition String passed is "BYE"*/
                                printf("The Client sends the file_name as : %s\n",recvBuffer);
                                
                                /*Call Read file contents from recvBuffer and send to Client*/
                                readFileContents(n_files, listHTMLFiles, recvBuffer, sendBuffer);
                                
                                
                                /*Send the client the reversed String*/
                                flag = send(client_fd, sendBuffer, 1024, 0);
                                
                                /*Reset Strings to 0*/
                                memset(recvBuffer, 0, 1024);
                                memset(sendBuffer, 0, 1024);                                                                
                
                        }
                        
                        
                }
                
        }
        
        return 0;
}
