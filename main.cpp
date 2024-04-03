/*********************
Name: James Castle
CS 7343 Programming Assignment 2 - Project 3
Purpose: Implementaation of the Dining Philosophers Problem (Project 3)
**********************/
#include "main.h"

enum philosopherStatus {thinking, hungry, eating};
// Setup a vector of 5 semaphores representing 5 forks
vector<binary_semaphore*> forks;
int NUMPHILOSOPHERS = 5;

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

bool Philosopher::bothForksAvailable(int philosopherId) {
    bool bothForksAvailable = false;
    bool rightAvailable = false;
    bool leftAvailable = forks[philosopherId]->try_acquire();
    if(leftAvailable){
        rightAvailable = forks[(philosopherId + 1) % NUMPHILOSOPHERS]->try_acquire();
    }
    if(leftAvailable && rightAvailable){
        bothForksAvailable = true;
    }
    return bothForksAvailable;
}

void Philosopher::pickup_forks(int philosopherId) {
    // Philosopher is hungry,
    this->status = hungry;
    // try to acquire the fork to the left and the right ONLY if BOTH AVAILABLE
    bool forksAvailable = this->bothForksAvailable(philosopherId);
    if(forksAvailable){
        this->forkInLeftHand = true;
        this->forkInRightHand = true;
        this->status = eating;
        cout << "Philosopher " << philosopherId << " is eating" << endl;
        this->coursesConsumed++;
        this->return_forks(philosopherId);
    }
    else{
        this->timeSinceLastEaten++;
    }
}

void Philosopher::return_forks(int philosopherId) {
    this->status = thinking;
    cout << "Philosopher " << philosopherId << " is thinking" << endl;
    forks[philosopherId]->release();
    forks[((philosopherId + 1) % NUMPHILOSOPHERS)]->release();
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
    int numCourses = 3;
    int numForks = 5;

    vector<Philosopher> philosophersTable;
    // Create 5 philosophers
    for(int i = 0; i < NUMPHILOSOPHERS; i++){
        Philosopher *philosopher = new Philosopher;
        philosopher->setId(i);
        philosophersTable.push_back(*philosopher);
    }

    // Create the 5 forks (of which, each philosopher will require 1 in each hand to eat)
    for (int i = 0; i < numForks; i++){
        binary_semaphore *fork = new binary_semaphore(1);
        forks.push_back(fork);
    }

    thread t1(&Philosopher::pickup_forks, philosophersTable[0], 0);
    thread t2(&Philosopher::pickup_forks, philosophersTable[1], 1);

    t1.join();
    t2.join();
    cout << "done" << endl;

    return 0;
}