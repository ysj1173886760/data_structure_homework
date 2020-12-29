//
// Created by shrimp on 2020/12/29.
//

#ifndef DATA_STRUCTURE_HOMEWORK_EMAIL_H
#define DATA_STRUCTURE_HOMEWORK_EMAIL_H

#include <unistd.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/inotify.h>
#include <string>

static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class Email {
public:
    Email();
    ~Email();

    // return 0, if your send email successfully
    // return 1, the email's account is wrong
    // return 2, failed to send email
    int send_email(const std::string& Target_email,
                   const std::string& Title,
                   const std::string& Content);

private:
    int port;
    int socket_client;
    std::string domain;

    std::string user_email;
    std::string user_password;

    std::string target_email;
    std::string title;
    std::string content;

    static const int email_length = 1024;
    char buff[email_length + 1];

    char* base64Encode(char const*, unsigned int);
    bool _create_connect();
    bool _login();
    bool _send_email_head();
    bool _send_email_text();
    bool _send(const std::string& str) const;
    bool _recv();
};


#endif //DATA_STRUCTURE_HOMEWORK_EMAIL_H
