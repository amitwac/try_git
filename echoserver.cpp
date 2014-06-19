#include<iostream>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

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
  
  while(1)
    {
      char ch;
      cout<<"server waiting...."<<endl;
      client_len = sizeof(client_address);
      client_local_sock = accept(server_sock, (struct sockaddr *) &client_address, &client_len);
      read(client_local_sock, &ch, 1);
      ch++;
      write(client_local_sock, &ch, 1);
      close(client_local_sock);
    }
  return 0;
}
