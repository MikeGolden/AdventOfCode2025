#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 10000
#define MAX_IDS 100000
#define LINE_LEN 128

typedef struct {
  int start;
  int end;
} Range;

bool contains(const Range *r, int value) {
  return value >= r->start && value <= r->end;
}

int main(void) {
  FILE *file = fopen("input.txt", "r");
  if (!file) {
    perror("Failed to open input.txt");
    return 1;
  }

  Range ranges[MAX_RANGES];
  int ids[MAX_IDS];

  int range_count = 0;
  int id_count = 0;
  bool parsing_ranges = true;

  char line[LINE_LEN];

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
      parsing_ranges = false;
      continue;
    }

    if (parsing_ranges) {
      sscanf(line, "%d-%d", &ranges[range_count].start,
             &ranges[range_count].end);
      range_count++;
    } else {
      ids[id_count++] = atoi(line);
    }
  }

  fclose(file);

  int fresh_count = 0;

  for (int i = 0; i < id_count; i++) {
    for (int j = 0; j < range_count; j++) {
      if (contains(&ranges[j], ids[i])) {
        fresh_count++;
        break;
      }
    }
  }

  printf("%d\n", fresh_count);
  return 0;
}
