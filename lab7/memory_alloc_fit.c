#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MemoryBlock {
    int start;
    int size;
    bool is_free;
    struct MemoryBlock *next;
} MemoryBlock;

MemoryBlock* create_memory(int partitions[], int n) {
    MemoryBlock *head = NULL, *current = NULL;
    int start = 0;
    for (int i = 0; i < n; i++) {
        MemoryBlock *block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        block->start = start;
        block->size = partitions[i];
        block->is_free = true;
        block->next = NULL;
        start += partitions[i];
        if (head == NULL) {
            head = block;
            current = head;
        } else {
            current->next = block;
            current = current->next;
        }
    }
    return head;
}

void print_free_partitions(MemoryBlock *head) {
    MemoryBlock *current = head;
    printf("Free partitions: ");
    bool first = true;
    while (current != NULL) {
        if (current->is_free) {
            if (!first) printf(", ");
            printf("%dK", current->size);
            first = false;
        }
        current = current->next;
    }
    if (first) printf("None");
    printf("\n\n");
}

bool first_fit(MemoryBlock *head, int size) {
    MemoryBlock *current = head;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (current->size > size) {
                MemoryBlock *new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                new_block->start = current->start + size;
                new_block->size = current->size - size;
                new_block->is_free = true;
                new_block->next = current->next;
                current->next = new_block;
            }
            current->size = size;
            current->is_free = false;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool best_fit(MemoryBlock *head, int size) {
    MemoryBlock *current = head, *best = NULL;
    int min_diff = __INT_MAX__;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            int diff = current->size - size;
            if (diff < min_diff) {
                min_diff = diff;
                best = current;
            }
        }
        current = current->next;
    }
    if (best != NULL) {
        if (best->size > size) {
            MemoryBlock *new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            new_block->start = best->start + size;
            new_block->size = best->size - size;
            new_block->is_free = true;
            new_block->next = best->next;
            best->next = new_block;
        }
        best->size = size;
        best->is_free = false;
        return true;
    }
    return false;
}

bool worst_fit(MemoryBlock *head, int size) {
    MemoryBlock *current = head, *worst = NULL;
    int max_size = -1;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            if (current->size > max_size) {
                max_size = current->size;
                worst = current;
            }
        }
        current = current->next;
    }
    if (worst != NULL) {
        if (worst->size > size) {
            MemoryBlock *new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            new_block->start = worst->start + size;
            new_block->size = worst->size - size;
            new_block->is_free = true;
            new_block->next = worst->next;
            worst->next = new_block;
        }
        worst->size = size;
        worst->is_free = false;
        return true;
    }
    return false;
}

void free_memory(MemoryBlock *head) {
    MemoryBlock *current = head;
    while (current != NULL) {
        MemoryBlock *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    int n_partitions, n_processes;

    printf("Enter number of memory partitions: ");
    scanf("%d", &n_partitions);
    int *partitions = (int*)malloc(n_partitions * sizeof(int));
    for (int i = 0; i < n_partitions; i++) {
        printf("Enter size of partition %d (KB): ", i+1);
        scanf("%d", &partitions[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &n_processes);
    int *processes = (int*)malloc(n_processes * sizeof(int));
    for (int i = 0; i < n_processes; i++) {
        printf("Enter size of process %d (KB): ", i+1);
        scanf("%d", &processes[i]);
    }

    // First-fit simulation
    MemoryBlock *first_head = create_memory(partitions, n_partitions);
    printf("\nFirst-fit Allocation Results:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d (%dK): %s\n",
              i+1, processes[i],
              first_fit(first_head, processes[i]) ? "Allocated" : "Failed");
        print_free_partitions(first_head);
    }
    free_memory(first_head);

    // Best-fit simulation
    MemoryBlock *best_head = create_memory(partitions, n_partitions);
    printf("Best-fit Allocation Results:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d (%dK): %s\n",
              i+1, processes[i],
              best_fit(best_head, processes[i]) ? "Allocated" : "Failed");
        print_free_partitions(best_head);
    }
    free_memory(best_head);

    // Worst-fit simulation
    MemoryBlock *worst_head = create_memory(partitions, n_partitions);
    printf("Worst-fit Allocation Results:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d (%dK): %s\n",
              i+1, processes[i],
              worst_fit(worst_head, processes[i]) ? "Allocated" : "Failed");
        print_free_partitions(worst_head);
    }
    free_memory(worst_head);

    free(partitions);
    free(processes);
    return 0;
}
