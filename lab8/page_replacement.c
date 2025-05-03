#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void simulateFIFO(int n_frames, int ref_len, int *ref_str);
void simulateLRU(int n_frames, int ref_len, int *ref_str);
void simulateOptimal(int n_frames, int ref_len, int *ref_str);

int main() {
    int n_frames, ref_len;
    printf("Enter the number of Frames: ");
    scanf("%d", &n_frames);
    printf("Enter the length of reference string: ");
    scanf("%d", &ref_len);
    int *ref_str = (int*)malloc(ref_len * sizeof(int));
    printf("Enter the reference string: ");
    for (int i = 0; i < ref_len; i++) {
        scanf("%d", &ref_str[i]);
    }

    simulateFIFO(n_frames, ref_len, ref_str);
    simulateLRU(n_frames, ref_len, ref_str);
    simulateOptimal(n_frames, ref_len, ref_str);

    free(ref_str);
    return 0;
}

void simulateFIFO(int n_frames, int ref_len, int *ref_str) {
    int *frames = (int*)malloc(n_frames * sizeof(int));
    memset(frames, -1, n_frames * sizeof(int));
    int front = 0;
    int page_faults = 0;
    char states[100][100];

    for (int i = 0; i < ref_len; i++) {
        int page = ref_str[i];
        bool found = false;
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }
        if (!found) {
            page_faults++;
            bool has_empty = false;
            for (int j = 0; j < n_frames; j++) {
                if (frames[j] == -1) {
                    frames[j] = page;
                    has_empty = true;
                    break;
                }
            }
            if (!has_empty) {
                frames[front] = page;
                front = (front + 1) % n_frames;
            }
            char state[100] = "";
            for (int j = 0; j < n_frames; j++) {
                char temp[10];
                if (frames[j] == -1)
                    sprintf(temp, "- ");
                else
                    sprintf(temp, "%d ", frames[j]);
                strcat(state, temp);
            }
            state[strlen(state) - 1] = '\0';
            sprintf(states[page_faults - 1], "%s", state);
        }
    }

    printf("FIFO Page Replacement Process:\n");
    for (int i = 0; i < page_faults; i++) {
        printf("PF No. %d: %s\n", i + 1, states[i]);
    }
    printf("FIFO Page Faults: %d\n\n", page_faults);
    free(frames);
}

void simulateLRU(int n_frames, int ref_len, int *ref_str) {
    int *frames = (int*)malloc(n_frames * sizeof(int));
    int *last_used = (int*)malloc(n_frames * sizeof(int));
    memset(frames, -1, n_frames * sizeof(int));
    for (int i = 0; i < n_frames; i++) last_used[i] = -1;
    int page_faults = 0;
    char states[100][100];
    int time = 0;

    for (int i = 0; i < ref_len; i++) {
        int page = ref_str[i];
        bool found = false;
        int pos = -1;
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] == page) {
                found = true;
                pos = j;
                break;
            }
        }
        if (found) {
            last_used[pos] = time++;
            continue;
        }
        page_faults++;
        int lru = 0;
        int min_time = last_used[0];
        for (int j = 1; j < n_frames; j++) {
            if (last_used[j] < min_time) {
                min_time = last_used[j];
                lru = j;
            }
        }
        bool has_empty = false;
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] == -1) {
                frames[j] = page;
                last_used[j] = time++;
                has_empty = true;
                break;
            }
        }
        if (!has_empty) {
            frames[lru] = page;
            last_used[lru] = time++;
        }
        char state[100] = "";
        for (int j = 0; j < n_frames; j++) {
            char temp[10];
            if (frames[j] == -1)
                sprintf(temp, "- ");
            else
                sprintf(temp, "%d ", frames[j]);
            strcat(state, temp);
        }
        state[strlen(state) - 1] = '\0';
        sprintf(states[page_faults - 1], "%s", state);
    }

    printf("LRU Page Replacement Process:\n");
    for (int i = 0; i < page_faults; i++) {
        printf("PF No. %d: %s\n", i + 1, states[i]);
    }
    printf("LRU Page Faults: %d\n\n", page_faults);
    free(frames);
    free(last_used);
}

void simulateOptimal(int n_frames, int ref_len, int *ref_str) {
    int *frames = (int*)malloc(n_frames * sizeof(int));
    memset(frames, -1, n_frames * sizeof(int));
    int page_faults = 0;
    char states[100][100];

    for (int i = 0; i < ref_len; i++) {
        int page = ref_str[i];
        bool found = false;
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }
        if (found) continue;

        page_faults++;
        bool has_empty = false;
        for (int j = 0; j < n_frames; j++) {
            if (frames[j] == -1) {
                frames[j] = page;
                has_empty = true;
                break;
            }
        }
        if (has_empty) {
            char state[100] = "";
            for (int j = 0; j < n_frames; j++) {
                char temp[10];
                if (frames[j] == -1)
                    sprintf(temp, "- ");
                else
                    sprintf(temp, "%d ", frames[j]);
                strcat(state, temp);
            }
            state[strlen(state) - 1] = '\0';
            sprintf(states[page_faults - 1], "%s", state);
            continue;
        }

        int farthest = -1;
        int replace_index = 0;
        for (int j = 0; j < n_frames; j++) {
            int current = frames[j];
            int k;
            for (k = i + 1; k < ref_len; k++) {
                if (ref_str[k] == current) {
                    break;
                }
            }
            if (k == ref_len) {
                replace_index = j;
                break;
            } else {
                if (k > farthest) {
                    farthest = k;
                    replace_index = j;
                }
            }
        }
        frames[replace_index] = page;

        char state[100] = "";
        for (int j = 0; j < n_frames; j++) {
            char temp[10];
            if (frames[j] == -1)
                sprintf(temp, "- ");
            else
                sprintf(temp, "%d ", frames[j]);
            strcat(state, temp);
        }
        state[strlen(state) - 1] = '\0';
        sprintf(states[page_faults - 1], "%s", state);
    }

    printf("Optimal Page Replacement Process:\n");
    for (int i = 0; i < page_faults; i++) {
        printf("PF No. %d: %s\n", i + 1, states[i]);
    }
    printf("Optimal Page Faults: %d\n\n", page_faults);
    free(frames);
}
