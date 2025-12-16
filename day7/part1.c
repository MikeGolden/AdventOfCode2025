#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 4096
#define MAX_COLS 4096

// Move large arrays to static storage (NOT stack)
static bool active[MAX_COLS];
static bool next_active[MAX_COLS];
static char grid[MAX_ROWS][MAX_COLS];

int count_splits(int rows, int cols) {
    memset(active, 0, sizeof(active));
    memset(next_active, 0, sizeof(next_active));

    int start_col = -1;
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') {
            start_col = c;
            break;
        }
    }

    if (start_col == -1) {
        fprintf(stderr, "Error: No 'S' found in first row\n");
        exit(1);
    }

    active[start_col] = true;
    int splits = 0;

    for (int r = 1; r < rows; r++) {
        memset(next_active, 0, sizeof(next_active));
        bool any = false;

        for (int c = 0; c < cols; c++) {
            if (!active[c]) continue;

            if (grid[r][c] == '^') {
                splits++;
                if (c > 0) {
                    next_active[c - 1] = true;
                    any = true;
                }
                if (c < cols - 1) {
                    next_active[c + 1] = true;
                    any = true;
                }
            } else {
                next_active[c] = true;
                any = true;
            }
        }

        if (!any) break;
        memcpy(active, next_active, sizeof(active));
    }

    return splits;
}

int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("input.txt");
        return 1;
    }

    int rows = 0;
    int cols = -1;

    while (rows < MAX_ROWS && fgets(grid[rows], MAX_COLS, f)) {
        size_t len = strlen(grid[rows]);

        // Trim newline and CR
        while (len > 0 && (grid[rows][len - 1] == '\n' || grid[rows][len - 1] == '\r')) {
            grid[rows][--len] = '\0';
        }

        if (len == 0) continue;  // skip empty lines

        if (cols == -1) cols = (int)len;
        if ((int)len != cols) {
            fprintf(stderr, "Error: Non-rectangular grid\n");
            exit(1);
        }

        rows++;
    }

    fclose(f);

    if (rows == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }

    printf("%d\n", count_splits(rows, cols));
    return 0;
}