#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_zeros_during_movement(int pos, int dist, int dir) {
    int first_step;

    if (dir == 1) {
        // Moving right: distance to reach 0 next time
        first_step = (100 - pos) % 100;
    } else {
        // Moving left: distance to reach 0 next time
        first_step = pos % 100;
    }

    // If first hit occurs immediately (pos == 0), next one is at step 100
    if (first_step == 0) {
        if (dist < 100)
            return 0;
        return 1 + (dist - 100) / 100;
    }

    // If not enough distance to hit zero
    if (dist < first_step)
        return 0;

    return 1 + (dist - first_step) / 100;
}

int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        printf("Failed to open input.txt\n");
        return 1;
    }

    char line[64];
    int pos1 = 50;  // Part 1 dial
    int pos2 = 50;  // Part 2 dial

    long part1 = 0;
    long part2 = 0;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        char dir_char = line[0];
        int dist = atoi(&line[1]);
        int dir = (dir_char == 'R') ? 1 : -1;

        // --- Part 2: count hits during movement ---
        part2 += count_zeros_during_movement(pos2, dist, dir);

        // --- Move Part 1 and check end position ---
        pos1 = (pos1 + dir * dist) % 100;
        if (pos1 < 0) pos1 += 100;
        if (pos1 == 0)
            part1++;

        // --- Move Part 2 final position (NO extra check here!) ---
        pos2 = (pos2 + dir * dist) % 100;
        if (pos2 < 0) pos2 += 100;
    }

    fclose(f);

    printf("Part 1: %ld\n", part1);
    printf("Part 2: %ld\n", part2);

    return 0;
}