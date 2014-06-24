#include<iostream>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const int MAX_BUFFER_LEN = 256;
using namespace std;

int main()
{
  int sockfd;
  int len; 
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = 9734;
  len = sizeof(address);

  //connect the client socket wih the server socket.
  int connectResult = connect(sockfd, (struct sockaddr *) &address, len);
  if(connectResult == -1)
    {
      cerr<<"connection failure."<<endl;
      return 1;
    }
  char buffer[MAX_BUFFER_LEN];
  
  do 
  {
     memset(buffer, 0, strlen(buffer));
     cin.getline(buffer, MAX_BUFFER_LEN);
     write(sockfd, buffer, MAX_BUFFER_LEN-1);
     read(sockfd, buffer, MAX_BUFFER_LEN-1);                      
     cout<<"returned message from server - "<<buffer<<endl;
   }while(strncmp(buffer, "exit", 4));
  
  
  close(sockfd);
  //cout<<"socket programming"<<endl;
  return 0;
}  
