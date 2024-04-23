//
// Created by 白天宇 on 2024/4/23.
//

#ifndef CPPSTUDY_READER_H
#define CPPSTUDY_READER_H

#include "processor.h"
#include "factory.h"

using namespace reflect;

class Reader : public Processor {
private:
    GETCLASSNAME("Reader");
public:
    Reader() {};

    ~Reader() {};

    int Process();
};

#endif //CPPSTUDY_READER_H
