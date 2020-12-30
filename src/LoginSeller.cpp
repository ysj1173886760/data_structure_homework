//
// Created by shrimp on 2020/12/27.
//

#include "LoginSeller.h"
#include <string>
#include <regex>

LoginSeller::LoginSeller() {
    DB& db = DB::getInstance();

    all_seller_data = db.select_all_seller_data();
}

bool LoginSeller::Login(const std::string &account, const std::string &password) {

    for (const auto& it : all_seller_data) {
        if (it.account == account && it.password == password) {
            return true;
        }
    }

    return false;
}

int LoginSeller::ChangePassword(const std::string &account,
                                const std::string &password,
                                const std::string &shop_owner_name,
                                const std::string &shop_owner_phone_number,
                                const std::string &new_password,
                                const std::string &new_confirm_password) {
    for (auto it : all_seller_data) {
        if (it.account == account) {

            if (it.password == password) {
                if (it.shop_owner_name == shop_owner_name
                    && it.shop_owner_phone_number == shop_owner_phone_number) {

                    if (ok_password(new_password, new_confirm_password)) {
                        it.password = new_password;

                        DB& db = DB::getInstance();
                        db.modify_seller_data(it.id, it);

                        return 0;
                    }
                    else return -3; // illegal password.
                }
                else return -2; // verification information error
            }
           else return -1; // password is wrong
        }
    }

    return -1; // account is wrong
}

int LoginSeller::ChangePayPassword(const std::string &account,
                                   const std::string &password,
                                   const std::string &pay_password,
                                   const std::string &shop_owner_name,
                                   const std::string &shop_owner_phone_number,
                                   const std::string &new_pay_password,
                                   const std::string &new_confirm_pay_password) {
    //
    for (auto it : all_seller_data) {
        if (it.account == account) {
            if (it.password == password && it.wallet.password == pay_password) {
                if (it.shop_owner_name == shop_owner_name && it.shop_owner_phone_number == shop_owner_phone_number) {
                    if (ok_pay_password(new_pay_password, new_confirm_pay_password)) {
                        it.wallet.password = new_pay_password;

                        DB& db = DB::getInstance();
                        db.modify_seller_data(it.id, it);

                        return 0; // ok change
                    }
                    else return 3; // pay password is illegal
                }
                else return 2; // verification wrong
            }
            else return 1; // password or pay password wrong
        }
    }

    return 1; // account is wrong
}

int LoginSeller::ForgotPassword(const std::string &account,
                                const std::string &shop_name,
                                const std::string &shop_owner_name,
                                const std::string &shop_owner_phone_number,
                                const std::string &shop_owner_id_number,
                                const std::string &new_password,
                                const std::string &new_confirm_password) {

    for (auto it : all_seller_data) {
        if (it.account == account) {
            if (it.shop_name == shop_name &&
            it.shop_owner_name == shop_owner_name &&
            it.shop_owner_phone_number == shop_owner_phone_number &&
            it.shop_owner_id_number == shop_owner_id_number) {
                if (ok_password(new_password, new_confirm_password)) {
                    it.password = new_password;

                    DB& db = DB::getInstance();
                    db.modify_seller_data(it.id, it);

                    return 0;
                }
                else return 3; // password is illegal
            }
            else return 2; // verification error
        }
    }

    return 1; // account is wrong
}


int LoginSeller::ForgotPayPassword(const std::string &account,
                                   const std::string &password,
                                   const std::string &shop_name,
                                   const std::string &shop_owner_name,
                                   const std::string &shop_owner_phone_number,
                                   const std::string &shop_owner_id_number,
                                   const std::string &new_pay_password,
                                   const std::string &new_confirm_pay_password) {

    for (auto it : all_seller_data) {
        if (it.account == account) {
            if (it.password == password) {
                if (it.shop_name == shop_name
                && it.shop_owner_name == shop_owner_name
                && it.shop_owner_phone_number == shop_owner_phone_number
                && it.shop_owner_id_number == shop_owner_id_number) {
                    if (ok_pay_password(new_pay_password, new_confirm_pay_password)) {
                        it.wallet.password = new_pay_password;

                        DB& db = DB::getInstance();
                        db.modify_seller_data(it.id, it);

                        return 0; // ok change
                    }
                    else return 3; // pay password is illegal
                }
                else return 2; // verification wrong
            }
            else return 1; // password is wrong
        }
    }

    return 1; // account is wrong
}

bool LoginSeller::ok_password(const std::string &password, const std::string &confirm_password) {

    if (password != confirm_password) return false;
    if (password.size() < 8 || password.size() > 16) return false;

    return true;
}


bool LoginSeller::ok_pay_password(const std::string &pay_password, const std::string &confirm_pay_password) {
    std::regex regex_pay_password("\\d*");

    if (pay_password.size() != 6) return false;
    if (pay_password != confirm_pay_password) return false;
    if (!std::regex_match(pay_password, regex_pay_password)) return false;

    return true;
}