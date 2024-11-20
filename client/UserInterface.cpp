#include "UserInterface.h"

UserInterface::UserInterface(int argc, char** argv) {
    int opt;
    while ((opt = getopt(argc, argv, "T:H:S:h")) != -1) {
        switch (opt) {
            case 'T':
                type = optarg;
                break;
            case 'H':
                hash = optarg;
                break;
            case 'S':
                side = optarg;
                break;
            case 'h':
                printHelp();
                exit(0);
            default:
                throw std::runtime_error("Invalid arguments");
        }
    }
    validate();
}

void UserInterface::printHelp() {
    std::cerr << "Usage: client -T <data_type> -H <hash_type> -S <salt_side>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -T data_type   Supported types: float, double, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t\n";
    std::cerr << "  -H hash_type   Supported hashes: MD5, SHA1, SHA224, SHA256\n";
    std::cerr << "  -S salt_side   Supported sides: server, client\n";
}

void UserInterface::validate() {
    if (type.empty() || hash.empty() || side.empty()) {
        throw std::runtime_error("Missing required parameters");
    }
}
