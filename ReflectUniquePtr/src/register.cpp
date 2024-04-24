//
// Created by 白天宇 on 2024/4/23.
//
#include "../include/register.h"

namespace reflect {
    Register::Register() {
        pfactory = new Factory();
    }

    Register::~Register() {
        if (pfactory) {
            delete pfactory;
            pfactory = nullptr;
        }
    }

    Factory *Register::registerFunction() {
        REGISTER(Reader::ClassName(), new Reader())
        REGISTER(Writer::ClassName(), new Writer())
        return pfactory;
    }
}
