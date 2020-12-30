//
// Created by shrimp on 2020/12/28.
//

#ifndef DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H
#define DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H

#include "RegisterManager.h"
#include "RegisterSeller.h"
#include "base.h"
#include "DB.h"
#include <string>
#include <vector>

class ManagerOperation {
public:
    static ManagerOperation& getInstance();

    // return vector about register_request_data
    std::vector<RegisterRequestData> view_application_list();

    // accept seller application
    // input: seller_account, seller_shop_name
    bool accept_shop_application(const std::string& account,
                                 const std::string& shop_name);

    // reject seller application
    // input: seller_account, seller_shop_name
    bool reject_shop_application(const std::string& account,
                                 const std::string& shop_name);

    // register a new manager (only support register new account by root manager)
    // input: root manager's account, root manager's password,
    //        new_guy_account, new_guy_password, new_guy_confirm_password.
    bool register_manager(const std::string& account,
                          const std::string& password,
                          const std::string& new_guy_account,
                          const std::string& new_guy_password,
                          const std::string& new_guy_confirm_password);

    // remove user data, because the user break the rule.
    // input: user_account, user_email.
    bool remove_user(const std::string& user_account,
                     const std::string& user_email);

    // remove seller data, because the seller break the rule.
    // input: seller_account, seller_shop_name, seller_phone_number
    bool remove_seller(const std::string& seller_account,
                       const std::string& seller_shop_name,
                       const std::string& seller_phone_number);

    // remove manager data, because the manager break the rule.
    // input: root_account, root_password, manager_account
    // only support root delete manager, of course root can't delete himself.
    bool remove_manager(const std::string& root_account,
                        const std::string& root_password,
                        const std::string& manager_account);
private:
    ManagerOperation() = default;

    // return a object about seller data
    SellerData copy_request_to_seller(const RegisterRequestData& rqd);
};


#endif //DATA_STRUCTURE_HOMEWORK_MANAGEROPERATION_H
