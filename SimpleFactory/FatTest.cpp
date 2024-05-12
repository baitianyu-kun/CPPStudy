//
// Created by 白天宇 on 2024/5/8.
//
#include<iostream>
#include<string>
using namespace std;
class Produce;//声明产品类
class Factory;//声明工厂类
class Factory
{
public:
    Factory(){
    }
    ~Factory(){
    }
    virtual Produce* create_produce(string class_name,string name)=0;
    //工厂生产产品,这里的纯虚函数是为了让子工厂生产产品
    //返回的是一个产品类指针，这就代表了一个产品
    //这里的name参数是为了返回想要的产品qwq
};
class Produce
{
public:
    Produce(){
    }
    ~Produce(){
    }
    virtual void show_myname()=0;//这里定义的纯虚函数是为了让派生类去实现这个函数，有纯虚函数的类是抽象类
};
//现在我要开始创建具体的产品了
class Produce_apple:public Produce//这是一个具体的苹果类
{
private:
    string name;
public:
    Produce_apple(string new_name="")
    {
        this->name = new_name;
    }
    virtual void show_myname()//重写父类函数
    {
        cout<<"我的名称是："<<name<<endl;
    }
};
class Produce_pear:public Produce//这是一个具体的梨类
{
private:
    string name;
public:
    Produce_pear(string new_name="")
    {
        this->name = new_name;
    }
    virtual void show_myname()//重写父类函数
    {
        cout<<"我的名称是："<<name<<endl;
    }
};
class Produce_badfruits:public Produce//这是一个具体的坏水果类
{
private:
    string name;
public:
    Produce_badfruits(){
    }
    virtual void show_myname()//重写父类函数
    {
        cout<<"此产品已经过期！！"<<endl;
    }
};
//下面开始创建具体的工厂类
class Factory_fruits:public Factory//这是个水果工厂类
{
public:
    Factory_fruits(){
    }
    ~Factory_fruits(){
    }
    virtual Produce* create_produce(string class_name,string name)
    {
        if (class_name=="apple")
        {
            Produce * my_produce = new Produce_apple(name);//创建name的apple产品
            return my_produce;
        }
        else if (class_name=="pear")
        {
            Produce * my_produce = new Produce_pear(name);//创建name的pear产品
            return my_produce;
        }
        else
        {
            Produce * my_produce = new Produce_badfruits();//创建name的pear产品
            return my_produce;
        }
    }
};
//初期的搭建工作已经完成，总结一下，我们搭建了两个抽象类：工厂类，产品类；
//产品派生类：苹果类，梨类，坏水果类
//工厂派生类：水果工厂类。
//现在我们要用这些东西了
int main()
{
    Factory * my_factory_fruits = new Factory_fruits();//创建一个抽象工厂对象"升级"为水果工厂
    //这里的"升级"是我的理解 ,事实上是类型转换
    Produce * my_produce_apple = my_factory_fruits->create_produce("apple","红富士");
    //创建抽象产品对象，"升级"为水果工厂加工出来的apple,红富士
    Produce * my_produce_pear = my_factory_fruits->create_produce("pear","冰糖雪梨");
    //创建抽象产品对象，"升级"为水果工厂加工出来的pear,冰糖雪梨
    Produce * my_produce_banana = my_factory_fruits->create_produce("banana","大香蕉");
    //创建抽象产品对象，"升级"为水果工厂加工出来的banana,大香蕉，但是工厂不能生产banana,所以只能生产badfruit坏水果
    my_produce_apple->show_myname();
    //产品显示自己的名称
    my_produce_pear->show_myname();
    //产品显示自己的名称
    my_produce_banana->show_myname();
    //产品显示自己的名称

    //下面是销毁内存
    delete my_factory_fruits;
    my_factory_fruits = NULL;
    delete my_produce_apple;
    my_produce_apple = NULL;
    delete my_produce_pear;
    my_produce_pear = NULL;
    delete my_produce_banana;
    my_produce_banana = NULL;
    return 0;
}
