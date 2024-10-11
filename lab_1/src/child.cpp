//
// Created by MaxPlays on 27/09/2024.
//

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char* argv[]) {
    FILE* file_old = fopen(argv[0], "w");
    dup2(fileno(file_old), 1);

    int one = 1;
    int zero = 0;

    std::string str;

    while(std::getline(std::cin, str)) {
        if (str == "!") {
            break;
        }

        if (str.ends_with('.') or str.ends_with(';')) {
            std::cout << str << std::endl;
            write(2, &one, sizeof(one));
        } else {
            write(2, &zero, sizeof(zero));
        }
    }

    close(fileno(file_old));

    return 0;
}
