#include "UserInterface.h"

UserInterface::UserInterface(int argc, char** argv) : serverPort(33333), configFile("~/.config/vclient.conf") {
    int opt;
    while ((opt = getopt(argc, argv, "a:p:i:o:c:h")) != -1) {
        switch (opt) {
            case 'a':
                serverAddress = optarg;
                break;
            case 'p':
                serverPort = std::stoi(optarg);
                break;
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'c':
                configFile = optarg;
                break;
            case 'h':
                printHelp();
                std::exit(0);
            default:
                handleError("Invalid option provided.");
                break;
        }
    }

    if (serverAddress.empty() || inputFile.empty() || outputFile.empty()) {
        handleError("Missing required parameters.");
    }
}

void UserInterface::printHelp() {
    std::cout << "Usage: client -a <server_address> -p <server_port> -i <input_file> -o <output_file> -c <config_file>\n";
    std::cout << "Options:\n";
    std::cout << "  -a server      Server address (required)\n";
    std::cout << "  -p port        Server port (optional, default: 33333)\n";
    std::cout << "  -i input_file  Input file name (required)\n";
    std::cout << "  -o output_file Output file name (required)\n";
    std::cout << "  -c config_file Configuration file with LOGIN and PASSWORD (optional, default: ~/.config/vclient.conf)\n";
    std::cout << "  -h             Display help\n";
}

void UserInterface::handleError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    std::exit(1);
}
