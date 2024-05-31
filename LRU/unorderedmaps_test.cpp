//
// Created by 白天宇 on 2024/5/29.
//
#include "iostream"
#include "unordered_map"
#include "map"
#include "queue"

using namespace std;

int main() {

    // unordered map find 时间复杂度为o1
//    unordered_map<int,int> maps;
//    maps.emplace(1,1);
//
//    cout<<maps[2]; // 访问没有的key会返回0

    // 使用map对key进行排序，插入数据复杂度是ologn
//    map<int,int> maps_count;
//    maps_count[0]++;
//    maps_count[0]++;
//    maps_count[0]++;
//    maps_count[1]++;
//    for (auto &item:maps_count) {
//        cout<<item.first<<" "<<item.second<<endl;
//    }

    class comp {
    public:
        bool operator()(const pair<int, int> &source, const pair<int, int> &target) {
            return source.second<target.second;
        }
    };

    // 优先队列插入o1
    priority_queue<pair<int,int>,vector<pair<int,int>>,comp> pq;

    pq.push(pair<int,int>(1,1));
    pq.push(pair<int,int>(2,3));
    pq.push(pair<int,int>(3,9));
    pq.push(pair<int,int>(4,12));


    // 从大到小排序
    cout<<pq.top().first<<" "<<pq.top().second;



}