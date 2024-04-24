//
// Created by 白天宇 on 2024/4/23.
//
#include "../include/factory.h"

namespace reflect {
    void Factory::Register(const string &className, Processor *processor) {
        auto tmp_iter = class_maps.find(className);
        if (tmp_iter == class_maps.end()) {
            // 不存在插入
            class_maps.insert(std::pair<string, unique_ptr<Processor>>(className, processor));
        } else {
            tmp_iter->second.reset(processor);
        }
    }

    // 维持的单例模式
    Processor *Factory::Create(const string &className) {
        auto tmp_iter = class_maps.find(className);
        if (tmp_iter == class_maps.end()) {
            cout << "In factory, " << className << " object is no exist.\n";
            return nullptr;
        } else {
            return tmp_iter->second.get();
        }
    }

    void Factory::Destroy() {
        class_maps.clear();
    }
}