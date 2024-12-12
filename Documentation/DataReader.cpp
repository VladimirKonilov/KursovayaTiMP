#ifndef DATAREADER_H
#define DATAREADER_H

#include <string>
#include <fstream>
#include <stdexcept>

/**
 * @file DataReader.h
 * @brief Класс для чтения данных из файла построчно.
 *
 * Этот класс используется для открытия файла, чтения данных построчно и проверки конца файла.
 * Он выбрасывает исключение, если файл не может быть открыт или если при чтении файла произошла ошибка.
 *
 * @author Конилов В.В.
 * @date 2024-12-12
 */
class DataReader {
private:
    std::ifstream file; ///< Поток для чтения из файла

public:
    /**
     * @brief Конструктор для инициализации объекта чтения данных из файла.
     *
     * Открывает указанный файл для чтения. Если файл не удается открыть, выбрасывает исключение.
     *
     * @param filename Имя файла для открытия.
     * @throws std::runtime_error если не удается открыть файл.
     */
    DataReader(const std::string& filename);

    /**
     * @brief Чтение следующей строки из файла.
     *
     * Читает одну строку из файла. Если строка успешно считана, она возвращается. 
     * Если конец файла достигнут, возвращается пустая строка.
     *
     * @return Считанная строка, или пустая строка, если конец файла.
     */
    std::string readNextLine();

    /**
     * @brief Проверка достижения конца файла.
     *
     * Проверяет, достигнут ли конец файла.
     *
     * @return true, если конец файла достигнут; false в противном случае.
     */
    bool eof() const;

    /**
     * @brief Деструктор для закрытия файла.
     *
     * Закрывает файл, если он открыт, освобождая ресурсы.
     */
    ~DataReader();
};

#endif // DATAREADER_H
