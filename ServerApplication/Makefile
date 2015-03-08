#This is a very basic make file generated for Linux only

# Environment 
CXX=g++
CC=g++
CFLAGS=-std=c++11
LDFLAGS=-shared -fPIC
RM=rm
CP=cp
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
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/XmlParser/ ./include/XmlParser/XmlParser.cpp -L./ -lzlogger -o libzxmlparser.so
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/ConfigReader/ -I./include/XmlParser/ ./include/ConfigReader/ConfigReader.cpp  -L./ -o libzconfig.so -lzlogger -lzxmlparser
	$(CXX) $(LDFLAGS) $(CFLAGS) -I./include/Logger/ -I./include/ConfigReader/ -I./include/ServerMain/ -I./include/Network\ Handler/ -I./include/SoundController/ ./include/ServerMain/ServerMain.cpp -L./ -o libzserver.so -lzlogger -lznetwork -lzsound -lzxmlparser



binaries:
	$(CXX) $(CFLAGS) main.cpp -L./ -I./include/Logger/ -I./include/SoundController/ -I./include/Network\ Handler/ -I./include/Database\ Connections/ -I./include/ConfigReader/ -I./include/ServerMain/ -lzlogger -lzsound -lznetwork -lzserver -lzconfig -lzxmlparser -o $(BINS)
	
install:
	$(CP) *so /usr/lib/
	$(CP) $(BINS) /usr/bin
	
uninstall:
	$(RM) /usr/bin/$(BINS) /usr/lib/libznetwork.so /usr/lib/libzserver.so /usr/lib/libzconfig.so /usr/lib/libzxmlparser.so /usr/lib/libzsound.so /usr/lib/libzlogger.so