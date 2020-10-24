# Process Scheduler Simulator
Wrote a simple simulation of various uniprocessor scheduling algorithms such as FCFS (First-Come-First-Served), Round Robin with quantum of 2, and EDF (Earliest-Deadline-First) as part of my Operating Systems university project.

## First-Come-First-Served
To run the FCFS simulator,use the following command compiled with GCC.
```
./fcfs process-list.txt
```
If no arguments are provided, the FCFS simulator will default into using the process-data.txt list of processes.

### Sample
| Process | Arrival time | Service time | Deadline |
|---------|--------------|--------------|----------|
| P1      | 0            | 3            | 5        |
| P2      | 1            | 6            | 7        |
| P3      | 4            | 4            | 6        |
| P4      | 6            | 2            | 2        |

### How it looks like
```

```
