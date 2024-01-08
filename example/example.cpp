/**
 * A simple example program with some functions to test.
 */

#include <iostream>
#include <string>

std::string get_name() {
    std::cout << "Enter your name: ";
    std::string name;
    getline(std::cin, name);
    return name;
}

bool check_name( const std::string& name ) {
    return name.size() > 0;
}

int main() {
    std::string name = get_name();
    if ( check_name( name ) ) {
        std::cout << "Hello, " << name << std::endl;
    } else {
        std::cout << "Your name did not contain any characters." << std::endl;
    }

    return 0;
}
