//
// Created by 白天宇 on 2024/5/29.
//
#include "iostream"
#include "unordered_map"
#include "queue"

using namespace std;

class LRUCache {
private:
    int _capacity = 0;
    unordered_map<int, int> maps;

    class comp {
    public:
        bool operator()(const pair<int, int> &source, const pair<int, int> &target) {
            return source.second > target.second;
        }
    };

    priority_queue<pair<int, int>, vector<pair<int, int>>, comp> pq;
public:
    LRUCache(int capacity) {
        _capacity = capacity;
    }

    int get(int key) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, comp> _pq;
        if (maps.find(key) != maps.end()) {
            int val = maps.at(key);
            // 当前元素使用频率+1
            for (int i = 0; i < maps.size(); ++i) {
                int _key = pq.top().first;
                int _val = pq.top().second;
                if (_key == key) {
                    _val = 2;
                } else {
                    _val = 1;
                }
                _pq.emplace(_key, _val);
                pq.pop();
            }
            pq.swap(_pq);
            return val;
        }
        return -1;
    }

    void put(int key, int value) {
        // key不存在插入，插入时候满足 capacity < _capacity, key存在的话更新value
        if (maps.find(key) == maps.end()) {
            if (maps.size() == _capacity) {
                // 移除最久未被使用的元素,就是pq top，使用频率最小的
                maps.erase(pq.top().first);
                pq.pop();
            }
            maps.emplace(key, value);
            // 当前元素的使用频率为1
            pq.emplace(key, 1);
        } else {
            maps[key] = value;
        }
    }

    void print_all() {
        cout << endl;
        for (int i = 0; i < _capacity; ++i) {
            cout << pq.top().first << " " << pq.top().second << endl;
            pq.pop();
        }
    }

    void print_all_maps() {
        cout << endl;
        for (auto item: maps) {
            cout << item.first << " " << item.second << endl;
        }
    }
};

int main() {
    LRUCache lRUCache(2);
    lRUCache.put(1, 1); // 缓存是 {1=1}
    lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}

//    lRUCache.print_all();
//    lRUCache.print_all_maps();

    lRUCache.get(1);    // 返回 1
    lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}

//    lRUCache.print_all();
//    lRUCache.print_all_maps();

    lRUCache.get(2);    // 返回 -1 (未找到)
    lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
//
    lRUCache.print_all();
    lRUCache.print_all_maps();

    cout<<lRUCache.get(1)<<endl;    // 返回 -1 (未找到)
    cout<<lRUCache.get(3)<<endl;    // 返回 3
    cout<<lRUCache.get(4)<<endl;    // 返回 4
}