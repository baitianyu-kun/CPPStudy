//
// Created by 白天宇 on 2024/5/29.
//
#include "iostream"
#include "unordered_map"

using namespace std;

class Node {
public:
    int key, value;
    Node *prev, *next;

    Node(int k = 0, int v = 0) : key(k), value(v) {}
};

class LRUCache {
private:
    int _capacity;
    Node *head; // 哨兵节点
    unordered_map<int, Node*> key_to_node;

    // 删除一个节点（抽出一本书）
    void remove(Node *x) {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }

    // 在链表头添加一个节点（把一本书放在最上面）
    void push_front(Node *x) {
        x->prev = head;
        x->next = head->next;
        x->prev->next = x;
        x->next->prev = x;
    }

    // 获取书，抽出来，放在最上面
    Node *get_node(int key) {
        auto it = key_to_node.find(key);
        if (it == key_to_node.end()) // 没有这本书
            return nullptr;
        auto node = it->second; // 有这本书
        remove(node); // 把这本书抽出来
        push_front(node); // 放在最上面
        return node;
    }

public:
    LRUCache(int capacity) : _capacity(capacity), head(new Node()) {
        head->prev = head;
        head->next = head;
    }

    int get(int key) {
        auto node = get_node(key);
        return node ? node->value : -1;
    }

    void put(int key, int value) {
        // 书在，更新
        auto node = get_node(key);
        if (node) {
            node->value = value;
            return;
        }
        // 新书，插入到front
//        key_to_node[key] = node = new Node(key, value);
        Node * node1 = new Node(key,value);
        key_to_node.insert(pair<int,Node*>(key,node1));
//        key_to_node.emplace(key,node1);
//        key_to_node[key] = node1;
//        key_to_node[key] = node = new Node(key, value); // 新书
        push_front(node1);
        if (key_to_node.size() > _capacity) {
            // 最后一本没用的书
            auto end_node = head->prev;
            key_to_node.erase(end_node->key);
            remove(end_node);
            delete end_node;
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