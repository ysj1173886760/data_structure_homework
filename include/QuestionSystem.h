//
// Created by dbdxzwh on 2020/12/26.
//

#ifndef DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H

#include "DB.h"
#include "base.h"
#include "BasicOperation.h"

class QuestionSystem {
public:
    //传递所问商家的编号以及问题
    bool CheckKeyWord(const std::string&, const std::vector<string>&);
    std::string GetAnswer(const std::string&, const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H
