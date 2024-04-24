//
// Created by 白天宇 on 2024/4/23.
//

#ifndef CPPSTUDY_FACTORY_H
#define CPPSTUDY_FACTORY_H

#include "processor.h"
#include "iostream"
#include "unordered_map"

using std::string;
using std::unordered_map;
using std::cout;
using std::endl;

namespace reflect {
/**
 * 在具体实现类的头文件添加这个宏，获取类名的方法
 */
    class Factory {
    private:
        // TODO 或者使用智能指针进行替换
        unordered_map<string, Processor *> class_maps;
        unordered_map<string, Processor *>::iterator maps_iter;
    public:
        Factory() {
            class_maps.clear();
        };

        ~Factory() = default;

        void Register(const string &className, Processor *processor); // 注册处理器
        Processor *Create(const string &className); // 通过类名的字符串，返回对应类的对象
        void Destroy(); // 销毁工厂中的对象
    };

#define GETCLASSNAME(str_class_name) \
    public:\
    static string ClassName() {\
        return str_class_name;\
    }\

}
#endif //CPPSTUDY_FACTORY_H
