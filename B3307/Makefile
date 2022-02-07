CXX = g++
CXXFLAGS = -std=c++11
SOURCES = src/AnaLog.cpp src/LogReader.cpp 
OBJECTS = $(subst .cpp,.o, $(SOURCES))
EXEC = bin/analog

all: $(EXEC)

debug: CXXFLAGS += -DMAP
debug: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) -o $(EXEC) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

setup:
	mkdir -pv bin/
clean:
	rm -f $(OBJECTS) $(EXEC)
