#include <stdio.h>

template <typename T>
class Stack {
public:
    Stack();
    ~Stack();

    T top() const;
    size_t size() const;

    void push(const T& data);
    void pop();

    void clear();

private:
    struct Node {
        Node(T data = T(), Node * next_node = nullptr)
                : data(data), next_node(next_node) {}

        Node(const Node& copy)
                : data(copy.data), next_node(copy.next_node) {}

        Node& operator=(const Node& right) {
            if (this != &right) {
                data = right.data;
                next_node = right.next_node;
            }

            return *this;
        }

        T data;
        Node * next_node;
    };

    void pushFirstNode(Node * node);
    void pushFrontNode(Node * node);

    void popFirstNode();
    void popFrontNode();

    size_t stack_size;
    Node * head_node;
};

template<typename T>
Stack<T>::Stack()
        : stack_size(size_t{0}), head_node(nullptr) {}

template<typename T>
Stack<T>::~Stack() {
    clear();
}

template<typename T>
T Stack<T>::top() const {
    return head_node->data;
}

template<typename T>
size_t Stack<T>::size() const {
    return stack_size;
}

template<typename T>
void Stack<T>::push(const T& data) {
    Node * new_node = new Node(data);
    stack_size ? pushFrontNode(new_node) : pushFirstNode(new_node);
    ++stack_size;
}

template<typename T>
void Stack<T>::pop() {
    if (stack_size == size_t{0})
        return;

    Node * remove_node = head_node;
    (stack_size == size_t{1}) ? popFirstNode() : popFrontNode();

    delete remove_node;
    --stack_size;
}

template<typename T>
void Stack<T>::clear() {
    if (stack_size == size_t{0})
        return;

    Node * remove;
    Node * current = head_node;

    while (stack_size) {
        remove = current;
        current = current->next_node;

        delete remove;
        --stack_size;
    }

    head_node = nullptr;
}

template<typename T>
void Stack<T>::pushFirstNode(Node * node) {
    head_node = node;
}

template<typename T>
void Stack<T>::pushFrontNode(Node * node) {
    node->next_node = head_node;
    head_node = node;
}

template<typename T>
void Stack<T>::popFirstNode() {
    head_node = nullptr;
}

template<typename T>
void Stack<T>::popFrontNode() {
    head_node = head_node->next_node;
}

struct Pair {
    int value;
    int index;

    Pair (const int& _value, const int& _index) {
        value = _value;
        index = _index;
    }
};

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

int main()
{
    typedef unsigned long long ull;
    ull N = readInt();

    Stack<Pair> possible_left_minimums;
    Stack<Pair> possible_right_minimums;
    possible_left_minimums.push(Pair(-1, -1));
    possible_right_minimums.push(Pair(-1, N));

    int numbers[N];
    int left_borders[N];
    int right_borders[N];
    ull prefix_sum[N + 1ULL];
    prefix_sum[0] = 0ULL;
    for (ull i = 0ULL; i < N; ++i) {
        numbers[i] = readInt();
        prefix_sum[i + 1ULL] = prefix_sum[i] + numbers[i];

        while (possible_left_minimums.top().value >= numbers[i]) {
            possible_left_minimums.pop();
        }

        left_borders[i] = possible_left_minimums.top().index + 1;
        possible_left_minimums.push(Pair(numbers[i], i));
    }

    for (int i = N - 1; i >= 0; --i) {
        while (possible_right_minimums.top().value >= numbers[i]) {
            possible_right_minimums.pop();
        }

        right_borders[i] = possible_right_minimums.top().index - 1;
        possible_right_minimums.push(Pair(numbers[i], i));
    }

    ull max_result = 0ULL;
    for (ull i = 0ULL; i < N; ++i) {
        ull sum = prefix_sum[right_borders[i] + 1ULL] - prefix_sum[left_borders[i]];
        ull current = numbers[i] * sum;

        if (current > max_result)
            max_result = current;
    }

    writeUnsignedLongLong(max_result);
}