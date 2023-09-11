template <typename T>
class Queue {
public:
	Queue();
	~Queue();

	size_t size() const;

	T back() const;
	void push(const T& data);

	T front() const;
	void pop();

	void clear();

private:
	struct Node {
		Node(T data = T(), Node * prev_node = nullptr, Node * next_node = nullptr)
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

	void popFirstNode();
	void popFrontNode();

	size_t queue_size;
	Node* head_node;
	Node* tail_node;
};

template<typename T>
Queue<T>::Queue()
	: queue_size(size_t{0}), head_node(nullptr), tail_node(nullptr) {}

template<typename T>
Queue<T>::~Queue() {
	clear();
}

template<typename T>
size_t Queue<T>::size() const {
	return queue_size;
}

template<typename T>
T Queue<T>::back() const {
	return tail_node->data;
}

template<typename T>
void Queue<T>::push(const T& data) {
	Node * new_node = new Node(data);
	queue_size ? pushBackNode(new_node) : pushFirstNode(new_node);
	++queue_size;
}

template<typename T>
T Queue<T>::front() const {
	return head_node->data;
}

template<typename T>
void Queue<T>::pop() {
	if (queue_size == size_t{0})
	    return;

	Node * remove_node = head_node;
	(queue_size == size_t{1}) ? popFirstNode() : popFrontNode();
	
	delete remove_node;
	--queue_size;
}

template<typename T>
void Queue<T>::clear() {
	if (queue_size == size_t{0})
	    return;

	Node * remove;
	Node * current = head_node;

	while (queue_size) {
		remove = current;
		current = current->next_node;
		
		delete remove;
		--queue_size;
	}

	head_node = nullptr;
	tail_node = nullptr;
}

template<typename T>
void Queue<T>::pushFirstNode(Node * node) {
	head_node = node;
	tail_node = node;
}

template<typename T>
void Queue<T>::pushBackNode(Node * node) {
	tail_node->next_node = node;
	node->prev_node = tail_node;
	tail_node = node;
}

template<typename T>
void Queue<T>::popFirstNode() {
	head_node = nullptr;
	tail_node = nullptr;
}

template<typename T>
void Queue<T>::popFrontNode() {
	head_node->next_node->prev_node = nullptr;
	head_node = head_node->next_node;
}