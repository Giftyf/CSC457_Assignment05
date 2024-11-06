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


    ptrPCB RTprocesses= NULL;
    ptrPCB P1processes= NULL;
    ptrPCB P2processes= NULL;
    ptrPCB P3processes= NULL;
    ptrPCB Eprocesses= NULL;
    ptrPCB SBprocesses= NULL;
    FILE *cfPtr = NULL;
    
    // Try to open the file "processes.txt". If it can't be opened, print an error message and exit.
    if((cfPtr = fopen("processes.txt", "r")) == NULL) {
        puts("File could not be opened");
        return 1; // Exit with an error code
    }
    else{
        // Read the file line by line until the of the line
        while (!feof(cfPtr)) {
            int processId,userId,arrivalTime, priority, expectedTimeRemaining, expectedMemoryNeed, expectedPctCPU,realTime;
            //check if the line has 8 inputs
            if(fscanf(cfPtr, "%d%d%d%d%d%d%d%d",  &processId, &userId, &arrivalTime, &priority,
               &expectedTimeRemaining, &expectedMemoryNeed, &expectedPctCPU, &realTime)==8){
                //fseek(cfPtr, -sizeof(int)*8, SEEK_CUR);
                ptrPCB temp = readBlock(processId, userId, arrivalTime, priority, expectedTimeRemaining, expectedMemoryNeed, expectedPctCPU, realTime);
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
            else{
                break;
            }
        }
        //close the file 
        fclose(cfPtr);

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

        ptrPCB Rprocess = NULL;
        printf("Running: \n");

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

        puts("");


        int user_choice =0;

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
            if(user_choice==1){
                if(Rprocess!=NULL){
                    AddToEnd(&Eprocesses,Rprocess);
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
                if(Rprocess!=NULL){
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
                int priority = 0;
                printf("What Priority process would you like to see? (0-3): ");
                scanf("%d", &priority);
                switch (priority)
                {
                case 0:
                    printf("================= Real Time =================\n");
                    displayQueue(RTprocesses);
                    break;
                case 1:
                    displayQueue(P1processes);
                    printf("================= Priority 1 =================\n");
                    break;
                case 2: 
                    printf("================= Priority 2 =================\n");
                    displayQueue(P2processes);
                    break;
                case 3: 
                    printf("================= Priority 3 =================\n");
                    displayQueue(P3processes);
                    break;
            }
            }
            else if(user_choice==4){
                int Aprocess = 0;
                printf("What simulated arrival time would you like to see processes from? (currently none higher than 100): ");
                scanf("%d", &Aprocess);
                printf("================= standby processes with arrival time = %d =================\n", Aprocess);
                printf("---------------------- process control blocks ----------------------\n");
                displayQueueArr(SBprocesses,Aprocess);
            }
            else if(user_choice==5){
                int Tprocess = 0;
                printf("What time remaining would you like to see processes longer than? (currently low numbers): ");
                scanf("%d", &Tprocess);
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
                char ch;
                printf("Enter any key to finish: ");
                scanf(" %c", &ch);
                exit(0);
            }
        }
        return 0;
    }
    
    
}


//function to add to the end of the list


void AddToEnd(ptrPCB *headRef, ptrPCB tooAddPtr){

    if (tooAddPtr == NULL){ //if incoming pointer is null
        return;
    }

    tooAddPtr->nextPtr = NULL;

    if(*headRef ==NULL){ //assign the incoming pointer as the head
        *headRef = tooAddPtr;
        return;
    }
    ptrPCB current = *headRef; //go through to the end of the list
    while(current->nextPtr != NULL){
        current = current->nextPtr;
    }

    current->nextPtr = tooAddPtr; //set the last node to point to the new node
    //if the head is null assign the incoming pointer as the head else loop through the queue until the end or next pointer is null    
    // WHen you reach the end you change the last pointer's NEXT pointer to the incoming new pointer 
    // incoming pointer's NEXT is null  
}


//function to read a line and return a pointer to a newly allocated PCB


ptrPCB readBlock(int processId, int userId, int arrivalTime, int priority,
                 int expectedTimeRemaining, int expectedMemoryNeed, int expectedPctCPU, int realTime) {
    ptrPCB newPCB = malloc(sizeof(PCB));

    // Populate the PCB structure with the values passed from the main function
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



//function to remove from the front of the list
ptrPCB GetAndRemoveFront(ptrPCB *headRef){
    if(headRef == NULL){ //if list is empty
        return NULL;
    }
    ptrPCB front = *headRef; //get first node
    *headRef = front->nextPtr; //update head to point to next node
    front->nextPtr = NULL; //dissconnect from the list

    return front ; //do I return the removed node?

    //removing from the front 
}


void displayBlock(ptrPCB block){
    ptrPCB process = block;
    printf("ID: %d Usr: %d Arr: %d Pri: %d Remain: %d Mem: %d Cput: %d RT: %d\n",
        process->processId,process->userId,process->arrivalTime,process->priority, 
        process->expectedTimeRemaining,process->expectedMemoryNeed, process->expectedPctCPU,process->realTime);
    return;

}

void displayQueue(ptrPCB headRef){
    printf("---------------------- process control blocks ----------------------\n");
    if(headRef == NULL){ //if list is empty
        return;
    }
    else{
        ptrPCB block = headRef;
        while(block!=NULL){
            displayBlock(block);
            block = block->nextPtr;
        }
    }
}
void displayQueueTime(ptrPCB headRef, int pTime){
    int count = 0;
    ptrPCB curr = headRef;
    while(curr!=NULL){
        if(curr->arrivalTime ==pTime){
            count++;
            displayBlock(curr);
        }
    }
    printf("Number found: %d \n", count);
}

void displayQueueArr(ptrPCB headRef, int aTime) {
    int count = 0;
    ptrPCB curr = headRef;
    while (curr != NULL) {
        if (curr->arrivalTime == aTime) {
            count++;
            displayBlock(curr);  
        }
        curr = curr->nextPtr;
    }

    printf("Number found: %d \n", count);
}

//shift+alt+A
