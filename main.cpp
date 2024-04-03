/*********************
Name: James Castle
CS 7343 Programming Assignment 2 - Project 3
Purpose: Implementaation of the Dining Philosophers Problem (Project 3)
**********************/
#include "main.h"

// Setup a vector of 5 semaphores representing 5 forks

Philosopher::Philosopher() {
    philosopherId = -1;
    status = 0; // 0 = Thinking; 1 = Hungry; 2 = Eating
    forkInLeftHand = false;
    forkInRightHand = false;
    timeSinceLastEaten = 0;
    coursesConsumed = 0;
}

int Philosopher::getId() {
    return this->philosopherId;
}

bool Philosopher::setId(int id) {
    bool success = false;
    this->philosopherId = id;
    if(this->philosopherId == id){
        success = true;
    }
    return success;
}

int randomRangeGen(int endRange, int startRange = 0, unsigned int seed = 42) {
    // General implementation borrowed from:
    // https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus
    int random;

    // Random pathway
    if (seed == 42) {
        random = startRange + (rand() % ((endRange - startRange) + 1));
    }
        // Set seed pathway
    else {
        // Modified with ChatGPT to take in a seed.
        // Initialize the random number generator with the provided seed
        std::mt19937 gen(seed);
        // Retrieve a random number between startRange and EndRange
        random = startRange + (gen() % ((endRange - startRange) + 1));
    }
    return random;
}

int main(){
    int numPhilosophers = 5;

    vector<Philosopher> philosophersTable;
    // Create 5 philosophers

    for(int i = 0; i < numPhilosophers; i++){
        Philosopher philosopher;
        philosopher.setId(i);
        philosophersTable.push_back(philosopher);
    }
    // Assumes 5 philosophers with 5 forks, and 2 forks required to eat (instead of chopsticks)

    cout << "done" << endl;

    return 0;
}