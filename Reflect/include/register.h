//
// Created by 白天宇 on 2024/4/23.
//

#ifndef CPPSTUDY_REGISTER_H
#define CPPSTUDY_REGISTER_H

#include "factory.h"
#include "processor.h"
#include "reader.h"
#include "writer.h"
using namespace reflect;

#define REGISTER(str, pt) pfactory->Register(str, pt);

class Register {
private:
    Factory *pfactory;
public:
    Register();

    ~Register();

    void registers();
};


#endif //CPPSTUDY_REGISTER_H
