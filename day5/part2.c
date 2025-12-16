#include <stdio.h>
#include <stdlib.h>

#define MAX_RANGES 100000

typedef struct {
    int start;
    int end;
} Range;

int compare(const void *a, const void *b) {
    return ((Range *)a)->start - ((Range *)b)->start;
}

int main(void) {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("input.txt");
        return 1;
    }

    Range ranges[MAX_RANGES];
    int count = 0;

    while (1) {
        char line[128];
        if (!fgets(line, sizeof(line), file)) break;
        if (line[0] == '\n') break;

        sscanf(line, "%d-%d", &ranges[count].start, &ranges[count].end);
        count++;
    }

    fclose(file);

    if (count == 0) {
        printf("0\n");
        return 0;
    }

    qsort(ranges, count, sizeof(Range), compare);

    int total = 0;
    int currentStart = ranges[0].start;
    int currentEnd = ranges[0].end;

    for (int i = 1; i < count; i++) {
        if (ranges[i].start > currentEnd + 1) {
            total += currentEnd - currentStart + 1;
            currentStart = ranges[i].start;
            currentEnd = ranges[i].end;
        } else if (ranges[i].end > currentEnd) {
            currentEnd = ranges[i].end;
        }
    }

    total += currentEnd - currentStart + 1;
    printf("%d\n", total);

    return 0;
}