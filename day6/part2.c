#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 2000
#define MAX_WIDTH 2000

char grid[MAX_LINES][MAX_WIDTH];
int rows = 0, width = 0;

long long solve_block(int start, int end) {
    long long nums[128];
    int count = 0;
    char op = 0;

    for (int c = end - 1; c >= start; c--) {
        char buf[64];
        int len = 0;

        for (int r = 0; r < rows; r++) {
            char ch = grid[r][c];
            if (isdigit(ch)) buf[len++] = ch;
            else if (ch == '+' || ch == '*') op = ch;
        }

        if (len > 0) {
            buf[len] = '\0';
            nums[count++] = atoll(buf);
        }
    }

    if (op == '+') {
        long long sum = 0;
        for (int i = 0; i < count; i++) sum += nums[i];
        return sum;
    }

    long long prod = 1;
    for (int i = 0; i < count; i++) prod *= nums[i];
    return prod;
}

int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("input.txt");
        return 1;
    }

    while (fgets(grid[rows], MAX_WIDTH, f)) {
        int len = strlen(grid[rows]);
        if (grid[rows][len - 1] == '\n') grid[rows][--len] = '\0';
        if (len > width) width = len;
        rows++;
    }
    fclose(f);

    for (int r = 0; r < rows; r++) {
        for (int c = strlen(grid[r]); c < width; c++) {
            grid[r][c] = ' ';
        }
    }

    int used[MAX_WIDTH] = {0};
    for (int c = 0; c < width; c++) {
        for (int r = 0; r < rows; r++) {
            if (grid[r][c] != ' ') {
                used[c] = 1;
                break;
            }
        }
    }

    long long total = 0;
    int start = -1;

    for (int c = 0; c <= width; c++) {
        int active = (c < width && used[c]);
        if (active && start == -1) start = c;
        else if (!active && start != -1) {
            total += solve_block(start, c);
            start = -1;
        }
    }

    printf("%lld\n", total);
    return 0;
}