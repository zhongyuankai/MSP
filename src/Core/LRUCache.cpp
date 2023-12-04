#include <unordered_map>
#include <iostream>
#include <vector>

struct Node {
    int key;
    int value;
    Node* pre;
    Node* next;
    Node() : Node(-1, -1, NULL, NULL) {}
    Node(int key_, int value_) : Node(key_, value_, NULL, NULL) {}
    Node(int key_, int value_, Node* pre_, Node* next_) : key(key_), value(value_), pre(pre_), next(next_) {}
};


class LRUCache {
public:


    LRUCache(int capacity_) : capacity(capacity_) {
        head = new Node();
        tail = new Node();
        head->pre = head;
        head->next = tail;
        tail->pre = head;
        tail->next = tail;
    }

    int get(int key) {
        std::cout << "get: " << key << std::endl;
        auto it = map.find(key);
        if (it == map.end()) {
            std::cout << -1  << std::endl;
            return -1;
        }

        Node* node = it->second;

        std::cout << node->value << std::endl;
        removeListNode(node);

        addListHead(node);
        printList();
        return node->value;
    }

    void put(int key, int value) {
        std::cout << "put: " << key << " " << value << std::endl;
        if (size == capacity) {
            Node* node = tail->pre;
            removeListNode(node);
            map.erase(node->key);
            delete node;
            --size;
            printList();
        }
        // 先忽略重复的问题
        Node* node = new Node(key, value);

        addListHead(node);

        map.emplace(key, node);
        ++size;
        printList();
    }

    void addListHead(Node* node) {
        head->next->pre = node;
        node->next = head->next;
        head->next = node;
        node->pre = head;
    }

    void removeListNode(Node* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
    }

    void printList() {
        Node* cur = head;
        while (cur->next != tail) {
            cur = cur->next;
            std::cout << cur->key << "->";
        }
        std::cout << std::endl;
    }

    int size;
    int capacity;
    std::unordered_map<int, Node*> map;
    Node* head;
    Node* tail;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

int main (int argc, char** argv) {

    int n = 2;
    int m = 3;
    std::vector<int> products {1, 2, 3};


    std::vector<int> persons(n);
    std::sort(products.begin(), products.end(), std::greater<int>());
    int sum = 0;
    int step = 1;
    int index = 0;
    for (int i = 0; i < m; ++i) {
        persons[index] += products[i];
        sum += products[i];
        if (index+step == n) {
            step = -1;
            continue;
        } else if (index+step == -1) {
            step = 1;
            continue;
        }
        index += step;

    }
    int ans = 0;
    int avg = sum / n;
    for (int i = 0; i < n; ++i) {
        ans += std::abs(persons[i] - avg);
    }

    std::cout << "ans = " << ans << std::endl;




















//    LRUCache* lRUCache = new LRUCache(2);
//    lRUCache->put(1, 1); // 缓存是 {1=1}
//    lRUCache->put(2, 2); // 缓存是 {1=1, 2=2}
//    lRUCache->get(1);    // 返回 1
//    lRUCache->put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
//    lRUCache->get(2);    // 返回 -1 (未找到)
//    lRUCache->put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
//    lRUCache->get(1);    // 返回 -1 (未找到)
//    lRUCache->get(3);    // 返回 3
//    lRUCache->get(4);    // 返回 4

    return 0;
}

