CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lcryptopp

TARGET = client
SOURCES = main.cpp UserInterface.cpp Communicator.cpp DataReader.cpp DataWriter.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
