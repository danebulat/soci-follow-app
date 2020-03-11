
#include "user.h"
#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;
using std::endl;


const char* User::get_user_string() {
    return user_as_string.c_str();
}

void User::reset() {
    id = 0;
    email = first_name = last_name = "";
    active = false;
}

User::operator const char*() {
    ostringstream oss;
    oss << "ID: " << id << " (active: " << active << ")"  << endl 
        << "Email: " << email << endl 
        << "Name: " << first_name << " " << last_name << endl;
    
    user_as_string = oss.str();
    return user_as_string.c_str();
}