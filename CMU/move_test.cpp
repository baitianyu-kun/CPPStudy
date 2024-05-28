//
// Created by 白天宇 on 2024/5/28.
//
#include "iostream"
#include "vector"

using namespace std;

class M {
public:
    int s = 12;
};

auto return_vec_test() -> vector<int> {
    vector<int> a = {1, 2, 3, 4, 5};
    // 传递的是左值，推导为左值引用，仍旧static_cast转换为右值引用。
    // 传递的是右值，推导为右值引用，仍旧static_cast转换为右值引用。
    // return std::move(a); // 延长a的声明周期，将左值变量转换为右值引用

    // c++11 函数内部返回对象使用move语义的最佳实践
    // cpp11直接return a就行，会自动调用move
    // 一句话，直接返回即可，一般不用任何特殊写法。
    // 当启动了c++11选项后，通过函数返回代码没有发生任何变化，但是已经使用了move语义，而不一定使用之前介绍的NRVO编译器优化技术。
    // 注意，右值引用rvalue reference是表达式计算完成后就不再存在的临时变量，左值是表达式计算完成后的变量。如果能够用&求址的，就是左值。

    // 在c++11以后，编译器自动执行了优化，只要该类型有移动构造函数即可保证效率

    // 旧版本了
    // 下面是stackoverflow上的一个讨论贴，比较有价值：
    // 结论是:
    // 1. 应该按照正常写法（Best Practice）返回local 变量
    // 2. 编译器会决定要么使用NRVO，要么使用Move语义来优化返回语句
    // 如果使用条件语句返回，会抑制编译器使用NRVO
    // 也不要将返回的值包装在另一个函数中返回，而应该仅仅返回local变量
    // 如果要返回一个local变量内的成员变量，需要显式的使用move
    // 3. 如果使用Move语义，需要返回的类型有move constructor，否则只会进行复制
    return a;
}


int main() {

    vector<int> b = return_vec_test();

    for (auto item: b) {
        cout << item << endl;
    }

}