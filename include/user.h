#ifndef USER_H
#define USER_H

#include <string>

using std::string;
using std::ostringstream;

class User {
public:
    int id;
    string email;
    string first_name;
    string last_name;
    int active;

private:
    string user_as_string;

public:
    const char* get_user_string();
    void reset();
    explicit operator const char*();
};

#endif