/* part1.c
 * Select exactly 2 digits per bank to maximize 10*tens + units.
 * Usage: gcc -O2 -std=c11 part1.c -o part1 && ./part1
 * Reads input.txt in current directory.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

typedef struct {
  int value;
  char digits[3]; // two digits + NUL
  int tens_idx;
  int units_idx;
} BankChoice;

int best_two(const char *line, BankChoice *out) {
  // Trim newline and whitespace
  int len = (int)strlen(line);
  while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
    len--;
  while (len > 0 && isspace((unsigned char)line[0])) {
    line++;
    len--;
  }
  if (len <= 0) {
    out->value = 0;
    out->digits[0] = '\0';
    out->tens_idx = out->units_idx = -1;
    return 0;
  }
  // Validate and copy
  for (int i = 0; i < len; ++i) {
    if (!isdigit((unsigned char)line[i]))
      return -1;
  }
  // Build suffix max and index
  char *s = (char *)malloc(len + 1);
  if (!s)
    return -1;
  memcpy(s, line, len);
  s[len] = '\0';

  char *suffix_max = (char *)malloc(len);
  int *suffix_idx = (int *)malloc(len * sizeof(int));
  if (!suffix_max || !suffix_idx) {
    free(s);
    free(suffix_max);
    free(suffix_idx);
    return -1;
  }

  suffix_max[len - 1] = s[len - 1];
  suffix_idx[len - 1] = len - 1;
  for (int i = len - 2; i >= 0; --i) {
    char right = s[i + 1];
    if (right >= suffix_max[i + 1]) {
      suffix_max[i] = right;
      suffix_idx[i] = i + 1;
    } else {
      suffix_max[i] = suffix_max[i + 1];
      suffix_idx[i] = suffix_idx[i + 1];
    }
  }

  int bestVal = -1;
  int bi = -1, bj = -1;
  for (int i = 0; i < len - 1; ++i) {
    int tens = s[i] - '0';
    int units = suffix_max[i] - '0';
    int val = tens * 10 + units;
    if (val > bestVal) {
      bestVal = val;
      bi = i;
      bj = suffix_idx[i];
    }
  }
  if (bi < 0) {
    out->value = 0;
    out->digits[0] = '\0';
    out->tens_idx = out->units_idx = -1;
  } else {
    out->value = bestVal;
    out->digits[0] = s[bi];
    out->digits[1] = s[bj];
    out->digits[2] = '\0';
    out->tens_idx = bi;
    out->units_idx = bj;
  }
  free(s);
  free(suffix_max);
  free(suffix_idx);
  return 0;
}

int main(void) {
  FILE *f = fopen("input.txt", "r");
  if (!f) {
    perror("open input.txt");
    return 1;
  }

  char line[MAX_LINE];
  BankChoice choices[10000];
  int count = 0;
  while (fgets(line, sizeof line, f)) {
    int onlyws = 1;
    for (char *p = line; *p; ++p)
      if (!isspace((unsigned char)*p)) {
        onlyws = 0;
        break;
      }
    if (onlyws)
      continue;
    if (count >= (int)(sizeof choices / sizeof choices[0])) {
      fprintf(stderr, "too many lines\n");
      break;
    }
    if (best_two(line, &choices[count]) != 0) {
      fprintf(stderr, "error processing line %d\n", count + 1);
      return 2;
    }
    count++;
  }
  fclose(f);

  long long total = 0;
  for (int i = 0; i < count; ++i) {
    printf("Bank %d: value=%02d digits=%s indices=(%d,%d)\n", i + 1,
           choices[i].value, choices[i].digits, choices[i].tens_idx,
           choices[i].units_idx);
    total += choices[i].value;
  }
  printf("---------------------------------------------------------------------"
         "---------------------------------\n");
  printf("Total joltage: %11d\n", total);
  return 0;
}
