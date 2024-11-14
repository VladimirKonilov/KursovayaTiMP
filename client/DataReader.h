#ifndef DATAREADER_H
#define DATAREADER_H

#include <vector>
#include <string>  // Добавляем эту строку для использования std::string

class DataReader {
public:
    std::vector<std::vector<int>> getVectors();
    std::string getCredentials();
};

#endif // DATAREADER_H
