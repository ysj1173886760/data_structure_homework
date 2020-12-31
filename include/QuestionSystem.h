//
// Created by dbdxzwh on 2020/12/26.
//

#ifndef DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H
#define DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H

#include "DB.h"
#include "base.h"
#include "BasicOperation.h"

class QuestionSystem {
private:
    //传递所问商家的编号以及关键字（将问题拆成一个个单词的vec）
    bool CheckKeyWord(const std::vector<std::string>&, const std::vector<std::string>&);
public:
    //传入商家的ID以及问题
    std::string GetAnswer(const std::string&, const std::string&);
};


#endif //DATA_STRUCTURE_HOMEWORK_QUESTIONSYSTEM_H
