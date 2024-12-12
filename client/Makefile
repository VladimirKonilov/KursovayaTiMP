CXX = g++
CXXFLAGS = -Wall -std=c++17

all: client

client: main.o Communicator.o UserInterface.o DataReader.o DataWriter.o 
	$(CXX) $(CXXFLAGS) -o client main.o Communicator.o UserInterface.o DataReader.o DataWriter.o -lcryptopp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o client
