# Имя исполняемого файла
TARGET = client

# Компилятор и флаги компиляции
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Список файлов исходного кода и объектных файлов
SRC = main.cpp DataReader.cpp DataWriter.cpp Communicator.cpp UserInterface.cpp
OBJ = $(SRC:.cpp=.o)

# Правило по умолчанию: компиляция всего проекта
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Правила для компиляции объектных файлов
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Команда для очистки всех объектных файлов и исполняемого файла
clean:
	rm -f $(OBJ) $(TARGET)

# Файл не является файлом, а командой (чтобы Make не искал "clean" как файл)
.PHONY: clean
