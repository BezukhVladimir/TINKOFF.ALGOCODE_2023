#include <vector>
#include <iostream>

class Task {
public:
    Task() {
        std::cin >> N >> M;
    }

    void solve() {
        Graph graph(N, M);
        graph.findCycle();
        graph.printHasCycle();
    }

private:
    class Graph {
    public:
        Graph() = delete;
        Graph(const unsigned &vertex_number,
              const unsigned &request_number) {
            adjacency_lists.resize(vertex_number + 1U);
            for (unsigned i = 0U; i < request_number; ++i) {
                unsigned first_vertex, second_vertex;
                std::cin >> first_vertex >> second_vertex;

                adjacency_lists[first_vertex].emplace_back(second_vertex);
            }
        }

        void dfs(const unsigned &vertex) {
            if (has_cycle)
                return;

            color[vertex] = 1U;
            for (const auto &adjacency_vertex : adjacency_lists[vertex]) {
                if (color[adjacency_vertex] == 1U) {
                    has_cycle = true;
                    return;
                } else {
                    dfs(adjacency_vertex);
                }

                if (has_cycle)
                    return;
            }

            color[vertex] = 2U;
        }

        void findCycle() {
            color.resize(adjacency_lists.size(), 0U);
            for (unsigned i = 1U; i < adjacency_lists.size(); ++i) {
                if (color[i] == 0U) {
                    dfs(i);

                    if (has_cycle) {
                        break;
                    }
                }
            }
        }

        void printHasCycle() {
            std::cout << has_cycle;
        }

    private:
        std::vector<std::vector<unsigned>> adjacency_lists;
        std::vector<unsigned> color;
        bool has_cycle = false;
    };

    unsigned N;
    unsigned M;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task B;
    B.solve();
}