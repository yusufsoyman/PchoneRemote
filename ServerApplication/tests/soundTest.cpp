/* 
 * File:   soundTest.cpp
 * Author: zgrw
 *
 * Created on Feb 23, 2015, 9:50:19 PM
 */

#include <stdlib.h>
#include <iostream>

using namespace std;

#include "curses.h"
#include "SoundController.h"
#include "Logger.h"
/*
 * Simple C++ Test Suite
 */


Logger *logger;

void test1() {
    SoundController sc;
    int in;
    initscr();
    noecho();
    char buffer[1024];
    while(1)
    {
        in = getch();
        sprintf(buffer, "Caught input %d", in);
        logger -> printDebugLog(buffer);
        if(in != 3 && in != ERR && in != 0)
        {
            if(in == 'q')
            {
                sc.increaseVolume();
            }
            else if (in == 'w')
            {
                sc.decreaseVolume();
            }
            else if (in == ' ')
            {
                sc.muteUnmute();
            }
            else if (in == 'x') //quit
            {
                break;
            }
            else
            {
                cout<<sc.getCurrentVolumeLevel()<<endl;
            }
        }
    }
}


int main(int argc, char** argv) {
    logger = Logger::getInstance();
    logger -> setLogConfig("./","test.log", Logger::DEBUG, true);
    std::cout << "%SUITE_STARTING% soundTest" << std::endl;
    logger -> printDebugLog("%SUITE_STARTING% soundTest");
    std::cout << "%SUITE_STARTED%" << std::endl;
    logger -> printDebugLog("%SUITE_STARTED");

    std::cout << "%TEST_STARTED% test1 (soundTest)" << std::endl;
    logger -> printDebugLog("%TEST_STARTED% test1 (soundTest)");
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (soundTest)" << std::endl;
    logger -> printDebugLog("%TEST_FINISHED% time=0 test1 (soundTest)");

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
    logger -> printDebugLog("%SUITE_FINISHED% time=0");
    //no need to call logger -> finalize(); since it will automatically be destroyed :)
    return (EXIT_SUCCESS);
}

