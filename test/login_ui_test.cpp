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
#include "SellerSystem.h"
#include "ServiceSystem.h"
#include "MoneySystem.h"
#include "QuestionSystem.h"

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
    COMMAND_DETAIL,
    COMMAND_MODIFY,
    COMMAND_ITEM,
    COMMAND_FIND,
    COMMAND_CART,
    COMMAND_SUBMIT,
    COMMAND_CHARGE,
    COMMAND_WITHDRAW,
    COMMAND_TRANSFER,
    COMMAND_ORDER,
    COMMAND_CONFIRM,
    COMMAND_RETURN,
    COMMAND_MESSAGE,//TODO:message
    COMMAND_ASK,
    COMMAND_ASSET,
    COMMAND_CLEAN,
    COMMAND_FORGET,
    COMMAND_HELP
};

enum {
    ARG_LABEL = 1,
    ARG_SHOP,
    ARG_PART,
    ARG_CURRENT,
    ARG_HISTORY
};

enum Status {
    STATUS_MANAGER = 1,
    STATUS_USER,
    STATUS_SELLER,
    STATUS_TOURIST
};

enum DataType {
    TYPE_REGISTER_REQUEST = 1,
    TYPE_ORDER,
    TYPE_MESSAGE,
    TYPE_ITEM_DATA,
    TYPE_NONE
};

std::unordered_map<std::string, int> mp;

RandomGenerator &ran_generator = RandomGenerator::getInstance();
BasicOperation BasicOp;

struct LIST {
    std::vector<RegisterRequestData> register_request_list;
    std::vector<ItemData> item_data_list;
    std::vector<Order> order_list;
    std::vector<std::string> message_list;

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
    mp["modify"] = COMMAND_MODIFY;
    mp["item"] = COMMAND_ITEM;
    mp["find"] = COMMAND_FIND;
    mp["cart"] = COMMAND_CART;
    mp["submit"] = COMMAND_SUBMIT;
    mp["charge"] = COMMAND_CHARGE;
    mp["order"] = COMMAND_ORDER;
    mp["withdraw"] = COMMAND_WITHDRAW;
    mp["transfer"] = COMMAND_TRANSFER;
    mp["confirm"] = COMMAND_CONFIRM;
    mp["return"] = COMMAND_RETURN;
    mp["message"] = COMMAND_MESSAGE;
    mp["asset"] = COMMAND_ASSET;
    mp["ask"] = COMMAND_ASK;
    mp["clean"] = COMMAND_CLEAN;
    mp["forget"] = COMMAND_FORGET;
    mp["help"] = COMMAND_HELP;

    mp["-l"] = ARG_LABEL;
    mp["-s"] = ARG_SHOP;
    mp["-p"] = ARG_PART;
    mp["-c"] = ARG_CURRENT;
    mp["-h"] = ARG_HISTORY;
}

void do_help() {
    printf("quit : to exit the system.\n");
    printf("login : to login the system.\n");
    printf("register : register new account.\n");
    printf("user seller manager is arguments for login and register.\n");
    printf("resend : resend the email.\n");
    printf("change: to change the email.\n");
    printf("back : back to the previous page when you registering.\n");
    printf("retry : try again when you failed.\n");
    printf("status : to check your login status.\n");
    printf("clear : clear the screen.\n");
    printf("logout : just logout.\n");
    printf("ls : list the info you need, followed by arguments.\n");
    printf("add : add something you need, followed by arguments, such as add manager add cart.\n");
    printf("rm : same as previous one, delete something.\n");
    printf("accept : accept the register request.\n");
    printf("reject : reject the register request.\n");
    printf("next : print next 10 info.\n");
    printf("previous : print previous 10 info.\n");
    printf("detail : show the detail, followed by a num.\n");
    printf("modify : modify something you want, follow by arguments.\n");
    printf("item : a argument, represent item.\n");
    printf("find : find items.\n");
    printf("cart : a argument, represent shop list.\n");
    printf("submit : submit your orders.\n");
    printf("charge : charge money.\n");
    printf("withdraw : withdraw money.\n");
    printf("order : a argument, represent orders.\n");
    printf("transfer : transfer money to others.\n");
    printf("confirm : confirm the order.\n");
    printf("return : return the order.\n");
    printf("message : a argument, represent messages.\n");
    printf("asset : show your asset.\n");
    printf("ask : ask questions.\n");
    printf("clean : clean the messages.\n");
    printf("forget : forget password.\n");
    printf("-l : parameter, represent label.\n");
    printf("-s : parameter, represent shop.\n");
    printf("-p : parameter, represent part.\n");
    printf("-c : parameter, represent current.\n");
    printf("-h : parameter, represent history.\n");
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

bool str2double(const std::string &str, double &value) {
    bool dig = false;
    int dig_size = 0;
    double sum = 0;
    for (char i : str) {
        if (i >= '0' && i <= '9') {
            if (dig) dig_size++;
            sum = sum * 10 + (i - '0');
        } else if (i == '.') {
            if (dig) return false;
            dig = true;
        } else {
            return false;
        }
    }
    value = sum / pow(10, dig_size);
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

void user_forget() {
    std::string identify_code = ran_generator.getIdentifyCode(6);
    std::string input_code;
    std::string account;
    Email mail;

    int code = 0;

    printf("Please input your account.\n");
    std::cin >> account;

    std::string id = BasicOp.get_user_id_by_account(account);
    if (id.empty()) {
        printf("wrong account.\n");
        return;
    }

    DB &db = DB::getInstance();
    UserData userData = db.select_user_data(id);
    std::string email =  userData.email;

    if (mail.send_email(email, "--password--", userData.password)) {
        printf("Your password has been sent to your email.\n");
    } else {
        printf("Error sending mail.\n");
    }
}

void seller_forget() {
    std::string account, shop_name, shop_owner_name,
                shop_owner_phone_number,
                shop_owner_id_number,
                new_password,
                new_confirm_password;

    printf("Please Enter Your Account.\n");
    std::cin >> account;
    printf("Please Enter Your Shop Name.\n");
    std::cin >> shop_name;
    printf("Please Enter Your Shop Owner Name.\n");
    std::cin >> shop_owner_name;
    printf("Please Enter Your Shop Owner ID Number.\n");
    std::cin >> shop_owner_id_number;
    printf("Please Enter Your New Password.\n");

    system("stty -echo");
    std::cin >> new_password;
    printf("Please Confirm Your Password.\n");
    std::cin >> new_confirm_password;
    system("stty echo");

    LoginSeller loginSeller;
    int res = loginSeller.ForgotPassword(account,
                                         shop_name,
                                         shop_owner_name,
                                         shop_owner_phone_number,
                                         shop_owner_id_number,
                                         new_password,
                                         new_confirm_password);
    switch (res) {
        case 0:
            printf("Change Password Succeed.\n");
            break;

        case 1:
            printf("Wrong Account.\n");
            break;

        case 2:
            printf("Verification error.\n");
            break;

        case 3:
            printf("Illegal Password.\n");
            break;

        default:
            err_system();
            break;
    }
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
    DB &db = DB::getInstance();
    switch (ui_list.type) {
        case TYPE_REGISTER_REQUEST:
            printf("%d: account: %s shop_name: %s.\n", i + 1,
                   ui_list.register_request_list[i].account.c_str(),
                   ui_list.register_request_list[i].shop_name.c_str());
            break;

        case TYPE_ITEM_DATA:
            printf("%d: name: %s price: %.2f sell_num: %d store_num: %d.\n",
                   i + 1,
                   ui_list.item_data_list[i].name.c_str(),
                   ui_list.item_data_list[i].price,
                   ui_list.item_data_list[i].sell_num,
                   ui_list.item_data_list[i].store_num);
            break;

        case TYPE_MESSAGE:
            printf("%s\n", ui_list.message_list[i].c_str());
            break;

        case TYPE_ORDER:
            printf("%d: name: %s price: %.2f time: %s buy_num: %d.\n",
                   i + 1,
                   db.select_item_data(ui_list.order_list[i].item_id).name.c_str(),
                   ui_list.order_list[i].price,
                   ui_list.order_list[i].time.c_str(),
                   ui_list.order_list[i].buy_num);
            break;

        default:
            err_system();
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
    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

void list_item() {
    ui_list.item_data_list = BasicOp.GetShopItems(status.id);
    ui_list.type = TYPE_ITEM_DATA;
    ui_list.max_row = ui_list.item_data_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("You don`t have items yet.\n");
        return;
    }
    do_next();
}

void print_order(Order order) {
    DB &db = DB::getInstance();
    ItemData itemData = db.select_item_data(order.item_id);
    std::string name = itemData.name;
    std::string shop_name = itemData.owner;

    printf("name: %s\n", name.c_str());
    printf("shop_name: %s\n", shop_name.c_str());
    printf("price: %.2f\n", order.price);
    printf("buy_num: %d\n", order.buy_num);
    printf("time: %s\n", order.time.c_str());

}

void list_cart() {
    std::vector<Order> shop_list;
    DB &db = DB::getInstance();
    shop_list = db.select_user_data(status.id).shop_list;

    for (const auto &x : shop_list) {
        print_order(x);
    }
}

void print_buy_request(const BuyItemRequestData &x) {
    DB &db = DB::getInstance();
    printf("item_name: %s\n", db.select_item_data(x.item_id).name.c_str());
    printf("remark: %s\n", x.remark.c_str());
    printf("user_name: %s\n", db.select_user_data(x.user_id).user_name.c_str());
    printf("time: %s\n", x.time.c_str());
    printf("buy_num: %d\n", x.buy_num);
    printf("price: %.2f\n", x.price);
}

void list_buy_request() {
    std::vector<BuyItemRequestData> buy_item_request;
    DB &db = DB::getInstance();
    buy_item_request = db.select_seller_data(status.id).buy_item_request_list;

    for (const auto &x : buy_item_request) {
        print_buy_request(x);
    }
}

void list_seller_message() {
    DB &db = DB::getInstance();
    ui_list.message_list = db.select_seller_data(status.id).message;
    ui_list.type = TYPE_MESSAGE;
    ui_list.max_row = ui_list.message_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("No more messages.\n");
        return;
    }

    do_next();
}

void seller_list(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_ITEM:
            list_item();
            break;

        case COMMAND_ORDER:
            list_buy_request();
            break;

        case COMMAND_MESSAGE:
            list_seller_message();
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void list_current_order() {
    DB &db = DB::getInstance();
    ui_list.order_list = db.select_user_data(status.id).current_order;
    ui_list.type = TYPE_ORDER;
    ui_list.max_row = ui_list.order_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("No more current order.\n");
        return;
    }

    do_next();
}

void list_history_order() {
    DB &db = DB::getInstance();
    ui_list.order_list = db.select_user_data(status.id).history_order;
    ui_list.type = TYPE_ORDER;
    ui_list.max_row = ui_list.order_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("You don`t have history order yet.\n");
        return;
    }

    do_next();
}

void list_order(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("list order", 1, commands.size() - 2);
        return;
    }

    switch (mp[commands[2]]) {
        case ARG_CURRENT:
            list_current_order();
            break;

        case ARG_HISTORY:
            list_history_order();
            break;

        default:
            err_command(commands[2]);
            break;
    }
}

void list_user_message(const std::vector<std::string> &commands) {
     DB &db = DB::getInstance();
     ui_list.message_list = db.select_user_data(status.id).message;
     ui_list.type = TYPE_MESSAGE;
     ui_list.max_row = ui_list.message_list.size();
     ui_list.curr_row = 0;

     if (ui_list.max_row == 0) {
         printf("No more messages.\n");
         return;
     }

     do_next();
}

void user_list(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_CART:
            list_cart();
            break;

        case COMMAND_ORDER:
            list_order(commands);
            break;

        case COMMAND_MESSAGE:
            list_user_message(commands);
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void do_list(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_list();
            break;

        case STATUS_SELLER:
            seller_list(commands);
            break;

        case STATUS_USER:
            user_list(commands);
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

        case TYPE_ITEM_DATA:
            ui_list.item_data_list[i].print();
            break;

        case TYPE_ORDER:
            ui_list.order_list[i].print();
            break;

        default:
            err_system();
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

void add_item() {
    ItemData itemData;
    printf("Please Enter New Item Name.\n");
    std::cin >> itemData.name;
    printf("Please Enter New Item description.\n");
    std::cin >> itemData.des;

    printf("Please Enter New Item Price.\n");
    std::string price;
    std::cin >> price;
    while (true) {
        if (!str2double(price, itemData.price)) {
            err_arg_type("item price", "double");
            std::cin >> price;
        } else {
            break;
        }
    }

    printf("Please Enter New Item Store Num.\n");
    std::string store;
    std::cin >> store;
    while (true) {
        if (!str2int(store, itemData.store_num)) {
            err_arg_type("item store", "int");
            std::cin >> store;
        } else {
            break;
        }
    }

    printf("Please Enter New Item Label Num.\n");
    int label_num = 0;
    std::string label_num_str;
    std::cin >> label_num_str;
    while (true) {
        if (!str2int(label_num_str, label_num)) {
            err_arg_type("label num", "int");
            std::cin >> label_num_str;
        } else if (label_num < 1){
            printf("New Item Label Must Have At Least 1 Label.\n");
            std::cin >> label_num_str;
        } else {
            break;
        }
    }

    printf("Please Enter %d Label.\n", label_num);
    std::vector<std::string> label(label_num);
    for (int i = 0; i < label_num; i++) {
        std::cin >> label[i];
    }
    itemData.label = label;

    itemData.sell_num = 0;
    DB &db = DB::getInstance();
    itemData.owner = db.select_seller_data(status.id).shop_name;

    SellerSystem sellerSystem;
    bool res = sellerSystem.insert_item(status.id, itemData);
    if (res) {
        printf("insert new item complete.\n");
    } else {
        printf("insert new item failed.\n");
    }
}

// add item
void seller_add(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("add", 1, commands.size() - 1);
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_ITEM:
            add_item();
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

// add cart shop_name item_name num
void add_shop_list(const std::vector<std::string> &commands) {
    if (commands.size() != 5) {
        err_arg_num("add cart", 3, commands.size() - 2);
        return;
    }

    int num = 0;
    if (!str2int(commands[4], num)) {
        err_arg_type("add cart num", "int");
        return;
    }

    ServiceSystem serviceSystem;
    bool res = serviceSystem.insert_shop_list(status.id, commands[2], commands[3], num);
    if (res) {
        printf("add cart complete.\n");
    } else {
        printf("add cart failed.\n");
    };
}

void delete_shop_list(const std::vector<std::string> &commands) {
    if (commands.size() != 5) {
        err_arg_num("rm cart", 3, commands.size() - 2);
        return;
    }

    int num = 0;
    if (!str2int(commands[4], num)) {
        err_arg_type("rm cart num", "int");
        return;
    }

    ServiceSystem serviceSystem;
    bool res = serviceSystem.remove_shop_list(status.id, commands[2], commands[3], num);
    if (res) {
        printf("rm cart complete.\n");
    } else {
        printf("rm cart failed.\n");
    };
}

void user_add(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_CART:
            add_shop_list(commands);
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void do_add(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_MANAGER:
            manager_add(commands);
            break;

        case STATUS_SELLER:
            seller_add(commands);
            break;

        case STATUS_USER:
            user_add(commands);
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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
        return;
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

void item_remove(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("rm item", 1, commands.size() - 2);
        return;
    }

    SellerSystem sellerSystem;
    bool res = sellerSystem.remove_item(status.id, commands[2]);
    if (res) {
        printf("Remove Item Complete.\n");
    } else {
        printf("Remove Item Failed.\n");
    }
}

void seller_remove(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch(mp[commands[1]]) {
        case COMMAND_ITEM:
            item_remove(commands);
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void user_remove(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_CART:
            delete_shop_list(commands);
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

        case STATUS_SELLER:
            seller_remove(commands);
            break;

        case STATUS_USER:
            user_remove(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void update_manager_ui_list() {
    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

    ManagerOperation &managerOperation = ManagerOperation::getInstance();
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

// find name
void do_find(const std::vector<std::string> &commands) {
    if (status.type != STATUS_USER) {
        err_permit();
        return;
    }

    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    ServiceSystem serviceSystem;
    if (commands.size() == 2) {
        // sim by default
        ui_list.item_data_list = serviceSystem.search_item_sim(commands[1]);
    } else if (commands.size() == 3) {
        switch (mp[commands[2]]) {
            case ARG_PART:
                ui_list.item_data_list = serviceSystem.search_item_part(commands[1]);
                break;

            case ARG_SHOP:
                ui_list.item_data_list = serviceSystem.search_shop(commands[1]);
                break;

            case ARG_LABEL:
                ui_list.item_data_list = serviceSystem.search_item_label(commands[1]);
                break;

            default:
                err_command(commands[2]);
                break;
        }
    } else {
        printf("error arg num.\n");
        return;
    }
    ui_list.type = TYPE_ITEM_DATA;
    ui_list.max_row = ui_list.item_data_list.size();
    ui_list.curr_row = 0;

    if (ui_list.max_row == 0) {
        printf("no matching results.\n");
        return;
    }

    do_next();
}

// modify item name which detail
void modify_item(const std::vector<std::string> &commands) {
    if (commands.size() != 5) {
        err_arg_num("modify item", 3, commands.size() - 2);
        return;
    }

    SellerSystem sellerSystem;
    ItemData itemData;
    itemData.name = commands[2];

    if (commands[3] == "price") {
        if (!str2double(commands[4], itemData.price)) {
            err_arg_type("modify item price", "double");
            return;
        }
        bool res = sellerSystem.modify_item(status.id, itemData, "price");
        if (res) {
            printf("modify price complete.\n");
        } else {
            printf("modify price failed.\n");
        }
        return;
    }

    if (commands[3] == "store") {
        if (!str2int(commands[4], itemData.store_num)) {
            err_arg_type("modify item store", "int");
            return;
        }
        bool res = sellerSystem.modify_item(status.id, itemData, "store_num");
        if (res) {
            printf("modify store complete.\n");
        } else {
            printf("modify store failed.\n");
        }
        return;
    }

    if (commands[3] == "des") {
        itemData.des = commands[4];
        bool res = sellerSystem.modify_item(status.id, itemData, "des");
        if (res) {
            printf("modify des complete.\n");
        } else {
            printf("modify des failed.\n");
        }
        return;
    }

    err_command(commands[3]);
}

void seller_modify(const std::vector<std::string> &commands) {
    if (commands.size() < 2) {
        printf("too few arguments.\n");
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_ITEM:
            modify_item(commands);
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void do_modify(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_SELLER:
            seller_modify(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void do_charge(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("charge", 1, commands.size() - 1);
        return;
    }

    double money;
    if (!str2double(commands[1], money)) {
        err_arg_type("charge", "double");
        return;
    }

    if (status.type == STATUS_MANAGER) {
        printf("you don`t have to charge.\n");
        return;
    }

    if (status.type == STATUS_TOURIST) {
        err_permit();
        return;
    }

    MoneySystem moneySystem;
    bool res = moneySystem.RechargeMoney(status.id, money);
    if (res) {
        printf("charge money complete.\n");
    } else {
        printf("charge money failed.\n");
    }
}

void do_withdraw(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("charge", 1, commands.size() - 1);
        return;
    }

    if (status.type == STATUS_MANAGER) {
        printf("no you don`t want to do this.\n");
        return;
    }

    if (status.type == STATUS_TOURIST) {
        err_permit();
        return;
    }

    double money;
    if (!str2double(commands[1], money)) {
        err_arg_type("charge", "double");
        return;
    }
    MoneySystem moneySystem;
    bool res = moneySystem.WithdrawMoney(status.id, money);
    if (res) {
        printf("withdraw money complete.\n");
    } else {
        printf("withdraw money failed.\n");
    }
}

void submit_order(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("submit", 1, commands.size() - 1);
        return;
    }

    ServiceSystem serviceSystem;
    bool res = serviceSystem.submit_shop_list(status.id, commands[1]);
    if (res) {
        printf("Order submit complete.\n");
    } else {
        printf("Order submit failed.\n");
    }
}

void do_submit(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_USER:
            submit_order(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void do_transfer(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("transfer", 2, commands.size() - 1);
        return;
    }

    if (status.type == STATUS_TOURIST || status.type == STATUS_MANAGER) {
        err_permit();
        return;
    }

    double money;
    if (!str2double(commands[2], money)) {
        err_arg_type("charge", "double");
        return;
    }
    MoneySystem moneySystem;
    bool res = moneySystem.TransferMoney(status.id, commands[1], money);
    if (res) {
        printf("transfer money complete.\n");
    } else {
        printf("transfer money failed.\n");
    }
}

void seller_confirm(const std::vector<std::string> &commands) {
    if (commands.size() > 1) {
        printf("too many arugments.\n");
        return;
    }

    if (status.type != STATUS_SELLER) {
        err_permit();
        return;
    }

    ServiceSystem serviceSystem;
    serviceSystem.deal_BuyItemRequest(status.id);
}

void user_confirm(const std::vector<std::string> &commands) {
    if (ui_list.type == TYPE_NONE) {
        err_using("confirm");
        return;
    }
    if (commands.size() != 2) {
        err_arg_num("confirm", 1, commands.size() - 1);
        return;
    }
    int ptr = 0;
    if (!str2int(commands[1], ptr)) {
        err_arg_type("confirm", "int");
        return;
    }
    if (ui_list.type != TYPE_ORDER) {
        printf("You have to use ls first.\n");
        return;
    }
    ptr--;
    if (ptr >= 0 && ptr < ui_list.max_row) {
        ServiceSystem serviceSystem;
        bool res = serviceSystem.confirm(status.id, ui_list.order_list[ptr]);
    } else {
        printf("Index out of range.\n");
    }
}

void do_confirm(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_SELLER:
            seller_confirm(commands);
            break;

        case STATUS_USER:
            user_confirm(commands);
            break;

        default:
            err_permit();
            break;
    }
}

void do_return(const std::vector<std::string> &commands) {
    if (ui_list.type == TYPE_NONE) {
        err_using("return");
        return;
    }
    if (commands.size() != 2) {
        err_arg_num("return", 1, commands.size() - 1);
        return;
    }
    int ptr = 0;
    if (!str2int(commands[1], ptr)) {
        err_arg_type("return", "int");
        return;
    }

    if (ui_list.type != TYPE_ORDER) {
        printf("You have to list order first.\n");
        return;
    }

    ptr--;
    if (ptr >= 0 && ptr < ui_list.max_row) {
        ServiceSystem serviceSystem;
        bool res = serviceSystem.returnItem(status.id, ui_list.order_list[ptr]);
        if (res) {
            printf("return order complete.\n");
        } else {
            printf("return order failed.\n");
        }
    } else {
        printf("Index out of range.\n");
    }
}

void do_ask(const std::vector<std::string> &commands) {
    if (commands.size() != 3) {
        err_arg_num("ask", 2, commands.size() - 1);
        return;
    }

    std::string name = BasicOp.get_seller_id_by_name(commands[1]);
    if (name.empty()) {
        printf("Cannot find this shop.\n");
        return;
    }

    QuestionSystem questionSystem;
    printf("%s", questionSystem.GetAnswer(name, commands[2]).c_str());
}

void do_asset(const std::vector<std::string> &commands) {
    if (commands.size() > 1) {
        printf("too many arguments.\n");
        return;
    }

    DB &db = DB::getInstance();
    Wallet wallet;

    switch (status.type) {
        case STATUS_TOURIST:
            err_permit();
            return;

        case STATUS_MANAGER:
            printf("You don`t need money.\n");
            return;

        case STATUS_USER:
            wallet = db.select_user_data(status.id).wallet;
            break;

        case STATUS_SELLER:
            wallet = db.select_user_data(status.id).wallet;
            break;

        default:
            err_system();
            return;
    }

    printf("Money: %.2f.\n", wallet.money);

}

void do_clean() {
    DB &db = DB::getInstance();
    if (status.type == STATUS_SELLER) {
        SellerData sellerData = db.select_seller_data(status.id);
        sellerData.message.clear();
        db.modify_seller_data(sellerData.id, sellerData);
        printf("clean complete.\n");
        return;
    }

    if (status.type == STATUS_USER) {
        UserData userData = db.select_user_data(status.id);
        userData.message.clear();
        db.modify_user_data(userData.id, userData);
        printf("clean complete.\n");
        return;
    }

    err_permit();

}

void forget(const std::vector<std::string> &commands) {
    if (commands.size() != 2) {
        err_arg_num("forget", 1, commands.size() - 1);
        return;
    }

    switch (mp[commands[1]]) {
        case COMMAND_USER:
            user_forget();
            break;

        case COMMAND_SELLER:
            seller_forget();
            break;

        default:
            err_command(commands[1]);
            break;
    }
}

void do_forget(const std::vector<std::string> &commands) {
    switch (status.type) {
        case STATUS_USER:
            printf("Why are you doing this?.\n");
            break;

        case STATUS_SELLER:
            printf("Why are you doing this?.\n");
            break;

        case STATUS_MANAGER:
            printf("Please contact root manager.\n");
            break;

        case STATUS_TOURIST:
            forget(commands);
            break;

        default:
            err_system();
            break;
    }
}

bool execute_command(char *input) {
    std::vector<std::string> commands = prepare_command(std::string(input));

    // check whether to clear ui_list
    bool clear_ui_list = false;
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
            do_list(commands);
            break;

        case COMMAND_ADD:
            do_add(commands);
            clear_ui_list = true;
            break;

        case COMMAND_REMOVE:
            do_remove(commands);
            clear_ui_list = true;
            break;

        case COMMAND_ACCEPT:
            do_accept(commands);
            clear_ui_list = true;
            break;

        case COMMAND_REJECT:
            do_reject(commands);
            clear_ui_list = true;
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

        case COMMAND_MODIFY:
            do_modify(commands);
            clear_ui_list = true;
            break;

        case COMMAND_FIND:
            do_find(commands);
            break;

        case COMMAND_SUBMIT:
            do_submit(commands);
            clear_ui_list = true;
            break;

        case COMMAND_WITHDRAW:
            do_withdraw(commands);
            break;

        case COMMAND_CHARGE:
            do_charge(commands);
            break;

        case COMMAND_TRANSFER:
            do_transfer(commands);
            break;

        case COMMAND_CONFIRM:
            do_confirm(commands);
            clear_ui_list = true;
            break;

        case COMMAND_RETURN:
            do_return(commands);
            clear_ui_list = true;
            break;

        case COMMAND_ASK:
            do_ask(commands);
            break;

        case COMMAND_ASSET:
            do_asset(commands);
            break;

        case COMMAND_CLEAN:
            do_clean();
            break;

        case COMMAND_FORGET:
            do_forget(commands);
            break;

        case COMMAND_HELP:
            do_help();
            break;

        default:
            err_command(commands[0]);
            break;
    }

    // hope this will work
    if (clear_ui_list) {
        ui_list.type = TYPE_NONE;
        ui_list.max_row = 0;
        ui_list.curr_row = 0;
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

    while (true) {
        char *input = readline("OSS> ");
        add_history(input);

        if (!execute_command(input)) break;

        free(input);
    }

    db.close();
    return 0;
}