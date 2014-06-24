#include<iostream>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

const int MAX_BUF_LEN = 256;

using namespace std;

int main()
{
  int server_sock;
  int client_local_sock;
  int server_len;
  socklen_t client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  //name the server socket
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); //any client can connect.
  server_address.sin_port = 9734;
  server_len = sizeof(server_address);

  bind(server_sock, (struct sockaddr *) &server_address, server_len);

  //create a connection queue for clients.
  listen(server_sock, 5); //5 is the number of clinets which can wait.

  char buff[MAX_BUF_LEN];
  client_len = sizeof(client_address);
  client_local_sock = accept(server_sock, (struct sockaddr *) &client_address, &client_len);  
  
  while(1)
    {
      cout<<"server waiting...."<<endl;
      
      read(client_local_sock, buff, MAX_BUF_LEN-1);
      write(client_local_sock, buff, MAX_BUF_LEN-1);
      
      //check if it is "exit" message from client
      if(!strncmp(buff, "exit", 4))
      {
       close(client_local_sock);
       client_local_sock = accept(server_sock, (struct sockaddr *) &client_address, &client_len);
      }
      
    }
  return 0;
}
