# SRTF Scheduling Algorithm Simulator

This project is a C implementation of the **Shortest Remaining Time First (SRTF)** CPU scheduling algorithm. It simulates the execution of processes based on their arrival and burst times, providing a detailed analysis of the scheduling performance.

## Features

- **Dynamic Process Input**: Supports simulation for 1 to 10 processes.
- **Detailed Simulation**: Displays the state of the system at each time unit (PID, State, Remaining Time).
- **Gantt Chart Visualization**: Generates a text-based Gantt chart to visualize the timeline of process execution.
- **Performance Metrics**: Calculates and displays individual and average:
  - Turnaround Time
  - Waiting Time
  - Response Time
- **Interactive Interface**: Allows users to restart the simulation with new inputs or exit the program.

## How to Compile and Run

### Prerequisites

- A C compiler (e.g., GCC).

### Compilation

Open your terminal or command prompt and navigate to the directory containing `q.c`. Run the following command:

```bash
gcc SRTF.c -o SRTF
```

### Execution

Run the compiled executable:

**Windows:**

```bash
STRF.exe
```

**Linux/macOS:**

```bash
./SRTF
```

## Usage

1. **Start the Program**: Run the executable.
2. **Enter Number of Processes**: Input the total number of processes you want to simulate (between 1 and 10).
3. **Enter Process Details**: For each process, enter:
   - **Arrival Time**: The time at which the process arrives in the ready queue.
   - **Burst Time**: The amount of CPU time required by the process.
4. **View Results**:
   - The program will show a step-by-step simulation log.
   - A Gantt chart will be displayed showing the execution order.
   - A summary table will show the performance metrics for each process.
   - Average Turnaround, Waiting, and Response times will be displayed at the end.
5. **Restart or Exit**: Press 'A' to restart the simulation with new values, or 'E' to exit.

## Example Output

```text
Enter number of processes (1-10): 3
Process 1 - Enter Arrival Time: 0
Process 1 - Enter Burst Time: 5
Process 2 - Enter Arrival Time: 1
Process 2 - Enter Burst Time: 3
Process 3 - Enter Arrival Time: 2
Process 3 - Enter Burst Time: 1

... (Simulation Steps) ...

Gantt Chart:
...

SRTF Performance Results:
PID  Arrival  Burst   Completion  Turnaround  Waiting   Response
-----------------------------------------------------------------
P1   0        5       9           9           4         0   
P2   1        3       4           3           0         0   
P3   2        1       3           1           0         0   

Average Turnaround Time = 4.33
Average Waiting Time     = 1.33
Average Response Time    = 0.00
```
