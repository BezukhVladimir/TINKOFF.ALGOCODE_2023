#include <stdio.h>

template <typename T>
class MinStack {
public:
	MinStack();
	~MinStack();

	T top() const;
	T min() const;
	bool empty() const;
	size_t size() const;

	void push(const T& data);
	void pop();

	void clear();

private:
	struct Node {
		Node(T data = T(), T min_below = T(), Node * next_node = nullptr)
			: data(data), min_below(min_below), next_node(next_node) {}

		Node(const Node& copy)
			: data(copy.data), min_below(copy.min_below), next_node(copy.next_node) {}

		Node& operator=(const Node& right) {
			if (this != &right) {
				data = right.data;
				min_below = right.min_below;
				next_node = right.next_node;
			}

			return *this;
		}
        
		T data;
		T min_below;
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
MinStack<T>::MinStack()
	: stack_size(size_t{0}), head_node(nullptr) {}

template<typename T>
MinStack<T>::~MinStack() {
	clear();
}

template<typename T>
T MinStack<T>::top() const {
	return head_node->data;
}

template<typename T>
T MinStack<T>::min() const {
	return head_node->min_below;
}

template<typename T>
size_t MinStack<T>::size() const {
	return stack_size;
}

template<typename T>
bool MinStack<T>::empty() const {
	return !stack_size;
}

template<typename T>
void MinStack<T>::push(const T& data) {
	Node * new_node = new Node(data);
	stack_size ? pushFrontNode(new_node) : pushFirstNode(new_node);
	++stack_size;
}

template<typename T>
void MinStack<T>::pop() {
	if (stack_size == size_t{0})
	    return;

	Node * remove_node = head_node;
	(stack_size == size_t{1}) ? popFirstNode() : popFrontNode();
	
	delete remove_node;
	--stack_size;
}

template<typename T>
void MinStack<T>::clear() {
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
void MinStack<T>::pushFirstNode(Node * node) {
    node->min_below = node->data;
	
	head_node = node;
}

template<typename T>
void MinStack<T>::pushFrontNode(Node * node) {
    node->min_below = ((head_node->min_below < node->data) ? head_node->min_below : node->data);
	
	node->next_node = head_node;
	head_node = node;
}

template<typename T>
void MinStack<T>::popFirstNode() {
	head_node = nullptr;
}

template<typename T>
void MinStack<T>::popFrontNode() {
	head_node = head_node->next_node;
}


template <typename T>
class MinQueue {
public:
	MinQueue();
	~MinQueue();

	size_t size() const;

	void push(const T& data);

	T front();
	void pop();

	void clear();
	
	T min();

private:
    void fromPushToPop();
    
    size_t queue_size;
    MinStack<T> to_pop;
    MinStack<T> to_push;
};

template<typename T>
MinQueue<T>::MinQueue()
	: queue_size(size_t{0}), to_pop(), to_push() {}

template<typename T>
MinQueue<T>::~MinQueue() {
	clear();
}

template<typename T>
size_t MinQueue<T>::size() const {
	return queue_size;
}

template<typename T>
void MinQueue<T>::push(const T& data) {
    ++queue_size;
    to_push.push(data);
}

template<typename T>
void MinQueue<T>::fromPushToPop() {
    if (to_pop.empty()) {
        
        while (!to_push.empty()) {
            to_pop.push(to_push.top());
            to_push.pop();
        }
    }
}

template<typename T>
T MinQueue<T>::front() {
    fromPushToPop();
    
    return to_pop.top();
}

template<typename T>
void MinQueue<T>::pop() {
    fromPushToPop();
    
    --queue_size;
    to_pop.pop();
}

template<typename T>
void MinQueue<T>::clear() {
    to_pop.clear();
    to_push.clear();
    queue_size = 0;
}

template<typename T>
T MinQueue<T>::min() {
    if (to_push.empty())
        return to_pop.min();
    
    if (to_pop.empty())
        return to_push.min();
    
    return ((to_push.min() < to_pop.min()) ? to_push.min() : to_pop.min());
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

int main()
{
    int N = readInt();
    int K = readInt();
    
    MinQueue<int> sequence_view;
    for (int i = 0; i < K; ++i)
        sequence_view.push(readInt());
        
    N = N - K + 1;
    for (int i = 0; i < N; ++i) {
        writeInt(sequence_view.min());
        putchar_unlocked(' ');
        
        sequence_view.pop();
        sequence_view.push(readInt());
    }
}