#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 5

int state[MAX];
int hungry[MAX];

void showMenu() {
    printf("\n1. One can eat at a time\n");
    printf("2. Two can eat at a time\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void allowEating(int total, int count) {
    int eating = 0;
    int i = 0;

    printf("Allow %s philosopher%s to eat at any time\n", count == 1 ? "one" : "two", count == 1 ? "" : "s");

    while (1) {
        eating = 0;

        for (i = 0; i < total; i++) {
            if (hungry[i] == 1) {
                printf("P %d is waiting\n", i + 1);
            }
        }

        for (i = 0; i < total && eating < count; i++) {
            if (hungry[i] == 1) {
                hungry[i] = 0;
                state[i] = 2;
                printf("P %d is granted to eat\n", i + 1);
                sleep(1);
                printf("P %d has finished eating\n", i + 1);
                state[i] = 0;
                eating++;
            }
        }

        int done = 1;
        for (i = 0; i < total; i++) {
            if (hungry[i] == 1) {
                done = 0;
                break;
            }
        }

        if (done)
            break;
    }
}

int main() {
    int total, hungryCount, choice;
    int i, pos;

    printf("Enter the total number of philosophers: ");
    scanf("%d", &total);

    for (i = 0; i < total; i++) {
        state[i] = 0;
        hungry[i] = 0;
    }

    printf("How many are hungry: ");
    scanf("%d", &hungryCount);

    for (i = 0; i < hungryCount; i++) {
        printf("Enter philosopher %d position (1 to %d): ", i + 1, total);
        scanf("%d", &pos);
        if (pos >= 1 && pos <= total)
            hungry[pos - 1] = 1;
    }

    do {
        showMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                allowEating(total, 1);
                break;
            case 2:
                allowEating(total, 2);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}
