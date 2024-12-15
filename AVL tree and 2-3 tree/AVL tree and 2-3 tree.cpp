#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <cmath>
using namespace std;

// 定義 AVL 樹節點結構
struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

// 定義 2-3 樹節點結構
struct TwoThreeNode {
    int keys[3]; // 調整大小為 3
    TwoThreeNode* children[4]; // 調整大小為 4
    int numKeys;

    TwoThreeNode() : keys{ -1, -1, -1 }, children{ nullptr, nullptr, nullptr, nullptr }, numKeys(0) {}
};

// AVL 樹的輔助函數
int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* insertAVL(AVLNode* node, int value) {
    if (!node) return new AVLNode(value);

    if (value < node->data)
        node->left = insertAVL(node->left, value);
    else if (value > node->data)
        node->right = insertAVL(node->right, value);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && value < node->left->data)
        return rightRotate(node);

    if (balance < -1 && value > node->right->data)
        return leftRotate(node);

    if (balance > 1 && value > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && value < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// 2-3 樹的插入輔助函數
TwoThreeNode* split(TwoThreeNode* parent, TwoThreeNode* child, int childIndex) {
    TwoThreeNode* newChild = new TwoThreeNode();
    int midKey = child->keys[1];

    newChild->keys[0] = child->keys[2];
    newChild->children[0] = child->children[2];
    newChild->children[1] = child->children[3];
    newChild->numKeys = 1;

    child->keys[1] = -1;
    child->keys[2] = -1;
    child->children[2] = nullptr;
    child->children[3] = nullptr;
    child->numKeys = 1;

    for (int i = parent->numKeys; i > childIndex; --i) {
        parent->keys[i] = parent->keys[i - 1];
        parent->children[i + 1] = parent->children[i];
    }

    parent->keys[childIndex] = midKey;
    parent->children[childIndex + 1] = newChild;
    parent->numKeys++;

    return parent;
}

TwoThreeNode* insertTwoThree(TwoThreeNode* root, int key) {
    if (!root) {
        root = new TwoThreeNode();
        root->keys[0] = key;
        root->numKeys = 1;
        return root;
    }

    if (root->children[0] == nullptr) {
        if (root->numKeys < 2) {
            if (key < root->keys[0]) {
                root->keys[1] = root->keys[0];
                root->keys[0] = key;
            }
            else {
                root->keys[1] = key;
            }
            root->numKeys++;
            return root;
        }
        else {
            TwoThreeNode* newRoot = new TwoThreeNode();
            newRoot->children[0] = root;
            return split(newRoot, root, 0);
        }
    }

    int childIndex = (key < root->keys[0]) ? 0 : (root->numKeys == 1 || key < root->keys[1]) ? 1 : 2;
    root->children[childIndex] = insertTwoThree(root->children[childIndex], key);

    if (root->children[childIndex]->numKeys == 3) {
        return split(root, root->children[childIndex], childIndex);
    }

    return root;
}


// 將 AVL Tree 轉換為陣列
void avlTreeToArray(AVLNode* root, vector<int>& arr, int index) {
    if (root == nullptr) return;
    if (index >= arr.size()) arr.resize(index + 1, -1);
    arr[index] = root->data;
    avlTreeToArray(root->left, arr, 2 * index + 1);
    avlTreeToArray(root->right, arr, 2 * index + 2);
}

// 將 2-3 Tree 轉換為陣列
void twoThreeTreeToArray(TwoThreeNode* root, vector<int>& arr, int index) {
    if (root == nullptr) return;
    if (index >= arr.size()) arr.resize(index + 1, -1);
    arr[index] = root->keys[0];
    if (root->numKeys == 2) {
        arr.resize(index + 2, -1);
        arr[index + 1] = root->keys[1];
    }
    if (root->children[0] != nullptr) {
        twoThreeTreeToArray(root->children[0], arr, 2 * index + 1);
    }
    if (root->children[1] != nullptr) {
        twoThreeTreeToArray(root->children[1], arr, 2 * index + 2);
    }
    if (root->numKeys == 2 && root->children[2] != nullptr) {
        twoThreeTreeToArray(root->children[2], arr, 2 * index + 3);
    }
}



// 打印樹狀圖（只顯示數字）
void printTree(const vector<int>& tree) {
    int size = static_cast<int>(tree.size());
    if (size == 0) {
        cout << "樹是空的" << endl;
        return;
    }

    int height = static_cast<int>(log2(size)) + 1;
    int spaces = static_cast<int>(pow(2, height - 1)) - 1;

    for (int i = 0; i < height; i++) {
        int levelNodes = static_cast<int>(pow(2, i));
        int startIdx = static_cast<int>(pow(2, i) - 1);
        int endIdx = min(static_cast<int>(pow(2, i + 1) - 2), size - 1);

        for (int j = 0; j < spaces; j++) {
            cout << " ";
        }

        for (int j = startIdx; j <= endIdx; j++) {
            if (tree[j] != -1) {
                cout << tree[j];
            }
            else {
                cout << " ";
            }
            if (j < endIdx) cout << "   ";
        }

        cout << endl;
        spaces /= 2;
    }
}

// 刪除 AVL Tree 節點的函數
AVLNode* deleteAVL(AVLNode* root, int key) {
    if (!root) return root;

    if (key < root->data) {
        root->left = deleteAVL(root->left, key);
    }
    else if (key > root->data) {
        root->right = deleteAVL(root->right, key);
    }
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else {
                *root = *temp;
            }
            delete temp;
        }
        else {
            AVLNode* temp = root->right;
            while (temp->left) temp = temp->left;
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data);
        }
    }

    if (!root) return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// 刪除 2-3 Tree 節點的函數 (簡化版，實際應用中需要更複雜的處理)
TwoThreeNode* deleteTwoThree(TwoThreeNode* root, int key) {
    // 這裡提供的是簡化版，實際應用中需要處理更多情況
    if (!root) return root;

    if (key < root->keys[0]) {
        root->children[0] = deleteTwoThree(root->children[0], key);
    }
    else if (key > root->keys[1]) {
        root->children[2] = deleteTwoThree(root->children[2], key);
    }
    else {
        root->children[1] = deleteTwoThree(root->children[1], key);
    }

    return root;
}

int main() {
    vector<int> numbers;
    unordered_set<int> unique_numbers; // 用於檢查重複數字
    cout << "請輸入一串無相同數字的數列 (按兩下 Enter 完成輸入):" << endl;

    string input;
    while (getline(cin, input)) {
        if (input.empty()) { // 偵測到空輸入 (按下 Enter)
            break;
        }

        // 使用 stringstream 分割空格分隔的數字
        stringstream ss(input);
        int num;
        while (ss >> num) {
            if (unique_numbers.insert(num).second) {  // 插入並檢查是否成功
                numbers.push_back(num);
            }
            else {
                cout << "數字 " << num << " 已存在，請重新輸入。" << endl;
            }
        }
    }

    // 建立 AVL Tree
    AVLNode* avlRoot = nullptr;
    for (int num : numbers) {
        avlRoot = insertAVL(avlRoot, num);
    }

    // 將 AVL Tree 轉換為陣列表示
    vector<int> avlArray;
    avlTreeToArray(avlRoot, avlArray, 0);

    cout << "AVL Tree 樹狀圖為：" << endl;
    printTree(avlArray);

    // 建立 2-3 Tree
    TwoThreeNode* twoThreeRoot = nullptr;
    for (int num : numbers) {
        twoThreeRoot = insertTwoThree(twoThreeRoot, num);
    }

    // 將 2-3 Tree 轉換為陣列表示
    vector<int> twoThreeArray;
    twoThreeTreeToArray(twoThreeRoot, twoThreeArray, 0);

    cout << "2-3 Tree 樹狀圖為：" << endl;
    printTree(twoThreeArray);

    // 提供插入和刪除的選項
    while (true) {
        cout << "選擇操作: 1. 插入 2. 刪除 3. 結束" << endl;
        int choice;
        cin >> choice;
        if (choice == 3) break;

        cout << "請輸入數字: ";
        int value;
        cin >> value;

        if (choice == 1) {
            avlRoot = insertAVL(avlRoot, value);
            twoThreeRoot = insertTwoThree(twoThreeRoot, value);
        }
        else if (choice == 2) {
            avlRoot = deleteAVL(avlRoot, value);
            twoThreeRoot = deleteTwoThree(twoThreeRoot, value);
        }

        // 更新並打印 AVL Tree
        avlArray.clear();
        avlTreeToArray(avlRoot, avlArray, 0);
        cout << "更新後的 AVL Tree 樹狀圖為：" << endl;
        printTree(avlArray);

        // 更新並打印 2-3 Tree
        twoThreeArray.clear();
        twoThreeTreeToArray(twoThreeRoot, twoThreeArray, 0);
        cout << "更新後的 2-3 Tree 樹狀圖為：" << endl;
        printTree(twoThreeArray);
    }

    return 0;
}
