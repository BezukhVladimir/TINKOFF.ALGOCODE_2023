#include <random>
#include <limits>
#include <utility>
#include <stdio.h>

template <typename TKey>
class Treap {
public:
    void add(const TKey& key);
    TKey next(const TKey& key);

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
    TKey _getMin(Node * root) const;

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
        return {left, root};
    } else {
        auto[left, right] = _split(root->right, key);
        root->right = left;
        return {root, right};
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
        left->right = _merge(left->right, right);
        return left;
    } else {
        right->left = _merge(left, right->left);
        return right;
    }
}

template <typename TKey>
TKey Treap<TKey>::_getMin(Node * root) const {
    if (root == nullptr) {
        return -1;
        // return TKey();
    }

    if (root->left == nullptr)
        return root->key;

    return _getMin(root->left);
}

template <typename TKey>
void Treap<TKey>::add(const TKey& key) {
    auto[left, right] = _split(treap_root, key);
    treap_root = _merge(
            _merge(left, new Node(key)), right);
}

template <typename TKey>
TKey Treap<TKey>::next(const TKey& key) {
    auto[left, right] = _split(treap_root, key);
    TKey next_value = _getMin(right);
    treap_root = _merge(left, right);

    return next_value;
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
    for (int iter = 0, y = 0; iter < n; ++iter) {
        char operation = getchar_unlocked();
        unsigned i = readUnsigned();

        if (operation == '+') {
            tree.add((i + y) % 1'000'000'000);
            y = 0;
        } else {
            y = tree.next(i);
            writeLnInt(y);
        }
    }
}