#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 10
#define QUANTUM 2

typedef struct {
    int id;
    int queue_num;
    int arrival_time;
    int burst_time;
    int remaining_burst;
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
            }
        }
    }
}

int find_next_system_arrival(Process system_queue[], int count, int current_time) {
    int min = INT_MAX;
    for (int i = 0; i < count; i++) {
        if (system_queue[i].arrival_time > current_time && system_queue[i].arrival_time < min) {
            min = system_queue[i].arrival_time;
        }
    }
    return min;
}

int main() {
    Process system_queue[MAX_PROCESSES];
    Process user_queue[MAX_PROCESSES];
    int system_count = 0, user_count = 0;
    int n, total_waiting = 0, total_turnaround = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        Process p;
        printf("Process %d (id queue(1/2) arrival burst): ", i+1);
        scanf("%d %d %d %d", &p.id, &p.queue_num, &p.arrival_time, &p.burst_time);
        p.remaining_burst = p.burst_time;
        p.waiting_time = p.turnaround_time = p.completion_time = 0;

        if (p.queue_num == 1) {
            system_queue[system_count++] = p;
        } else {
            user_queue[user_count++] = p;
        }
    }

    sort_by_arrival(system_queue, system_count);
    sort_by_arrival(user_queue, user_count);

    int current_time = 0, executed = 0;
    int sys_idx = 0, user_idx = 0;
    int system_active = 0;

    printf("\nGantt Chart:\n");
    while (executed < n) {
        int found = 0;
        for (int i = 0; i < system_count; i++) {
            Process *p = &system_queue[(sys_idx + i) % system_count];
            if (p->remaining_burst > 0 && p->arrival_time <= current_time) {
                int exec_time = (p->remaining_burst > QUANTUM) ? QUANTUM : p->remaining_burst;
                printf("Q1-%d [%d-%d] ", p->id, current_time, current_time + exec_time);

                p->remaining_burst -= exec_time;
                current_time += exec_time;
                found = 1;
                system_active = 1;

                if (p->remaining_burst == 0) {
                    p->completion_time = current_time;
                    p->turnaround_time = current_time - p->arrival_time;
                    p->waiting_time = p->turnaround_time - p->burst_time;
                    total_waiting += p->waiting_time;
                    total_turnaround += p->turnaround_time;
                    executed++;
                }

                sys_idx = (sys_idx + i + 1) % system_count;
                break;
            }
        }

        if (found) continue;
        if (user_idx < user_count) {
            Process *p = &user_queue[user_idx];
            if (p->arrival_time <= current_time) {
                int next_system_arrival = find_next_system_arrival(system_queue, system_count, current_time);
                int exec_time = p->burst_time;

                if (next_system_arrival != INT_MAX && (current_time + p->burst_time) > next_system_arrival) {
                    exec_time = next_system_arrival - current_time;
                }

                printf("Q2-%d [%d-%d] ", p->id, current_time, current_time + exec_time);
                current_time += exec_time;
                p->burst_time -= exec_time;
                found = 1;
                system_active = 0;

                if (p->burst_time == 0) {
                    p->completion_time = current_time;
                    p->turnaround_time = current_time - p->arrival_time;
                    p->waiting_time = p->turnaround_time - p->remaining_burst;
                    total_waiting += p->waiting_time;
                    total_turnaround += p->turnaround_time;
                    executed++;
                    user_idx++;
                }
                else {
                    for (int i = user_count; i > user_idx; i--) {
                        user_queue[i] = user_queue[i-1];
                    }
                    user_queue[user_idx] = *p;
                    user_count++;
                    user_idx++;
                }
            }
        }

        if (!found) {
            int next_arrival = INT_MAX;
            if (sys_idx < system_count)
                next_arrival = system_queue[sys_idx].arrival_time;
            if (user_idx < user_count && user_queue[user_idx].arrival_time < next_arrival)
                next_arrival = user_queue[user_idx].arrival_time;

            if (next_arrival != INT_MAX) {
                printf("IDLE [%d-%d] ", current_time, next_arrival);
                current_time = next_arrival;
            }
        }
    }

    Process all_processes[MAX_PROCESSES];
    int k = 0;
    for (int i = 0; i < system_count; i++) all_processes[k++] = system_queue[i];
    for (int i = 0; i < user_count; i++) all_processes[k++] = user_queue[i];
    sort_by_arrival(all_processes, k);

    printf("\n\nProcess Details (Sorted by Arrival Time):\n");
    printf("ID\tQueue\tArrival\tBurst\tCompletion\tWait\tTurnaround\n");
    for (int i = 0; i < k; i++) {
        Process p = all_processes[i];
        if (p.queue_num == 1) {
            printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
                  p.id, p.queue_num, p.arrival_time, p.remaining_burst + p.waiting_time,
                  p.completion_time, p.waiting_time, p.turnaround_time);
        } else {
            printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
                  p.id, p.queue_num, p.arrival_time, p.remaining_burst + p.waiting_time,
                  p.completion_time, p.waiting_time, p.turnaround_time);
        }
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);

    return 0;
}
