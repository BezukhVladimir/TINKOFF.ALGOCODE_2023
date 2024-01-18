#include <stdio.h>

template <typename T>
class Stack {
public:
	Stack();
	~Stack();

	T top() const;
	bool empty() const;
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
bool Stack<T>::empty() const {
	return !stack_size;
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

struct DynamicString {
private:
    int current_size;
    int current_index;
    char * buffer;
    
    void doublingSize() {
        current_size <<= 1;
        char * new_buffer = new char[current_size];
        
        for (int i = current_index - 1; i >= 0; --i) {
            new_buffer[i] = buffer[i];
        }
        
        delete buffer;
        buffer = new_buffer;
    }

public:
    DynamicString() {
        current_size = 4096;
        current_index = 0;
        buffer = new char[current_size];
    }
    
    ~DynamicString() {
        delete buffer;
    }
    
    void addChar(const char& c)
    {
        if (current_index == current_size) {
            doublingSize();
        }
        
        buffer[current_index++] = c;
    }
    
    void addInt(int n) {
        if (n < 0) {
            addChar('-');
            n = -n;
        }
    
        int i = 0;
        char reverse[10];
        
        do
            reverse[i++] = n % 10;
        while (n /= 10);
        
        while (i--) {
            char digit = reverse[i] + '0';
            addChar(digit);
        }
    }
    
    void print()
    {
        for (int i = 0; i < current_size; ++i) {
            if (buffer[i] == '\0') {
                return;
            }
            
            putchar_unlocked(buffer[i]);
        }
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

class ThirdTask {
private:
    int N;
    int operations_counter;
    DynamicString result_buffer;
    
    void addOperation(const int& current_operation, const int& wagons_number) {
        result_buffer.addInt(current_operation);
        result_buffer.addChar(' ');
        result_buffer.addInt(wagons_number);
        result_buffer.addChar('\n');
        ++operations_counter;
    }
    
    void changeOperation(int& current_operation, int& wagons_number) {
        addOperation(current_operation, wagons_number);
    
        wagons_number = 0;
        current_operation = ((current_operation == 1) ? 2 : 1);
    }

public:
    ThirdTask() {
        N = readInt();
        operations_counter = 0;
    }
    
    void solve()
    {
        Stack<int> stub_track;
        int expected_wagon = 1;
        int remaining_wagons = N;
    
        int wagons_number = 0;
        int current_operation = 1;
        while (remaining_wagons || (!stub_track.empty() && stub_track.top() == expected_wagon)) {
            if (!stub_track.empty() && stub_track.top() == expected_wagon) {
                if (current_operation != 2) {
                    changeOperation(current_operation, wagons_number);
                }
            
                stub_track.pop();
                ++expected_wagon;
            
                ++wagons_number;
            } else {
                if (current_operation != 1) {
                    changeOperation(current_operation, wagons_number);
                }
                
                stub_track.push(readInt());
                --remaining_wagons;
                
                ++wagons_number;
            }
        }
    
        changeOperation(current_operation, wagons_number);
        result_buffer.addChar('\0');
        
        if (expected_wagon > N) {
            print();
        } else {
            putchar_unlocked('0');
        }
    }
    
    void print() {
        writeLnInt(operations_counter);   
        result_buffer.print();
    }
};

int main()
{
    ThirdTask task;
    task.solve();
}