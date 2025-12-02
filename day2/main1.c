#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long sum_invalid_ids(const char *line) {
  long long total = 0;
  char buffer[2048];
  strncpy(buffer, line, sizeof(buffer));
  buffer[sizeof(buffer) - 1] = '\0';

  char *token = strtok(buffer, ",");
  while (token) {
    long long lo, hi;
    if (sscanf(token, "%11lld-%11lld", &lo, &hi) == 2) {

      int max_digits = snprintf(NULL, 0, "%11lld", hi);

      for (int n = 1; n <= max_digits / 2; n++) {
        long long pow10n = (long long)pow(10, n);

        long long xMinValid = (long long)pow(10, n - 1);
        long long xMaxValid = (long long)pow(10, n) - 1;

        long long denom = pow10n + 1;

        long long xMin = (lo + denom - 1) / denom;
        long long xMax = hi / denom;

        if (xMax < xMinValid || xMin > xMaxValid) {
          token = strtok(NULL, ",");
          continue;
        }

        if (xMin < xMinValid)
          xMin = xMinValid;
        if (xMax > xMaxValid)
          xMax = xMaxValid;

        if (xMin <= xMax) {
          long long count = xMax - xMin + 1;
          long long sumX = (xMin + xMax) * count / 2;
          total += sumX * denom;
        }
      }
    }
    token = strtok(NULL, ",");
  }

  return total;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  if (!fp) {
    perror("input.txt");
    return 1;
  }

  char line[4096];
  if (!fgets(line, sizeof(line), fp)) {
    fprintf(stderr, "input.txt is empty\n");
    return 1;
  }
  fclose(fp);

  line[strcspn(line, "\r\n")] = 0;

  long long result = sum_invalid_ids(line);
  printf("%11lld\n", result);

  return 0;
}
