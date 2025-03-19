#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    int id;
    int queue_num;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} Process;

void sort_by_arrival(Process queue[], int count) {
    for (int i = 0; i < count-1; i++) {
        for (int j = 0; j < count-i-1; j++) {
            if (queue[j].arrival_time > queue[j+1].arrival_time) {
                Process temp = queue[j];
                queue[j] = queue[j+1];
                queue[j+1] = temp;
    }}}}

int main() {
    Process system_queue[MAX_PROCESSES];
    Process user_queue[MAX_PROCESSES];
    Process all_processes[MAX_PROCESSES];
    int system_count = 0, user_count = 0;
    int n, total_waiting = 0, total_turnaround = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        Process p;
        printf("Process %d (id queue(1/2) arrival burst): ", i+1);
        scanf("%d %d %d %d", &p.id, &p.queue_num, &p.arrival_time, &p.burst_time);
        p.waiting_time = p.turnaround_time = p.completion_time = 0;

        if (p.queue_num == 1) {
            system_queue[system_count++] = p;
        } else {
            user_queue[user_count++] = p;
        }
    }
    sort_by_arrival(system_queue, system_count);
    sort_by_arrival(user_queue, user_count);
    int current_time = 0, executed = 0, s_idx = 0, u_idx = 0;
    printf("\nGantt Chart:\n");
    while (executed < n) {
        int found = 0;
        if (s_idx < system_count) {
            Process *p = &system_queue[s_idx];
            if (p->arrival_time <= current_time) {
                p->waiting_time = current_time - p->arrival_time;
                current_time += p->burst_time;
                p->completion_time = current_time;
                p->turnaround_time = current_time - p->arrival_time;
                printf("Q1-%d [%d-%d] ", p->id, p->completion_time - p->burst_time, p->completion_time);
                total_waiting += p->waiting_time;
                total_turnaround += p->turnaround_time;
                s_idx++;
                executed++;
                found = 1;
            }
        }
        if (!found && u_idx < user_count) {
            Process *p = &user_queue[u_idx];
            if (p->arrival_time <= current_time) {
                p->waiting_time = current_time - p->arrival_time;
                current_time += p->burst_time;
                p->completion_time = current_time;
                p->turnaround_time = current_time - p->arrival_time;
                printf("Q2-%d [%d-%d] ", p->id, p->completion_time - p->burst_time, p->completion_time);
                total_waiting += p->waiting_time;
                total_turnaround += p->turnaround_time;
                u_idx++;
                executed++;
                found = 1;
            }
        }
        if (!found) {
            int next_arrival = INT_MAX;
            if (s_idx < system_count)
                next_arrival = system_queue[s_idx].arrival_time;
            if (u_idx < user_count && user_queue[u_idx].arrival_time < next_arrival)
                next_arrival = user_queue[u_idx].arrival_time;

            if (next_arrival != INT_MAX) {
                printf("IDLE [%d-%d] ", current_time, next_arrival);
                current_time = next_arrival;
        }}}
    int total = system_count + user_count;
    int k = 0;
    for (int i = 0; i < system_count; i++) all_processes[k++] = system_queue[i];
    for (int i = 0; i < user_count; i++) all_processes[k++] = user_queue[i];
    sort_by_arrival(all_processes, total);
    printf("\n\nProcess Details (Sorted by Arrival Time):\n");
    printf("ID\tQueue\tArrival\tBurst\tCompletion\tWait\tTurnaround\n");
    for (int i = 0; i < total; i++) {
        Process p = all_processes[i];
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
              p.id, p.queue_num, p.arrival_time, p.burst_time,
              p.completion_time, p.waiting_time, p.turnaround_time);
    }
    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);
}
