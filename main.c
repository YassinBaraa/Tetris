#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>

/*GRID CONFIG:
    0   empty space
    1   floating block
    2   landded block
*/

// GLOBAL VARIABLES
#define ROWS 20
#define COLS 10
int grid[ROWS][COLS] = {0};
bool game_end = false;
// bool collision = false;
bool landded = true;

int o_block[2][2] = {{1, 1}, {1, 1}};
int i_block[4][1] = {{1}, {1}, {1}, {1}};
int s_block[2][3] = {{0, 1, 1}, {1, 1, 0}};
int z_block[2][3] = {{1, 1, 0}, {0, 1, 1}};
int l_block[3][2] = {{1, 0}, {1, 0}, {1, 1}};
int j_block[3][2] = {{0, 1}, {0, 1}, {1, 1}};
int t_block[2][3] = {{1, 1, 1}, {0, 1, 0}};

// TODO:
/*
    // dodati neki block observer
    // kad se block spusti, droppaj sljedeci, tj.ponovi loop

 */

bool check_collision()
{
    return false;
}

void if_block_drop()
{
    for (int i = ROWS - 1; i > -1; i--)
    {
        for (int j = COLS - 1; j > -1; j--)
        {
            if (grid[i][j] == 1 && check_collision() == false)
            {
                grid[i + 1][j] = grid[i][j];
                grid[i][j] = 0;
            }
        }
    }
}

// idea: hidden polja kao assistencija za init
void add_block(int *block, int row_num, int col_num)
{
    //*((arr+i*n) + j)
    // for petlja za init cijelog bloka
    // ovo dolje samo inita zadnji red. treba pomaknuti red za 1 red dolje i initat sljedeci
    // for (int i = row_num - 1; i > row_num - 2; i--)
    for (int i = row_num - 1; i > -1; i--)
    {
        if_block_drop();
        for (int j = col_num - 1; j > -1; j--)
        {
            grid[0][4 + j] = *((block + i * col_num) + j);
            /*if (check_collision() == true)
            {
                return;
            }*/
        }

        // print updated grid
        for (int i = 0; i < ROWS; ++i)
        {
            for (int j = 0; j < COLS; ++j)
            {
                printf("%d ", grid[i][j]);
                /*if (grid[j][i] == 0)
                {
                    printf("%c", 254);
                }*/
            }
            printf("\n");
        }
        printf("\n");
    }
    sleep(1);

    // reset grid
    /*for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            grid[i][j] = 0;
        }
    }*/

    /*
    while (block_at_bottom == false)
    {
        drop_down();
    }*/
}

void run()
{
    // if statements koji podesavaju usmjerenje i brzinu spustanja
    // srand(time(NULL));
    if (landded == true)
    {
        landded = false;
        int value = rand() % (6 - 0 + 1) + 0;
        // printf("%d\n", value);
        switch (value)
        {
        case 0:
            add_block((int *)o_block, 2, 2);
            break;
        case 1:
            add_block((int *)i_block, 4, 1);
            break;
        case 2:
            add_block((int *)s_block, 2, 3);
            break;
        case 3:
            add_block((int *)z_block, 2, 3);
            break;
        case 4:
            add_block((int *)l_block, 3, 2);
            break;
        case 5:
            add_block((int *)j_block, 3, 2);
            break;
        case 6:
            add_block((int *)t_block, 2, 3);
            break;

        default:
            break;
        }
    }
}

void start_game()
{
    while (game_end == false) // podesiti condition da se zaustavi kada je predena granica
    {
        run();
    }
    // igra se zaustavila. kraj
}

int main()
{
    // displaying the layout grid
    printf("======= TETRIS=======\n");

    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            printf("%d ", grid[i][j]);
            /*if (grid[i][j] == 0)
            {
                printf("%c", 254);
            }*/
        }
        printf("\n");
    }

    start_game();
    printf("%s", "game endded");
}