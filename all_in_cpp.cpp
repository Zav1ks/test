// начало структуры: include
#include <iostream>
#include <cstdint>
// конец структуры: include

// начало структуры: define (без влияния)
#define CPP_APP_NAME "CPP-TEST"
// конец структуры: define (без влияния)


// начало структуры: define (с влиянием на код)
#define TIMES2(x) ((x) * 2)
#define DEFAULT_SCALE 3
// конец структуры: define (с влиянием на код)


// начало структуры: undef
#ifdef DEFAULT_SCALE
#undef DEFAULT_SCALE
#endif
// конец структуры: undef


// начало структуры: ifndef (с влиянием)
#ifndef CPP_TEST_GUARD
#define CPP_TEST_GUARD 1
#endif
// конец структуры: ifndef (с влиянием)


// начало структуры: ifndef (без влияния, ветка не влияет)
#ifndef UNDEFINED_FOR_BRANCH_CPP
#define BRANCH_VALUE_CPP 11
#else
#define BRANCH_VALUE_CPP 0
#endif
// конец структуры: ifndef (без влияния)


// начало структуры: else/endif (с влиянием через макро)
#ifdef CPP_APP_NAME
static const int g_app_len_cpp = (int)std::char_traits<char>::length(CPP_APP_NAME);
#else
static const int g_app_len_cpp = 0;
#endif
// конец структуры: else/endif (с влиянием через макро)


// начало структуры: namespace
namespace util {
    inline int clamp(int v, int lo, int hi) {
        return (v < lo) ? lo : (v > hi) ? hi : v;
    }
}
// конец структуры: namespace


// начало структуры: variable
static int g_value = 10;
// конец структуры: variable


// начало структуры: typedef
using u64 = std::uint64_t;
// конец структуры: typedef


// начало структуры: enum
enum class Color {
    Red = 1,
    Green = 2,
    Blue = 4
};
// конец структуры: enum


// начало структуры: enumerator
enum Mode {
    Auto = 0,
    Manual = 1
};
// конец структуры: enumerator


// начало структуры: class (constructor/destructor + method)
class Box {
public:
    // constructor
    Box(int w, int h)
        : width(w), height(h) {}

    // destructor
    ~Box() = default;

    int area() const {
        return width * height;
    }

    void bump(int delta) {
        g_value += delta;
    }

private:
    int width;
    int height;
};
// конец структуры: class (constructor/destructor + method)


// начало структуры: struct (C++ struct)
struct Point {
    int x;
    int y;
};
// конец структуры: struct (C++ struct)


// начало структуры: function
static int add(int a, int b) {
    return a + b;
}
// конец структуры: function


// начало структуры: function (использование define/typedef/enum/namespace)
static void run_cpp_examples() {
    Point p{ .x = 5, .y = TIMES2(6) };
    u64 acc = static_cast<u64>(p.x) + static_cast<u64>(p.y);

    Color c = Color::Green;
    (void)c;

    int clamped = util::clamp(42, 0, 10);
    (void)clamped;

    Box b(2, 4);
    b.bump(add(1, 2));
    (void)acc;
    (void)b.area();
}
// конец структуры: function (использование define/typedef/enum/namespace)


// начало структуры: function (main)
int main() {
    run_cpp_examples();
    std::cout << CPP_APP_NAME << ": " << g_value << "\n";
    return 0;
}
// конец структуры: function (main)
