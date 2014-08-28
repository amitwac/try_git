#include "HTTPServer.h"


const int PORT = 8000;
const int CLIENT_QUEUE_LEN = 5;

  //--------------------------------------------------------------------------------------------------------------------------------
  // GET / HTTP/1.1                                                                                                                 |

  // Host: rddev03v3.punin.sen.symantec.com:16415                                                                                   |

  // Connection: keep-alive                                                                                                         |

  // Cache-Control: max-age=0                                                                                                       |

  // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8                                             |

  // User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.153 Safari/537.36      |

  // Accept-Encoding: gzip,deflate,sdch                                                                                             |

  // Accept-Language: en-US,en;q=0.8,hi;q=0.6,ms;q=0.4                                                                              |

  // Cookie: xxxxxxxxxx                                                                                                             |
  //--------------------------------------------------------------------------------------------------------------------------------

int CBufferedReadAndWrite::trim(char *buffer)
{
  int len = strlen(buffer)-1;
  while( !isalnum(buffer[len]) && len>=0 )
    {
      buffer[len--] = '\0';
    }
  return 0;
}


int CBufferedReadAndWrite::read(int sock_fd, unsigned int nbytes)
{
  cout<<"ENTER read"<<endl;
  int retval = 0;

  FILE* stream = fdopen(sock_fd, "a+"); //open a stream linked with passed file descriptor
  if(NULL == stream)
    {
      cout<<"failed to open stream for socket"<<endl;
      return retval;
    }
  char temp_line[nbytes];

  //read request header
  //size_t found;
  //string tempstr;

  while(1)
    {
      fgets(temp_line, nbytes, stream);
      trim(temp_line);
      if(temp_line[0] == '\0')
        {
          //we received blank line;
          break;
        }
      vbuff.push_back(temp_line);
    };
  //end of header, need to read "Content-Length" header to decide if we need to read body or not.
  cout<<"EXIT read"<<endl;
  return retval;
}

int CBufferedReadAndWrite::printBuffer()
{
  cout<<"Printing HTTP REQUEST"<<endl;
  int retval = 0;
  vector<string>::const_iterator citr;
  citr = vbuff.begin();
  while(citr != vbuff.end())
    {
      cout<<*citr<<endl;;
      citr++;
    }
  cout<<"EXIT printBuffer"<<endl;
  return retval;
}

int CHTTPServer::readReqMsg(int sockfd)
{
  int retval = 0;
  int ret = m_cbufreadwrite.read(sockfd);
  if(ret != 0)
    {
      //some error occured.
      retval = ret;
    }
  return retval;
}

int CHTTPServer::writeResMsg()
{
  int retval = 0;

  return retval;
}


int CHTTPServer::sendResMsg(int sockfd)
{
  int retval = 0;
  cout<<"ENTER SendResMsg"<<endl;
  FILE* stream = fdopen(sockfd, "w+"); //open a stream linked with passed file descriptor
  if(NULL == stream)
    {
      cout<<"failed to open stream for socket"<<endl;
      return retval;
    }

  string resStrHeader = m_cresmsg.getVersion() + " " + m_cresmsg.getStatus() + " " + m_cresmsg.getReasonPhrase();
  string resStrBodyline1 = "<!DOCTYPE html>";
  string resStrBodyline2 = "<html>";
  string resStrBodyline3 = "<body>";
  string resStrBodyline4 = "<h1 My Home Page</h1>>";
  string resStrBodyline5 = "</body>";
  string resStrBodyline6 = "</html>";

  if( fputs(resStrHeader.c_str(), stream) > 0 );
  {
    cout<<"header written successfully"<<endl;
  }
  if( fputs("\n", stream) > 0 );
  {
    cout<<"blank line written"<<endl;
  }
  fputs(resStrBodyline1.c_str(), stream);
  fputs(resStrBodyline2.c_str(), stream);
  fputs(resStrBodyline3.c_str(), stream);
  fputs(resStrBodyline4.c_str(), stream);
  fputs(resStrBodyline5.c_str(), stream);
  fputs(resStrBodyline6.c_str(), stream);
  fputs("\n", stream);
  fclose(stream);
  cout<<"EXIT sendResMsg"<<endl;
  return retval;
 }

int CHTTPServer::printReqMsg()
{
  m_cbufreadwrite.printBuffer();
  return 0;
}


int CHTTPServer::closeConn(int sockfd)
{
  cout<<"ENTER closeConn"<<endl;
  int retval = 0;
  close(sockfd);
  cout<<"EXIT closeConn"<<endl;
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

  //client_len = sizeof(client_address);
  //while(1)
  //{
      client_local_sock = accept(server_sock, NULL, NULL);

      CHTTPServer httpserver;
      httpserver.readReqMsg(client_local_sock);
      httpserver.printReqMsg();
      //httpserver.writeResMsg();
      httpserver.sendResMsg(client_local_sock);
      httpserver.closeConn(client_local_sock);
      //}
  cout<<"EXIT main"<<endl;
 return 0;
}
