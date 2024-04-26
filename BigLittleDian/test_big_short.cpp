//
// Created by 白天宇 on 2024/4/26.
//
#include "iostream"

using namespace std;

int main() {

    // 联合包括一个 short 类型的 value 和一个 char 数组 union_bytes，其大小与 short 类型相同。
    // 如果系统是大端字节序，则 union_bytes[0] 中存储的是高位字节，即 0x01，而 union_bytes[1] 中存储的是低位字节，即 0x02。
    // 如果系统是小端字节序，则 union_bytes[0] 中存储的是低位字节，即 0x02，而 union_bytes[1] 中存储的是高位字节，即 0x01。
    union {
        short value;
        char union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    // 0x01 = 1 x 16^0 = 1，所以可以用union来进行判断
    // cout<<1<<" "<<0x01<<endl;
    // cout<<(test.union_bytes[1] == 1)<<" "<<(test.union_bytes[1] == 0x01)<<endl;
    if ((test.union_bytes[0] == 1) && test.union_bytes[1] == 2) {
        cout << "big dian" << endl;
    } else if ((test.union_bytes[1] == 1) && test.union_bytes[0] == 2) {
        cout << "little dian" << endl;
    }

}