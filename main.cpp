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
    else {
        // release the left fork
        forks[philosopherId]->release();
    }
    return bothForksAvailable;
}

void Philosopher::pickup_forks(int philosopherId) {

    // try to acquire the fork to the left and the right ONLY if BOTH AVAILABLE
    bool forksAvailable = this->bothForksAvailable(philosopherId);

    // Critical Section
    if(forksAvailable){
        this->forkInLeftHand = true;
        this->forkInRightHand = true;
        this->status = eating;
        string message = "Philosopher " + to_string(philosopherId) + " is eating course " +
                        to_string(this->coursesConsumed + 1);
        this->atomPrint(&message);
        this->coursesConsumed++;
        // spend between 1 and 3 seconds eating
        int eatingTime = randomRangeGen(3, 1, 42);
        this_thread::sleep_for(chrono::seconds(eatingTime));
        this->return_forks(philosopherId);
    }
}

void Philosopher::return_forks(int philosopherId) {
    this->status = thinking;
    string message = "Philosopher " + to_string(philosopherId) + " is returning forks";
    atomPrint(&message);
    forks[philosopherId]->release();
    forks[((philosopherId + 1) % NUMPHILOSOPHERS)]->release();
}

void Philosopher::philoSim(Philosopher *currentPhilo, int numCourses) {
    while(currentPhilo->coursesConsumed < numCourses) {
        int thinkTime = randomRangeGen(3, 1, 42);
        currentPhilo->status = thinking;
        // Have the Philosopher think for (1 - 3 seconds)
        string message = "Philosopher " + to_string(currentPhilo->philosopherId) + " Thinking for " +
                          to_string(thinkTime) + " seconds";
        atomPrint(&message);
        this_thread::sleep_for(chrono::seconds(thinkTime));

        // Get Hungry
        currentPhilo->status = hungry;
        // Try to pickup forks, while hungry
        int attempt = 0;
        while(currentPhilo->status == hungry){
            string message = "Philo " + to_string(currentPhilo->philosopherId) + " trying to pickup forks (Attempt "
                             + to_string(attempt) + ")";
            atomPrint(&message);

            pickup_forks(currentPhilo->philosopherId);

            attempt++;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    string message = "Philosopher " + to_string(currentPhilo->philosopherId)  + " done eating all " +
                      to_string(numCourses) + " courses";
    atomPrint(&message);
}

void Philosopher::atomPrint(std::string *message) {
    // Modified with ChatGPT to atomically print
    lock_guard<mutex> lock(outputMutex); // Lock the mutex
    cout << *message << endl; // Print the message
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

    vector<Philosopher*> philosophersTable;
    // Create 5 philosophers
    for(int i = 0; i < NUMPHILOSOPHERS; i++){
        auto *philosopher = new Philosopher;
        philosopher->setId(i);
        philosophersTable.push_back(philosopher);
    }

    // Create the 5 forks (of which, each philosopher will require 1 in each hand to eat)
    for (int i = 0; i < numForks; i++){
        binary_semaphore *fork = new binary_semaphore(1);
        forks.push_back(fork);
    }


    thread philosopher0(&Philosopher::philoSim, philosophersTable[0], philosophersTable[0], numCourses);
    thread philosopher1(&Philosopher::philoSim, philosophersTable[1], philosophersTable[1], numCourses);
    thread philosopher2(&Philosopher::philoSim, philosophersTable[2], philosophersTable[2], numCourses);
    thread philosopher3(&Philosopher::philoSim, philosophersTable[3], philosophersTable[3], numCourses);
    thread philosopher4(&Philosopher::philoSim, philosophersTable[4], philosophersTable[4], numCourses);

    philosopher0.join();
    philosopher1.join();
    philosopher2.join();
    philosopher3.join();
    philosopher4.join();

    cout << "done" << endl;

    return 0;
}