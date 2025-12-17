#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ROWS 4096
#define MAX_COLS 4096

static char grid[MAX_ROWS][MAX_COLS];
static long long dp[MAX_COLS];
static long long next_dp[MAX_COLS];

int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("input.txt");
        return 1;
    }

    int rows = 0;
    int cols = -1;

    while (rows < MAX_ROWS && fgets(grid[rows], MAX_COLS, f)) {
        int len = strlen(grid[rows]);

        while (len > 0 &&
              (grid[rows][len - 1] == '\n' || grid[rows][len - 1] == '\r')) {
            grid[rows][--len] = '\0';
        }

        if (len == 0) continue;

        if (cols == -1) cols = len;
        else if (len != cols) {
            fprintf(stderr, "Error: non-rectangular grid\n");
            return 1;
        }

        rows++;
    }
    fclose(f);

    int start_col = -1;
    for (int c = 0; c < cols; c++) {
        if (grid[0][c] == 'S') {
            start_col = c;
            break;
        }
    }

    if (start_col == -1) {
        fprintf(stderr, "Error: no S found\n");
        return 1;
    }

    memset(dp, 0, sizeof(dp));
    dp[start_col] = 1;

    long long total = 0;

    for (int r = 0; r < rows; r++) {
        memset(next_dp, 0, sizeof(next_dp));

        for (int c = 0; c < cols; c++) {
            long long count = dp[c];
            if (count == 0) continue;

            // exiting bottom
            if (r == rows - 1) {
                total += count;
                continue;
            }

            if (grid[r][c] == '^') {
                if (c > 0) next_dp[c - 1] += count;
                if (c < cols - 1) next_dp[c + 1] += count;
            } else {
                next_dp[c] += count;
            }
        }

        memcpy(dp, next_dp, sizeof(dp));
    }

    printf("%lld\n", total);
    return 0;
}