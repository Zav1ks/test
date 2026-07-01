// начало структуры: include
#include <stdio.h>
#include <stdint.h>
// конец структуры: include

// начало структуры: define (без влияния)
#define APP_NAME "C-TEST"
// конец структуры: define (без влияния)


// начало структуры: define (с влиянием на код)
#define SQUARE(x) ((x) * (x))
#define DEFAULT_VAL 7
// конец структуры: define (с влиянием на код)


// начало структуры: undef
#ifdef DEFAULT_VAL
#undef DEFAULT_VAL
#endif
// конец структуры: undef


// начало структуры: ifndef (с влиянием)
#ifndef TEST_GUARD_MACRO
#define TEST_GUARD_MACRO 1
#endif
// конец структуры: ifndef (с влиянием)


// начало структуры: ifndef (без влияния, ветка не влияет)
#ifndef UNDEFINED_FOR_BRANCH
#define BRANCH_VALUE 123
#else
#define BRANCH_VALUE 0
#endif
// конец структуры: ifndef (без влияния)


// начало структуры: ifdef/else/endif (с влиянием)
#ifdef APP_NAME
static const int g_app_name_len = (int)sizeof(APP_NAME) - 1;
#else
static const int g_app_name_len = 0;
#endif
// конец структуры: ifdef/else/endif (с влиянием)


// начало структуры: variable
static int g_counter = 0;
// конец структуры: variable


// начало структуры: function
static int add(int a, int b) {
    return a + b;
}
// конец структуры: function


// начало структуры: enum
enum Color {
    COLOR_RED = 1,
    COLOR_GREEN = 2,
    COLOR_BLUE = 4
};
// конец структуры: enum


// начало структуры: enumerator
enum Mode {
    MODE_AUTO = 0,
    MODE_MANUAL = 1
};
// конец структуры: enumerator


// начало структуры: typedef
typedef uint32_t u32;
// конец структуры: typedef


// начало структуры: struct
typedef struct Point {
    int x;
    int y;
} Point;
// конец структуры: struct


// начало структуры: function (с использованием struct/typedef/enum/define)
static void example_logic(void) {
    Point p = { .x = DEFAULT_VAL, .y = SQUARE(DEFAULT_VAL) };

    enum Color c = COLOR_GREEN;
    u32 v = (u32)(p.x + p.y + c);

    (void)v;
    g_counter += add(1, 2);
}
// конец структуры: function (с использованием struct/typedef/enum/define)


// начало структуры: function (main)
int main(void) {
    example_logic();
    printf("%s: %d\n", APP_NAME, g_counter);
    return 0;
}
// конец структуры: function (main)
