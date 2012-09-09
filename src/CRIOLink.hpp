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

#ifndef CRIOLink_hpp
#define CRIOLink_hpp

class CRIOLink
{
public:
    // Functions
    CRIOLink(); // Constructor
    void initServer(); // Server Initialization
    void waitForPing(); // Wait for Ping
    void sendData(float /* Velocity */, int /* Height */, float /* Azimuth */, float /* Tilt */); // Send Data
    void sendData(); // Send Empty Data
    ~CRIOLink(); // Destructor

    // Variables
private:
    // Functions
    void deInitServer(); // Server Deinitialization
    void sendMessage(std::string); // Send Message
    std::string recvMessage(); // Receive Message

    // Variables
    int serverInit;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd;
    int clientfd;
    std::string port;
    int backlog;
};

#endif /* CRIOLink_hpp */
