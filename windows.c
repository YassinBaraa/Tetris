#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const char g_szClassName[] = "myWindowClass";

// TODO:
// delete row after tetris = DONE
// game_end detection = DONE
// score count = DONE
// moving pieces right and left = DONE
// frontend = DONE-ish
// Seperate game logic from message loop = DONE
//  pieces rotation

// PROBLEMS:
//  colission detection algorthm needs to look at the entire piece at once not bit by bit.
//          because it searches one bit by bit part of the block falls through
// Calculate points function

/*GRID CONFIG:
    0   empty space
    1   floating block
    2   landded block
*/
#define ROWS 20
#define COLS 10
int grid[ROWS][COLS] = {0};
int score = 0;
int multiplier = 1;
bool landed = true;
bool collision = false;
bool game_end = false;

// Block definitions
int o_block[2][2] = {{1, 1}, {1, 1}};
int i_block[4][1] = {{1}, {1}, {1}, {1}};
int s_block[2][3] = {{0, 1, 1}, {1, 1, 0}};
int z_block[2][3] = {{1, 1, 0}, {0, 1, 1}};
int l_block[3][2] = {{1, 0}, {1, 0}, {1, 1}};
int j_block[3][2] = {{0, 1}, {0, 1}, {1, 1}};
int t_block[2][3] = {{1, 1, 1}, {0, 1, 0}};

// Forward declarations
void update_grid(HWND hwnd);
void start_game(HWND hwnd);
void stop_game(HWND hwnd);
void run(HWND hwnd);

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
                landed = true;
                collision = true;
                return collision;
            }
        }
    }

    collision = false;
    return false;
}

void rotate_block()
{
}

void move_left()
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 1; j < COLS; ++j)
        {
            if (grid[i][j] == 1 && grid[i][j - 1] == 0 && !check_collision())
            {
                grid[i][j - 1] = 1;
                grid[i][j] = 0;
            }
        }
    }
}

void move_right()
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = COLS - 2; j > -1; j--)
        {
            if (grid[i][j] == 1 && grid[i][j + 1] == 0 && !check_collision())
            {
                grid[i][j + 1] = 1;
                grid[i][j] = 0;
            }
        }
    }
}

void calculate_points()
{
    if (score >= 5000)
    {
        multiplier = 2;
    }
    else if (score >= 2500)
    {
        multiplier = 1.5;
    }
    else if (score >= 1000)
    {
        multiplier = 1.25;
    }
    else
    {
        multiplier = 1.0;
    }
    score += (100 * multiplier);
    printf("%d\n", score);
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
        // update_grid(hwnd);
    }
}

void run(HWND hwnd)
{
    srand(time(0));
    if (landed)
    {
        landed = false;
        // int value = rand() % (6 - 0 + 1) + 0;
        int value = 0;
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

    /*while (!collision)
    {
        if_block_drop();
        tetris_check();
        update_grid(hwnd);
        Sleep(1000); // Adjust sleep time as needed
    }*/

    if (!collision)
    {
        if_block_drop();
        tetris_check();
        update_grid(hwnd);
    }
    else
    {
        collision = false;
        landed = true;
    }

    if (game_end)
    {
        stop_game(hwnd);
    }
    // handles only one game "tick" per call
    // timer separate game logic from the message loop
}

/*void start_game(HWND hwnd)
{

    while (!game_end)
    {
        run(hwnd);
    }
}*/

void start_game(HWND hwnd)
{
    SetTimer(hwnd, 1, 1000, NULL); // Set up a timer with a 1-second interval
}

void stop_game(HWND hwnd)
{
    KillTimer(hwnd, 1); // Stop the timer when the game ends
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT: // case for creating the grid in window
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // Set text color and background mode
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);

        char buffer[256];
        int y = 20;

        // Print the grid
        for (int i = 0; i < ROWS; ++i)
        {
            int x = 20;
            for (int j = 0; j < COLS; ++j)
            {
                if (grid[i][j] == 0)
                {
                    sprintf(buffer, ". ");
                    // sprintf(buffer, "%c ", 35);
                }
                else
                {
                    sprintf(buffer, "0 ");
                    // sprintf(buffer, "%c ", 219);
                }
                TextOut(hdc, x, y, buffer, strlen(buffer));
                x += 20; // Adjust spacing
            }
            y += 20; // Move to the next line
        }

        // Print the score
        sprintf(buffer, "SCORE: %4d", score);
        TextOut(hdc, 20, y + 20, buffer, strlen(buffer));

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_KEYDOWN:           // keyboard listener
        if (wParam == VK_LEFT) // left arrow key
        {
            // printf("left key detected\n");
            move_left();
        }
        else if (wParam == VK_RIGHT) // right arrow key
        {
            // printf("right key detected\n");
            move_right();
        }
        else if (wParam == VK_DOWN) // down arrow key
        {
            // printf("down key detected\n");
            if_block_drop(); // has the same properties needed for move_down function
        }
        else if (wParam == VK_SPACE)
        {
            // printf("rotation key detected\n");
            rotate_block(); // has the same properties needed for move_down function
        }
        InvalidateRect(hwnd, NULL, TRUE);
        update_grid(hwnd);
        break;
    case WM_TIMER: // timer separate game logic from the message loop
        if (!game_end)
        {
            run(hwnd);
        }
        else
        {
            stop_game(hwnd);
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void update_grid(HWND hwnd)
{
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "TETRIS",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 250, 500, // Adjust size as needed
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Start the game
    start_game(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}
// compile and run:
//  gcc -o tetris_window windows.c -lgdi32
// .\tetris_window.exe
