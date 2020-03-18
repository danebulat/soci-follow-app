/* App functions:
   x Select user based on ID or email address
   x View user's followers
   x View user's followees
   x View messages
   x Send a message to another user
 */

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include "user.h"
#include "user_utils.h"
#include <exception>
#include <iostream>
#include <string>

using namespace soci;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::string;

// Application class to process input
class AppInput {

    enum UserOperations {
        EXIT=0,
        FETCH_FOLLOWEES,
        FETCH_FOLLOWERS,
        VIEW_MESSAGES,
        SEND_MESSAGE,
        CHANGE_USER,
        DISPLAY_INPUT_OPTIONS = 9
    };

public:
    static void display_input_options() {
        cout << endl << "\t1 - List followees (users that are being followed by this user)" 
            << endl << "\t2 - List followers (users that are following this user)" 
            << endl << "\t3 - View messages"
            << endl << "\t4 - Send a message"
            << endl << "\t5 - Select another user"
            << endl << "\t9 - Display input options"
            << endl << "\t0 - Exit"
            << endl << endl;
    }

    static bool process_user_input(session& sql, User& user) {

        // Get input
        cout << "\t> ";
        int input = 0;
        cin >> input;
        cout << "\tYou have selected: " << input << endl;
        
        // Process input
        switch (input) {
            case EXIT: {
                return true;
                break;
            }
            case FETCH_FOLLOWEES: {
                UserUtils::fetch_followees(sql, user);
                break;
            }
            case FETCH_FOLLOWERS: {
                UserUtils::fetch_followers(sql, user);
                break;
            }
            case DISPLAY_INPUT_OPTIONS: {
                display_input_options();
                break;
            }
            case VIEW_MESSAGES: {
                UserUtils::display_user_messages(sql, user);
                break;
            }
            case SEND_MESSAGE: {
                UserUtils::send_message_to_user(sql, user);
                break;
            }
            case CHANGE_USER: {
                UserUtils::fetch_user_by_email(sql, user);
                UserUtils::verify_user(user);
                display_input_options();
                break;
            }
            default: {
                cout << "\tError!" << endl;
                break;
            }
        } // end switch

        return false;
    }
};

int main() {
    try {

        // Connect to database
        session sql(mysql, "db=social_db user=developer password=Secure123");

        // Select user from database
        User user;
        UserUtils::fetch_user_by_email(sql, user);

        // Verify user was found and display user information
        if (!UserUtils::verify_user(user)) {
            return 1;
        }
        
        // Input options for user
        AppInput::display_input_options();

        // Start loop for user operations
        bool exit = false;
        
        while (!exit) {
            exit = AppInput::process_user_input(sql, user);
        }
    }
    catch (const mysql_soci_error& e) {
        cerr << "MySQL error: " << e.err_num_ << " " << e.what() << endl;
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << " " << e.what() << endl;
    }

    return 0;
}