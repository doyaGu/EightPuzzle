/*
    -------------
    | A | B | C |
    -------------
    | D | E | F |
    -------------
    | G | H | I |
    -------------
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #define DEBUG 1
#define GRID_SIZE 9

typedef struct grid_
{
    int data[GRID_SIZE];
    short step;
    short result;
} grid_t;

typedef enum pos_
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    END = -1
} pos_t;

inline int get(grid_t *grid, pos_t pos)
{
    return grid->data[pos];
}

inline void set(grid_t *grid, pos_t pos, int value)
{
    grid->data[pos] = value;
}

inline int *at(grid_t *grid, pos_t pos)
{
    return &grid->data[pos];
}

inline void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

inline int find(const int nums[], int n, int value)
{
    int i = 0;
    for (; nums[i] != value && i < n; i++)
        continue;
    return i;
}

void rotate_left(int nums[], int n, int k)
{
    k %= n;
    int count = 0;
    for (int start = n - 1; count < n; start--)
    {
        int cur = start;
        int prev = nums[start];
        do
        {
            int next = (n + (cur - k)) % n;
            swap(&nums[next], &prev);
            cur = next;
            count++;
        } while (start != cur);
    }
}

void input(grid_t *board)
{
    if (!board)
        return;

    puts("Input the pieces by order:");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        scanf("%d", at(board, i));
    }
}

void println_nums(int nums[], int n)
{
    int last = n - 1;
    printf("[");
    for (int i = 0; i != last; i++)
        printf("%d, ", nums[i]);
    printf("%d]\n", nums[last]);
}

void println_board(grid_t *board)
{
    if (!board)
        return;

    puts("-------------");
    printf("| %d | %d | %d |\n", get(board, A), get(board, B), get(board, C));
    puts("-------------");
    printf("| %d | %d | %d |\n", get(board, D), get(board, E), get(board, F));
    puts("-------------");
    printf("| %d | %d | %d |\n", get(board, G), get(board, H), get(board, I));
    puts("-------------");
}

bool dummy_cond(grid_t *board)
{
    return true;
}

bool step1_cond(grid_t *board)
{
    return get(board, C) == 1 || get(board, C) == 0;
}

void move_until(grid_t *board, const pos_t route[], bool (*cond_func)(grid_t *))
{
    if (!board)
        return;

    if (cond_func == NULL)
        cond_func = &dummy_cond;

    int n = 0;
    for (; route[n] != END; n++)
        continue;
    if (n == 0)
        return;

    int zero = find(at(board, 0), GRID_SIZE, 0);
    int next = (find(route, n, zero) + 1) % n;
    for (int i = zero, j = route[next]; cond_func(board) && j != END; i = j, j = route[++next])
        swap(at(board, i), at(board, j));
}

void move_to(grid_t *board, const pos_t route[], int id, pos_t pos)
{
    if (!board || id <= 0)
        return;

    int n = 0;            // 路径长度
    int stage[GRID_SIZE]; // 暂存路径，最长路径默认为 GRID_SIZE
    for (int i = route[0]; i != END; i = route[++n])
        stage[n] = get(board, i);
    if (n == 0)
        return;
#ifdef DEBUG
    println_nums(stage, n);
#endif

    int i = find(route, n, pos); // 目标位置坐标
    int j = find(stage, n, id);  // 目标棋子坐标
    int offset = (i < j) ? (j - i) : (j + (n - i));
    rotate_left(stage, n, offset - 1);
#ifdef DEBUG
    println_nums(stage, n);
#endif

    // 完成后续移动
    for (int k = find(stage, n, 0); stage[i] != id || stage[(i + 1) % n] != 0; k++)
        swap(&stage[k % n], &stage[(k + 1) % n]);
#ifdef DEBUG
    println_nums(stage, n);
#endif

    // 按路径回填
    for (i = route[0], j = 0; i != END; i = route[++j])
        set(board, i, stage[j]);
}

int step(grid_t *board)
{
    if (!board)
        return board->step;

    switch (board->step)
    {
    case 0:
        board->step = 1;
        break;
    case 1:
        board->step = 2;
        break;
    case 2:
        board->step = 3;
        break;
    case 3:
        board->step = (get(board, C) != 3) ? 4 : 6;
        break;
    case 4:
        board->step = 5;
        break;
    case 5:
        board->step = 6;
        break;
    case 6:
        board->step = (get(board, I) != 5) ? 7 : 9;
        break;
    case 7:
        board->step = 8;
        break;
    case 8:
        board->step = 9;
        break;
    case 9:
        board->step = 0;
        break;
    default:
        break;
    }

    return board->step;
}

void execute(grid_t *board)
{
    static const pos_t route1[] = {I, H, G, D, A, B, C, F, E, END};
    static const pos_t route2[] = {D, G, H, I, F, E, B, A, END};
    static const pos_t route3[] = {F, C, B, E, D, G, H, I, END};
    static const pos_t route4[] = {D, G, H, I, F, E, END};
    static const pos_t route5[] = {D, A, B, E, F, C, B, A, D, END};
    static const pos_t route6[] = {E, D, G, H, I, F, END};
    static const pos_t route7[] = {G, H, E, D, END};
    static const pos_t route8[] = {D, A, B, C, F, E, H, I, F, C, B, A, D, END};
    static const pos_t route9[] = {D, G, H, E, END};

    if (!board)
        return;

    switch (board->step)
    {
    case 0:
    case 1:
        board->result = 0;
        move_until(board, route1, &step1_cond);
        break;
    case 2:
        move_to(board, route2, 1, A);
        break;
    case 3:
        move_to(board, route3, 2, B);
        break;
    case 4:
        move_to(board, route4, 3, E);
        break;
    case 5:
        move_until(board, route5, NULL);
        break;
    case 6:
        move_to(board, route6, 4, F);
        break;
    case 7:
        move_to(board, route7, 5, E);
        break;
    case 8:
        move_until(board, route8, NULL);
        break;
    case 9:
        move_to(board, route9, 6, H);
        board->result = (get(board, G) == 7 && get(board, D) == 8) ? 1 : 0;
        break;
    default:
        break;
    }
}

void process(grid_t *board)
{
    if (!board)
        return;

    puts("Start:");
    while (step(board) != 0)
    {
        println_board(board);
        execute(board);
        printf("Step %d:\n", board->step);
    }
    println_board(board);
    puts((board->result) ? "Success" : "Failed");
}

int main()
{
    grid_t board = {{2, 0, 1, 4, 6, 5, 3, 7, 8}};
    // grid_t board = {{2, 8, 3, 1, 6, 4, 7, 0, 5}};

    // input(&board);
    process(&board);

    return 0;
}
