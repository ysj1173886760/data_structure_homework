//
// Created by sheep on 2020/12/29.
//

#include <cstdio>
#include <string>
#include <vector>
#include <editline/readline.h>
#include <editline/history.h>
#include <unordered_map>
#include "LoginUser.h"
#include "RandomGenerator.hpp"
#include "DB.h"
#include "Email.h"
#include "RegisterUser.h"
#include "RegisterSeller.h"
#include "RegisterManager.h"
#include "LoginManager.h"
#include "LoginSeller.h"

enum {
    COMMAND_QUIT = 1,
    COMMAND_LOGIN,
    COMMAND_REGISTER,
    COMMAND_USER,
    COMMAND_SELLER,
    COMMAND_MANAGER,
    COMMAND_RESEND,
    COMMAND_CHANGE,
    COMMAND_BACK,
    COMMAND_RETRY,
    COMMAND_STATUS,
    COMMAND_CLEAR,
    COMMAND_LOGOUT
};

enum Status{
    STATUS_MANAGER = 1,
    STATUS_USER,
    STATUS_SELLER,
    STATUS_TOURIST
};

std::unordered_map<std::string, int> mp;

RandomGenerator &ran_generator = RandomGenerator::getInstance();
BasicOperation BasicOp;

struct STATUS {
    std::string id;
    enum Status type;
    std::string name;
    std::string account;
}status;

void init() {
    status.type = STATUS_TOURIST;

    mp["quit"] = COMMAND_QUIT;
    mp["login"] = COMMAND_LOGIN;
    mp["register"] = COMMAND_REGISTER;
    mp["user"] = COMMAND_USER;
    mp["manager"] = COMMAND_MANAGER;
    mp["seller"] = COMMAND_SELLER;
    mp["resend"] = COMMAND_RESEND;
    mp["change"] = COMMAND_CHANGE;
    mp["back"] = COMMAND_BACK;
    mp["retry"] = COMMAND_RETRY;
    mp["status"] = COMMAND_STATUS;
    mp["clear"] = COMMAND_CLEAR;
    mp["logout"] = COMMAND_LOGOUT;
}

void err_arg_num(const std::string &main_command, int expect, int now) {
    printf("Error, %s expect %d arguments, got %d.\n", main_command.c_str(), expect, now);
}

void err_command(const std::string &command) {
    printf("Error, unknown command %s.\n", command.c_str());
}

void err_system() {
    printf("Something wrong with the system, please contact us.\n");
}

std::vector<std::string> prepare_command(std::string str) {
    vector<string> res;
    int lst = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            if (i != lst) res.push_back(str.substr(lst, i - lst));
            lst = i + 1;
        }
    }
    if (str.size() != lst) res.push_back(str.substr(lst, str.size() - lst));
    return move(res);
}

void user_register() {
    std::string email;
    std::string identify_code = ran_generator.getIdentifyCode(6);
    std::string input_code;
    Email mail;
    int code = 0;

    printf("Please Enter Your Email.\n");
    std::cin >> email;

    do {
        code = mail.send_email(email, "--identify code--", identify_code);
        switch (code) {
            case 0:
                printf("Please Enter the identify code we`ve just sent.\n");
                break;
            case 1:
                printf("there is something wrong with your email account.\n");
                break;
            case 2:
                printf("Failed to send mail, Enter change to change your email or resend to resend.\n");
                break;
        }
        std::cin >> input_code;
        // not a email
        if (input_code != identify_code) {
            printf("Wrong Identify Code.\n");
            bool flag = true;
            while (flag) {
                std::cin >> input_code;
                switch (mp[input_code]) {
                    case COMMAND_RESEND:
                        printf("Resending your identify code.\n");
                        flag = false;
                        break;
                    case COMMAND_CHANGE:
                        printf("Please Enter Your New Email.\n");
                        std::cin >> email;
                        printf("Change complete.\n");
                        flag = false;
                        break;
                    case COMMAND_BACK:
                        return;
                    default:
                        if (identify_code == input_code) flag = false;
                        else printf("Wrong Identify Code.\n");
                        break;
                }
            }
        }
    } while (input_code != identify_code);

    std::string account, password, confirm_password, user_name, real_name,
            phone_number, id_number, pay_password, confirm_pay_password;
    RegisterUser registerUser;

    int res = 0;
    do {
        printf("Please Enter Your Account.\n");
        std::cin >> account;
        printf("Please Enter Your Password.\n");

        system("stty -echo");
        std::cin >> password;
        printf("Please Confirm Your Password.\n");
        std::cin >> confirm_password;
        system("stty echo");

        printf("Please Enter Your User Name.\n");
        std::cin >> user_name;
        printf("Please Enter Your Real Name.\n");
        std::cin >> real_name;
        printf("Please Enter Your Phone Number.\n");
        std::cin >> phone_number;
        printf("Please Enter Your ID Number.\n");
        std::cin >> id_number;
        printf("Please Enter Your Pay Password.\n");

        system("stty -echo");
        std::cin >> pay_password;
        printf("Please Confirm Your Pay Password.\n");
        std::cin >> confirm_pay_password;
        system("stty echo");

        printf("Registering...\n");
        res = registerUser.Register(account, password, confirm_password, user_name, real_name,
                                    phone_number, email, id_number, pay_password, confirm_pay_password);

        if (res != 0) {
//            printf("Registration failed, Enter retry to retry.\n");
            switch (res) {
                case 1:
                    printf("Account or user name already exists.\n");
                    break;

                case 2:
                    printf("Passwords illegal.\n");
                    break;

                case 3:
                    printf("Phone, Email, ID number is illegal.\n");
                    break;

                case 4:
                    printf("Please confirm your pay password.\n");
                    break;

                default:
                    err_system();
                    break;
            }
            std::string command;
            bool flag = true;
            while (flag) {
                std::cin >> command;
                switch (mp[command]) {
                    case COMMAND_RETRY:
                        flag = false;
                        break;
                    case COMMAND_BACK:
                        return;
                    default:
                        err_command(command);
                        break;
                }
            }
        }
    } while (res);

    printf("Registration complete.\n");
}

void seller_register() {
    std::string account, password, confirm_password, shop_name, shop_address,
            shop_owner_name, shop_owner_phone_number, shop_owner_id_number,
            pay_password, confirm_pay_password;

    RegisterSeller registerSeller;

    int res = 0;
    do {
        printf("Please Enter Your Account.\n");
        std::cin >> account;
        printf("Please Enter Your Password.\n");

        system("stty -echo");
        std::cin >> password;
        printf("Please Confirm Your Password.\n");
        std::cin >> confirm_password;
        system("stty echo");

        printf("Please Enter Your Shop Name.\n");
        std::cin >> shop_name;
        printf("Please Enter Your Shop Address.\n");
        std::cin >> shop_address;
        printf("Please Enter Shop Owner Name.\n");
        std::cin >> shop_owner_name;
        printf("Please Enter Shop Owner Phone Number.\n");
        std::cin >> shop_owner_phone_number;
        printf("Please Enter Shop Owner ID Number.\n");
        std::cin >> shop_owner_id_number;
        printf("Please Enter Your Pay Password.\n");

        system("stty -echo");
        std::cin >> pay_password;
        printf("Please Confirm Your Pay Password.\n");
        std::cin >> confirm_pay_password;
        system("stty echo");

        printf("Registering...\n");
        res = registerSeller.Register(account, password, confirm_password, shop_name,
                                shop_address, shop_owner_name, shop_owner_phone_number,
                                shop_owner_id_number, pay_password, confirm_pay_password);

        if (res != 0) {
//            printf("Registration failed, Enter retry to retry.\n");
            switch (res) {
                case 1:
                    printf("Account or Shop name already exists.\n");
                    break;

                case 2:
                    printf("Passwords illegal.\n");
                    break;

                case 3:
                    printf("Phone, ID number is illegal.\n");
                    break;

                case 4:
                    printf("Please confirm your pay password.\n");
                    break;

                default:
                    err_system();
                    break;
            }
            std::string command;
            bool flag = true;
            while (flag) {
                std::cin >> command;
                switch (mp[command]) {
                    case COMMAND_RETRY:
                        flag = false;
                        break;
                    case COMMAND_BACK:
                        return;
                    default:
                        err_command(command);
                        break;
                }
            }
        }
    } while (res);

    printf("Your application has been sent to manager, please wait patiently.\n");
}

void do_register(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("register", 2, commands.size());
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_USER:
            user_register();
            break;

        case COMMAND_SELLER:
            seller_register();
            break;

        case COMMAND_MANAGER:
            printf("If You Want To Be Manager, Please Contact Root Manager.\n");
            break;

        default:
            printf("unknown args %s.\n", commands[1].c_str());
            break;
    }
}

void user_login(const std::vector<std::string> &commands) {
    LoginUser login;
    bool res = login.Login(commands[2], commands[3]);
    //login result
//    if (res) {
//        printf("login success, welcome back %s.\n", db.select_user_data(user_id).user_name.c_str());
//    } else {
//        printf("login failed.\n");
//    }
    if (res) {
        std::string user_id = BasicOp.get_user_id_by_account(commands[2]);
        DB &db = DB::getInstance();
        UserData user = db.select_user_data(user_id);
        printf("login success, welcome back %s.\n", user.user_name.c_str());
        status.type = STATUS_USER;
        status.account = user.account;
        status.id = user_id;
        status.name = user.user_name;
    } else {
        printf("login failed.\n");
    }
}

void manager_login(const std::vector<std::string> &commands) {
    LoginManager loginManager;
    bool res = loginManager.Login(commands[2], commands[3]);
    if (res) {
        std::string manager_id = BasicOp.get_manager_id_by_account(commands[2]);
        DB &db = DB::getInstance();
        ManagerData managerData = db.select_manager_data(manager_id);
        printf("login success, welcome back manager.\n");
        status.type = STATUS_MANAGER;
        status.account = managerData.account;
        status.id = manager_id;
    } else {
        printf("login failed.\n");
    }
}

void seller_login(const std::vector<std::string> &commands) {
    LoginSeller loginSeller;
    bool res = loginSeller.Login(commands[2], commands[3]);
    if (res) {
        std::string seller_id = BasicOp.get_seller_id_by_account(commands[2]);
        DB &db = DB::getInstance();
        SellerData sellerData = db.select_seller_data(seller_id);
        printf("login success, welcome back.\n");
        status.type = STATUS_SELLER;
        status.account = sellerData.account;
        status.id = seller_id;
    } else {
        printf("login failed.\n");
    }
}

void do_login(const std::vector<std::string> &commands) {
    if (commands.size() != 4) {
        err_arg_num("login", 3, commands.size() - 1);
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_SELLER:
            seller_login(commands);
            break;
        case COMMAND_USER:
            user_login(commands);
            break;
        case COMMAND_MANAGER:
            manager_login(commands);
            break;
        default:
            printf("unknown args %s.\n", commands[1].c_str());
            break;
    }
}

void print_status() {
    switch (status.type) {
        case STATUS_MANAGER:
            if (status.account == "account") {
                printf("Login as top-manager.\n");
            } else {
                printf("Login as manager.\n");
                printf("Your account is %s.\n", status.account.c_str());
            }
            break;

        case STATUS_TOURIST:
            printf("Wandering as a tourist.\n");
            break;

        case STATUS_USER:
            printf("Login as user.\n");
            printf("Your account is %s.\n", status.account.c_str());
            break;

        case STATUS_SELLER:
            printf("Login as seller.\n");
            printf("Your account is %s.\n", status.account.c_str());
            break;

        default:
            err_system();
            break;
    }
}

void clear() {
    system("clear");
}

void logout() {
    status.type = STATUS_TOURIST;
    status.id = "";
    status.account = "";
    status.name = "";
    printf("Logout Success.\n");
}

bool execute_command(char *input) {
    std::vector<std::string> commands = prepare_command(std::string(input));

    // if there`s no command, just quit
    if (commands.empty()) return true;
    // commands[0] is main command
    switch (mp[commands[0]]) {
        case COMMAND_QUIT:
            return false;
            break;

        case COMMAND_REGISTER:
            do_register(commands);
            break;

        case COMMAND_LOGIN:
            do_login(commands);
            break;

        case COMMAND_STATUS:
            print_status();
            break;

        case COMMAND_CLEAR:
            clear();
            break;

        case COMMAND_LOGOUT:
            logout();
            break;

        default:
            err_command(commands[0]);
            break;
    }
    return true;
}

int main() {
    system("clear");
    DB &db = DB::getInstance();
    init();
    db.open();

    puts("Online Shopping System Version 0.1");
    puts("Enter help to see manual");

    while (1) {
        char *input = readline("OSS> ");
        add_history(input);

        if (!execute_command(input)) break;

        free(input);
    }

    db.close();
    return 0;
}