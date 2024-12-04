//
// Created by Aleksandr Mikhailov on 22.11.2024.
//

#include <string>

class SharedFile {
private:
    int fd_;
    const std::string name_;
public:
    explicit SharedFile(std::string name);

    int getFd() const;

    ~SharedFile();
};

