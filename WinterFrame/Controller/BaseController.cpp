//
// Created by 白天宇 on 2024/5/15.
//
#include "iostream"
#include "unordered_map"
#include "functional"

using namespace std;

struct HttpServletRequest {
};
struct HttpServletResponse {
};

// 将路径和需要调用的函数进行存储，使用unordered map，key为string，val为需要调用的函数
static unordered_map<string, function<void(HttpServletRequest *, HttpServletResponse *)>> funPathMap;


template<class T>
struct BaseController {

    static struct AutoInit {
        void doNothing() const {}

        explicit AutoInit() {
            static T t;
        }
    } init; // 一个变量或叫对象

    struct PathFunction {
        PathFunction(const string &path,
                     const function<void(HttpServletRequest *, HttpServletResponse *)> &func) {
            // 非静态函数可以访问静态成员，但是静态函数无法访问非静态变量，但是可以通过魔法来进行访问
            funPathMap.emplace(path, func);
        }
    };
};

template<class T>
// 在类外生成静态变量，但是模版用不到的话就不会实例化，只要静态成员没有用到那就不会被实例化
// 所以要告诉编译器用到init
typename BaseController<T>::AutoInit BaseController<T>::init;

// 进行继承
#define M_CONCAT_(a, b) a##b
#define M_CONCAT(a, b) M_CONCAT_(a, b)
#define RequestMapping(func, url) \
const PathFunction M_CONCAT(p, __LINE__) = { url,[this](HttpServletRequest *request, HttpServletResponse *response) {init.doNothing();this->func(request,response);} };\

struct MyController : public BaseController<MyController> {

//    const PathFunction p56 = {"user/name", [this](HttpServletRequest *request, HttpServletResponse *response) {
//        init.doNothing(); // 用一下init，从而使得编译器去实例化这个变量
//        this->pp(request, response); // 这里的this是我现在继承的对象的自身指针
//    }}; // 初始化列表的方式，实际上是调用了PathFunction的构造函数

    // Lambda 以[]子句开头。它指定捕获哪些变量，以及捕获是通过值还是通过引用进行的。就是入参
    RequestMapping(pp,"user/name")
    void pp(HttpServletRequest *request, HttpServletResponse *response){
    }

    RequestMapping(pp1,"user/name1")
    void pp1(HttpServletRequest *request, HttpServletResponse *response){
    }

};

int main() {

    for (const auto &item: funPathMap){
        cout<<item.first<<endl;
    }


}