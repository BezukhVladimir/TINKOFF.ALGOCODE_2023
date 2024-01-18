#include <vector>
#include <iostream>
#include <algorithm>

class Task {
public:
    Task() {
        std::cin >> N >> M;
    }

    void solve() {
        Graph graph(N, M);
        graph.findComponents();
        graph.printComponents();
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
                adjacency_lists[second_vertex].emplace_back(first_vertex);
            }
        }

        void dfs(const unsigned &vertex, std::vector<unsigned> &component) {
            is_visited[vertex] = true;
            component.emplace_back(vertex);
            for (const auto &adjacency_vertex : adjacency_lists[vertex]) {
                if (!is_visited[adjacency_vertex]) {
                    dfs(adjacency_vertex, component);
                }
            }
        }

        void findComponents() {
            is_visited.resize(adjacency_lists.size() + 1U, false);
            for (unsigned vertex = 1U; vertex < adjacency_lists.size(); ++vertex) {
                if (!is_visited[vertex]) {
                    std::vector<unsigned> component;
                    dfs(vertex, component);
                    std::sort(component.begin(), component.end());
                    components.emplace_back(component);
                }
            }
        }

        void printComponents() {
            std::cout << components.size() << '\n';

            for (const auto &component : components) {
                std::cout << component.size() << '\n';

                for (const auto &vertex : component) {
                    std::cout << vertex << ' ';
                }
                std::cout << '\n';
            }
        }

    private:
        std::vector<std::vector<unsigned>> adjacency_lists;
        std::vector<std::vector<unsigned>> components;
        std::vector<bool> is_visited;
    };

    unsigned N;
    unsigned M;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task A;
    A.solve();
}