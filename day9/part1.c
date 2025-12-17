#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} Point;

int main(void) {
  FILE *f = fopen("input.txt", "r");
  if (!f) {
    perror("input.txt");
    return 1;
  }

  size_t cap = 1024;
  size_t n = 0;
  Point *points = malloc(cap * sizeof(Point));

  while (1) {
    Point p;
    if (fscanf(f, "%d,%d", &p.x, &p.y) != 2)
      break;

    if (n == cap) {
      cap *= 2;
      points = realloc(points, cap * sizeof(Point));
    }
    points[n++] = p;
  }
  fclose(f);

  long long max_area = 0;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      long long area = (long long)(abs(points[i].x - points[j].x) + 1) *
                       (long long)(abs(points[i].y - points[j].y) + 1);

      if (area > max_area)
        max_area = area;
    }
  }

  printf("%11lld\n", max_area);

  free(points);
  return 0;
}
