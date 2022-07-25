#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

struct PROCESS
{
    string NAME;
    int BURST_TIME;
    int ARRIVAL_TIME;
    int REPONSE_TIME;
    int TURN_AROUND_TIME;
    int FINISH_TIME;
    int WAITING_TIME;
    bool done = false;
    int BURST_TIME_REMAINING;
};

double avg_waiting_time = 0;
double avg_turn_around_time = 0;

void Add_Process(int &size, PROCESS *&P, PROCESS process)
{
    PROCESS *newArr = new PROCESS[size + 1];
    for (int index = 0; index < size; index++)
    {
        newArr[index] = P[index];
    }
    newArr[size] = process;
    size++;
    P = newArr;
}

void swap(PROCESS &p1, PROCESS &p2)
{
    PROCESS tmp;
    tmp = p1;
    p1 = p2;
    p2 = tmp;
}

void sortByArrivalTime(PROCESS *&P, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (P[i].ARRIVAL_TIME > P[j].ARRIVAL_TIME)
            {
                swap(P[i], P[j]);
            }
        }
    }
}

void sortByBurstTimeRemaining(PROCESS *&P, int n, int time_current)
{
     for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (P[i].BURST_TIME_REMAINING > P[j].BURST_TIME_REMAINING)
            {
                swap(P[i], P[j]);
            }
        }
    }
       for (int i = 0; i < n; i++)
        {
            if (P[i].done == false &&P[i].ARRIVAL_TIME<=time_current)
            {
                swap(P[i], P[0]);
                break;
            }
        }
}

void Input_Process(PROCESS *&P, int &size)
{
    cout << "Input the number of Processes: ";
    cin >> size;

    PROCESS *newArr = new PROCESS[size];

    for (int i = 0; i < size; i++)
    {
        cin.ignore(32767, '\n');
        cout << "----------------[" << i << "]-----------------\n";
        cout << "ARRIVAL_TIME: ";
        cin >> newArr[i].ARRIVAL_TIME;

        cout << "BUSRT_TIME: ";
        cin >> newArr[i].BURST_TIME;
        
        newArr[i].NAME = (char)('A' + i);
         newArr[i].BURST_TIME_REMAINING = newArr[i].BURST_TIME;
        
    }
    P = newArr;
}

string space(int n)
{
    string res;
    for (int i = 0; i < n; i++)
    {
        res += " ";
    }
    return res;
}

void Output_Process(int numberOfprocesses, PROCESS *P)
{
    sortByArrivalTime(P, numberOfprocesses);
    string Attribute[6] = {"Job", "Arrival_Time", "Burst_Time", "Finish_Time", "Turnaround_Time", "Waiting_Time"};

    int num_columns = sizeof(Attribute) / sizeof(Attribute[0]);
    vector<vector<string>> board;
    vector<string> row {"\tShortest Remaining Time First, SRTF\n" } ;                         
    board.push_back(row); 
    for (int i = 0; i < numberOfprocesses + 1; i++)
    {
        vector<string> row;
        if (i == 0)
        {
            for (int j = 0; j < num_columns; j++)
            {
                if (j == 0)
                    row.push_back("|" + Attribute[0] + "|");
                else
                    row.push_back(Attribute[j] + "|");
            }
            board.push_back(row);
        }
        else
        {
            int ele = i - 1;
            string Value[6] = {P[ele].NAME, to_string(P[ele].ARRIVAL_TIME), to_string(P[ele].BURST_TIME), to_string(P[ele].FINISH_TIME), to_string(P[ele].TURN_AROUND_TIME), to_string(P[ele].WAITING_TIME)};
            for (int j = 0; j < num_columns; j++)
            {
                if (j == 0)
                    row.push_back("|" + space(Attribute[0].length() - Value[0].length()) + Value[0] + "|");
                else
                    row.push_back(space(Attribute[j].length() - Value[j].length()) + Value[j] + "|");
            }
            board.push_back(row); 
            
        }
         
    }       
         row = {"Average Turn Around Time: " +to_string(avg_turn_around_time)+"\n",
                         "Average Turn Waiting Time: "  +to_string(avg_waiting_time)+"\n"  } ;
        board.push_back(row); 

   
    for (int i = 0; i < numberOfprocesses + 2+1; i++)
    {
        for (int j = 0; j < num_columns; j++)
        {
            if (i==numberOfprocesses+1+1)
            {
                cout << board[i][0].c_str();
                cout << board[i][1].c_str();
                break;
            }
             if (i==0)
            {
                cout << board[i][0].c_str();
                break;
            }
            
            cout << board[i][j].c_str();
        }
        cout << "\n";
    }
}

bool isCompleted(PROCESS *P, int Size)
{
    for (int i = 0; i < Size; i++)
    {
        if (!P[i].done)
        {
            return false;
        }
    }
    return true;
}

void AVG(PROCESS *P,int Size){
    for (int i = 0; i < Size; i++)
    {
        avg_turn_around_time += (double)P[i].TURN_AROUND_TIME/Size;
        avg_waiting_time += (double)P[i].WAITING_TIME/Size;
    }
}

int main()
{
    int numberOfprocesses = 1;
    PROCESS *PROCESSES = new PROCESS[numberOfprocesses];
    Input_Process(PROCESSES, numberOfprocesses);
   
    int i = 0;  
     for(int time_current=0;!isCompleted(PROCESSES,numberOfprocesses); time_current++)
    {
        sortByBurstTimeRemaining(PROCESSES,numberOfprocesses,time_current);
        PROCESSES[i].BURST_TIME_REMAINING--;

        if(PROCESSES[i].BURST_TIME_REMAINING==0)
        {
            PROCESSES[i].done = true;
            PROCESSES[i].FINISH_TIME = time_current+1; 
            PROCESSES[i].TURN_AROUND_TIME =  PROCESSES[i].FINISH_TIME -  PROCESSES[i].ARRIVAL_TIME;
            PROCESSES[i].WAITING_TIME =  PROCESSES[i].TURN_AROUND_TIME - PROCESSES[i].BURST_TIME;
        }
    }
 
    AVG(PROCESSES,numberOfprocesses);
    Output_Process(numberOfprocesses, PROCESSES);

    return 0;
}