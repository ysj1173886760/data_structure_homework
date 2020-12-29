//
// Created by shrimp on 2020/12/29.
//

#include "Email.h"
#include <iostream>
#include <cstring>

Email::Email() {
    port = 25;
    domain = "smtp.qq.com";
    socket_client = 0;

    user_email = "1953663575@qq.com";
    user_password = "qwguniuqziuiecai";

    target_email = "";
    title = "";
    content = "";
}

Email::~Email() {
    close(socket_client);
}

char * Email::base64Encode(const char *origSigned, unsigned int origLength) {
    auto const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set
    if (orig == nullptr) return nullptr;

    unsigned const numOrig24BitValues = origLength / 3;
    bool havePadding = origLength > numOrig24BitValues * 3;
    bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
    unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
    char* result = new char[numResultBytes + 3]; // allow for trailing '/0'

    unsigned i;
    for (i = 0; i < numOrig24BitValues; ++i)
    {
        result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
        result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
        result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
        result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
    }

    if (havePadding)
    {
        result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
        if (havePadding2)
        {
            result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
            result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
        }
        else
        {
            result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
            result[4 * i + 2] = '=';
        }
        result[4 * i + 3] = '=';
    }

    result[numResultBytes] = '\0';
    return result;
}

int Email::send_email(const std::string &Target_email, const std::string &Title, const std::string &Content) {
    target_email = Target_email;
    title = Title;
    content = Content;

    if (!_create_connect()) return 1;
    if (!_login()) return 1;
    if (!_send_email_head()) return 1;
    if (!_send_email_text()) return 2;
//    if (!_send_email_end()) return 5;
    return 0;
}

bool Email::_create_connect() {
    int socket_client_temp = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sock_addr = {};
    hostent* p = gethostbyname(domain.c_str());

    sock_addr.sin_port = htons(port);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = *(unsigned long*)p->h_addr_list[0];

    int err = connect(socket_client_temp, (sockaddr*) &sock_addr, sizeof(sock_addr));
    if (err != 0) return false;

    socket_client = socket_client_temp;
    if (!_recv()) return false;

    return true;
}

bool Email::_login() {
    std::string str;
    str = "EHLO " + user_email + "\r\n";
    if (!_send(str) || !_recv()) return false;

    str.clear();
    str = "AUTH LOGIN\r\n";
    if (!_send(str) || !_recv()) return false;

    char* e_code = base64Encode(user_email.c_str(), user_email.size());
    str = e_code;
    str += "\r\n";
    delete[] e_code;

    if (!_send(str) || !_recv()) return false;

    str.clear();
    e_code = base64Encode(user_password.c_str(), user_password.size());
    str = e_code;
    str += "\r\n";
    delete[] e_code;

    if (!_send(str) || !_recv()) return false;
    return true;
}

bool Email::_send_email_head() {
    std::string str;
    str = "MAIL FROM: <" + user_email + ">\r\n";
    if (!_send(str) || !_recv()) return false;

    str.clear();
    str = "RCPT TO: <" + target_email + ">\r\n";
    if (!_send(str) || !_recv()) return false;
    if (strstr(buff, "550") != nullptr) return false;

    str.clear();
    str = "DATA\r\n";
    if (!_send(str) || !_recv()) return false;

    str.clear();
    str += "FROM: " + user_email + "\r\n";
    str += "TO:" + target_email + "\r\n";
    str += "SUBJECT: " + title + "\r\n";
    str += "MIME-Version: 1.0";
    str += "\r\n";
    str += "Content-Type: text/plain;";
    str +="\r\n";
    if (!_send(str)) return false;

    return true;
}

bool Email::_send_email_text() {
    std::string str;
    str += "Content-Type: text/plain;";
    str += "charset=\"utf-8\"\r\n";
    str += content;
    str += "\r\n.\r\n";
    if (!_send(str) || !_recv()) return false;
    str.clear();
    str = "QUIT\r\n";
    return _send(str);
}


bool Email::_send(const std::string &str) const {
    int err = send(socket_client, str.c_str(), str.size(), 0);

    if (err < 0) return false;

    std::cout << str.c_str() << std::endl;
    return true;
}

bool Email::_recv() {
    memset(buff, 0, sizeof(char) * (email_length) + 1);
    int err = recv(socket_client, buff, email_length, 0);

    if (err < 0) return false;

    std::cout << buff << std::endl;

    return true;
}