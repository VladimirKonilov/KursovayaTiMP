#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <fstream>
#include <string>
#include <stdexcept>

class DataWriter {
    std::ofstream file;

public:
    explicit DataWriter(const std::string& filename);
    void writeLine(const std::string& line);
    ~DataWriter();
};

#endif // DATA_WRITER_H
