#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y, z;
} Point;

typedef struct {
    long long dist2;
    int a, b;
} Edge;

typedef struct {
    int *parent;
    int *rank;
    int components;
} UnionFind;

/* ---------- Union-Find ---------- */

int find(UnionFind *uf, int x) {
    if (uf->parent[x] != x)
        uf->parent[x] = find(uf, uf->parent[x]);
    return uf->parent[x];
}

int unite(UnionFind *uf, int a, int b) {
    int ra = find(uf, a);
    int rb = find(uf, b);
    if (ra == rb) return 0;

    if (uf->rank[ra] < uf->rank[rb]) {
        uf->parent[ra] = rb;
    } else if (uf->rank[ra] > uf->rank[rb]) {
        uf->parent[rb] = ra;
    } else {
        uf->parent[rb] = ra;
        uf->rank[ra]++;
    }
    uf->components--;
    return 1;
}

/* ---------- Sorting ---------- */

int cmp_edge(const void *a, const void *b) {
    const Edge *ea = a;
    const Edge *eb = b;
    if (ea->dist2 < eb->dist2) return -1;
    if (ea->dist2 > eb->dist2) return 1;
    return 0;
}

/* ---------- Main ---------- */

int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        perror("input.txt");
        return 1;
    }

    /* ---- Read points dynamically ---- */
    size_t cap = 1024;
    size_t n = 0;
    Point *points = malloc(cap * sizeof(Point));

    while (1) {
        Point p;
        if (fscanf(f, "%d,%d,%d", &p.x, &p.y, &p.z) != 3)
            break;

        if (n == cap) {
            cap *= 2;
            points = realloc(points, cap * sizeof(Point));
        }
        points[n++] = p;
    }
    fclose(f);

    /* ---- Build edges ---- */
    long long edgeCount = (long long)n * (n - 1) / 2;
    Edge *edges = malloc(edgeCount * sizeof(Edge));

    long long idx = 0;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            long long dx = (long long)points[i].x - points[j].x;
            long long dy = (long long)points[i].y - points[j].y;
            long long dz = (long long)points[i].z - points[j].z;

            edges[idx++] = (Edge){
                dx * dx + dy * dy + dz * dz,
                (int)i,
                (int)j
            };
        }
    }

    qsort(edges, idx, sizeof(Edge), cmp_edge);

    /* ---- Union-Find ---- */
    UnionFind uf;
    uf.parent = malloc(n * sizeof(int));
    uf.rank = calloc(n, sizeof(int));
    uf.components = (int)n;

    for (size_t i = 0; i < n; i++)
        uf.parent[i] = (int)i;

    /* ---- Kruskal until fully connected ---- */
    for (long long i = 0; i < idx; i++) {
        if (unite(&uf, edges[i].a, edges[i].b)) {
            if (uf.components == 1) {
                long long result =
                    (long long)points[edges[i].a].x *
                    (long long)points[edges[i].b].x;

                printf("%lld\n", result);
                break;
            }
        }
    }

    free(points);
    free(edges);
    free(uf.parent);
    free(uf.rank);
    return 0;
}