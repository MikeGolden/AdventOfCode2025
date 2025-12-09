/* part2.c
 * Select exactly K digits per bank (K=12) to maximize the resulting K-digit
 * number (preserving order). Uses a monotonic stack greedy algorithm.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define K 12

// We use unsigned __int128 for accumulation to be robust for large values.
// Helper to print __int128.
static void uint128_to_string(unsigned __int128 v, char *buf, size_t bufsz) {
  if (bufsz == 0)
    return;
  if (v == 0) {
    strncpy(buf, "0", bufsz);
    buf[bufsz - 1] = '\0';
    return;
  }
  char tmp[64];
  int pos = 0;
  while (v > 0) {
    int digit = (int)(v % 10);
    tmp[pos++] = '0' + digit;
    v /= 10;
  }
  int i;
  int outpos = 0;
  for (i = pos - 1; i >= 0 && outpos + 1 < (int)bufsz; --i) {
    buf[outpos++] = tmp[i];
  }
  buf[outpos] = '\0';
}

typedef struct {
  unsigned __int128 value;
  char digits[K + 1]; // chosen digits string (null terminated)
  int indices[K];
  int len;
} Choice;

int best_k(const char *line, int k, Choice *out) {
  // Trim newline
  int len = (int)strlen(line);
  while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
    len--;
  // skip leading whitespace
  int start = 0;
  while (start < len && isspace((unsigned char)line[start]))
    start++;
  if (start >= len) {
    out->value = 0;
    out->digits[0] = '\0';
    out->len = 0;
    return 0;
  }
  int n = len - start;
  const char *s = line + start;
  for (int i = 0; i < n; ++i) {
    if (!isdigit((unsigned char)s[i]))
      return -1;
  }
  if (k > n)
    return -1;

  int toRemove = n - k;
  char *stackDigits = (char *)malloc(n);
  int *stackIdx = (int *)malloc(n * sizeof(int));
  if (!stackDigits || !stackIdx) {
    free(stackDigits);
    free(stackIdx);
    return -1;
  }
  int stackLen = 0;

  for (int i = 0; i < n; ++i) {
    char d = s[i];
    while (stackLen > 0 && toRemove > 0 && stackDigits[stackLen - 1] < d) {
      stackLen--;
      toRemove--;
    }
    stackDigits[stackLen] = d;
    stackIdx[stackLen] = i;
    stackLen++;
  }
  if (toRemove > 0) {
    stackLen -= toRemove;
  }
  // choose first k
  unsigned __int128 val = 0;
  for (int i = 0; i < k; ++i) {
    out->digits[i] = stackDigits[i];
    out->indices[i] = stackIdx[i];
    val = val * 10 + (unsigned __int128)(stackDigits[i] - '0');
  }
  out->digits[k] = '\0';
  out->value = val;
  out->len = k;
  free(stackDigits);
  free(stackIdx);
  return 0;
}

int main(void) {
  FILE *f = fopen("input.txt", "r");
  if (!f) {
    perror("open input.txt");
    return 1;
  }

  char line[MAX_LINE];
  Choice choices[10000];
  int count = 0;
  while (fgets(line, sizeof line, f)) {
    // skip empty lines
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
    if (best_k(line, K, &choices[count]) != 0) {
      fprintf(stderr, "error processing line %d\n", count + 1);
      return 2;
    }
    count++;
  }
  fclose(f);

  unsigned __int128 total = 0;
  char buf[128];
  for (int i = 0; i < count; ++i) {
    uint128_to_string(choices[i].value, buf, sizeof buf);
    printf("Bank %d: value=%s  digits=%s  indices=[", i + 1, buf,
           choices[i].digits);
    for (int j = 0; j < choices[i].len; ++j) {
      if (j)
        printf(", ");
      printf("%d", choices[i].indices[j]);
    }
    printf("]\n");
    total += choices[i].value;
  }
  printf("---------------------------------------------------------------------"
         "-\n");
  uint128_to_string(total, buf, sizeof buf);
  printf("Total joltage (sum of per-bank %d-digit maxima): %s\n", K, buf);
  return 0;
}
