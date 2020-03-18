#include <soci/soci.h>
#include "user_utils.h"
#include "user.h"
#include <iostream>
#include <string>

using namespace soci;

using std::string;
using std::endl;
using std::cout;
using std::cin;


// Specialization of soci::type_conversion structure
namespace soci {

    template<>
    struct type_conversion<User> {

        // ORM requires that the base_type is values object
        typedef values base_type;

        // Database -> User Object
        static void from_base(values const& v, indicator ind, User& user) {
            user.id = v.get<int>("id");
            user.email = v.get<string>("email");
            user.first_name = v.get<string>("first_name");
            user.last_name = v.get<string>("last_name");
            user.active = v.get<int>("active");
        }
        
        // User Object -> Database
        static void to_base(const User& user, values& v, indicator& ind) {
            v.set("id", user.id);
            v.set("email", user.email);
            v.set("first_name", user.first_name);
            v.set("last_name", user.last_name);
            v.set("active", user.active);
        }
    };
};

void UserUtils::display_user_rowset(const rowset<row>& rs) {
    for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const row& row = *it;

        cout << "\tID: " << row.get<int>(0) << " (active: " << row.get<int>(4) << ")" << endl
             << "\tEmail: " << row.get<string>(1) << endl
             << "\tName: " << row.get<string>(2) << " " << row.get<string>(3) << endl
             << "\t------" << endl << endl;
    }
}

void UserUtils::fetch_user_by_email(session& sql, User& user) {
    user.reset();

    string email_address;
    cout << "Select a user by entering their email address: ";
    cin >> email_address;
    
    sql << "SELECT * FROM users WHERE email=:e", use(email_address, "e"), into(user);
}

int UserUtils::check_user_exists_with_email(session& sql, const string& email_address) {
    int id = 0;
    
    sql << "SELECT id FROM users WHERE email=:email", 
        use(email_address, "email"), into(id);
    
    return id;
}

bool UserUtils::verify_user(User& user) {
    if (user.email == "") {
        cout << "User not found!" << endl;
        return false;
    }
    
    cout << endl << "User selected from database: " << endl << endl;
    cout << static_cast<const char*>(user);
    return true;
}

void UserUtils::fetch_followees(session& sql, const User& user) {
    cout << endl;
    rowset<row> rs = (sql.prepare << "SELECT u2.id, u2.email, u2.first_name, u2.last_name, u2.active "
        "FROM users u1 "
        "INNER JOIN followings f ON u1.id=f.follower "
        "INNER JOIN users u2 ON u2.id=f.followee "
        "WHERE u1.id=:id", 
        use(user.id, "id"));
    
    display_user_rowset(rs);
}

void UserUtils::fetch_followers(session& sql, const User& user) {
    cout << endl;
    rowset<row> rs = (sql.prepare << 
        "SELECT u2.id, u2.email, u2.first_name, u2.last_name, u2.active "
        "FROM users u1 "
        "INNER JOIN followings f ON u1.id=f.followee "
        "INNER JOIN users u2 ON u2.id=f.follower "
        "WHERE u1.id=:id", 
        use(user.id, "id"));

    display_user_rowset(rs);
}

void UserUtils::send_message_to_user(session& sql, const User& sender) {    
    
    // Get an email address from user
    string email_address;
    cout << "\t> Enter recipient email address: ";
    cin >> email_address;

    // Check if email address exists and get id of recipient user
    int to_id = check_user_exists_with_email(sql, email_address); 
    if (to_id == 0) {
        cout << "\tError: User not found in database!" << endl << endl;
        return;
    }

    // Get message from user
    string message;
    cout << "\t> Enter a short message: ";
    std::getline(cin >> std::ws, message);

    // Check if message contains some text
    if (message.size() == 0) {
        cout << "\tError: Message must contain some text!" << endl << endl;
        return;
    }

    // Insert message into database
    sql << "INSERT INTO messages(to_id, from_id, message) "
           "VALUES(:to_id, :from_id, :message)",
           use(to_id, "to_id"),
           use(sender.id, "from_id"),
           use(message, "message");
    
    cout << "\tYour message has been sent!" << endl << endl;
}

void UserUtils::display_user_messages(session& sql, const User& user) {

    // Check if user has any messages
    int count = 0;
    sql << "SELECT COUNT(id) FROM messages WHERE to_id=:to_id",
        use(user.id), into(count);
    
    if (count == 0) {
        cout << "\tThis user hasen't received any messages!" << endl << endl;
        return;
    }

    // Get user's message rows
    rowset<row> rs = (sql.prepare << 
        "SELECT from_id, message "
        "FROM messages WHERE to_id=:to_id",
        use(user.id));
    
    // Display messages
    int i = 1;
    for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const row& row = *it;
        cout << "\t" << i << ". " << row.get<string>(1) << " (From User ID: " 
                << row.get<int>(0) << ")" << endl;
        ++i;
    }

    cout << endl;
}