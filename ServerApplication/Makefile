#This is a very basic make file generated for Linux only

# Environment 
CXX=g++
CC=g++
CFLAGS=-std=c++11
LDFLAGS=-shared -fPIC
RM=rm
BINS=pchoneremote


# all
all: libraries binaries

# clean
clean:
$(RM) $(BINS) libznetwork.so libzsound.so libzdbconnector.so libzconfig.so libzserver.so libzlogger.so



libraries:
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ ./include/Logger/Logger.cpp -o libzlogger.so
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/Network\ Handler/ ./include/Network\ Handler/NetworkHandler.cpp -L./ -o libznetwork.so -lzlogger -lpthread
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/SoundController/ ./include/SoundController/SoundController.cpp -L./ -o libzsound.so -lzlogger -lasound
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/Database\ Connections/ ./include/Database\ Connections/DBAdapter.cpp -L./ -o libzdbconnector.so -lmysqlclient -lsqlite3 -lzlogger
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/ConfigReader/ ./include/ConfigReader/ConfigReader.cpp  -L./ -o libzconfig.so -lzlogger
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/ConfigReader/ -I./include/ServerMain/ -I./include/Network\ Handler/ -I./include/SoundController/ ./include/ServerMain/ServerMain.cpp -L./ -o libzserver.so -lzlogger -lznetwork -lzsound



binaries:
	$(CXX) $(CFLAGS) main.cpp -L./ -I./include/Logger/ -I./include/SoundController/ -I./include/Network\ Handler/ -I./include/Database\ Connections/ -I./include/ConfigReader/ -I./include/ServerMain/ -lzlogger -lzsound -lznetwork -lzdbconnector -lzserver -lzconfig -o $(BINS)