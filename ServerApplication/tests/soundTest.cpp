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
/*
 * Simple C++ Test Suite
 */

void test1() {
    SoundController sc;
    int in;
    initscr();
    noecho();
    while(1)
    {
        in = getch();
        if(in != 3 && in != ERR && in != 0)
        {
            if(in == 'q')
                sc.increaseVolume();
            else if (in == 'w')
                sc.decreaseVolume();
            else if (in == ' ')
                sc.muteUnmute();
            else
            {
                cout<<sc.getCurrentVolumeLevel()<<endl;
            }
        }
    }
}


int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% soundTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (soundTest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (soundTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

