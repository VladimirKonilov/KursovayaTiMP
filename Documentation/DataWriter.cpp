#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <fstream>
#include <string>
#include <stdexcept>

/**
 * @file DataWriter.h
 * @brief Класс для записи данных в файл.
 *
 * Этот класс предоставляет функциональность для записи строк в файл.
 * В случае ошибок при открытии файла или записи выбрасываются исключения.
 *
 * @author Конилов В.В.
 * @date 2024-12-12
 */
class DataWriter {
private:
    std::ofstream file; ///< Поток для записи в файл.

public:
    /**
     * @brief Конструктор для инициализации объекта и открытия файла для записи.
     *
     * Открывает файл для записи. Если файл не удается открыть, выбрасывает исключение.
     *
     * @param filename Имя файла для записи.
     * @throws std::runtime_error Если файл не удается открыть.
     */
    explicit DataWriter(const std::string& filename);

    /**
     * @brief Запись строки в файл.
     *
     * Записывает переданную строку в файл и добавляет символ новой строки в конце.
     *
     * @param line Строка, которая будет записана в файл.
     */
    void writeLine(const std::string& line);

    /**
     * @brief Деструктор для закрытия файла.
     *
     * Закрывает файл, если он открыт, и освобождает все ресурсы.
     */
    ~DataWriter();
};

#endif // DATA_WRITER_H
