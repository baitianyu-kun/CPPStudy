//
// Created by 白天宇 on 2024/5/27.
//
#include <emmintrin.h>

#include "iostream"
#include "unordered_map"

using namespace std;

struct Node {
    int pass = 0;
    int end = 0;
    // 使用字符来作为路径
    unordered_map<char, Node *> sons;

    Node() : pass(0), end(0), sons() {
    }

    // 使用空对象来进行分配
    Node(int _pass, int _end) : pass(_pass), end(_end), sons() {
    }
};

class Trie {
private:
    Node *root;

public:
    // 初始化根结点
    Trie() : root(new Node) {
    }

    void print_all() {
        print_all(root);
    }

    void print_all(Node *p, string prefix = "") {
        if (p == nullptr) return;
        // 以end大于0结尾证明有一个字符串存在，所以把之前累积的prefix全都进行输出
        if (p->end > 0) {
            cout << prefix << endl;
        }
        // for找当前节点的左右两个节点都包含哪些
        for (auto &item: p->sons) {
            char path = item.first;
            Node *next = item.second;
            // 根据当前的path和next进行递归调用，并且自动把path进行拼接，直到end!=0，即构成一个完整字符串
            print_all(next, prefix + path);
        }
    }

    // 某个前缀的出现次数
    int search_pre(string &word) {
        if (word == "") return 0;
        Node *p = root;
        char path;
        for (int i = 0; i < word.length(); ++i) {
            path = word[i];
            if (p->sons.find(path) == p->sons.end()) {
                return 0;
            }
            p = p->sons.at(path);
        }
        // 到最后一个节点，其的pass就是经过该节点的字符串数，即该前缀的出现次数
        // 在最后返回时，返回的是字符串尾节点的 pass 值，它代表有多少个字符串经过了这个节点
        return p->pass;
    }

    void insert(string &word) {
        if (word == "") return;
        // 从根节点开始遍历
        Node *p = root;
        p->pass++; // 经过根节点，根节点数目加一
        char path;
        for (int i = 0; i < word.length(); i++) {
            path = word[i];
            if (p->sons.find(path) == p->sons.end())
                p->sons.emplace(path, new Node);
            p = p->sons.at(path); // 指针后移，指向新的点
            p->pass++; // 经过这个path，所以pass增加
        }
        p->end++; // 遍历完以后，end表示有多少个以当前路径结尾的字符串，所以在末尾++
    }

    int search(string &word) const {
        if (word == "") return 0;
        Node *p = root;
        char path;
        for (int i = 0; i < word.length(); ++i) {
            path = word[i];
            if (p->sons.find(path) == p->sons.end()) {
                return 0;
            }
            p = p->sons.at(path);
        }
        // 经过遍历后，节点指针一定会来到最后一个字符路径的尾节点，
        // 这个节点的 end 记录了总共有多少个字符串以这个字符路径结尾，所以直接返回 end 即可
        return p->end;
    }

    void deletes(string &word) {
        if (word == "") return;
        Node *p = root;
        p->pass--; // 根节点--
        char path;
        for (int i = 0; i < word.length(); ++i) {
            path = word[i];
            //如果 node的pass属性--后是0，那么需要将节点引用置为 null，以便回收内存，不是node的下一个
            if (--p->sons.at(path)->pass == 0) {
                p->sons.at(path) = nullptr;
                return;
            }
            p = p->sons.at(path); // 移动节点指针
        }
        // pass都清空后，说明该字符串不经过上面的节点了，此时end代表有多少字符串以这个结尾，所以end--
        // 遍历后沿途的 pass 都已经--，最后 end--
        p->end--;
    }
};

int main() {
    Trie trie;
    string str1 = "abcd";
    trie.insert(str1);

    trie.print_all();

    // string se_str = "abc";
    // string se_str1 = "abcd";
    // string se_str2 = "abd";
    // cout << trie.search(se_str) << endl;
    // cout << trie.search(se_str1) << endl;
    // cout << trie.search(se_str2) << endl;
    //
    // string se_pre_str = "a";
    // string se_pre_str1 = "ab";
    // string se_pre_str2 = "abc";
    //
    // cout << trie.search_pre(se_pre_str) << endl;
    // cout << trie.search_pre(se_pre_str1) << endl;
    // cout << trie.search_pre(se_pre_str2) << endl;
    //
    cout<<endl;
    string delete_str = "abcd";
    trie.deletes(delete_str);
    trie.print_all();
}
