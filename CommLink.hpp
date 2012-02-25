#ifndef CommLink_hpp
#define CommLink_hpp

class CommLink {
public:
  // Functions
  CommLink(); // Constructor
  void waitForPing(); // Wait for Ping
  void sendData(int /* Distance */, int /* Height */, float /* Azimuth */, float /* Tilt */); // Send Data
  ~CommLink(); // Destructor

  // Variables
private:
  // Functions
  void initServer(); // Server Initialization
  void deInitServer(); // Server Deinitialization
  void sendMessage(std::string); // Send Message
  std::string recvMessage(); // Receive Message

  // Variables
  struct sockaddr_storage client_addr;
  socklen_t addr_size;
  struct addrinfo hints;
  struct addrinfo *res;
  int sockfd;
  int clientfd;
  std::string port;
  int backlog;
};

#endif /* CommLink_hpp */
