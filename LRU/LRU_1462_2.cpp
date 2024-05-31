//
// Created by 白天宇 on 2024/5/31.
//
#include "iostream"
#include "unordered_map"

using namespace std;

struct Node {

    int key, value;
    Node *prev, *next;

    Node() {}

    Node(int _key, int _value) : key(_key), value(_value), prev(nullptr), next(nullptr) {}

};

class LRUCache {
private:
    int capacity = 0;
    Node *dummy; // 哨兵节点，前后都有元素
    unordered_map<int, Node *> maps;

    // 移除节点
    void removeEle(Node *x) {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }

    // 将节点放到最上方
    void push_top(Node *x) {
        x->prev = dummy;
        x->next = dummy->next;
        dummy->next->prev = x;
        dummy->next = x;
    }

    // 获取的时候，根据当前key找到对应的node，然后把node返回并放到最上方
    Node *getNode(int key) {
        if (maps.find(key) == maps.end()) {
            return nullptr;
        }
        auto node = maps.at(key);
        // 抽出来，放到最上方
        removeEle(node);
        push_top(node);
        return node;
    }


public:

    LRUCache(int _capacity) : capacity(_capacity), dummy(new Node) {
        // 自身的上一个指向上一个，自身的下一个也指向下一个，再往中间插入，就可以实现一个环状
        dummy->prev = dummy;
        dummy->next = dummy;
    }

    int get(int key) {
        auto node = getNode(key);
        return node ? node->value : -1;
    }

    void put(int key, int value) {
        // 如果存在的话就更新value，并放在最上方
        auto node = getNode(key);
        if (node) {
            node->value = value;
            return;
        }
        // 不存在就添加，并放在最上方
        auto node1 = new Node(key,value);
        maps.emplace(key,node1);
        push_top(node1);
        // 移除老的元素
        if (maps.size()>capacity){
            auto node2 = dummy->prev;
            maps.erase(node2->key); // maps移除
            removeEle(node2); // 删除节点
            delete node2;
        }
    }
};


int main() {
    LRUCache lRUCache(2);
    lRUCache.put(1, 1); // 缓存是 {1=1}
    lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
    lRUCache.get(1);    // 返回 1
    lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    lRUCache.get(2);    // 返回 -1 (未找到)
    lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    lRUCache.get(1);    // 返回 -1 (未找到)
    lRUCache.get(3);    // 返回 3
    lRUCache.get(4);

}