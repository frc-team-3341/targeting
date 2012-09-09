/*
 *    This file is part of FRC Team 3341 Aimer.
 *
 *    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "CRIOLink.hpp"

using namespace std;

// Public Methods
CRIOLink::CRIOLink()
{
    // Variable Initializations
    serverInit = 0;
}

void CRIOLink::initServer()
{
    // Variable Initializations
    serverInit = 1;
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

void CRIOLink::waitForPing()
{
    if (recvMessage().empty()) exit(0);
}

void CRIOLink::sendData(float velocity, int height, float azimuth, float tilt)
{
    stringstream datatoSend;
    datatoSend <<velocity <<";" <<height <<";" <<azimuth <<";" <<tilt;
    sendMessage(datatoSend.str());
}

void CRIOLink::sendData()
{
    string datatoSend="No rectangle";
    sendMessage(datatoSend);
}

CRIOLink::~CRIOLink()
{
    if (serverInit)
        deInitServer();
}

// Private Methods
void CRIOLink::deInitServer()
{
    shutdown(sockfd, 2);
    shutdown(clientfd, 2);
}

void CRIOLink::sendMessage(string msg)
{
    char *cmsg=new char[msg.size()+1];
    strcpy(cmsg, msg.c_str());
    int msglength=strlen(cmsg);
    for (int bytes_sent = 0; bytes_sent < msglength;)
        bytes_sent += send(clientfd, cmsg, msglength, 0);
    delete[] cmsg;
}

string CRIOLink::recvMessage()
{
    int length=256;
    char *input=new char[length];
    stringstream inputStream;
    if (recv(clientfd, input, length, 0))
        inputStream <<input;
    else
        inputStream <<"";
    delete[] input;

    return inputStream.str();
}
