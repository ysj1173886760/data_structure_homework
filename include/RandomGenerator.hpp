//
// Created by sheep on 2020/12/27.
//

#ifndef DATA_STRUCTURE_HOMEWORK_RANDOMGENERATOR_H
#define DATA_STRUCTURE_HOMEWORK_RANDOMGENERATOR_H

#include <random>
#include <string>
#include <ctime>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<double> ran_double;
    std::uniform_int_distribution<int> ran_int;
    std::uniform_int_distribution<int> ran_char;

    RandomGenerator() {
        mt = std::mt19937 (rd());
        ran_double = std::uniform_real_distribution<double> (0.0, 1000.0);
        ran_int = std::uniform_int_distribution<int> (0, 10000000);
        ran_char = std::uniform_int_distribution<int> (0, 25);
    }

public:
    RandomGenerator (const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    static RandomGenerator& getInstance() {
        static RandomGenerator instance;
        return instance;
    }

    void reset_int_distribution(int lower_bound, int upper_bound) {
        ran_int = std::uniform_int_distribution<int> (lower_bound, upper_bound);
    }

    void reset_double_distribution(double lower_bound, double upper_bound) {
        ran_double = std::uniform_real_distribution<double> (lower_bound, upper_bound);
    }

    int getRandomInt(int lower_bound, int upper_bound) {
        return ran_int(mt) % (upper_bound - lower_bound) + lower_bound;
    }

    double getRandomDouble(double lower_bound, double upper_bound) {
        reset_double_distribution(lower_bound, upper_bound);
        return ran_double(mt);
    }

    char getRandomChar() {
        return ran_char(mt) + 'a';
    }

    std::string getRandomString(int size) {
        string ans = "";
        while (size--) {
            ans += getRandomChar();
        }
        return ans;
    }

    std::string getIdentifyCode(int size = 6) {
        std::string ans = "";
        for (int i = 0; i < size; i++) {
            int type = getRandomInt(0, 3);
            switch (type) {
                case 0:
                    ans += getRandomInt(0, 10) + '0';
                    break;
                case 1:
                    ans += getRandomInt(0, 26) + 'a';
                    break;
                case 2:
                    ans += getRandomInt(0, 26) + 'A';
                    break;
                default:
                    break;
            }
        }
        return ans;
    }
};


#endif //DATA_STRUCTURE_HOMEWORK_RANDOMGENERATOR_H
