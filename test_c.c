// Class from C++ for chech clangd

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

// Defining macros with constant value
#define PI 3.14159265359

// Defining macros with expression
#define PI (22 / 7)

// Defining parameterized macros with expression
#define CIRCLE_AREA(r) (3.14 * r * r)
#define SQUARE_AREA(s) (s * s)

// Variable

// Student data
int studentID = 15;
int studentAge = 23;
float studentFee = 75.25;
char studentGrade = 'B';
double val = 1.4521;
bool a = 10.248f;

long int d = 99998L;

void* ptr = &val;

int x = 5, y = 6, z = 50;

unsigned int x;
unsigned y = 12;
int a, b, c;
long int myVar = 0x12345678;
long z;
char first = 'a', second, third = 'c';  // first and third initialized, second not
float big_number = 6.02e+23;

// Typedef

typedef long long ll;

typedef int* ip;

typedef struct Students {
    char name[50];
    char branch[50];
    int ID_no;
} stu;

typedef struct {
    char name[50];
    char branch[50];
    int ID_no;
} stu;



// Struct

struct MyStructure {   // Structure declaration
  int myNum;           // Member (int variable)
  char myLetter;       // Member (char variable)
}; // End the structure with a semicolon

// Create a structure called myStructure
struct scholarHat {
  int myNum;
  char myLetter;
};

// Child structure declaration
struct child {
    int x;
    char c;
};

// Parent structure declaration
struct parent {
    int a;
    struct child b;
};


// Other data

union union_name {
data_type member1;
data_type member2;
 // Add more members as needed
};

enum enum_name {
 constant1,
 constant2,
 constant3,
 // Add more constants as needed
};