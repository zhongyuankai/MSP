
#include <Core/PODArray.h>
#include <Thread/ThreadPool.h>
#include <Core/Types.h>
#include <iostream>

void testThreadPool() {
    ThreadPool pool(3, 5);
    pool.submit([](){ std::cout << "hello world" << std::endl; });

}





class TestVector
{
public:
    std::vector<TestVector *> v;
};


int main() {

    TestVector vector;
    size_t size = vector.v.size();

    std::cout << size << std::endl;
    std::cout << "hello world" << std::endl;

}






















//class Node {
//public:
//    int val;
//    Node* left;
//    Node* right;
//    Node* next;
//
//    Node() : val(0), left(NULL), right(NULL), next(NULL) {}
//
//    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}
//
//    Node(int _val, Node* _left, Node* _right, Node* _next)
//            : val(_val), left(_left), right(_right), next(_next) {}
//};
//
//class Solution {
//public:
//    vector<string> commonChars(vector<string>& A) {
//        vector<int> minVec(26, 100);
//        for (string s: A) {
//            int size = s.size();
//            vector<int> vec(26, 0);
//            for (int i = 0; i < size; i++) {
//                vec[s[i] - 'a']++;
//            }
//            for (int i = 0; i < 26; i++) {
//                minVec[i] = min(minVec[i], vec[i]);
//            }
//        }
//        vector<string> ans;
//        for (int i = 0; i < 26; i++) {
//            for (int j = 0; j < minVec[i]; j++) {
//                string s(1,i+'a');
//                ans.push_back(s);
//            }
//        }
//        return ans;
//    }
//};