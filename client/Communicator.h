#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Communicator {
    int socket_fd;
    struct sockaddr_in server_addr;

public:
    Communicator(const std::string& ip, int port);
    void connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage(size_t bufferSize = 1024);
    ~Communicator();
};

#endif // COMMUNICATOR_H
