#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to hold the grid and its dimensions
typedef struct {
    char** data;
    int height;
    int* row_lengths;
} Grid;

// Function to load the grid from a file
Grid load_grid(const char* path);

// Function to count accessible rolls
int count_accessible_rolls(const Grid* grid);

// Function to free the memory allocated for the grid
void free_grid(Grid* grid);

int main() {
    Grid grid = load_grid("input.txt");
    if (grid.data == NULL) {
        // Error handling is done within load_grid
        return 1;
    }

    int result = count_accessible_rolls(&grid);
    printf("%d\n", result);

    free_grid(&grid);

    return 0;
}

Grid load_grid(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Could not open file");
        return (Grid){NULL, 0, NULL};
    }

    int capacity = 10;
    Grid grid;
    grid.height = 0;
    grid.data = malloc(capacity * sizeof(char*));
    grid.row_lengths = malloc(capacity * sizeof(int));

    if (grid.data == NULL || grid.row_lengths == NULL) {
        perror("Failed to allocate initial grid memory");
        fclose(file);
        free(grid.data);
        free(grid.row_lengths);
        return (Grid){NULL, 0, NULL};
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        // Resize grid if necessary
        if (grid.height >= capacity) {
            capacity *= 2;
            char** new_data = realloc(grid.data, capacity * sizeof(char*));
            int* new_row_lengths = realloc(grid.row_lengths, capacity * sizeof(int));

            if (new_data == NULL || new_row_lengths == NULL) {
                perror("Failed to reallocate grid memory");
                free(line);
                fclose(file);
                // Free previously allocated memory before returning
                for (int i = 0; i < grid.height; i++) {
                    free(grid.data[i]);
                }
                free(grid.data);
                free(grid.row_lengths);
                return (Grid){NULL, 0, NULL};
            }
            grid.data = new_data;
            grid.row_lengths = new_row_lengths;
        }
        
        // Strip newline characters
        if (read > 0 && (line[read - 1] == '\n' || line[read - 1] == '\r')) {
            line[read - 1] = '\0';
            read--;
        }
        if (read > 0 && (line[read - 1] == '\r')) {
            line[read - 1] = '\0';
            read--;
        }

        if (strlen(line) > 0) {
            grid.data[grid.height] = malloc((strlen(line) + 1) * sizeof(char));
            if (grid.data[grid.height] == NULL) {
                 perror("Failed to allocate memory for grid row");
                 // free previously allocated memory
                 free(line);
                 fclose(file);
                 for (int i = 0; i < grid.height; i++) {
                    free(grid.data[i]);
                 }
                 free(grid.data);
                 free(grid.row_lengths);
                 return (Grid){NULL, 0, NULL};
            }
            strcpy(grid.data[grid.height], line);
            grid.row_lengths[grid.height] = strlen(line);
            grid.height++;
        }
    }

    free(line);
    fclose(file);

    return grid;
}

int count_accessible_rolls(const Grid* grid) {
    if (grid == NULL || grid->data == NULL || grid->height == 0) {
        return 0;
    }

    int accessible = 0;
    const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},  /*  */  {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (int r = 0; r < grid->height; r++) {
        for (int c = 0; c < grid->row_lengths[r]; c++) {
            if (grid->data[r][c] != '@') {
                continue;
            }

            int adjacent = 0;
            for (int i = 0; i < 8; i++) {
                int nr = r + directions[i][0];
                int nc = c + directions[i][1];

                if (nr >= 0 && nr < grid->height && nc >= 0 && nc < grid->row_lengths[nr]) {
                    if (grid->data[nr][nc] == '@') {
                        adjacent++;
                    }
                }
            }

            if (adjacent < 4) {
                accessible++;
            }
        }
    }

    return accessible;
}

void free_grid(Grid* grid) {
    if (grid != NULL && grid->data != NULL) {
        for (int i = 0; i < grid->height; i++) {
            free(grid->data[i]);
        }
        free(grid->data);
        free(grid->row_lengths);
        grid->data = NULL;
        grid->row_lengths = NULL;
        grid->height = 0;
    }
}
