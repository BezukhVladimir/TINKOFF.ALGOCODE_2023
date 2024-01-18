#include <random>
#include <limits>
#include <utility>
#include <stdio.h>

template <typename TKey>
class Treap {
public:
    void insert(const TKey& key);
    void erase(const TKey& key);
    TKey find(const TKey& key) const;

    Treap()
            : treap_root(nullptr) {};
    ~Treap() {
        delete treap_root;
    }

private:
    class Node {
    public:
        static const unsigned MIN_PRIORITY = std::numeric_limits<unsigned>::min();
        static const unsigned MAX_PRIORITY = std::numeric_limits<unsigned>::max();

        TKey key;
        size_t priority;
        size_t tree_size = 1;

        Node * left  = nullptr;
        Node * right = nullptr;

        Node(TKey init_k, size_t init_p)
                : key(init_k), priority(init_p) {}
        Node(TKey init_k)
                : Node(init_k, _generatePriority()) {}
        ~Node() {
            delete left;
            delete right;
        }

    private:
        size_t _generatePriority() const;
    };

    std::pair<Node *, Node *> _split(Node * root, const TKey& key);
    Node * _merge(Node * left, Node * right);

    Node * _updateTreeSize(Node * root);
    size_t _getTreeSize(const Node * root) const;
    Node * _find(Node * root, const TKey& key) const;

    Node * treap_root;
};

template <typename TKey>
size_t Treap<TKey>::Node::_generatePriority() const {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(MIN_PRIORITY, MAX_PRIORITY);

    return distribution(rng);
}

template <typename TKey>
std::pair<typename Treap<TKey>::Node *,
          typename Treap<TKey>::Node *> Treap<TKey>::_split(Node * root,
                                                            const TKey& key) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    if (key <= root->key) {
        auto[left, right] = _split(root->left, key);
        root->left = right;

        return {_updateTreeSize(left), _updateTreeSize(root)};
    } else {
        auto[left, right] = _split(root->right, key);
        root->right = left;

        return {_updateTreeSize(root), _updateTreeSize(right)};
    }
}

template <typename TKey>
typename Treap<TKey>::Node * Treap<TKey>::_merge(Node * left, Node * right) {
    if (left == nullptr) {
        return right;
    }

    if (right == nullptr) {
        return left;
    }

    if (left->priority > right->priority) {
        left->right = _merge(_updateTreeSize(left->right),
                             _updateTreeSize(right));
        return _updateTreeSize(left);
    } else {
        right->left = _merge(_updateTreeSize(left),
                             _updateTreeSize(right->left));
        return _updateTreeSize(right);
    }
}

template <typename TKey>
void Treap<TKey>::insert(const TKey& key) {
    auto[left1, right1] = _split(treap_root, key);
    auto[left2, right2] = _split(right1, key + 1);
    treap_root = _merge(
            _merge(left1, new Node(key)),
            right2);
}

template <typename TKey>
void Treap<TKey>::erase(const TKey& key) {
    auto[left1, right1] = _split(treap_root, key);
    auto[left2, right2] = _split(right1, key + 1);
    treap_root = _merge(left1, right2);
}

template <typename TKey>
TKey Treap<TKey>::find(const TKey& key) const {
    return _find(treap_root, key)->key;
}

template <typename TKey>
typename Treap<TKey>::Node * Treap<TKey>::_find(Node * root, const TKey& key) const {
    if (root == nullptr) {
        return nullptr;
    }

    if (_getTreeSize(root->right) == key - 1) {
        return root;
    } else if (_getTreeSize(root->right) > key - 1) {
        return _find(root->right, key);
    } else {
        return _find(root->left, key - _getTreeSize(root->right) - 1);
    }
}

template <typename TKey>
size_t Treap<TKey>::_getTreeSize(const Node * root) const {
    return root == nullptr ? size_t{ 0 } : root->tree_size;
}

template <typename TKey>
typename Treap<TKey>::Node * Treap<TKey>::_updateTreeSize(Node * root) {
    if (root != nullptr) {
        root->tree_size = _getTreeSize(root->left)
                        + _getTreeSize(root->right) + 1;
    }

    return root;
}

unsigned readUnsigned()
{
    char c = getchar_unlocked();
    while (true) {
        if (c == ' ') { // c < '0' || '9' < c
            c = getchar_unlocked();
            continue;
        }

        break;
    }

    unsigned input = 0U;
    while ('0' <= c && c <= '9') {
        input = input * 10U + c - '0';
        c = getchar_unlocked();
    }

    return input;
}

int readInt()
{
    bool is_neg = false;
    int c = getchar_unlocked();
    while (true) {
        if (c == '-') {
            is_neg = true;
            c = getchar_unlocked();
            break;
        }

        if (c == ' ') { // c < '0' || '9' < c
            c = getchar_unlocked();
            continue;
        }

        break;
    }

    int input = 0;
    while ('0' <= c && c <= '9') {
        input = input * 10 + c - '0';
        c = getchar_unlocked();
    }

    return is_neg ? -input : input;
}

void writeInt(int n)
{
    if (n < 0) {
        putchar_unlocked('-');
        n = -n;
    }

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

void writeLnInt(int n) {
    writeInt(n);
    putchar_unlocked('\n');
}

int main()
{
    unsigned n = readUnsigned();
    Treap<int> tree;
    for (unsigned iter = 0; iter < n; ++iter) {
        char operation = getchar_unlocked();

        if (operation == '+' || operation == '1') {
            operation = getchar_unlocked();
            int k = readInt();

            tree.insert(k);
        } else if (operation == '0') {
            int k = readInt();

            writeLnInt(tree.find(k));
        } else {
            operation = getchar_unlocked();
            int k = readInt();

            tree.erase(k);
        }
    }
}