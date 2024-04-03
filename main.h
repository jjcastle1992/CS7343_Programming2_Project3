/*********************
Name: James Castle
CS 7343 Programming Assignment 2 - Project 3
Purpose: Interface for the Dining Philosophers Problem (Project 3)
**********************/
#ifndef PROJECT3_MAIN_H
#define PROJECT3_MAIN_H

#include <random>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore>

using namespace std;

class Philosopher {
public:
    // Constructor
    Philosopher();

    // Getters
    int getId();

    // Setters
    bool setId(int);

    // Utilities
    bool bothForksAvailable(int); // checks to see if both forks available
    void pickup_forks(int); // i.e. wish to eat. Sleep between 1 - 3 seconds
    void return_forks(int); // i.e. done eating. Sleep between 1 - 3 seconds


private:
    int philosopherId;
    int status; // 0 = Thinking; 1 = Hungry; 2 = Eating
    bool forkInLeftHand;
    bool forkInRightHand;
    int timeSinceLastEaten;
    int coursesConsumed;
};

int randomRangeGen(int, int, unsigned int);

#endif //PROJECT3_MAIN_H
