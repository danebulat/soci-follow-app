#ifndef USER_UTILS
#define USER_UTILS

#include <soci/soci.h>
#include "user.h"

using namespace soci;

class UserUtils {
    friend class AppInput;

public:
    static void fetch_user_by_email(session& sql, User& user);
    static bool verify_user(User& user);
    static void send_message_to_user(session& sql, const User& sender);
    static void display_user_messages(session& sql, const User& user);

private:
    static int check_user_exists_with_email(session& sql, const string& email_address);
    static void display_user_rowset(const rowset<row>& rs);
    static void fetch_followees(session& sql, const User& user);
    static void fetch_followers(session& sql, const User& user) ;
};

#endif