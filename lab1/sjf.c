#include <stdio.h>

int main() {
int bt[20], wt[20], tat[20], p[20], n, i, j, temp;
float wtavg, tatavg;
printf("\tSJF Scheduling\t");
printf("\nEnter the number of processes is ");
scanf("%d", &n);
for(i = 0; i < n; i++) {
    printf("\nEnter Burst Time for Process %d is ", i + 1);
    scanf("%d", &bt[i]);
    p[i] = i + 1;
}
for(i = 0; i < n - 1; i++) {
    for(j = i + 1; j < n; j++) {
        if(bt[i] > bt[j]) {
            temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
            temp = p[i]; p[i] = p[j]; p[j] = temp;
        }}}

wt[0] = wtavg = 0;
tat[0] = tatavg = bt[0];

for(i = 1; i < n; i++) {
    wt[i] = wt[i - 1] + bt[i - 1];
    tat[i] = wt[i] + bt[i];
    wtavg += wt[i];
    tatavg += tat[i];
}
    printf("\nPROCESS\tBURST TIME\tWAITING TIME\tTURNAROUND TIME\n");
    for(i = 0; i < n; i++) {
        printf("\nP%d\t%d\t\t%d\t\t%d", p[i], bt[i], wt[i], tat[i]);
    }
    printf("\n\nAverage Waiting Time -- %.2f", wtavg / n);
    printf("\nAverage Turnaround Time -- %.2f\n", tatavg / n);
    return 0;
}

