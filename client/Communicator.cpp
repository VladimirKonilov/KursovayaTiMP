#include "Communicator.h"

Communicator::Communicator(const std::string& ip, int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw std::runtime_error("Socket creation failed");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid IP address");
    }
}

void Communicator::connectToServer() {
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Connection to server failed");
    }
}

void Communicator::sendMessage(const std::string& message) {
    send(socket_fd, message.c_str(), message.size(), 0);
}

std::string Communicator::receiveMessage(size_t bufferSize) {
    char buffer[bufferSize];
    ssize_t bytesRead = recv(socket_fd, buffer, bufferSize - 1, 0);
    if (bytesRead <= 0) {
        throw std::runtime_error("Failed to receive message");
    }
    buffer[bytesRead] = '\0';
    return std::string(buffer);
}

Communicator::~Communicator() {
    close(socket_fd);
}
