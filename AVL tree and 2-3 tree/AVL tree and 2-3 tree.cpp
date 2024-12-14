#include <iostream>
#include <vector>
#include <sstream> // 用於分割輸入
#include <unordered_set> // 用於檢查重複數字
using namespace std;

// AVL Tree：自平衡二元搜尋樹
void avlTree(vector<int>& tree);
void avlTreeHelper(vector<int>& tree, int index, int spaces);

// 2-3 Tree：自平衡多路樹（最多包含2或3個子節點）
void twoThreeTree(vector<int>& tree);
void twoThreeTreeHelper(vector<int>& tree, int index, int spaces);

int main() {
    vector<int> n;
    unordered_set<int> unique_numbers; // 用於檢查重複數字
    cout << "請輸入一串無相同數字的數列 (按兩下 Enter 完成輸入):" << endl;

    string input;
    while (getline(cin, input)) {
        if (input.empty()) // 偵測到空輸入 (按下 Enter)
        {
            break;
        }

        // 使用 stringstream 分割空格分隔的數字
        stringstream ss(input);
        int num;
        while (ss >> num) {
            if (unique_numbers.find(num) != unique_numbers.end()) {
                cout << "數字 " << num << " 已存在，請重新輸入。" << endl;
                continue;
            }
            n.push_back(num);
            unique_numbers.insert(num);
        }
    }

    cout << "AVL Tree 陣列為：" << endl;
    avlTree(n);

    cout << "2-3 Tree 陣列為：" << endl;
    twoThreeTree(n);

    return 0;
}

// AVL Tree 的實作
void avlTree(vector<int>& tree) {
    avlTreeHelper(tree, 0, 0);
    cout << endl;
}

void avlTreeHelper(vector<int>& tree, int index, int spaces) {
    if (index >= tree.size()) return;

    spaces += 10;
    avlTreeHelper(tree, 2 * index + 2, spaces); // 處理右子樹

    for (int i = 10; i < spaces; i++) cout << " ";
    cout << tree[index] << endl;

    avlTreeHelper(tree, 2 * index + 1, spaces); // 處理左子樹
}

// 2-3 Tree 的實作
void twoThreeTree(vector<int>& tree) {
    twoThreeTreeHelper(tree, 0, 0);
    cout << endl;
}

void twoThreeTreeHelper(vector<int>& tree, int index, int spaces) {
    if (index >= tree.size()) return;

    spaces += 10;
    twoThreeTreeHelper(tree, 2 * index + 2, spaces); // 處理右子樹

    for (int i = 10; i < spaces; i++) cout << " ";
    cout << tree[index] << endl;

    twoThreeTreeHelper(tree, 2 * index + 1, spaces); // 處理左子樹
}
