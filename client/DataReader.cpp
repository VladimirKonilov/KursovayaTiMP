#include "DataReader.h"
#include <string>

std::vector<std::vector<int>> DataReader::getVectors() {
    // Возвращаем 4 вектора по 4 значения
    return {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
}

std::string DataReader::getCredentials() {
    // Возвращаем логин и пароль в формате "user:P@ssW0rd"
    return "user:P@ssW0rd";
}
