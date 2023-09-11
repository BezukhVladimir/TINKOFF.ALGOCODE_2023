#include <random>
#include <limits>
#include <utility>
#include <stdio.h>

template <typename TKey, typename TSumma>
class Treap {
public:
    void add(const TKey& key);
    TSumma sum(const TKey& left, const TKey& right);
    bool isFind(const TKey& key) const;

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
        TSumma tree_summa;

        Node * left  = nullptr;
        Node * right = nullptr;

        Node(TKey init_k, size_t init_p)
                : key(init_k), priority(init_p), tree_summa(init_k) {}
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

    Node * _updateTreeSumma(Node * root);
    TSumma _getTreeSumma(const Node * root) const;
    Node * _find(Node * root, const TKey& key) const;

    Node * treap_root;
};

template <typename TKey, typename TSumma>
size_t Treap<TKey, TSumma>::Node::_generatePriority() const {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(MIN_PRIORITY, MAX_PRIORITY);

    return distribution(rng);
}

template <typename TKey, typename TSumma>
std::pair<typename Treap<TKey, TSumma>::Node *,
          typename Treap<TKey, TSumma>::Node *> Treap<TKey, TSumma>::_split(Node * root,
                                                                            const TKey& key) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }

    if (key <= root->key) {
        auto[left, right] = _split(root->left, key);
        root->left = right;

        return {_updateTreeSumma(left), _updateTreeSumma(root)};
    } else {
        auto[left, right] = _split(root->right, key);
        root->right = left;

        return {_updateTreeSumma(root), _updateTreeSumma(right)};
    }

}

template <typename TKey, typename TSumma>
typename Treap<TKey, TSumma>::Node * Treap<TKey, TSumma>::_merge(Node * left, Node * right) {
    if (left == nullptr) {
        return right;
    }

    if (right == nullptr) {
        return left;
    }

    if (left->priority > right->priority) {
        left->right = _merge(_updateTreeSumma(left->right),
                             _updateTreeSumma(right));
        return _updateTreeSumma(left);
    } else {
        right->left = _merge(_updateTreeSumma(left),
                             _updateTreeSumma(right->left));
        return _updateTreeSumma(right);
    }
}

template <typename TKey, typename TSumma>
void Treap<TKey, TSumma>::add(const TKey& key) {
    auto[left1, right1] = _split(treap_root, key);
    auto[left2, right2] = _split(right1, key + 1);
    treap_root = _merge(
                _merge(left1, new Node(key)),
                right2);
}

template <typename TKey, typename TSumma>
TSumma Treap<TKey, TSumma>::sum(const TKey& left, const TKey& right) {
    auto[left1, right1] = _split(treap_root, left);
    auto[left2, right2] = _split(right1, right + 1);
    TSumma y = _getTreeSumma(left2);

    Node * merged = _merge(left2, right2);
    _merge(left1, merged);

    return y;
}

template <typename TKey, typename TSumma>
bool Treap<TKey, TSumma>::isFind(const TKey& key) const {
    return _find(treap_root, key) != nullptr;
}

template <typename TKey, typename TSumma>
typename Treap<TKey, TSumma>::Node * Treap<TKey, TSumma>::_find(Node * root, const TKey& key) const {
    if (root == nullptr) {
        return nullptr;
    }

    if (key == root->key) {
        return root;
    } else if (key < root->key) {
        return _find(root->left, key);
    } else {
        return _find(root->right, key);
    }
}

template <typename TKey, typename TSumma>
TSumma Treap<TKey, TSumma>::_getTreeSumma(const Node * root) const {
    return root == nullptr ? TKey() : root->tree_summa;
}

template <typename TKey, typename TSumma>
typename Treap<TKey, TSumma>::Node * Treap<TKey, TSumma>::_updateTreeSumma(Node * root) {
    if (root != nullptr) {
        root->tree_summa = _getTreeSumma(root->left)
                         + _getTreeSumma(root->right) + root->key;
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

void writeUnsignedLongLong(unsigned long long n)
{
    int i = 0;
    char reverse[20];

    do
        reverse[i++] = n % 10;
    while (n /= 10);

    while (i--) {
        char digit = reverse[i] + '0';
        putchar_unlocked(digit);
    }
}

void writeLnUnsignedLongLong(unsigned long long n) {
    writeUnsignedLongLong(n);
    putchar_unlocked('\n');
}

int main()
{
    unsigned n = readUnsigned();

    typedef unsigned long long ull;
    Treap<unsigned, ull> tree;
    for (ull iter = 0ULL, y = 0ULL; iter < n; ++iter) {
        char operation = getchar_unlocked();

        if (operation == '+') {
            unsigned i = readUnsigned();

            i = (i + y) % 1'000'000'000U;

            if (!tree.isFind(i)) {
                tree.add(i);
            }

            y = 0ULL;
        } else {
            unsigned l = readUnsigned();
            unsigned r = readUnsigned();

            y = tree.sum(l, r);
            writeLnUnsignedLongLong(y);
        }
    }
}