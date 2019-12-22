#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define FPS 1

enum state {DEAD, ALIVE};

typedef struct grid {
    enum state **ground;
    int width;
    int height;
} Grid;


void initGrid(Grid *, int, int);

void freeGrid(Grid *);

int countNeighbors(const Grid, int, int);

void step(Grid *);

void printGrid(const Grid);

int main(int argc, char* argv[]) {
    int height = 0;
    int width = 0;

    if (argc < 3) {
        printf("Usage: %s <width> <height>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    width = atoi(argv[1]);
    height = atoi(argv[2]);

    Grid *grid = malloc(sizeof(Grid));
    initGrid(grid, width, height);

    grid->ground[0][1] = ALIVE;
    grid->ground[1][2] = ALIVE;
    grid->ground[2][2] = ALIVE;
    grid->ground[2][1] = ALIVE;
    grid->ground[2][0] = ALIVE;

    while (1) {
        printGrid(*grid);
        step(grid);
        sleep(1/FPS);
    }

    freeGrid(grid);
}

void initGrid(Grid *grid, int width, int height) {
    if (width < 1 || height < 1)
        exit(EXIT_FAILURE);

    grid->ground = malloc(sizeof(enum state *) * height);
    grid->height = height;
    grid->width = width;


    for (int i = 0; i < height; ++i) {
        grid->ground[i] = malloc(sizeof(enum state) * width);
        for (int j = 0; j < width; ++j) {
            grid->ground[i][j] = DEAD;
        }
    }
}

void freeGrid(Grid *grid) {
    for (int i = 0; i < grid->height; ++i)
        free(grid->ground[i]);
    free(grid->ground);
    free(grid);
}

int countNeighbors(const Grid grid, int row, int column) {
    int count = 0;

    if (row > 0) {
        count += grid.ground[row-1][column];
        if (column > 0) count += grid.ground[row - 1][column - 1];
        if (column < (grid.width - 1)) count += grid.ground[row - 1][column + 1];
    }
    if (row < grid.height - 1) {
        count += grid.ground[row + 1][column];
        if (column > 0) count += grid.ground[row + 1][column - 1];
        if (column < (grid.width - 1)) count += grid.ground[row + 1][column + 1];
    }

    if (column > 0) count += grid.ground[row][column - 1];
    if (column < (grid.width - 1)) count += grid.ground[row][column + 1];

    return count;
}

void step(Grid *grid) {
    Grid *next_grid = malloc(sizeof(Grid));
    initGrid(next_grid, grid->width, grid->height);

    for (int i = 0; i < grid->height; ++i) {
        for (int j = 0; j < grid->width; ++j) {
            switch (countNeighbors(*grid, i, j))
            {
            case 0:
                next_grid->ground[i][j] = DEAD;
                break;
            case 1:
                next_grid->ground[i][j] = DEAD;
                break;
            case 2:
                next_grid->ground[i][j] = grid->ground[i][j];
                break;
            case 3:
                next_grid->ground[i][j] = ALIVE;
                break;
            default:
                next_grid->ground[i][j] = DEAD;
                break;
            }
        }
    }

    for (int i = 0; i < grid->height; ++i)
        free(grid->ground[i]);
    free(grid->ground);

    grid->ground = next_grid->ground;

    free(next_grid);
}

void printGrid(const Grid grid) {
    system("clear");
    for (int i = 0; i < grid.height; ++i) {
        for (int j = 0; j < grid.width; ++j)
            printf("%c", grid.ground[i][j] == ALIVE ? 'O' : '_');
        printf("\n");
    }
}
