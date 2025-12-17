#include <stdio.h>
#include <stdlib.h>

#define MAXN 2000

typedef struct {
  long long x, y, z;
} Point;

typedef struct {
  long long dist;
  int a, b;
} Edge;

Point points[MAXN];
Edge edges[MAXN * MAXN / 2];
int parent[MAXN], size[MAXN];

int find(int x) {
  if (parent[x] != x)
    parent[x] = find(parent[x]);
  return parent[x];
}

void unite(int a, int b) {
  a = find(a);
  b = find(b);
  if (a == b)
    return;
  if (size[a] < size[b]) {
    int t = a;
    a = b;
    b = t;
  }
  parent[b] = a;
  size[a] += size[b];
}

int cmp(const void *a, const void *b) {
  long long d = ((Edge *)a)->dist - ((Edge *)b)->dist;
  return (d > 0) - (d < 0);
}

int main(void) {
  FILE *f = fopen("input.txt", "r");
  if (!f)
    return 1;

  int n = 0;
  while (fscanf(f, "%11lld,%11lld,%11lld", &points[n].x, &points[n].y,
                &points[n].z) == 3) {
    n++;
  }
  fclose(f);

  int e = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      long long dx = points[i].x - points[j].x;
      long long dy = points[i].y - points[j].y;
      long long dz = points[i].z - points[j].z;
      edges[e++] = (Edge){dx * dx + dy * dy + dz * dz, i, j};
    }
  }

  qsort(edges, e, sizeof(Edge), cmp);

  for (int i = 0; i < n; i++) {
    parent[i] = i;
    size[i] = 1;
  }

  for (int i = 0; i < 1000; i++) {
    unite(edges[i].a, edges[i].b);
  }

  int comp[MAXN] = {0};
  for (int i = 0; i < n; i++) {
    comp[find(i)]++;
  }

  int a = 0, b = 0, c = 0;
  for (int i = 0; i < n; i++) {
    int s = comp[i];
    if (s > a) {
      c = b;
      b = a;
      a = s;
    } else if (s > b) {
      c = b;
      b = s;
    } else if (s > c) {
      c = s;
    }
  }

  printf("%d\n", a * b * c);
  return 0;
}
