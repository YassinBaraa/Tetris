#include <stdio.h>

#define ROWS 10
#define COLS 20

int main()
{
    int grid[ROWS][COLS] = {0};

    printf("======= TETRIS=======\n");
    for (int i = 0; i < COLS; ++i)
    {
        for (int j = 0; j < ROWS; ++j)
        {
            printf("%d ", grid[j][i]);
            /*if (grid[j][i] == 0)
            {
                printf("%c", 254);
            }*/
        }
        printf("\n");
    }
}