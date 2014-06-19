#include<iostream>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

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
  char ch = 'A';
  write(sockfd, &ch, 1);
  read(sockfd, &ch, 1);
  cout<<"returned message from server - "<<ch<<endl;
  close(sockfd);
  //cout<<"socket programming"<<endl;
  return 0;
}  
