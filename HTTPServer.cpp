#include "HTTPServer.h"

const int MAX_BUF_LEN = 256;
const int PORT = 8080;
const int CLIENT_QUEUE_LEN = 5;

int CBufferedReader::bufread(int sock_fd, unsigned int nbytes)
{
  int retval = -1;

  FILE* stream = fdopen(sock_fd, "a+"); //open a stream linked with passed file descriptor
  if(NULL == stream)
    {
      cout<<"failed to open stream fro socket"<<endl;
      return retval;
    }
  char temp_line[nbytes];

  //read request header
  fgets(temp_line, nbytes, stream); //read MAX_BUF_LEN bytes or till we get newline character, whichever comes first

  while(strncmp(temp_line, "\n", 1))
    {
      if(strlen(temp_line) < nbytes -1) //we encountered newline, so need to discard newline from temp_line
	{
	  temp_line[strlen(temp_line)] = '\0';
	}
      vbuff.push_back(temp_line);
      fgets(temp_line, nbytes, stream);
    }
  //end of header, need to read "Content-Length" header to decide if we need to read body or not.

  return retval;
}

int main()
{
  int server_sock;
  int client_local_sock;
  int server_len;
  struct sockaddr_in server_address;
 
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  //name the server socket
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); //any client can connect.
  server_address.sin_port = PORT;
  server_len = sizeof(server_address);

  bind(server_sock, (struct sockaddr *) &server_address, server_len);

  //create a connection queue for clients.
  listen(server_sock, CLIENT_QUEUE_LEN); //number of clinets which can wait.

  CBufferedReader myreader;
  client_local_sock = accept(server_sock, NULL, NULL);
  myreader.bufread(client_local_sock, MAX_BUF_LEN);
}
