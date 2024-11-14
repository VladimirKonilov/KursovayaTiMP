#ifndef DATAWRITER_H
#define DATAWRITER_H

#include <string>

class DataWriter {
public:
    void writeResult(const std::string &result);
    void writeError(const std::string &errorMessage);
};

#endif // DATAWRITER_H
