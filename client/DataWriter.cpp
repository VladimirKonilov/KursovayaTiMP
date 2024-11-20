#include "DataWriter.h"

DataWriter::DataWriter(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing");
    }
}

void DataWriter::writeLine(const std::string& line) {
    file << line << std::endl;
}

DataWriter::~DataWriter() {
    if (file.is_open()) {
        file.close();
    }
}
