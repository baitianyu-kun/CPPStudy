//
// Created by 白天宇 on 2024/4/23.
//

#ifndef CPPSTUDY_PROCESSOR_H
#define CPPSTUDY_PROCESSOR_H

#include "iostream"

namespace reflect {
    class Processor {
    public:
        Processor() {}

        virtual ~Processor() {}

        virtual int Process() = 0; // 返回0表示处理成功，否则处理失败
    private:

    };
}

#endif //CPPSTUDY_PROCESSOR_H
