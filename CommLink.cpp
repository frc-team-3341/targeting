#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "CommLink.hpp"

using namespace std;

// Public Methods
CommLink::CommLink() {
  initServer();
}

void CommLink::waitForPing() {
  if (recvMessage().empty()) exit(0);
}

void CommLink::sendData(int distance, int height, float azimuth, float tilt) {
  stringstream datatoSend;
  datatoSend <<distance <<";" <<height <<";" <<azimuth <<";" <<tilt;
  sendMessage(datatoSend.str());
}

CommLink::~CommLink() {
  deInitServer();
}

// Private Methods
void CommLink::initServer() {
  // Variable Initializations
  memset(&hints, 0, sizeof hints); // Zero Out Structure
  hints.ai_family=AF_UNSPEC; // Use IPv4 or IPv6
  hints.ai_flags=AI_PASSIVE; // Auto-determine IP
  port="3341";
  backlog=1;
  getaddrinfo(NULL, port.c_str(), &hints, &res); // Resolve IP Information

  // Listen for Client Connection
  sockfd=socket(res->ai_family, res->ai_socktype, res->ai_protocol); // Open Socket
  bind(sockfd, res->ai_addr, res->ai_addrlen); // Bind Socket
  listen(sockfd, backlog); // Listen for Connections

  // Accept Client Connection
  addr_size=sizeof client_addr;
  clientfd=accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

  // Notify Client of Status
  string msg="Ready";
  sendMessage(msg);
}

void CommLink::deInitServer() {
  close(sockfd);
  close(clientfd);
}

void CommLink::sendMessage(string msg) {
  char *cmsg=new char[msg.size()+1];
  strcpy(cmsg, msg.c_str());
  int msglength=strlen(cmsg);
  for (int bytes_sent = 0; bytes_sent < msglength;)
    bytes_sent += send(clientfd, cmsg, msglength, 0);
  delete[] cmsg;
}

string CommLink::recvMessage() {
  int length=256;
  char *input=new char[length];
  stringstream inputStream;
  if (recv(clientfd, input, length, 0))
    inputStream <<input;
  else
    inputStream <<"Connection closed.";
  delete[] input;

  return inputStream.str();
}
