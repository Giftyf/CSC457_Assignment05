#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>



/*
*Team Memebers: Nigist Legesse(Legessen1@lasalle.edu) and David Johnson(Johnsond55@lasalle.edu)
*Date: November 6, 2024
*Purpose: Given a file (where each line contains the properties of a process), this program creates 
    six queues and places each process in its respective queue. The placement depends on whether the 
    process has arrived and, if it has, whether it is a real-time process. For non-real-time processes, 
    the priority level determines the appropriate queue. The program then provides the user with options 
    to either finish the running process or manage the process if its time is up. FIFO queues are used 
    to implement this scheduling system.The program also provide the option to view the queues.

*/

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


typedef struct processblock PCB;
typedef PCB* ptrPCB;

//function header
void displayQueueArr(ptrPCB headRef, int aTime);
void displayQueueTime(ptrPCB headRef, int pTime);
void displayQueue(ptrPCB headRef);
void displayBlock(ptrPCB block);
void AddToEnd(ptrPCB *headRef, ptrPCB tooAddPtr);
ptrPCB GetAndRemoveFront(ptrPCB *headRef);
ptrPCB readBlock(int processId, int userId, int arrivalTime, int priority,
                 int expectedTimeRemaining, int expectedMemoryNeed, int expectedPctCPU, int realTime);


int main(void){

    //declare and initialize queues
    ptrPCB RTprocesses= NULL; //queue for Real time processes
    ptrPCB P1processes= NULL; //queue for processes with priority  1
    ptrPCB P2processes= NULL; //queue for processes with priority  2 
    ptrPCB P3processes= NULL; //queue for processes with priority  3
    ptrPCB Eprocesses= NULL; //queue for finished processes
    ptrPCB SBprocesses= NULL; //queue for processes that have not arrived yet, Standby processes
    
    FILE *cfPtr = NULL;
    
    // Try to open the file "processes.txt". If it can't be opened, print an error message and exit.
    if((cfPtr = fopen("processes.txt", "r")) == NULL) {
        puts("File could not be opened");
        return 1; // Exit with an error code
    }
    else{
        // Read the file line by line until the of the line
        while (!feof(cfPtr)) {
            //temprory varialbes to store the properties of a block
            int processId,userId,arrivalTime, priority, expectedTimeRemaining, expectedMemoryNeed, expectedPctCPU,realTime;

            //check if the line has 8 inputs while reading into the varialbes
            if(fscanf(cfPtr, "%d%d%d%d%d%d%d%d",  &processId, &userId, &arrivalTime, &priority,
               &expectedTimeRemaining, &expectedMemoryNeed, &expectedPctCPU, &realTime)==8){

                //call the readBlock function to read the variables and build them into PCB and returns a pointer to the PCB
                ptrPCB temp = readBlock(processId, userId, arrivalTime, priority, expectedTimeRemaining, expectedMemoryNeed, expectedPctCPU, realTime);
                
                //check if the process has arrived push it to its respective queue based on its priority and RealTime
                if(temp->arrivalTime==0){
                    if(temp->realTime==1){
                        AddToEnd(&RTprocesses,temp);
                    }
                    else if(temp->priority==1){
                        AddToEnd(&P1processes, temp);
                    }
                    else if(temp->priority==2){
                        AddToEnd(&P2processes,temp);
                    }
                    else if(temp->priority==3){
                        AddToEnd(&P3processes, temp);
                    }
                }
                else 
                {
                    AddToEnd(&SBprocesses, temp);
                }
            }
            //break out of the while loop if we don't have 8 inputs per line
            else{
                break;
            }
        }
        //close the file 
        fclose(cfPtr);

        //printing the queues
        printf("Authors: Nigist Legesse & David Johnson");
        printf("================= Initial Simplified Beginnings of Scheduling Program =================\n");
        printf("                  On Hold Waiting for Arrival\n");
        displayQueue(SBprocesses);
        printf("================= Real Time =================\n");
        displayQueue(RTprocesses);
        printf("================= Priority 1 =================\n");
        displayQueue(P1processes);
        printf("================= Priority 2 =================\n");
        displayQueue(P2processes);
        printf("================= Priority 3 =================\n");
        displayQueue(P3processes);

        puts("");

        //This code enable us as continue executing the code after the user inserts a character
        char ch;
        printf("Enter any key to continue: ");
        scanf(" %c", &ch);
        puts("");

        //initialize the running process to NULL
        ptrPCB Rprocess = NULL;
        printf("Running: \n");

        //pick a running process( We pick them based on their priority, Real time processes have high priority->priority=1->priority=2->priority=3)
        if (RTprocesses != NULL) {
            Rprocess = GetAndRemoveFront(&RTprocesses);
        } 
        else if (P1processes != NULL) {
            Rprocess = GetAndRemoveFront(&P1processes);
        } 
        else if (P2processes != NULL) {
            Rprocess = GetAndRemoveFront(&P2processes);
        } 
        else if (P3processes != NULL) {
            Rprocess = GetAndRemoveFront(&P3processes);
        }

        //Once we go through all Queues that have arrived we check if we have a running process
        if (Rprocess != NULL) {
            //display the process
            displayBlock(Rprocess);
        } else {
            //no runnuning process found
            printf("No process to run.\n");
        } 

        puts("");


        int user_choice =0;
        //a loop to to ask users for choice until the user inserts 7 to exit of the code
        while (user_choice != 7){
            printf("choose (by number) one of the below actions to do\n");
            printf("> 1 Running Process Finishes _ \n");
            printf("> 2 Running Process time is up for now _ \n");
            printf("> 3 See Processes for Priority (0-3 (zero is real time)) _ \n");
            printf("> 4 See Arrival for time _ \n");
            printf("> 5 See Long process more time remaining than _ \n");
            printf("> 6 See All Queues \n");
            printf("> 7 EXIT \n");
            printf("Your choice: ");
            scanf("%d", &user_choice);
            //print new line
            puts("");
            //handle user choice
            if(user_choice==1){
                printf("Running: \n");
                //check if there is a running process
                if(Rprocess!=NULL){
                    //push the running process to finished process(Exited processes)
                    AddToEnd(&Eprocesses,Rprocess);

                    //pick the next running process
                    if (RTprocesses != NULL) {
                        Rprocess = GetAndRemoveFront(&RTprocesses);
                    } 
                    else if (P1processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P1processes);
                    } 
                    else if (P2processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P2processes);
                    } 
                    else if (P3processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P3processes);
                    }

                    if (Rprocess != NULL) {
                        displayBlock(Rprocess);
                    } else {
                        printf("No process to run.\n");
                    } 

                }
                else{
                    puts("No Running process.");
                }

            }
            else if(user_choice==2){
                printf("Running: \n");
                //push the running process back to it's respective queue since the running time is up
                if(Rprocess!=NULL){
                    //Check for it's property(realtime and priority) to identify which queue it belongs to
                    if(Rprocess->realTime==1){
                        AddToEnd(&RTprocesses, Rprocess);
                    }
                    else if(Rprocess->priority==1){
                        AddToEnd(&P1processes, Rprocess);
                    }
                    else if(Rprocess->priority==2){
                        AddToEnd(&P2processes, Rprocess);
                    }
                    else if(Rprocess->priority==3){
                        AddToEnd(&P3processes, Rprocess);
                    }

                    //Pick the next running process
                    if (RTprocesses != NULL) {
                        Rprocess = GetAndRemoveFront(&RTprocesses);
                    } 
                    else if (P1processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P1processes);
                    } 
                    else if (P2processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P2processes);
                    } 
                    else if (P3processes != NULL) {
                        Rprocess = GetAndRemoveFront(&P3processes);
                    }

                    if (Rprocess != NULL) {
                        displayBlock(Rprocess);
                    } 
                    else {
                        printf("No process to run.\n");
                    } 

                }
                else{
                    puts("No Running process.");
                }
            }
            else if(user_choice==3){
                int priority = -1;
                //Ask user to enter which queue they would like to see
                puts("What Priority process would you like to see? (0-3): ");
                printf("Please enter a value between 0 and 3 ==> ");
                //read user input
                scanf("%d", &priority);

                //handle invalid user inputs
                while(priority<=0 && priority>=3){
                    printf("Invalid Value!! Please enter a value between 0 and 3 ==> ");
                    scanf("%d", &priority); 
                }

                //Switch based on user input
                switch (priority)
                {
                case 0:
                    //display real time process
                    printf("================= Real Time =================\n");
                    displayQueue(RTprocesses);
                    break;
                case 1:
                    //display process with priority 1
                    displayQueue(P1processes);
                    printf("================= Priority 1 =================\n");
                    break;
                case 2: 
                    //display process with priority 2
                    printf("================= Priority 2 =================\n");
                    displayQueue(P2processes);
                    break;
                case 3: 
                    //display process with priority 3
                    printf("================= Priority 3 =================\n");
                    displayQueue(P3processes);
                    break;
            }
            }
            else if(user_choice==4){
                int Aprocess = 0;
                //prompt user for arrival time 
                printf("What simulated arrival time would you like to see processes from? (currently none higher than 100): ");
                //read user input
                scanf("%d", &Aprocess);
                printf("================= standby processes with arrival time = %d =================\n", Aprocess);
                printf("---------------------- process control blocks ----------------------\n");
                //call a helper function to display standby processes with given arrival time
                displayQueueArr(SBprocesses,Aprocess);
            }
            else if(user_choice==5){
                int Tprocess = 0;
                //prompt user to enter arrival time
                printf("What time remaining would you like to see processes longer than? (currently low numbers): ");
                //read user input
                scanf("%d", &Tprocess);

                //call a helper function to display all the queues that have arrived and who have longer remaining time than specified
                printf("================= Real Time =================\n");
                displayQueueTime(RTprocesses,Tprocess);
                printf("================= Priority 1 =================\n");
                displayQueueTime(P1processes,Tprocess);
                printf("================= Priority 2 =================\n");
                displayQueueTime(P2processes,Tprocess);
                printf("================= Priority 3 =================\n");
                displayQueueTime(P3processes,Tprocess);  
            }
            else if(user_choice==6){
                //display all the process in the queues except for standby processes
                printf("================= Real Time =================\n");
                displayQueue(RTprocesses);
                printf("================= Priority 1 =================\n");
                displayQueue(P1processes);
                printf("================= Priority 2 =================\n");
                displayQueue(P2processes);
                printf("================= Priority 3 =================\n");
                displayQueue(P3processes);
                printf("================= Finished =================\n");
                displayQueue(Eprocesses);

            }
            else if(user_choice==7){
                //Exit out of the function
                char ch;
                printf("Enter any key to finish: ");
                scanf(" %c", &ch);
                exit(0);
            }
            puts("");
        }
        return 0;
    }
    
    
}



//function for pushing a PCB pointer to the end of a queue
void AddToEnd(ptrPCB *headRef, ptrPCB tooAddPtr){

    if (tooAddPtr == NULL){ //if incoming pointer is null
        return;
    }
    //make sure the next pointer of the tooAddPtr is null
    tooAddPtr->nextPtr = NULL;

    if(*headRef ==NULL){ //if the queue is empty assign the incoming pointer as the head
        *headRef = tooAddPtr;
        return;
    }
    ptrPCB current = *headRef; //go through to the end of the list
    while(current->nextPtr != NULL){
        current = current->nextPtr;
    }

    current->nextPtr = tooAddPtr; //set the last node to point to the new node
}


//function to return a pointer to newly allocated PCB
ptrPCB readBlock(int processId, int userId, int arrivalTime, int priority,
                 int expectedTimeRemaining, int expectedMemoryNeed, int expectedPctCPU, int realTime) {
    ptrPCB newPCB = malloc(sizeof(PCB));

    //read into PCB with the values passed function
    newPCB->processId = processId;
    newPCB->userId = userId;
    newPCB->arrivalTime = arrivalTime;
    newPCB->priority = priority;
    newPCB->expectedTimeRemaining = expectedTimeRemaining;
    newPCB->expectedMemoryNeed = expectedMemoryNeed;
    newPCB->expectedPctCPU = expectedPctCPU;
    newPCB->realTime = realTime;
    newPCB->nextPtr = NULL;

    return newPCB;
}



//function to pop from the front of a queue
ptrPCB GetAndRemoveFront(ptrPCB *headRef){
    if(headRef == NULL){ //if list is empty
        return NULL;
    }
    ptrPCB front = *headRef; //get first node
    *headRef = front->nextPtr; //update head to point to next node
    front->nextPtr = NULL; //dissconnect from the list

    return front ; //return the removed node

}


//function to display the details of a given PCB
//parameter are passed by name
void displayBlock(ptrPCB block){
    ptrPCB process = block;
    printf("Id %-4d Usr: %-3d Arr: %-2d Pri: %-2d Remain: %-3d Mem: %-5d CPU: %-3d\n",
        process->processId,process->userId,process->arrivalTime,process->priority, 
        process->expectedTimeRemaining,process->expectedMemoryNeed, process->expectedPctCPU);
    return;

}

//function to display all the process blocks withing a given queue
//the name of the queue is passed to the function
void displayQueue(ptrPCB headRef){
    printf("---------------------- process control blocks ----------------------\n");
    if(headRef == NULL){ //if list is empty
        return;
    }
    else{
        ptrPCB block = headRef;
        while(block!=NULL){
            //iterating through the queue and calling displayblock to display each process
            displayBlock(block);
            block = block->nextPtr;
        }
    }
}

//function to display process control block with remaining time greater than a specified value
void displayQueueTime(ptrPCB headRef, int pTime){
    
    int count = 0; //variable to count how many processes match the condition
    ptrPCB curr = headRef; //pointer to go through the list starting from the head of the given queue
    while(curr!=NULL){
        //iterate through the given queue and display the blocks/ the proceses that have a remaining time specified by the user
        if(curr->expectedTimeRemaining >pTime){
            count++;
            displayBlock(curr);
        }
        curr = curr ->nextPtr;
    }
    //print the number of process found
    printf("Number found: %d \n", count);
}


//display process control blocks that have a specified arrival time
void displayQueueArr(ptrPCB headRef, int aTime) {
    int count = 0; //variable to count how many process match the condition
    ptrPCB curr = headRef; //pointer to go trhough the given queue starting from the head
    while (curr != NULL) {
        //traverse through the queue and display the blocks/ the processes that have the specified arrival time
        if (curr->arrivalTime == aTime) {
            count++;
            displayBlock(curr);  
        }
        curr = curr->nextPtr;
    }
    //print the total number of processes found
    printf("Number found: %d \n", count);
}

