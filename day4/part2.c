#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 10000
#define MAX_LEN   4096

// Loads non-empty lines from input.txt into grid; returns number of rows.
int load_grid(const char *path, char **grid, int *rows) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;

    char buf[MAX_LEN];
    int r = 0;
    while (fgets(buf, sizeof(buf), f)) {
        // remove newline(s)
        buf[strcspn(buf, "\r\n")] = 0;

        // skip empty lines
        int nonempty = 0;
        for (char *p = buf; *p; ++p) {
            if (!isspace((unsigned char)*p)) { nonempty = 1; break; }
        }
        if (!nonempty) continue;

        grid[r] = malloc(strlen(buf) + 1);
        if (!grid[r]) {
            // cleanup and exit on allocation failure
            for (int i = 0; i < r; ++i) free(grid[i]);
            fclose(f);
            return 0;
        }
        strcpy(grid[r], buf);
        r++;
        if (r >= MAX_LINES) break;
    }
    fclose(f);
    *rows = r;
    return 1;
}

// Count '@' neighbors; used by find_accessible.
int count_adjacent_at(char **grid, int rows, int r, int c) {
    const int dr[8] = {-1,-1,-1,0,0,1,1,1};
    const int dc[8] = {-1,0,1,-1,1,-1,0,1};
    int adj = 0;
    for (int k = 0; k < 8; ++k) {
        int nr = r + dr[k], nc = c + dc[k];
        if (nr < 0 || nr >= rows) continue;
        int nlen = strlen(grid[nr]);
        if (nc < 0 || nc >= nlen) continue;
        if (grid[nr][nc] == '@') {
            adj++;
            if (adj >= 4) return adj; // short-circuit
        }
    }
    return adj;
}

// Repeatedly remove accessible '@' cells until stable. Return total removed.
int remove_until_stable(char **grid, int rows) {
    int total = 0;
    // We will allocate a dynamic array to hold coordinates to remove each round.
    // For simplicity, allocate worst-case rows * maxlen, but here use a simple loop:
    while (1) {
        // find accessible cells in this round
        int found = 0;
        // We'll store indices in two arrays for simplicity.
        int *rem_r = malloc(sizeof(int) * rows * 2); // conservative
        int *rem_c = malloc(sizeof(int) * rows * 2);
        if (!rem_r || !rem_c) {
            free(rem_r);
            free(rem_c);
            return total; // stop if allocation fails
        }
        int rem_count = 0;
        for (int r = 0; r < rows; ++r) {
            int len = strlen(grid[r]);
            for (int c = 0; c < len; ++c) {
                if (grid[r][c] != '@') continue;
                int adj = count_adjacent_at(grid, rows, r, c);
                if (adj < 4) {
                    rem_r[rem_count] = r;
                    rem_c[rem_count] = c;
                    rem_count++;
                }
            }
        }

        if (rem_count == 0) {
            free(rem_r);
            free(rem_c);
            break;
        }

        // remove them
        for (int i = 0; i < rem_count; ++i) {
            int rr = rem_r[i], cc = rem_c[i];
            if (grid[rr][cc] == '@') {
                grid[rr][cc] = '.';
                total++;
            }
        }
        free(rem_r);
        free(rem_c);
    }
    return total;
}

int main(void) {
    // allocate pointer array for lines
    char *grid[MAX_LINES];
    int rows = 0;
    if (!load_grid("input.txt", grid, &rows)) {
        fprintf(stderr, "Failed to read input.txt or out of memory\n");
        return 1;
    }

    int result = remove_until_stable(grid, rows);
    printf("%d\n", result);

    // cleanup lines
    for (int i = 0; i < rows; ++i) free(grid[i]);
    return 0;
}