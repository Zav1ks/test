void ImitFailBCVM (void);

int main() {
    // Create an object of the Car class
    Car myCar("Toyota", "Camry", 2022);
    // TEST test

    // Call a member method to display details
    myCar.displayDetails();
    // Access a member variable using a getter method
    std::cout << "My car's brand is: " << myCar.getBrand() << std::endl;

    return 0;
}
