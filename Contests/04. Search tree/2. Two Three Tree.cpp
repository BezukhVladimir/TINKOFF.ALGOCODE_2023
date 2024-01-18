#include <stdio.h>

unsigned readUnsigned() {
    int c = getchar_unlocked();
    while (true) {
        if (c == ' ') { // c < '0' || '9' < c
            c = getchar_unlocked();
            continue;
        }

        break;
    }

    unsigned input = 0;
    while ('0' <= c && c <= '9') {
        input = input * 10 + c - '0';
        c = getchar_unlocked();
    }

    return input;
}

void writeUnsigned(unsigned n) {
    int i = 0;
    char reverse[10];

    do
        reverse[i++] = n % 10;
    while (n /= 10);

    while (i--) {
        char digit = reverse[i] + '0';
        putchar_unlocked(digit);
    }
}

template <typename TKey>
class TwoThreeTree {
public:
    void insert(const TKey& key);
    void print() {
        bool flag_sign = true;
        bool flag_first = true;
        char prev_depth;
        char current_depth = 'A';
        root->print(flag_sign, flag_first, prev_depth, current_depth);
    }

    TwoThreeTree() {
        root = nullptr;
    }

    ~TwoThreeTree() {
        delete root;
    }

private:
    struct Node {
        Node * parent = nullptr;
        Node * childs[4] = { nullptr, nullptr, nullptr, nullptr };
        TKey keys[2] = { NULL, NULL };
        size_t keys_number = 0;
        size_t childs_number = 0;

        Node(const TKey& key) {
            keys[0] = key;
            keys[1] = key;
            keys_number = 1;
        }

        Node(Node * first_child_, Node * second_child_,
             const TKey& first_key_, const TKey& second_key_,
             Node * parent_ = nullptr) {
            childs[0] = first_child_;
            childs[1] = second_child_;
            keys[0] = first_key_;
            keys[1] = second_key_,
            parent = parent_;
            childs_number = 2;
            keys_number = 2;
        }

        ~Node() {
            delete childs[0];
            delete childs[1];
            delete childs[2];
            delete childs[3];
        }

        bool is_leaf() {
            return (childs[0] == nullptr) && (childs[1] == nullptr) && (childs[2] == nullptr) && (childs[3] == nullptr);
        }

        void print(bool& flag_sign, bool& flag_first, char& prev_depth, char& current_depth) {
            if (this->is_leaf()) {
                flag_sign = true;
                if (flag_first) {
                    flag_first = false;
                    writeUnsigned(keys[0]);
                } else {
                    putchar_unlocked(' ');
                    putchar_unlocked(prev_depth);
                    putchar_unlocked(' ');
                    writeUnsigned(keys[0]);
                }
            } else {
                for (size_t i = 0; i < childs_number; ++i) {
                    if (flag_sign) {
                        prev_depth = current_depth;
                        flag_sign = false;
                    }

                    ++current_depth;
                    childs[i]->print(flag_sign, flag_first, prev_depth, current_depth);
                    --current_depth;
                }
            }
        }
    };

    void insertToNode(Node * n, Node * node);
    void createParent(Node * first_child, Node * second_child);

    void swapNode(Node *& x, Node *& y);
    void sort2(Node * childs[]);
    void sort3(Node * childs[]);
    void sort4(Node * childs[]);
    void sortChilds(Node * node);

    Node * search(const TKey& key) const;
    void split(Node * node);
    TKey getMin(Node * subtree);
    TKey getMax(Node * subtree);
    void updateKeys(Node * update);

    Node * root;
};

template <typename TKey>
void TwoThreeTree<TKey>::swapNode(Node *& x, Node *& y) {
    Node * t = x;
    x = y;
    y = t;
}

template <typename TKey>
void TwoThreeTree<TKey>::sort2(Node * childs[]) {
    if (childs[0]->keys[0] > childs[1]->keys[0]) {
        swapNode(childs[0], childs[1]);
    }
}

template <typename TKey>
void TwoThreeTree<TKey>::sort3(Node * childs[]) {
    sort2(childs);
    if (childs[0]->keys[0] > childs[2]->keys[0]) {
        swapNode(childs[0], childs[2]);
    }
    if (childs[1]->keys[0] > childs[2]->keys[0]) {
        swapNode(childs[1], childs[2]);
    }
}

template <typename TKey>
void TwoThreeTree<TKey>::sort4(Node * childs[]) {
    sort3(childs);
    if (childs[3]->keys[0] < childs[1]->keys[0]) {
        swapNode(childs[3], childs[2]);
        swapNode(childs[2], childs[1]);

        if (childs[1]->keys[0] < childs[0]->keys[0]) {
            swapNode(childs[1], childs[0]);
        }
    } else if (childs[3]->keys[0] < childs[2]->keys[0]) {
        swapNode(childs[3], childs[2]);
    }
}

template <typename TKey>
void TwoThreeTree<TKey>::sortChilds(Node * node) {
    if (node->childs_number < 2) {
        return;
    }

    if (node->childs_number == 2) {
        sort2(node->childs);
    } else {
        if (node->childs_number == 3) {
            sort3(node->childs);
        } else if (node->childs_number == 4) {
            sort4(node->childs);
        }
    }
}

template <typename TKey>
typename TwoThreeTree<TKey>::Node * TwoThreeTree<TKey>::search(const TKey& key) const {
    Node * current = root;
    while (!current->is_leaf()) {
        if (current->childs_number == 2) {
            if (key < current->childs[0]->keys[1]) {
                current = current->childs[0];
            } else {
                current = current->childs[1];
            }
        } else if (key < current->childs[0]->keys[1]) {
            current = current->childs[0];
        } else if (key < current->childs[1]->keys[1]) {
            current = current->childs[1];
        } else {
            current = current->childs[2];
        }
    }

    return current;
}

template <typename TKey>
void TwoThreeTree<TKey>::createParent(Node * first_child, Node * second_child) {
    Node * parent = new Node(first_child, second_child, first_child->keys[0], second_child->keys[1]);
    first_child->parent = parent;
    second_child->parent = parent;
    root = parent;
    sortChilds(root);
    updateKeys(second_child);
}

template <typename TKey>
void TwoThreeTree<TKey>::insertToNode(Node * n, Node * node) {
    node->parent->childs[node->parent->childs_number++] = n;
    n->parent = node->parent;
    if (node->parent->keys_number < 2) {
        node->parent->keys[node->parent->keys_number++] = n->keys[0];
    }

    sortChilds(node->parent);
    split(node->parent);
    updateKeys(n);
}

template <typename TKey>
void TwoThreeTree<TKey>::split(Node * node) {
    if (node->childs_number > 3) {
        Node * n = new Node(node->childs[2], node->childs[3],
                            node->childs[2]->keys[0], node->childs[3]->keys[1], node->parent);
        node->childs[2]->parent = n;
        node->childs[3]->parent = n;
        node->childs[2] = nullptr;
        node->childs[3] = nullptr;
        node->childs_number = 2;
        node->keys_number = 2;
        node->keys[1] = node->childs[1]->keys[1];

        if (node->parent != nullptr) {
            insertToNode(n, node);
        } else {
            createParent(node, n);
        }
    }
}

template <typename TKey>
TKey TwoThreeTree<TKey>::getMin(Node * subtree) {
    if (subtree->is_leaf()) {
        return subtree->keys[0];
    }

    subtree->keys[0] = getMin(subtree->childs[0]);
    return subtree->keys[0];
}

template <typename TKey>
TKey TwoThreeTree<TKey>::getMax(Node * subtree) {
    if (subtree->is_leaf()) {
        return subtree->keys[0];
    }

    if (subtree->childs_number == 2) {
        subtree->keys[1] = getMax(subtree->childs[1]);
        return subtree->keys[1];
    } else if (subtree->childs_number == 3) {
        subtree->keys[1] = getMax(subtree->childs[2]);
        return subtree->keys[1];
    } else if (subtree->childs_number == 4) {
        subtree->keys[1] = getMax(subtree->childs[3]);
        return subtree->keys[1];
    }
}

template <typename TKey>
void TwoThreeTree<TKey>::updateKeys(Node * update) {
    Node * a = update->parent;
    while (a != nullptr) {
        if (a->childs[0] != nullptr) {
            a->keys[0] = getMin(a);
            a->keys_number = 1;
        }
        if (a->childs[1] != nullptr) {
            a->keys[1] = getMax(a);
            a->keys_number = 2;
        }

        a = a->parent;
    }
}

template <typename TKey>
void TwoThreeTree<TKey>::insert(const TKey& key) {
    Node * n = new Node(key);

    if (root == nullptr) {
        root = n;
        return;
    }

    Node * node = search(key);
    if (node->parent == nullptr) {
        createParent(node, n);
    } else {
        insertToNode(n, node);
    }
}

int main()
{
    unsigned n = readUnsigned();

    TwoThreeTree<unsigned> tree;
    for (unsigned i = 0U; i < n; ++i) {
        unsigned value = readUnsigned();
        tree.insert(value);
    }

    tree.print();
}