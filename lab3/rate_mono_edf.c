#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_PROCESS 10

typedef struct {
    int id;
    int burst_time;
    float priority;
} Task;

int num_of_process;
int execution_time[MAX_PROCESS], period[MAX_PROCESS], remain_time[MAX_PROCESS], deadline[MAX_PROCESS], remain_deadline[MAX_PROCESS];

void get_process_info(int selected_algo) {
    printf("Enter total number of processes (maximum %d): ", MAX_PROCESS);
    scanf("%d", &num_of_process);
    if (num_of_process < 1) {
        exit(0);
    }

    for (int i = 0; i < num_of_process; i++) {
        printf("\nProcess %d:\n", i + 1);
        printf("==> Execution time: ");
        scanf("%d", &execution_time[i]);
        remain_time[i] = execution_time[i];
        if (selected_algo == 2) {
            printf("==> Deadline: ");
            scanf("%d", &deadline[i]);
        } else {
            printf("==> Period: ");
            scanf("%d", &period[i]);
        }
    }
}

int max(int a, int b, int c) {
    return (a >= b && a >= c) ? a : (b >= a && b >= c) ? b : c;
}

int get_observation_time(int selected_algo) {
    return (selected_algo == 1) ? max(period[0], period[1], period[2]) : max(deadline[0], deadline[1], deadline[2]);
}

void print_schedule(int process_list[], int cycles) {
    printf("\nScheduling:\n\nTime: ");
    for (int i = 0; i < cycles; i++)
        printf("| %02d ", i);
    printf("|\nProcessor: ");

    for (int i = 0; i < cycles; i++) {
        if (process_list[i] == 0)
            printf("| IDLE ");
        else
            printf("| P%d ", process_list[i]);
    }
    printf("|\n");
}

bool check_schedulability_rate_monotonic() {
    float utilization = 0;
    for (int i = 0; i < num_of_process; i++)
        utilization += (1.0 * execution_time[i]) / period[i];

    float limit = num_of_process * (pow(2, 1.0 / num_of_process) - 1);
    return utilization <= limit;
}

bool check_schedulability_earliest_deadline_first() {
    float utilization = 0;
    for (int i = 0; i < num_of_process; i++)
        utilization += (1.0 * execution_time[i]) / deadline[i];

    return utilization <= 1.0;
}

void rate_monotonic(int time) {
    if (!check_schedulability_rate_monotonic()) {
        printf("\nGiven problem is not schedulable under Rate Monotonic Scheduling.\n");
        return;
    }

    int process_list[100] = {0}, min, next_process;
    for (int i = 0; i < time; i++) {
        min = 999;
        next_process = -1;
        for (int j = 0; j < num_of_process; j++) {
            if (remain_time[j] > 0 && min > period[j]) {
                min = period[j];
                next_process = j;
            }
        }
        if (next_process != -1) {
            process_list[i] = next_process + 1;
            remain_time[next_process]--;
        }
        for (int k = 0; k < num_of_process; k++) {
            if ((i + 1) % period[k] == 0)
                remain_time[k] = execution_time[k];
        }
    }
    print_schedule(process_list, time);
}

void earliest_deadline_first(int time) {
    if (!check_schedulability_earliest_deadline_first()) {
        printf("\nGiven problem is not schedulable under Earliest Deadline First Scheduling.\n");
        return;
    }

    int process_list[time], process[num_of_process], max_deadline, min_deadline, current_process = 0;
    bool is_ready[num_of_process];

    for (int i = 0; i < num_of_process; i++) {
        is_ready[i] = true;
        process[i] = i + 1;
    }

    max_deadline = deadline[0];
    for (int i = 1; i < num_of_process; i++)
        if (deadline[i] > max_deadline)
            max_deadline = deadline[i];

    for (int i = 0; i < num_of_process; i++)
        for (int j = i + 1; j < num_of_process; j++)
            if (deadline[j] < deadline[i]) {
                int temp = execution_time[j]; execution_time[j] = execution_time[i]; execution_time[i] = temp;
                temp = deadline[j]; deadline[j] = deadline[i]; deadline[i] = temp;
                temp = process[j]; process[j] = process[i]; process[i] = temp;
            }

    for (int i = 0; i < num_of_process; i++) {
        remain_time[i] = execution_time[i];
        remain_deadline[i] = deadline[i];
    }

    for (int t = 0; t < time; t++) {
        if (current_process != -1) {
            --execution_time[current_process];
            process_list[t] = process[current_process];
        } else {
            process_list[t] = 0;
        }

        for (int i = 0; i < num_of_process; i++) {
            --deadline[i];
            if (execution_time[i] == 0 && is_ready[i]) {
                deadline[i] += remain_deadline[i];
                is_ready[i] = false;
            }
            if (deadline[i] <= remain_deadline[i] && !is_ready[i]) {
                execution_time[i] = remain_time[i];
                is_ready[i] = true;
            }
        }

        min_deadline = max_deadline;
        current_process = -1;
        for (int i = 0; i < num_of_process; i++) {
            if (deadline[i] <= min_deadline && execution_time[i] > 0) {
                current_process = i;
                min_deadline = deadline[i];
            }
        }
    }
    print_schedule(process_list, time);
}

int main() {
    int option, observation_time;

    while (1) {
        printf("\n1. Rate Monotonic\n2. Earliest Deadline first\n\nEnter your choice: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                get_process_info(option);
                observation_time = get_observation_time(option);
                rate_monotonic(observation_time);
                break;
            case 2:
                get_process_info(option);
                observation_time = get_observation_time(option);
                earliest_deadline_first(observation_time);
                break;
            default:
                printf("\nInvalid Statement");
        }
    }
    return 0;
}
