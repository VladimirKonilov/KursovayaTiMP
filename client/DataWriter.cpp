#include "DataWriter.h"
#include <fstream>
#include <iostream>

void DataWriter::writeResult(const std::string &result) {
    std::ofstream outFile("result.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << "Результат: " << result << std::endl;
        outFile.close();
    } else {
        std::cerr << "Не удалось открыть файл для записи результата." << std::endl;
    }
}

void DataWriter::writeError(const std::string &errorMessage) {
    std::ofstream outFile("error_log.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << "Ошибка: " << errorMessage << std::endl;
        outFile.close();
    } else {
        std::cerr << "Не удалось открыть файл для записи ошибки." << std::endl;
    }
}
