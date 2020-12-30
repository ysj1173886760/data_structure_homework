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
#include "ManagerOperation.h"

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
    COMMAND_LOGOUT,
    COMMAND_LIST,
    COMMAND_REMOVE,
    COMMAND_ADD,
    COMMAND_ACCEPT,
    COMMAND_REJECT,
    COMMAND_NEXT,
    COMMAND_PREVIOUS,
    COMMAND_DETAIL
};

enum Status {
    STATUS_MANAGER = 1,
    STATUS_USER,
    STATUS_SELLER,
    STATUS_TOURIST
};

enum DataType {
    TYPE_REGISTER_REQUEST = 1,
    TYPE_NONE
};

std::unordered_map<std::string, int> mp;

RandomGenerator &ran_generator = RandomGenerator::getInstance();
BasicOperation BasicOp;

struct LIST {
    std::vector<RegisterRequestData> register_request_list;

    enum DataType type;
    int curr_row, max_row;
}ui_list;

struct STATUS {
    std::string id;
    enum Status type;
    std::string name;
    std::string account;
}status;

void init() {
    ui_list.type = TYPE_NONE;
    ui_list.max_row = 0;
    ui_list.curr_row = 0;

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
    mp["ls"] = COMMAND_LIST;
    mp["add"] = COMMAND_ADD;
    mp["rm"] = COMMAND_REMOVE;
    mp["accept"] = COMMAND_ACCEPT;
    mp["reject"] = COMMAND_REJECT;
    mp["next"] = COMMAND_NEXT;
    mp["previous"] = COMMAND_PREVIOUS;
    mp["detail"] = COMMAND_DETAIL;
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

void err_permit() {
    printf("You Don`t Have The Permission To Execute This Command.\n");
}

void err_using(const std::string &commands) {
    printf("Error using commands %s.\n", commands.c_str());
}

void err_arg_type(const std::string &main_command, const std::string &expect) {
    printf("Error %s arg type, Expect %s.\n", main_command.c_str(), expect.c_str());
}

bool str2int(const std::string &str, int &value) {
    value = 0;
    for (const auto &x : str) {
        if (x > '9' || x < '0') return false;
        value = value * 10 + (x - '0');
    }
    return true;
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

    if (status.type != STATUS_TOURIST) {
        printf("Error, you have to logout first.\n");
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

    ui_list.type = TYPE_NONE;
    ui_list.max_row = 0;
    ui_list.curr_row = 0;

    printf("Logout Success.\n");
}

void print_list(int i) {
    switch (ui_list.type) {
        case TYPE_REGISTER_REQUEST:
            printf("%d: account: %s shop_name: %s.\n", i + 1,
                   ui_list.register_request_list[i].account.c_str(),
                   ui_list.register_request_list[i].shop_name.c_str());
            break;

        default:
            break;
    }
}

void do_next() {
    if (ui_list.type == TYPE_NONE) {
        err_using("next");
        return;
    }
    if (ui_list.curr_row < ui_list.max_row) {
        int maxx = min(ui_list.max_row, ui_list.curr_row + 10);
        while (ui_list.curr_row < maxx) {
            print_list(ui_list.curr_row);
            ui_list.curr_row++;
        }
    } else {
        printf("Already at the bottom of the list.\n");
    }
}

void manager_list() {
    ManagerOperation managerOperation;
    ui_list.register_request_list = managerOperation.view_application_list();
    ui_list.type = TYPE_REGISTER_REQUEST;
    ui_list.max_row = ui_list.register_request_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("No more register request.\n");
        return;
    }
    // default to print next 10 info
    do_next();
}

void do_list() {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_list();
            break;

        default:
            err_permit();
            break;
    }
}

void print_detail(int i) {
    switch (ui_list.type) {
        case TYPE_REGISTER_REQUEST:
            ui_list.register_request_list[i].print();
            break;

        default:
            break;
    }
}

// TODO: check the previous command

void do_previous() {
    if (ui_list.type == TYPE_NONE) {
        err_using("previous");
        return;
    }
    if (ui_list.curr_row > 0) {
        int minn = max(0, ui_list.curr_row - 10);
        while (ui_list.curr_row > minn) {
            ui_list.curr_row--;
            print_list(ui_list.curr_row);
        }
    } else {
        printf("Already at the top of the list.\n");
    }
}

void manager_add(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("add", 2, commands.size() - 1);
        return;
    }

    if (status.account != "account") {
        printf("only root manager can add new manager.\n");
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.register_manager("account", "password",
                                                 commands[1],
                                                 commands[2],
                                                 commands[2]);
    if (res) {
        printf("Add new manager success.\n");
    } else {
        printf("Add new manager failed.\n");
    }
}

void do_add(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_add(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void delete_user(const std::vector<std::string> &commands) {
    if (commands.size() != 4) {
        err_arg_num("rm user", 2, commands.size() - 2);
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.remove_user(commands[2], commands[3]);
    if (res) {
        printf("delete user success.\n");
    } else {
        printf("delete user failed.\n");
    }
}

void delete_seller(const std::vector<std::string> &commands) {
    if (commands.size() != 5) {
        err_arg_num("rm seller", 3, commands.size() - 2);
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.remove_seller(commands[1],
                                              commands[2],
                                              commands[3]);
    if (res) {
        printf("delete seller success.\n");
    } else {
        printf("delete seller failed.\n");
    }
}

void delete_manager(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("rm manager", 1, commands.size() - 2);
        return;
    }

    if (status.account != "account") {
        printf("only root manager can delete manager.\n");
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.remove_manager("account",
                                               "password",
                                               commands[2]);
    if (res) {
        printf("delete manager success.\n");
    } else {
        printf("delete manager failed.\n");
    }
}

void manager_remove(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
    }

    switch (mp[commands[1]]) {
        case COMMAND_USER:
            delete_user(commands);
            break;

        case COMMAND_SELLER:
            delete_seller(commands);
            break;

        case COMMAND_MANAGER:
            delete_manager(commands);
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void do_remove(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_remove(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void update_manager_ui_list() {
    ManagerOperation managerOperation;
    ui_list.register_request_list = managerOperation.view_application_list();
    ui_list.curr_row = 0;
    ui_list.max_row = ui_list.register_request_list.size();
}

void manager_accept(const std::vector<std::string> &commands) {
    if (ui_list.type == TYPE_NONE) {
        err_using("accept");
        return;
    }
    if (commands.size() != 3) {
        err_arg_num("accept", 2, commands.size() - 1);
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.accept_shop_application(commands[1], commands[2]);
    if (res) {
        printf("Accept shop application success.\n");
        update_manager_ui_list();
    } else {
        printf("Accept shop application failed.\n");
    }
}

void do_accept(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_accept(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void manager_reject(const std::vector<std::string> &commands) {
    if (ui_list.type == TYPE_NONE) {
        err_using("reject");
        return;
    }
    if (commands.size() != 3) {
        err_arg_num("reject", 2, commands.size() - 1);
        return;
    }

    ManagerOperation managerOperation;
    bool res = managerOperation.reject_shop_application(commands[1], commands[2]);
    if (res) {
        printf("Reject shop application success.\n");
        update_manager_ui_list();
    } else {
        printf("Reject shop application failed.\n");
    }
}

void do_reject(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_reject(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void do_detail(const std::vector<std::string> &commands) {
    if (ui_list.type == TYPE_NONE) {
        err_using("detail");
        return;
    }
    if (commands.size() != 2) {
        err_arg_num("detail", 1, commands.size() - 1);
        return;
    }
    int ptr = 0;
    if (!str2int(commands[1], ptr)) {
        err_arg_type("detail", "int");
        return;
    }
    ptr--;
    if (ptr >= 0 && ptr < ui_list.max_row) {
        print_detail(ptr);
    } else {
        printf("Index out of range.\n");
    }
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
            //TODO: check args num
        case COMMAND_STATUS:
            print_status();
            break;

        case COMMAND_CLEAR:
            clear();
            break;

        case COMMAND_LOGOUT:
            logout();
            break;

        case COMMAND_LIST:
            do_list();
            break;

        case COMMAND_ADD:
            do_add(commands);
            break;

        case COMMAND_REMOVE:
            do_remove(commands);
            break;

        case COMMAND_ACCEPT:
            do_accept(commands);
            break;

        case COMMAND_REJECT:
            do_reject(commands);
            break;

        case COMMAND_NEXT:
            do_next();
            break;

        case COMMAND_PREVIOUS:
            do_previous();
            break;

        case COMMAND_DETAIL:
            do_detail(commands);
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