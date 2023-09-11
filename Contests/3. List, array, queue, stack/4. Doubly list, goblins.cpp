#include <stdio.h>

template <typename T>
class DoublyList {
private:
	struct Node;

public:
	DoublyList();
	~DoublyList();

	size_t size() const;

	void pushBack(const T& data);
	void pushFront(const T& data);

	void popBack();
	void popFront();
	
	T back() const;
	T front() const;

	void clear();

private:
	struct Node {
		Node(T data = T(), Node* prev_node = nullptr, Node * next_node = nullptr)
			: data(data), prev_node(prev_node), next_node(next_node) {}

		Node(const Node& copy)
			: data(copy.data), prev_node(copy.prev_node), next_node(copy.next_node) {}

		Node& operator=(const Node& right) {
			if (this != &right) {
				data = right.data;
				prev_node = right.prev_node;
				next_node = right.next_node;
			}

			return *this;
		}

		T data;
		Node * prev_node;
		Node * next_node;
	};

	void pushFirstNode(Node * node);
	void pushBackNode(Node * node);
	void pushFrontNode(Node * node);

	void popFirstNode();
	void popBackNode();
	void popFrontNode();

	size_t list_size;
	Node* head_node;
	Node* tail_node;
};

template<typename T>
DoublyList<T>::DoublyList()
	: list_size(size_t{0}), head_node(nullptr), tail_node(nullptr) {}

template<typename T>
DoublyList<T>::~DoublyList() {
	clear();
}

template<typename T>
size_t DoublyList<T>::size() const {
	return list_size;
}

template<typename T>
void DoublyList<T>::pushBack(const T& data) {
	Node * new_node = new Node(data);
	list_size ? pushBackNode(new_node) : pushFirstNode(new_node);
	++list_size;
}

template<typename T>
void DoublyList<T>::pushFront(const T& data) {
	Node * new_node = new Node(data);
	list_size ? pushFrontNode(new_node) : pushFirstNode(new_node);
	++list_size;
}

template<typename T>
void DoublyList<T>::popBack() {
	if (list_size == size_t{0})
	    return;

	Node * remove_node = tail_node;
	(list_size == size_t{1}) ? popFirstNode() : popBackNode();
	
	delete remove_node;
	--list_size;
}

template<typename T>
void DoublyList<T>::popFront() {
	if (list_size == size_t{0})
	    return;

	Node * remove_node = head_node;
	(list_size == size_t{1}) ? popFirstNode() : popFrontNode();
	
	delete remove_node;
	--list_size;
}

template<typename T>
void DoublyList<T>::clear()
{
	if (list_size == size_t{0})
	    return;

	Node * remove;
	Node * current = head_node;

	while (list_size) {
		remove = current;
		current = current->next_node;
		
		delete remove;
		--list_size;
	}

	head_node = nullptr;
	tail_node = nullptr;
}

template<typename T>
void DoublyList<T>::pushFirstNode(Node * node) {
	head_node = node;
	tail_node = node;
}

template<typename T>
void DoublyList<T>::pushBackNode(Node* node) {
	tail_node->next_node = node;
	node->prev_node = tail_node;
	tail_node = node;
}

template<typename T>
void DoublyList<T>::pushFrontNode(Node * node) {
	node->next_node = head_node;
	head_node->prev_node = node;
	head_node = node;
}

template<typename T>
void DoublyList<T>::popFirstNode() {
	head_node = nullptr;
	tail_node = nullptr;
}

template<typename T>
void DoublyList<T>::popBackNode() {
	tail_node->prev_node->next_node = nullptr;
	tail_node = tail_node->prev_node;
}

template<typename T>
void DoublyList<T>::popFrontNode() {
	head_node->next_node->prev_node = nullptr;
	head_node = head_node->next_node;
}

template<typename T>
T DoublyList<T>::front() const {
	return head_node->data;
}

template<typename T>
T DoublyList<T>::back() const {
	return tail_node->data;
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

DoublyList<int> first_half;
DoublyList<int> second_half;

void equalize() {
    if (first_half.size() < second_half.size()) {
        first_half.pushBack(second_half.front());
        second_half.popFront();
    } else if (first_half.size() > second_half.size() + 1) {
        second_half.pushFront(first_half.back());
        first_half.popBack();
    }
}

int main()
{
    int N = readInt();
    
    char c;
    while (N-- > 0) {
        c = getchar_unlocked();
        
        if (c == '-') {
            c = getchar_unlocked();
            
            writeLnInt(first_half.front());
            first_half.popFront();
        } else if (c == '+') {
            second_half.pushBack(readInt());
        } else {
            first_half.pushBack(readInt());
        }
     
        equalize();
    }
}