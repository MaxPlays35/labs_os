//
// Created by Aleksandr Mikhailov on 04.12.2024.
//

#include <istream>
#include <ostream>
#include <dlfcn.h>
#include <iostream>

typedef float(*piPointer)(int k);
typedef std::string(*translationPtr)(long x);

const inline std::string lib1 = "./liblab_4_first_lib.dylib";
const inline std::string lib2 = "./liblab_4_second_lib.dylib";


void dynamic(std::istream & in, std::ostream & out) {
    void* lib = dlopen(lib1.c_str(), RTLD_LAZY);
    if (!lib) {
        std::cerr << dlerror() << std::endl;
        return;
    }

    auto pi = reinterpret_cast<piPointer>(dlsym(lib, "pi"));
    auto translation = reinterpret_cast<translationPtr>(dlsym(lib, "translation"));

    int option = 0;
    int argument = 0;
    bool flag = false;

    while (true) {
        out << "Please enter an option: ";
        in >> option;
        switch (option) {
            case -1:
                out << "Exiting\n";
                dlclose(lib);
                return;
            case 0:
                flag = !flag;
                dlclose(lib);

                if (flag) {
                    lib = dlopen(lib2.c_str(), RTLD_LAZY);

                    if (!lib) {
                        std::cerr << dlerror() << std::endl;
                        return;
                    }

                    std::cout << "Changed library to: " << lib2 << std::endl;
                } else {
                    lib = dlopen(lib2.c_str(), RTLD_LAZY);

                    if (!lib) {
                        std::cerr << dlerror() << std::endl;
                        return;
                    }

                    std::cout << "Changed library to: " << lib1 << std::endl;
                }

                pi = reinterpret_cast<piPointer>(dlsym(lib, "pi"));
                translation = reinterpret_cast<translationPtr>(dlsym(lib, "translation"));
                break;
            case 1:
                in >> argument;
                out << "The result of calculation: " << pi(argument) << std::endl;
                break;
            case 2:
                in >> argument;
                out << "The result of translation: " << translation(argument) << std::endl;
                break;
            default:
                out << "Invalid option!" << std::endl;
                break;
        }
    }
}

int main() {
    dynamic(std::cin, std::cout);

    return 0;
}
