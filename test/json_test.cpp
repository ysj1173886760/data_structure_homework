//
// Created by sheep on 2020/12/24.
//

#include <fstream>
#include <iostream>
#include <json.h>
#include <cassert>
#include <errno.h>
#include <string.h>

using namespace std;

void read_json() {
    ifstream ifs;
    ifs.open("../data/data.json");
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(ifs, root, false))
    {
        cout << "reader parse error: " << strerror(errno) << endl;
        return;
    }

    string name;
    int age;
    int size;

    size = root.size();
    cout << "total " << size << " elements" << endl;
    for (int i = 0; i < size; ++i)
    {
        name = root[i]["name"].asString();
        age = root[i]["age"].asInt();

        cout << "name: " << name << ", age: " << age << endl;
    }
    ifs.close();
}

void write_json() {
    Json::Value root;
    Json::FastWriter writer;
    Json::Value person;


    person["age"] = 28;
    person["name"] = "sb";
    root.append(person);

    person["age"] = 20;
    person["name"] = "test";
    root.append(person);
    string json_file = writer.write(root);

    ofstream ofs;
    ofs.open("../data/data.json");
    assert(ofs.is_open());
    ofs << json_file;
    ofs.close();
    return;
}
int main()
{
    read_json();
    write_json();
    return 0;
}