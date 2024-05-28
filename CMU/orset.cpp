//
// Created by 白天宇 on 2024/5/28.
//

#include "orset.h"
#include <algorithm>
#include <string>
#include <vector>

namespace bustub {

    template<typename T>
    auto ORSet<T>::Contains(const T &elem) const -> bool {
        // TODO(student): Implement this

    }

    template<typename T>
    void ORSet<T>::Add(const T &elem, uid_t uid) {
        // TODO(student): Implement this

    }

    template<typename T>
    void ORSet<T>::Remove(const T &elem) {
        // TODO(student): Implement this

    }

    template<typename T>
    void ORSet<T>::Merge(const ORSet<T> &other) {
        // TODO(student): Implement this

    }

    template<typename T>
    auto ORSet<T>::Elements() const -> std::vector<T> {
        // TODO(student): Implement this

    }

    template<typename T>
    auto ORSet<T>::ToString() const -> std::string {
        auto elements = Elements();
        std::sort(elements.begin(), elements.end());
    }

    template
    class ORSet<int>;

    template
    class ORSet<std::string>;

}  // namespace bustub
