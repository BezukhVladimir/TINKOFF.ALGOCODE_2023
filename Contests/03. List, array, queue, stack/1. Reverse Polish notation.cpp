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

long int readUnsignedInt(const long int& sum = 0) {
    int input = sum;
    
    char c = getchar_unlocked();
    while ('0' <= c && c <= '9') {
        input = input * 10 + c - '0';
        c = getchar_unlocked();
    }
    
    return input;
}

void writeInt(long int n)
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

template<typename T>
void performOperation(Stack<T>& operands, const char& operation) {
    int right = operands.top();
    operands.pop();
    
    int left = operands.top();
    operands.pop();
    
    if (operation == '-')
        operands.push(left - right);
    else if (operation == '+')
        operands.push(left + right);
    else
        operands.push(left * right);
}

int main()
{
    Stack<long int> operands;
    
    char input = getchar_unlocked();
    while (input != EOF) {
        if ('0' <= input && input <= '9') {
            operands.push(readUnsignedInt(input - '0'));
        } else if (input == '-') {
            input = getchar_unlocked();
            
            if ('0' <= input && input <= '9') {
                operands.push(-readUnsignedInt(input - '0'));
            } else {
                performOperation(operands, '-');
            }
        } else if (input == '+') {
            performOperation(operands, '+');
        } else if (input == '*') {
            performOperation(operands, '*');
        }
        
        input = getchar_unlocked();
    }
    
    writeInt(operands.top());
}