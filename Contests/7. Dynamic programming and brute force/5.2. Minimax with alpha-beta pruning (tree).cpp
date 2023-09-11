#include <limits>
#include <iostream>

namespace minimax_with_alpha_beta_pruning {
// https://www.youtube.com/watch?v=zp3VMe0Jpf8

    typedef unsigned long long T_ull;
    const T_ull _NODE_CHILDREN_NUMBER = 2ULL;

    class Tree {
    public:
        Tree() = delete;
        Tree(T_ull depth, const T_ull &a,
             const T_ull &b,
             const T_ull &c, const T_ull &m) {
            _a = a;
            _b = b;
            _c = c;
            _m = m;
            _depth = depth;
            _size  = 0ULL;
            _root  = create_tree(_depth + 1ULL);
        }

        void print() {
            std::cout << _root->data;
        }

        void minimax() {
            maxValue(_root);
        }

    private:
        struct Node {
            T_ull data;
            T_ull alpha;
            T_ull beta;

            bool is_leaf   = false;
            bool has_alpha = false;
            bool has_beta  = false;

            Node * children[_NODE_CHILDREN_NUMBER] = { nullptr, nullptr };

            Node() {}
            Node(const T_ull &data_) {
                is_leaf = true;
                data = data_;
            }
        };

        Node * create_tree(const T_ull &depth) {
            if (depth == 0ULL) {
                return nullptr;
            }

            Node * new_node;
            if (depth == 1ULL) {
                new_node = new Node(_size++ % _m);
            } else {
                new_node = new Node();
            }

            for (T_ull i = 0ULL; i < _NODE_CHILDREN_NUMBER; ++i) {
                new_node->children[i] = create_tree(depth - 1ULL);
            }

            return new_node;
        }

        T_ull maxValue(Node * current) {
            if (current->is_leaf) {
                return calculateData(current);
            }

            current->data = std::numeric_limits<T_ull>::min();

            for (T_ull i = 0ULL; i < _NODE_CHILDREN_NUMBER; ++i) {
                alphaBetaPropagation(current, current->children[i]);
                T_ull child_value = minValue(current->children[i]);

                if (child_value > current->data) {
                    current->data = child_value;
                }

                if (current->has_beta) {
                    if (child_value >= current->beta) {
                        return current->data;
                    }
                }

                if (current->has_alpha) {
                    if (child_value > current->alpha) {
                        current->alpha = child_value;
                    }
                } else {
                    current->has_alpha = true;
                    current->alpha = child_value;
                }
            }

            return current->data;
        }

        T_ull minValue(Node * current) {
            if (current->is_leaf) {
                return calculateData(current);
            }

            current->data = std::numeric_limits<T_ull>::max();

            for (T_ull i = 0ULL; i < _NODE_CHILDREN_NUMBER; ++i) {
                alphaBetaPropagation(current, current->children[i]);
                T_ull child_value = maxValue(current->children[i]);

                if (child_value < current->data) {
                    current->data = child_value;
                }

                if (current->has_alpha) {
                    if (child_value <= current->alpha) {
                        return current->data;
                    }
                }

                if (current->has_beta) {
                    if (child_value < current->beta) {
                        current->beta = child_value;
                    }
                } else {
                    current->has_beta = true;
                    current->beta = child_value;
                }
            }

            return current->data;
        }

        T_ull calculateData(Node * current) {
           T_ull i  = current->data;
           T_ull ii = (i * i) % _m;
           current->data = (_a * ii + _b * i + _c) % _m;

           return current->data;
        }

        void alphaBetaPropagation(const Node * parent, Node * child) {
            if (parent->has_alpha) {
                child->has_alpha = true;
                child->alpha = parent->alpha;
            }
            if (parent->has_beta) {
                child->has_beta = true;
                child->beta = parent->beta;
            }
        }

        Node * _root;
        T_ull _depth;
        T_ull _size;
        T_ull _a, _b, _c, _m;
    };

} // namespace minimax_with_alpha_beta_pruning

int main() {
    using namespace minimax_with_alpha_beta_pruning;

    T_ull n, a, b, c, m;
    std::cin >> n >> a >> b >> c >> m;

    Tree tree(n, a, b, c, m);
    tree.minimax();
    tree.print();
}