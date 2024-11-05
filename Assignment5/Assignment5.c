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
void displayQueueArr(ptrPCB *headRef, int aTime);
void displayQueueTime(ptrPCB *headRef, int pTime);
void displayQueue(ptrPCB *headRef);
void displayBlock(ptrPCB *block);
ptrPCB GetAndRemoveFront(ptrPCB *headRef);
ptrPCB readBlock(FILE *fptr);
void AddToEnd(ptrPCB *headRef, ptrPCB tooAddPtr);

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
            //check if the line has 8 inputs
            if(fscanf(cfPtr, "%d%d%d%d%d%d%d%d")==8){
                fseek(cfPtr, -sizeof(int)*8, SEEK_CUR);
                ptrPCB temp = readBlock(cfPtr);
                PCB process1=*temp;
                if(process1.arrivalTime==0){
                    if(process1.realTime==1){
                        AddToEnd(RTprocesses,&process1);
                    }
                    else if(process1.priority==1){
                        AddToEnd(P1processes, &process1);
                    }
                    else if(process1.priority==2){
                        AddToEnd(P2processes,&process1);
                    }
                    else if(process1.priority==3){
                        AddToEnd(P3processes, &process1);
                    }
                }
                else AddtoEnd(SBprocesses, &process1);
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
        printf("================= Real Time =================");
        displayQueue(RTprocesses);
        printf("================= Priority 1 =================");
        displayQueue(P1processes);
        printf("================= Priority 2 =================");
        displayQueue(P2processes);
        printf("================= Priority 3 =================");
        displayQueue(P3processes);

        ptrPCB Rprocess= NULL;
        if(RTprocesses != NULL){
            Rprocess = GetAndRemoveFront(RTprocesses);
            printf("Running: \n");
            displayBlock(Rprocess);
        }
        else if(P1processes != NULL){
            Rprocess = GetAndRemoveFront(P1processes);
            printf("Running: \n");
            displayBlock(Rprocess);
        }
        else if(P2processes != NULL){
            Rprocess = GetAndRemoveFront(P2processes);
            printf("Running: \n");
            displayBlock(Rprocess);
        }
        else if(P3processes != NULL){
            Rprocess = GetAndRemoveFront(P3processes);
            printf("Running: \n");
            displayBlock(Rprocess);
        }      
        else{
            Rprocess = NULL;
            printf("Running: \n");
            printf("No process to run.");
        }  
        //This code enable us as continue executing the code after the user inserts a character
        char ch;
        printf("Enter any key to continue: ");
        scanf(" %c", &ch);
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

            }
            else if(user_choice==2){

            }
            else if(user_choice==3){
                int priority = 0;
                printf("What Priority process would you like to see? (0-3): ");
                scanf("%d", priority);
                switch (priority)
                {
                case 0:
                    printf("================= Real Time =================");
                    displayQueue(RTprocesses);
                    break;
                case 1:
                    displayQueue(P1processes);
                    printf("================= Priority 1 =================");
                    break;
                case 2: 
                    printf("================= Priority 2 =================");
                    displayQueue(P2processes);
                    break;
                case 3: 
                    printf("================= Priority 3 =================");
                    displayQueue(P3processes);
                    break;
            }
            }
            else if(user_choice==4){
                int Aprocess = 0;
                Printf("What simulated arrival time would you like to see processes from? (currently none higher than 100): ");
                scanf("%d", Aprocess);
                printf("================= standby processes with arrival time = %d =================", Aprocess);
                printf("---------------------- process control blocks ----------------------\n");
                displayQueueArr(RTprocesses,Aprocess);
            }
            else if(user_choice==5){
                int Tprocess = 0;
                Printf("What time rmaining would you like to see processes longer than? (currently low numbers): ");
                scanf("%d", Tprocess);
                printf("================= Real Time =================");
                displayQueueTime(RTprocesses,Tprocess);
                printf("================= Priority 1 =================");
                displayQueueTime(P1processes,Tprocess);
                printf("================= Priority 2 =================");
                displayQueueTime(P2processes,Tprocess);
                printf("================= Priority 3 =================");
                displayQueueTime(P3processes,Tprocess);
                printf("================= Finished =================");
                displayQueueTime(Eprocesses,Tprocess);
                
            }
            else if(user_choice==6){
                printf("================= Real Time =================");
                displayQueue(RTprocesses);
                printf("================= Priority 1 =================");
                displayQueue(P1processes);
                printf("================= Priority 2 =================");
                displayQueue(P2processes);
                printf("================= Priority 3 =================");
                displayQueue(P3processes);
                printf("================= Finished =================");
                displayQueue(Eprocesses);

            }
            else if(user_choice==7){
                char ch;
                printf("Enter any key to finish: ");
                scanf(" %c", &ch);
                exit(0);
            }
        }
    }
    
    return 0;
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


ptrPCB readBlock(FILE *fptr){
    PCB *block=(ptrPCB)malloc(sizeof(PCB));
    fscanf(fptr, "%d%d%d%d%d%d%d%d",  &block->processId,
               &block->userId,
               &block->arrivalTime,
               &block->priority,
               &block->expectedTimeRemaining,
               &block->expectedMemoryNeed,
               &block->expectedPctCPU,
               &block->realTime);
    block->processState = NEW;
    block->nextPtr =NULL;
    return &block;
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


void displayBlock(ptrPCB *block){
    PCB *process = block;
    printf("ID: %d Usr: %d St: %s Arr: %d Pri: %d Remain: %d Mem: %d Cput: %d RT: %d\n",
        process->processId,process->userId,process->processState,process->arrivalTime,process->priority, 
        process->expectedTimeRemaining,process->expectedMemoryNeed, process->expectedPctCPU,process->realTime);
    return;

}

void displayQueue(ptrPCB *headRef){
    printf("---------------------- process control blocks ----------------------\n");
    if(headRef == NULL){ //if list is empty
        return;
    }
    else{
        PCB *block = headRef;
        while(block->nextPtr!=NULL){
            displayBlock(block);
            block = block->nextPtr;
        }
    }
}
void displayQueueTime(ptrPCB *headRef, int pTime){
    int count = 0;
    if(headRef != NULL){ //if list is empty
        ptrPCB curr = headRef;
        do{
            PCB *process = curr;
            if(process->expectedTimeRemaining>pTime){
                displayBlock(&process);
                count++;
            }
            curr = curr->nextPtr;

        }while(curr->nextPtr==NULL);
        
    }
    prinft("Number found: %d", count);
}

void displayQueueArr(ptrPCB *headRef, int aTime){
    int count = 0;
    if(headRef !=NULL){
        ptrPCB curr = headRef;
        do{
            PCB *process = curr;
            if(process->arrivalTime==aTime){
                count++;
                displayBlock(&process);
            }
        }while(curr->nextPtr==NULL);
    }
}
//shift+alt+A
