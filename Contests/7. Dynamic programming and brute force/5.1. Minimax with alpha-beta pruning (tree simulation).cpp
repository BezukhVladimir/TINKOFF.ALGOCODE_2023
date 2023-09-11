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
            _root  = new Node();
        }

        void minimax() {
            std::cout << alphaBetaPruning(*_root, true, _depth);
        }

    private:
        struct Node {
            T_ull value;
            T_ull alpha = std::numeric_limits<T_ull>::min();
            T_ull beta  = std::numeric_limits<T_ull>::max();

            Node() {}
            Node(Node * parent) {
                alpha = parent->alpha;
                beta  = parent->beta;
            }
        };

        T_ull alphaBetaPruning(Node current, bool isMaxPlayer, T_ull current_depth) {
            if (current_depth == 0ULL) {
                return calculateData(_size++ % _m);

            }

            if (isMaxPlayer) {
                current.value = std::numeric_limits<T_ull>::min();
                for (T_ull i = 0ULL; i < _NODE_CHILDREN_NUMBER; ++i) {
                    T_ull child_value = alphaBetaPruning(Node(current), !isMaxPlayer, current_depth - 1ULL);

                    if (child_value > current.value) {
                        current.value = child_value;
                    }

                    if (child_value >= current.beta) {
                        updateSize(current_depth);
                        return current.value;
                    }

                    if (child_value > current.alpha) {
                        current.alpha = child_value;
                    }
                }
            } else {
                current.value = std::numeric_limits<T_ull>::max();
                for (T_ull i = 0ULL; i < _NODE_CHILDREN_NUMBER; ++i) {
                    T_ull child_value = alphaBetaPruning(Node(current), !isMaxPlayer, current_depth - 1ULL);

                    if (child_value < current.value) {
                        current.value = child_value;
                    }

                    if (child_value <= current.alpha) {
                        updateSize(current_depth);
                        return current.value;
                    }

                    if (child_value < current.beta) {
                        current.beta = child_value;
                    }
                }
            }

            return current.value;
        }

        T_ull calculateData(const T_ull &i) const {
            T_ull ii = (i * i) % _m;
            return (_a * ii + _b * i + _c) % _m;
        }

        void updateSize(const T_ull &depth) {
            T_ull subsize = 2ULL;
            for (T_ull power = 1ULL; power < depth; ++power) {
                subsize <<= 1ULL;
            }

            T_ull shift = _size % subsize;
            shift = ((shift == 0ULL) ? 0ULL : (subsize - shift));
            _size += shift;
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
}