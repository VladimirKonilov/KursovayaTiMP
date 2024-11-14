#include "UserInterface.h"
#include "Communicator.h"
#include "DataReader.h"
#include "DataWriter.h"

int main() {
    UserInterface ui;
    DataReader dataReader;
    DataWriter dataWriter;
    Communicator communicator("127.0.0.1", 33333);  // Сервер на localhost:33333

    // Получение данных для аутентификации и векторов
    std::string credentials = dataReader.getCredentials();
    std::vector<std::vector<int>> vectors = dataReader.getVectors();

    try {
        ui.displayMessage("Попытка аутентификации на сервере...");

        // Отправка данных для аутентификации и векторов
        bool authenticated = communicator.authenticateAndSendVectors(credentials, vectors);

        if (authenticated) {
            ui.displayMessage("Аутентификация успешна.");
            dataWriter.writeResult("Аутентификация успешна.");
        } else {
            ui.displayMessage("Аутентификация не удалась.");
            dataWriter.writeResult("Аутентификация не удалась.");
        }
    } catch (const std::exception &e) {
        ui.displayError(e.what());
        dataWriter.writeError(e.what());
    }

    return 0;
}
