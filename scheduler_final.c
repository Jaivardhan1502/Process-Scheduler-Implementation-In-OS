#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};

// Function to swap two processes (for sorting)
void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

// Function to display results in table format
void displayResults(struct Process p[], int n) {
    float avg_wait = 0, avg_turn = 0;
    printf("\nPID\tAT\tBT\tPRI\tWT\tTAT\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].priority, p[i].waiting_time, p[i].turnaround_time);
        avg_wait += p[i].waiting_time;
        avg_turn += p[i].turnaround_time;
    }
    printf("------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", avg_wait / n);
    printf("Average Turnaround Time: %.2f\n\n", avg_turn / n);
}

// Function to display Gantt Chart
void displayGanttChart(int gantt[], int gantt_time[], int count) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < count; i++)
        printf(" | P%d ", gantt[i]);
    printf("|\n");
    for (int i = 0; i < count; i++)
        printf("%d\t", gantt_time[i]);
    printf("%d\n\n", gantt_time[count]);
}

// FCFS Scheduling
void fcfs(struct Process p[], int n) {
    int current_time = 0;
    int gantt[MAX_PROCESSES], gantt_time[MAX_PROCESSES + 1], g_count = 0;

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].arrival_time > p[j + 1].arrival_time)
                swap(&p[j], &p[j + 1]);

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;
        gantt[g_count] = p[i].pid;
        gantt_time[g_count] = current_time;
        g_count++;
        p[i].waiting_time = current_time - p[i].arrival_time;
        current_time += p[i].burst_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
    }
    gantt_time[g_count] = current_time;

    printf("\n--- First Come First Serve (FCFS) ---\n");
    displayResults(p, n);
    displayGanttChart(gantt, gantt_time, g_count);
}

// SJF (Non-preemptive)
void sjf(struct Process p[], int n) {
    int completed = 0, current_time = 0, visited[MAX_PROCESSES] = {0};
    int gantt[MAX_PROCESSES], gantt_time[MAX_PROCESSES + 1], g_count = 0;

    while (completed < n) {
        int idx = -1, min_bt = 9999;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time <= current_time && p[i].burst_time < min_bt) {
                min_bt = p[i].burst_time;
                idx = i;
            }
        }
        if (idx != -1) {
            visited[idx] = 1;
            gantt[g_count] = p[idx].pid;
            gantt_time[g_count] = current_time;
            g_count++;

            p[idx].waiting_time = current_time - p[idx].arrival_time;
            current_time += p[idx].burst_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            completed++;
        } else {
            current_time++;
        }
    }
    gantt_time[g_count] = current_time;

    printf("\n--- Shortest Job First (SJF) ---\n");
    displayResults(p, n);
    displayGanttChart(gantt, gantt_time, g_count);
}

// Priority Scheduling (Non-preemptive)
void priorityScheduling(struct Process p[], int n) {
    int completed = 0, current_time = 0, visited[MAX_PROCESSES] = {0};
    int gantt[MAX_PROCESSES], gantt_time[MAX_PROCESSES + 1], g_count = 0;

    while (completed < n) {
        int idx = -1, highest_pri = 9999;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time <= current_time && p[i].priority < highest_pri) {
                highest_pri = p[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            visited[idx] = 1;
            gantt[g_count] = p[idx].pid;
            gantt_time[g_count] = current_time;
            g_count++;

            p[idx].waiting_time = current_time - p[idx].arrival_time;
            current_time += p[idx].burst_time;
            p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;
            completed++;
        } else {
            current_time++;
        }
    }
    gantt_time[g_count] = current_time;

    printf("\n--- Priority Scheduling ---\n");
    displayResults(p, n);
    displayGanttChart(gantt, gantt_time, g_count);
}

// Round Robin Scheduling
void roundRobin(struct Process p[], int n, int quantum) {
    int remaining = n, current_time = 0;
    int gantt[100], gantt_time[101], g_count = 0;

    for (int i = 0; i < n; i++)
        p[i].remaining_time = p[i].burst_time;

    while (remaining > 0) {
        int executed = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0 && p[i].arrival_time <= current_time) {
                gantt[g_count] = p[i].pid;
                gantt_time[g_count] = current_time;
                g_count++;

                if (p[i].remaining_time > quantum) {
                    current_time += quantum;
                    p[i].remaining_time -= quantum;
                } else {
                    current_time += p[i].remaining_time;
                    p[i].waiting_time = current_time - p[i].arrival_time - p[i].burst_time;
                    p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
                    p[i].remaining_time = 0;
                    remaining--;
                }
                executed = 1;
            }
        }
        if (!executed) current_time++; // idle CPU if no process ready
    }
    gantt_time[g_count] = current_time;

    printf("\n--- Round Robin Scheduling ---\n");
    displayResults(p, n);
    displayGanttChart(gantt, gantt_time, g_count);
}

// Copy process array to avoid modifying original data
void copyProcesses(struct Process src[], struct Process dest[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

// Main Program
int main() {
    int n, choice, quantum;
    struct Process processes[MAX_PROCESSES], temp[MAX_PROCESSES];

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("\nEnter Arrival Time, Burst Time, and Priority for Process %d: ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
    }

    do {
        printf("\n===== CPU SCHEDULING MENU =====\n");
        printf("1. First Come First Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF)\n");
        printf("3. Priority Scheduling\n");
        printf("4. Round Robin Scheduling\n");
        printf("5. Exit\n");
        printf("Choose Scheduling Algorithm: ");
        scanf("%d", &choice);

        copyProcesses(processes, temp, n);

        switch (choice) {
            case 1: fcfs(temp, n); break;
            case 2: sjf(temp, n); break;
            case 3: priorityScheduling(temp, n); break;
            case 4:
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                roundRobin(temp, n, quantum);
                break;
            case 5:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
