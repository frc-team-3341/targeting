#ifndef CRIOLink_hpp
#define CRIOLink_hpp

class CRIOLink {
public:
  // Functions
  CRIOLink(); // Constructor
  void initServer(); // Server Initialization
  void waitForPing(); // Wait for Ping
  void sendData(int /* Distance */, int /* Height */, float /* Azimuth */, float /* Tilt */); // Send Data
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
