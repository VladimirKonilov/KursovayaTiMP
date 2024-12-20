#ifndef DATA_READER_H
#define DATA_READER_H

#include <fstream>
#include <string>
#include <stdexcept>

/**
 * @file DataReader.h
 * @brief Класс для чтения данных из файла построчно.
 *
 * Этот класс предназначен для открытия файла, чтения данных из файла построчно, а также для проверки конца файла.
 * В случае ошибок при открытии файла или чтении выбрасываются исключения.
 *
 * @author Конилов В.В.
 * @date 2024-12-12
 */
class DataReader {
private:
    std::ifstream file; ///< Поток для чтения из файла.

public:
    /**
     * @brief Конструктор для инициализации объекта и открытия файла.
     *
     * Открывает файл для чтения. Если файл не удается открыть, выбрасывает исключение.
     *
     * @param filename Имя файла для открытия.
     * @throws std::runtime_error если файл не удается открыть.
     */
    explicit DataReader(const std::string& filename);

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
     * @return true, если конец файла достигнут, иначе false.
     */
    bool eof() const;

    /**
     * @brief Деструктор для закрытия файла.
     *
     * Закрывает файл, если он открыт, и освобождает все ресурсы.
     */
    ~DataReader();
};

#endif // DATA_READER_H
