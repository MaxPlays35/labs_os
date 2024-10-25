//
// Created by MaxPlays on 27/09/2024.
//

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Bad usage" << std::endl;
        return -1;
    }

    FILE* file_old = fopen(argv[0], "w");
    dup2(fileno(file_old), STDOUT_FILENO);

    constexpr int one = 1;
    int zero = 0;

    std::string str;

    while(std::getline(std::cin, str)) {
        if (str == "!") {
            break;
        }

        if (str.ends_with('.') or str.ends_with(';')) {
            std::cout << str << std::endl;
            write(STDERR_FILENO, &one, sizeof(one));
        } else {
            write(STDERR_FILENO, &zero, sizeof(zero));
        }
    }

    close(fileno(file_old));

    return 0;
}
