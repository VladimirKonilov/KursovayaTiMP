CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/include/UnitTest++
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lUnitTest++

TARGET = client_tests
SOURCES = client_tests.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
