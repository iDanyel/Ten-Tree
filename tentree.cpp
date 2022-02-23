#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

template <typename T, typename V> class TenTree {
private:
    class TenTreeNode;

    TenTreeNode* root = nullptr;
    unordered_map<V, T> map;
    function<V(T)> func;

    class TenTreeNode {
    public:
        int positives = 0, negatives = 0;
        TenTreeNode** children;

        TenTreeNode() {
            children = new TenTreeNode*[10];

            for (int i = 0; i < 10; i++)
                children[i] = nullptr;
        }
    };

    T powerOfTen(T e) {
        T p = 1;

        while (e / 10) {
            p *= 10;
            e /= 10;
        }

        return p;
    }

    void insert(TenTreeNode* root, T e, T p, bool sign) {
        if (!root->children[e / p % 10])
            root->children[e / p % 10] = new TenTreeNode;

        if (p == 1) {
            if (sign)
                root->children[e % 10]->negatives++;
            else
                root->children[e % 10]->positives++;

            return;
        }

        insert(root->children[e / p % 10], e, p / 10, sign);
    }

    int count(TenTreeNode* root, T e, T p, bool sign) {
        if (p == 1) {
            if (sign)
                return root->children[e % 10]->negatives;

            return root->children[e % 10]->positives;
        }

        if (root->children[e / p % 10])
            return count(root->children[e / p % 10], e, p / 10, sign);

        return 0;
    }

public:
    TenTree() {
        root = new TenTreeNode;
        this->func = [](T e) {
            return e;
        };
    }

    TenTree(function<V(T)> func) {
        root = new TenTreeNode;
        this->func = func;
    }

    void insert(T e) {
        V v = func(e);

        map[v] = e;
        insert(root, abs(v), powerOfTen(abs(v)), v < 0);
    }

    int count(T e) {
        return count(root, abs(e), powerOfTen(abs(e)), e < 0);
    }

    void sort(vector<T>& v) {
        queue<pair<TenTreeNode*, T>> q;
        vector<T> v_, v__;

        v.erase(v.begin(), v.end());
        q.push({root, 0});

        while (!q.empty()) {
            TenTreeNode* currNode = q.front().first;
            T currNo = q.front().second;

            for (int i = 0; i < 10; i++)
                if (currNode->children[i]) {
                    q.push({currNode->children[i], currNo * 10 + i});

                    T e = map[currNo * 10 + i];

                    for (int j = 0; j < currNode->children[i]->negatives; j++)
                        v_.push_back(e);

                    for (int j = 0; j < currNode->children[i]->positives; j++)
                        v__.push_back(e);
                }

            q.pop();
        }

        for (int i = v_.size() - 1; i >= 0; i--)
            v.push_back(-v_[i]);

        for (int i = 0; i < v__.size(); i++)
            v.push_back(v__[i]);
    }
};

int main() {
    TenTree<int, int>* tenTree = new TenTree<int, int>;
    vector<int> v, res;

    for (int i = 10000000; i >= 0; i--) {
        tenTree->insert(i);
        v.push_back(i);
    }

    //tenTree->sort(res);
    sort(v.begin(), v.end());

    return 0;
}
