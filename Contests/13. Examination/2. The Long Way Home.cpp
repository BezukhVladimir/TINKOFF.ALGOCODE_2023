#include <bits/stdc++.h>

class Stack {
public:
    int top() const;
    void cut(const int& data);
    void push(const int& data);
    void pop();

private:
    std::vector<int> stack;
};

int Stack::top() const {
    if (stack.size() == 0) {
        return 0;
    }

    return stack[stack.size() - 1];
}

void Stack::cut(const int& data) {
    if (stack.size() == 0) {
        return;
    }

    auto iter = find(stack.begin(), stack.end(), data);
    if (iter != stack.end()) {
        stack.erase(iter);
    }
}

void Stack::push(const int& data) {
    stack.push_back(data);
}

void Stack::pop() {
    if (stack.size() > 0) {
        stack.pop_back();
    }
}

void automata() {
    int N;
    std::cin >> N;

    int input;
    Stack stack;

    for (int i = 1; i <= N; ++i) {
        std::cin >> input;

        if (input > 0) {
            stack.push(input);
        } else if (input < 0) {
            stack.cut(-input);
        } else {
            stack.pop();
        }

        std::cout << stack.top() << '\n';
    }
}

int main() {
    automata();
}