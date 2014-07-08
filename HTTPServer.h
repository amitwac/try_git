#include<iostream>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<map>
#include<time.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

using namespace std;

const int CONT_LEN = 100;

class CBufferedReader
{
 private:
  vector<string> vbuff;
 public:
 CBufferedReader(): vbuff(NULL)
    {
    }
  int bufread(int sock_fd, unsigned int nbytes); //read from socket into the string vector
};


class CResMsg
{
private:
  string m_sversion;
  int m_nstatus;
  string m_sreasonPhrase;
  map<string,string> m_mresponseHeader;
  string m_sbody;

public:
  CResMsg(string version = "HTTP/1.1", int code = 200, string phrase = "ok"):
    m_sversion(version),
    m_nstatus(code),
    m_sreasonPhrase(phrase)
  {
    time_t current_time = time(NULL);
    char* c_time = ctime(&current_time);
    m_mresponseHeader["Date:"] = c_time;/*get_date_unix_method()*/;
    m_mresponseHeader["Last-Modified:"] = c_time/*get last modified date*/;
    m_mresponseHeader["Content-Type:"] = "text/html";
    m_mresponseHeader["Content-Length:"] = CONT_LEN;
    m_mresponseHeader["Connection:"] = "close";
    m_sbody = "";
  }
};

