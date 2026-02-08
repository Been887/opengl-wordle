#pragma once

#include <iostream>
#include <string.h>
#include <ctime>

extern int gameIteration;

std::string answers[6];
std::string words[20] {//20 5 letter words
    "ODIUM", "HELLO", "AISLE", "SHIPS", "EATEN", "JELLY", "CIVIL", "CLAIM", "EXACT", "DOZEN", "VAPID", "ASSAY", "PIQUE", "COWER", "JUMPY", "BRICK", "DRAPE", "OASIS", "ANGEL", "MOTOR"
};
int word;

enum letterColours {
    grey, yellow, green
};
letterColours letters[5][6];

void setArrays() {
    for(int j= 0;j < 6; j++) {//letters 2d array
        for(int i= 0; i<5; i++) {
            letters[i][j] = grey;
        }
    }

    for(int i = 0; i<6; i++) {
        answers[i] = "";
    }
}

bool charSearch(char needle, std::string haystack) {
    for(int i = 0; i<haystack.length(); i++) {
        if(needle == haystack.at(i)) return true;
    }

    return false;
}

int randomNumber(int lowest, int largest) {
    std::srand(std::time(0));
    return lowest+(rand() % (largest-lowest+1));
}
