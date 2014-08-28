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
const int MAX_BUF_LEN = 256;

class CBufferedReadAndWrite
{
 private:
  vector<string> vbuff;
 public:
 CBufferedReadAndWrite(): vbuff(NULL)
    {
    }
  int read(int sock_fd, unsigned int nbytes = MAX_BUF_LEN);
  int write(int sock_fd, unsigned int nbytes = MAX_BUF_LEN);
  int printBuffer();
  int trim(char *buffer);
};


class CResMsg
{
private:
  string m_sversion;
  string m_sstatus;
  string m_sreasonPhrase;
  map<string,string> m_mresponseHeader;
  string m_sbody;

public:
  CResMsg(string version = "HTTP/1.1", string code = "404", string phrase = "ok"):
    m_sversion(version),
    m_sstatus(code),
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
    int setHeadder(string head, string value);

    string getVersion()
    {
      return m_sversion;
    }

    string getStatus()
    {
      return m_sstatus;
    }

    string getReasonPhrase()
    {
      return m_sreasonPhrase;
    }
};

class CHTTPServer
{
 private:
  CResMsg m_cresmsg;
  CBufferedReadAndWrite m_cbufreadwrite;
 public:
 CHTTPServer(string ver = "HTTP/1.1", string code = "200", string phrase = "ok"):
  m_cresmsg(CResMsg(ver, code, phrase)),
  m_cbufreadwrite(CBufferedReadAndWrite())
    {
    }
  int writeResMsg();
  int sendResMsg(int sockfd);
  int readReqMsg(int sockfd);
  int closeConn(int sockfd);
  int printReqMsg();
};
