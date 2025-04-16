#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

int n, m;

void calculate_need() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

bool is_safe(int safe_seq[], int visited_states[][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = { false };

    for (int i = 0; i < m; i++)
        work[i] = available[i];

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                bool can_allocate = true;
                for (int j = 0; j < m; j++) {
                    if (need[p][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    for (int k = 0; k < m; k++)
                        work[k] += allocation[p][k];

                    safe_seq[count++] = p;
                    finish[p] = true;
                    found = true;

                    for (int k = 0; k < m; k++)
                        visited_states[count - 1][k] = work[k];
                }
            }
        }

        if (!found) return false;
    }

    return true;
}

bool request_resources(int pid, int request[]) {
    for (int i = 0; i < m; i++) {
        if (request[i] > need[pid][i] || request[i] > available[i])
            return false;
    }

    for (int i = 0; i < m; i++) {
        available[i] -= request[i];
        allocation[pid][i] += request[i];
        need[pid][i] -= request[i];
    }

    int safe_seq[MAX_PROCESSES];
    int visited_states[MAX_PROCESSES][MAX_RESOURCES];
    if (is_safe(safe_seq, visited_states)) {
        return true;
    } else {
        for (int i = 0; i < m; i++) {
            available[i] += request[i];
            allocation[pid][i] -= request[i];
            need[pid][i] += request[i];
        }
        return false;
    }
}

int main() {
    printf("Enter number of processes -- ");
    scanf("%d", &n);
    printf("Enter number of resources -- ");
    scanf("%d", &m);

    for (int i = 0; i < n; i++) {
        printf("Enter details for P%d\n", i);
        printf("Enter allocation -- ");
        for (int j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);

        printf("Enter Max -- ");
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    }

    printf("Enter Available Resources -- ");
    for (int i = 0; i < m; i++)
        scanf("%d", &available[i]);

    calculate_need();

    printf("\nProcess\tAllocation\tMax\t\tNeed\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < m; j++)
            printf("%d ", allocation[i][j]);
        printf("\t\t");
        for (int j = 0; j < m; j++)
            printf("%d ", max[i][j]);
        printf("\t\t");
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    int pid, request[MAX_RESOURCES];
    printf("\nEnter New Request Details --\n");
    printf("Enter pid -- ");
    scanf("%d", &pid);
    printf("Enter Request for Resources -- ");
    for (int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    if (request_resources(pid, request)) {
        int safe_seq[MAX_PROCESSES];
        int visited_states[MAX_PROCESSES][MAX_RESOURCES];

        is_safe(safe_seq, visited_states);

        for (int i = 0; i < n; i++) {
            int p = safe_seq[i];
            printf("P%d is visited( ", p);
            for (int j = 0; j < m; j++) {
                printf("%d", visited_states[i][j]);
                if (j != m - 1) printf(" ");
            }
            printf(" )\n");
        }

        printf("SYSTEM IS IN SAFE STATE\n");
        printf("The Safe Sequence is -- (");
        for (int i = 0; i < n; i++) {
            printf("P%d", safe_seq[i]);
            if (i != n - 1) printf(" ");
        }
        printf(" )\n");
    } else {
        printf("Request cannot be granted. System is in UNSAFE STATE\n");
    }

    return 0;
}
