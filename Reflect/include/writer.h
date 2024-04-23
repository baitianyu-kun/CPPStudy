//
// Created by 白天宇 on 2024/4/23.
//

#ifndef CPPSTUDY_WRITER_H
#define CPPSTUDY_WRITER_H

#include "processor.h"
#include "factory.h"

using namespace reflect;

class Writer : public Processor {
private:
GETCLASSNAME("Writer");
public:
    Writer() {};

    ~Writer() {};

    int Process();
};

#endif //CPPSTUDY_WRITER_H
