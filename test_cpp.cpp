// Class 
class Car {
private:
    std::string brand;
    std::string model;
    int year;

public:
    // Constructor
    Car(std::string b, std::string m, int y) {
        brand = b;
        model = m;
        year = y;
    }

    // Member method to display car details
    void displayDetails() {
        std::cout << "Brand: " << brand << std::endl;
        std::cout << "Model: " << model << std::endl;
        std::cout << "Year: " << year << std::endl;
    }

    // Getter method for brand
    std::string getBrand() {
        return brand;
    }
};


// Define

#define fori10 for (int i=0; i<=10; i++)

// some code...

fori10 {
    // do stuff to i
}

// compile the following if debugging is turned on and defined
#ifdef DEBUG
// some code
#endif


#define DEBUG

// Variable

int myNum = 5;               // Integer (whole number without decimals)

double myFloatNum = 5.99;    // Floating point number (with decimals)

char myLetter = 'D';         // Character

string myText = "Hello";     // String (text)

bool myBoolean = true;       // Boolean (true or false)

int myNum = 5;               // Integer (whole number without decimals)
double myFloatNum = 5.99;    // Floating point number (with decimals)
char myLetter = 'D';         // Character
string myText = "Hello";     // String (text)
bool myBoolean = true;       // Boolean (true or false)

// Typedef

// simple typedef
typedef unsigned long ulong;
 
// more complicated typedef
typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];

 
// common C idiom to avoid having to write "struct S"
typedef struct { int a; int b; } S, *pS;
 
 
// error: storage-class-specifier cannot appear in a typedef declaration
// typedef static unsigned int uint;
 
// typedef can be used anywhere in the decl-specifier-seq
long unsigned typedef int long ullong;
// more conventionally spelled "typedef unsigned long long int ullong;"
 
// std::add_const, like many other metafunctions, use member typedefs
template<class T>
struct add_const
{
    typedef const T type;
};
 
typedef struct Node
{
    struct listNode* next; // declares a new (incomplete) struct type named listNode
} listNode; // error: conflicts with the previously declared struct name
 
// C++20 error: "struct with typedef name for linkage" has member functions
typedef struct { void f() {} } C_Incompatible;

// Struct

struct {
  string brand;
  string model;
  int year;
} myCar1, myCar2; // We can add variables by separating them with a comma here

struct car {  // This structure is now named "car"
  string brand;
  string model;
  int year;
};