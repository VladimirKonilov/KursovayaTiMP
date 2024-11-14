#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>
#include <vector>  // Necessary for using std::vector
#include <stdexcept>

class Communicator {
public:
    Communicator(const std::string &host, int port);
    bool authenticateAndSendVectors(const std::string &credentials, const std::vector<std::vector<int>> &vectors);

private:
    std::string createHttpRequest(const std::string &jsonData, const std::string &host);
    std::string formatVectorsAsJson(const std::vector<std::vector<int>> &vectors);  // Declare the function here
    std::string serverHost;
    int serverPort;
};

#endif // COMMUNICATOR_H
