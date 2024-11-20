#include "DataReader.h"

DataReader::DataReader(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
}

std::string DataReader::readNextLine() {
    std::string line;
    if (std::getline(file, line)) {
        return line;
    }
    return {};
}

bool DataReader::eof() const {
    return file.eof();
}

DataReader::~DataReader() {
    if (file.is_open()) {
        file.close();
    }
}
