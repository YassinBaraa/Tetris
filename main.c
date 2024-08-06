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
#define ROWS 10 /*20 */
#define COLS 10
int grid[ROWS][COLS] = {0};
bool game_end = false;
bool landded = true;
bool collision = false;

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
    //nacin da zadrzim shape blocka
 */

void update_grid()
{
    sleep(1);
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

void turn_all_ones_into_twos()
{
    for (int i = ROWS - 1; i > -1; i--)
    {
        for (int j = COLS - 1; j > -1; j--)
        {
            if (grid[i][j] == 1)
            {
                grid[i + 1][j] = 2;
                grid[i][j] = 0;
            }
        }
    }
}

bool check_collision()
{

    // collision check goes here
    for (int i = ROWS - 1; i > -1; i--)
    {
        for (int j = COLS - 1; j > -1; j--)
        {
            // stop if you touch a landed block                              stop if you are at the end of grid
            if ((i != ROWS - 1 && grid[i][j] == 1 && grid[i + 1][j] == 2) || (grid[i][j] == 1 && i == ROWS - 1))
            {
                // problem 2 : landing the blocks without footing
                grid[i][j] = 2;
                turn_all_ones_into_twos();
                landded = true;
                collision = true;
                return collision;
            }
        }
    }

    collision = false;
    return false;
}

void if_block_drop()
{
    for (int i = ROWS - 1; i > -1; i--)
    {
        for (int j = COLS - 1; j > -1; j--)
        {
            if (grid[i][j] == 1 && check_collision() == false /* && grid[i + 1][j] != 2 && grid[i + 1][j] != ROWS - 1*/)
            {
                grid[i + 1][j] = grid[i][j];
                grid[i][j] = 0;
            }
            // treba nadodati neki else.
        }
    }
}

// idea: hidden polja kao assistencija za init
void add_block(int *block, int row_num, int col_num)
{
    //*((arr+i*n) + j)
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

        /////////print updated grid
        update_grid();
    }
}

void run()
{
    // if statements koji podesavaju usmjerenje i brzinu spustanja
    srand(time(0));
    if (landded == true)
    {
        printf("add block\n");
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

    while (collision == false)
    {
        if_block_drop();
        update_grid();
    }
    printf("left\n");
}

void start_game()
{
    while (game_end == false) // podesiti condition da se zaustavi kada je predena granica
    // dodati function za uvijet check_game_end
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
    printf("\n");

    start_game();
    printf("%s", "game endded");
}