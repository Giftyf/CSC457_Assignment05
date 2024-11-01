#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>

typedef enum States {NEW} State;
const char *stateNames[1] = {"New"};

// A simplified process control block used for this assignment purpose.
struct processblock {
    int processId;
    int userId;             
    State processState;       // Not input - initially NEW
    int arrivalTime;          // time units in simulation, not actual time
    int priority;             // base priority
    int expectedTimeRemaining;
    int expectedMemoryNeed;
    int expectedPctCPU;       // to get an idea of whether CPU bound or IO bound
    bool realTime;            // 0 or 1 - whether a real-time process or not (real-time processes may need immediate attention)
	struct processblock *nextPtr; // pointer to next processblock
};

// Add your code below
//David Johnson

