//CLIENT
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd = 0;
    int bytesReceived = 0,i=0,k=0;
    char recvBuff[256];
    char buffer[20];
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0, sizeof(serv_addr));

    while(1)
    {
	    /* Create a socket first */
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if(sockfd==-1)
	    {
		printf("\n Error in socket creation\n");
		exit(1);
	    }
	    printf("Socket created successfully\n");
	    /* Initialize sockaddr_in data structure */
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(5000); // port
	    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	   /* Attempt a connection */
	    k=connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	    if(k==-1)
	    {
		printf("Error in connecting to server\n");
		exit(1);
	    }
	    printf("Connection established\n");
    		strcpy(buffer,"end");
      	printf("Enter command:\t");
      	scanf("%s",buffer);
      	/*---- Send message to the socket of the incoming connection ----*/
      	k=send(sockfd,buffer,20,0);
                if(k==-1)
                {
                    printf("Error in sending\n");
                    exit(1);
				}
        	printf("Sending command %s\n",buffer);
      	if(strncmp(buffer,"end",3)==0)//Use "end" to end communication with server
            {
                printf("Terminating the connection\n");
                break;
            }
       	 else if(strncmp(buffer,"list",4)==0)
       	 {
		       /* Create file where data will be stored */
		       FILE *fp,*fp1;
		       fp = fopen("filesdata.txt", "w"); 
		       if(NULL == fp)
		       {
		           printf("Error opening file\n");
		           return 1;
		       }
		       /* Receive data in chunks of 256 bytes */
		       printf("Recieving reply\n");
		       while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
		       {
		           fwrite(recvBuff, 1,bytesReceived,fp);
		       }
		       if(bytesReceived < 0)
		       {
		           printf("\n Read Error \n");
		       }
		       fclose(fp);
		       fp1=fopen("filesdata.txt","r");
		       if(NULL == fp1)
		       {
		           printf("Error opening file\n");
		           return 1;
		       }
		       printf("List of files:\n");
		       printf("\n*****************\n");
		       while(fgets(recvBuff,256,fp1))
		           printf("%s",recvBuff);
		       printf("\n*****************\n");
		       fclose(fp1);
        }
        else
        {
             //Create file where data will be stored
            FILE *fp2,*fp3;
            fp2 = fopen(buffer, "w"); 
            if(NULL == fp2)
            {
                printf("Error opening file\n");
                return 1;
            }
         	  printf("Recieving reply\n");
            // Receive data in chunks of 256 bytes 
            while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
            {
                fwrite(recvBuff, 1,bytesReceived,fp2);
            }
            if(bytesReceived < 0)
            {
                printf("\n Read Error \n");
            }
            fclose(fp2);
            
            fp3=fopen(buffer,"r");
            if(NULL == fp3)
            {
                printf("Error opening file\n");
                return 1;
            }
            //Contents of files or negative feedback
            printf("\n*****************\n");
            while(fgets(recvBuff,256,fp3))
                printf("%s",recvBuff);
            printf("\n*****************\n");
            fclose(fp3);
        }
	close(sockfd);
    }
    
    exit(0);
    return 0;
}

//Reference:http://codereview.stackexchange.com/questions/43914/client-server-implementation-in-c-sending-data-files
//Reference:https://osandnetworkingcslab.wordpress.com/client-server-chat-using-tcp/
