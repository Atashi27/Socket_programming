//SERVER
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int listenfd = 0,k=0;
    int connfd = 0,nread,let;
    struct sockaddr_in serv_addr,client;
    char sendBuff[1025],buffer[50];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd==-1)
    {
        printf("Error in socket creation");
        exit(1);
    }
    printf("Socket created successfully\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    k=bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(k==-1)
    {
	    printf("Error in binding\n");
	    exit(1);
    }
    printf("Binded successfully\n");
    
    while(1)
    {
	    printf("Waiting for client to connect\n");
	    if(listen(listenfd, 20) == -1)
	    {
			printf("Failed to listen\n");
			exit(1);
	    }
	    printf("Connection established\n");
	    let=sizeof(client);
	    connfd = accept(listenfd, (struct sockaddr*)&client ,&let);
	    if(connfd==-1)
	    {
		    printf("Error in temporary socket creation\n");
		    exit(1);
		}   
         strcpy(buffer,"end");
         printf("Waiting to receive\n");
	    k=recv(connfd, buffer,50, 0);
	    FILE *fp2 = fopen(buffer,"r");
	    if(k==-1)
          {
              printf("Error in receiving\n");
              exit(1);
          }
 	    /*---- Print the received message ----*/
  	    printf("Data received: %s\n",buffer);
  	    if(strncmp(buffer,"end",3)==0)
  	      {
  	          printf("Connection terminated\n");
           	break;
  	      }
        else if(strncmp(buffer,"list",4)==0)
        {
            FILE *fp = fopen("files.txt","w");
    	    	  system("ls>>files.txt");
            fclose(fp);
            /* Open the file that we wish to transfer */
            FILE *fp1 = fopen("files.txt","r");
            if(fp1==NULL)
            {
                printf("File open error");
                return 1;   
            }   
            /* Read data from file and send it */
            printf("Sending reply\n");
	       printf("Sending contents of files.txt\n");
    	       while(1)
            {
    		      /* First read file in chunks of 256 bytes */
                unsigned char buff[256];
                nread = fread(buff,1,256,fp1);
                //printf("Bytes read %d \n", nread);        
                /* If read was success, send data. */                
                //write(connfd,nread,1);
                if(nread > 0)
                {
                    write(connfd, buff, nread);
                }
                // Either there was error, or we reached end of file.
                if (nread < 256)
                {
                    if (feof(fp1))  
    		            printf("Successfully sent\n");
                    if (ferror(fp1))
                        printf("Error reading\n");
      		        break;
                }
            }
            fclose(fp1);
	    	}
	    	else if(fp2!=NULL)
	    	{
		    	//Read data from file and send it 
		    	printf("Sending reply\n");
			printf("Sending contents of %s\n",buffer);
			while(1)
		       {
				    //First read file in chunks of 256 bytes 
				  unsigned char buff[256];
				  nread = fread(buff,1,256,fp2);
				  //Bytes read is nread      
				  // If read was success, send data.
				  if(nread > 0)
				  {
				     
				      write(connfd, buff, nread);
				  }

				  // Either there was error, or we reached end of file.
				   
				  if (nread < 256)
				  {
				      if (feof(fp2))  
						  printf("Successfully sent\n");
				      if (ferror(fp2))
				          printf("Error reading\n");
		  		        break;
				  }
		       }
	    	}
	 	else
	 	{
			printf("Sending reply\n");
			write(connfd, "Unidentified command", 20);
		}
	    close(connfd);
    }
    exit(0);
    return 0;
}
//Reference://http://codereview.stackexchange.com/questions/43914/client-server-implementation-in-c-sending-data-files
//Reference:https://osandnetworkingcslab.wordpress.com/client-server-chat-using-tcp/

