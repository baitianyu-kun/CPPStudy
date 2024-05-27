//
// Created by 白天宇 on 2024/5/27.
//
#include "iostream"
#include "unordered_map"

using namespace std;

class Node {
private:
    int pass = 0;
    int end = 0;
    // 字符ascii码作为路径来判断是否存在元素，用hashmap
    unordered_map<int, Node *> nexts;
public:
    void pass_add() {
        pass++;
    }

    void end_add() {
        end++;
    }

    Node() : pass(0), end(0), nexts() {}

    // 使用空对象来进行分配
    Node(int _pass, int _end) : pass(_pass), end(_end), nexts() {}
};

class Trie {
private:
    Node *root;
public:
    // 初始化根结点
    Trie() : root(new Node) {}

    void insert(string word) {
        Node *p = root;
        p->pass_add();
    }

};

int main() {


}