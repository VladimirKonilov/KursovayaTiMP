#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/**
 * @file Communicator.h
 * @brief Класс для взаимодействия с сервером через сокеты.
 * 
 * Этот класс предоставляет методы для подключения к серверу, отправки и получения сообщений.
 * 
 * @author Конилов В.В.
 * @date 2024-12-12
 */
class Communicator {
private:
    int socketFd; ///< Идентификатор сокета
    std::string serverAddress; ///< Адрес сервера
    int serverPort; ///< Порт сервера

public:
    /**
     * @brief Конструктор для инициализации объекта связи с сервером.
     * 
     * Конструктор создает объект, который будет использоваться для установления связи с сервером.
     * 
     * @param serverAddress Адрес сервера.
     * @param serverPort Порт сервера.
     */
    Communicator(const std::string& serverAddress, int serverPort);

    /**
     * @brief Деструктор для закрытия сокета, если он был открыт.
     * 
     * Деструктор закрывает сокет, если он был открыт, чтобы освободить ресурсы.
     */
    ~Communicator();

    /**
     * @brief Метод для подключения к серверу.
     * 
     * Создает сокет, устанавливает подключение с сервером. Выбрасывает исключение, 
     * если не удается создать сокет или подключиться к серверу.
     * 
     * @throws std::runtime_error если сокет не удалось создать или подключение не удалось.
     */
    void connectToServer();

    /**
     * @brief Метод для отправки строки сообщения на сервер.
     * 
     * Этот метод отправляет данные в виде строки. В случае ошибки выбрасывается исключение.
     * 
     * @param message Строка сообщения, которую нужно отправить.
     * @throws std::runtime_error если отправка данных не удалась.
     */
    void sendMessage(const std::string& message);

    /**
     * @brief Метод для отправки произвольных данных на сервер.
     * 
     * Отправляет данные определенного размера на сервер.
     * 
     * @param data Указатель на данные для отправки.
     * @param size Размер данных для отправки.
     * @throws std::runtime_error если отправка данных не удалась.
     */
    void sendMessage(const char* data, size_t size);

    /**
     * @brief Метод для получения сообщения от сервера в виде строки.
     * 
     * Читает данные от сервера и возвращает их в виде строки. Если размер полученных 
     * данных меньше ожидаемого, возвращается только полученные данные.
     * 
     * @param bufferSize Размер буфера для чтения данных.
     * @return Полученное сообщение.
     * @throws std::runtime_error если не удалось получить данные.
     */
    std::string receiveMessage(size_t bufferSize = 1024);

    /**
     * @brief Метод для получения сообщения от сервера и записи в указанный буфер.
     * 
     * Метод читает указанное количество байтов в буфер. Если количество полученных данных
     * не соответствует ожидаемому, выбрасывается исключение.
     * 
     * @param buffer Указатель на буфер для записи полученных данных.
     * @param size Размер данных, которые необходимо получить.
     * @throws std::runtime_error если количество полученных данных не совпадает с ожидаемым.
     */
    void receiveMessage(char* buffer, size_t size);
};

#endif // COMMUNICATOR_H
