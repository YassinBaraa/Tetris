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

// TODO:
// delete row after tetris = DONE
// game_end detection = DONE
// score count = DONE

// moving pieces right and left
// pieces rotation
// frontend

// GLOBAL VARIABLES
#define ROWS 5 /*20 */
#define COLS 10
int grid[ROWS][COLS] = {0};
int score = 0;
int multiplier = 1;
bool landded = true;
bool collision = false;
bool game_end = false;

int o_block[2][2] = {{1, 1}, {1, 1}};
int i_block[4][1] = {{1}, {1}, {1}, {1}};
int s_block[2][3] = {{0, 1, 1}, {1, 1, 0}};
int z_block[2][3] = {{1, 1, 0}, {0, 1, 1}};
int l_block[3][2] = {{1, 0}, {1, 0}, {1, 1}};
int j_block[3][2] = {{0, 1}, {0, 1}, {1, 1}};
int t_block[2][3] = {{1, 1, 1}, {0, 1, 0}};

/*bool game_end()
{
    for (int j = 0; j < COLS; ++j)
    {
        if (grid[ROWS - 1][j] == 2)
        {
            return true;
        }
    }
    return false;
}*/

void calculate_points()
{
    if (score >= 1000)
    {
        multiplier = 1.25;
    }
    else if (score >= 2500)
    {
        multiplier = 1.25;
    }
    else if (score >= 5000)
    {
        multiplier = 1.5;
    }
    else
    {
        multiplier = 1.5;
    }
    score += 100 * multiplier;
    printf("+100 points\n");
}

void tetris_check()
{
    bool tetris = true;
    for (int j = 0; j < COLS; ++j)
    {
        if (grid[ROWS - 1][j] == 0)
        {
            tetris = false;
        }
    }

    if (tetris == true)
    {
        printf("TETRIS\n");
        calculate_points();
        for (int i = ROWS - 1; i > -1; i--)
        {
            for (int j = COLS - 1; j > -1; j--)
            {
                if (grid[i][j] == 2)
                {
                    grid[i + 1][j] = grid[i][j];
                    grid[i][j] = 0;
                }
            }
        }
    }
}

void update_grid()
{
    sleep(1);
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            // printf("%d ", grid[i][j]);
            if (grid[i][j] == 0)
            {
                printf("%c ", 35);
            }
            else
            {
                printf("%c ", 254);
            }
        }
        printf("\n");
    }
    printf("SCORE: %d \n", score);
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
            if (grid[0][(COLS / 2) - 1 + j] == 2)
            {
                game_end = true;
                return;
            }
            else
            {
                // grid[0][4 + j] = *((block + i * col_num) + j);
                grid[0][(COLS / 2) - 1 + j] = *((block + i * col_num) + j);
            }
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
        tetris_check();
        update_grid();
    }
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
    update_grid();

    /*
        // tetris test zadnji red je pun
        for (int i = ROWS - 1; i > ROWS - 2; i--)
        {
            for (int j = COLS - 1; j > -1; j--)
            {
                grid[i][j] = 2;
            }
        }
    */
    start_game();
    printf("%s", "game endded");
}