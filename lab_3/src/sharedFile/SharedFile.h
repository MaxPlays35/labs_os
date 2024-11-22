//
// Created by Aleksandr Mikhailov on 22.11.2024.
//

#include <string>

class SharedFile {
private:
    int fd_;
    std::string name_;
public:
    explicit SharedFile(const std::string & name);

    int getFd() const;

    ~SharedFile();
};

