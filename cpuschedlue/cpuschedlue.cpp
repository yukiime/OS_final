#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <random>
using namespace std;
// time slice
#define TIME_ROUND 20 
// minimum number of processes
#define MIN_PROCESS_NUM 15 
// Maximum number of additional processes to add
#define MAX_EXTRA_PROCESS_NUM 5 
// latest arrival time
#define MAX_ARRIVAL_TIME 150 
// Maximum time brust
#define MAX_BURST_TIME 50
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    // end time
    int end_time;
    // remaining execution time
    int remaining_execution_time;
};

int rand_process_num(int minNum, int extra)
{
    return (rand() % extra) + minNum;
}

bool compareByArrivalTime(const Process* p1, const Process* p2) 
{
    return p1->arrival_time < p2->arrival_time;
}

vector<Process*> init_process_array(int process_num)
{
    // Create a vector to store the process list
    vector<Process*> processes;

    for(int i = 0;i < process_num;i++)
    {
        Process* p = new Process();
        p->id = i;
        p->arrival_time = (rand() % MAX_ARRIVAL_TIME);
        p->burst_time = (rand() % MAX_BURST_TIME);
        p->remaining_execution_time = p->burst_time;
        processes.push_back(p);
    }
    
    // Sort processes according to arrival_time
    sort(processes.begin(), processes.end(), compareByArrivalTime);

    return processes; 
}

void print_Process(const vector<Process*> processes)
{
    cout << "Process Id\tArrival Time\tBurst Time" << endl;
    for (const auto& process : processes) 
    {
        cout << "\t" << process->id << "\t" << process->arrival_time << "\t\t" << process->burst_time << endl;
    }
}

void print_Process_executed(const vector<Process*> processes)
{
    cout << "Process Id\tArrival Time\tBurst Time\tEnd Time" << endl;
    for (const auto& process : processes) 
    {
        cout << "\t" << process->id << "\t" << process->arrival_time << "\t\t" << process->burst_time << "\t\t" << process->end_time << endl;
    }
}

void print_average_executionTime(int totalExecutionTime,int totalProcesses)
{
    cout << "average execution time : " << float(totalExecutionTime) / float(totalProcesses) <<endl;
}

// round-robin-schedule simulation
int RoundRobin_schedule(vector<Process*>& processes, int time_round) 
{
    // Simulates a ready queue for processes
    queue<Process*> ready_queue;  
    // current time
    int current_time = 0;       
    // executed time
    int executed_time = 0;      

    // to preserve the original process order
    queue<Process*> original_processes;
    for (const auto& process : processes) 
    {
        original_processes.push(process);
    }

    while (!original_processes.empty() || !ready_queue.empty()) 
    {
        
        while (!original_processes.empty()) 
        {   
            // Add processes whose arrival time is less than or equal to the current time to the ready queue
            if(original_processes.front()->arrival_time <= current_time)
            {
                ready_queue.push(original_processes.front());
                original_processes.pop();
            }
            // Because of the order, so the following are not
            else
            {
                break;
            }
        }

        // ready queue is not empty
        if (!ready_queue.empty()) 
        {
            Process* current_process = ready_queue.front();
            ready_queue.pop();

            // Simulate the CPU executing the current process
            int execution_time = min(time_round, current_process->remaining_execution_time);
            current_process->remaining_execution_time -= execution_time;
            current_time += execution_time;
            executed_time += execution_time;

            // Output information about the currently executing process
            cout << "Executing Process ID: " << current_process->id << " (Remaining Time: " << current_process->remaining_execution_time << ")" << endl;

            // Add the unfinished process back to the ready queue
            if (current_process->remaining_execution_time > 0) 
            {
                ready_queue.push(current_process);
            }
            else
            {
                current_process->end_time = current_time;
            }
        }
        // The ready queue is empty, but there are still new processes coming later
        else if (!original_processes.empty())
        {
            current_time = original_processes.front()->arrival_time;
            // Output waiting information
            cout << "CPU is waiting unitl " << current_time << endl;
        }
        else
        {
            cout << "All processes executed!" << endl;
            break;
        }
    }

    return executed_time;
}

int main() 
{
    cout << endl << "-----------------------------Initial Processes-----------------------------" << endl;
    int process_num = rand_process_num(MIN_PROCESS_NUM, MAX_EXTRA_PROCESS_NUM);
    vector<Process*> processes = init_process_array(process_num);
    print_Process(processes);

    cout << endl << "-----------------------------Round Robin Execution-----------------------------" << endl;
    int executed_time = RoundRobin_schedule(processes, TIME_ROUND);

    cout << endl << "-----------------------------Completed Processes-----------------------------" << endl;
    print_Process_executed(processes);

    cout << endl << "-----------------------------The Average Execution Time-----------------------------" << endl;
    print_average_executionTime(executed_time,process_num);
    return 0;
}