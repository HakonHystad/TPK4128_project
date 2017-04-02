#makefile for mifareCrack
exe=TPK4128
C=gcc
CXX = g++
CXXFLAGS = --std=c++11
objects=build/main.o build/RC522.o build/MFrec.o build/crapto1.o build/crypto1.o build/httpPostMaker.o build/serverClientCom.o build/standardsocket.o build/hdmiI2Cdriver.o build/hdmiLCD.o build/i2c_LCD.o

exe: $(objects)
	g++ $(objects) -o $(exe) -lpthread
build/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

build/%.o: src/LCD/%.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

build/%.o: src/RFID/%.cpp 
	$(CXX) -c $(CXXFLAGS) $^ -o $@

build/%.o: src/RFID/%.c
	$(C) -c $^ -o $@

build/%.o: src/network/%.cpp 
	$(CXX) -c $(CXXFLAGS) $^ -o $@

clean:
	rm $(exe) $(objects)
