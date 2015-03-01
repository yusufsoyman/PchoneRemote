#This is a very basic make file generated for Linux only

# Environment 
CXX=g++
CC=g++
CFLAGS=-std=c++11
LDFLAGS=-shared -fPIC
RM=rm
BINS=pchoneremote


# clean
clean:
#$(RM) $(LIBS) $(BINS)

# all
all: libraries binaries

libraries:
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ ./include/Logger/Logger.cpp -o libzlogger.so
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/  -I./include/Network\ Handler/ ./include/Network\ Handler/NetworkHandler.cpp -L./ -o libznetwork.so -lzlogger -lpthread
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/SoundController/ ./include/SoundController/SoundController.cpp -L./ -o libzsound.so -lzlogger -lasound
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/Database\ Connections/ ./include/Database\ Connections/DBAdapter.cpp -L./ -o libzdbconnector.so -lmysqlclient -lsqlite3


binaries:
	$(CXX) $(CFLAGS) PchoneRemote.cpp -L./ -I./include/Logger/ -I./include/SoundController/ -I./include/Network\ Handler/ -I./include/Database\ Connections/ -lzlogger -lzsound -lznetwork -lzdbconnector -o $(BINS)