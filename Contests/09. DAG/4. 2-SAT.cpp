#include <vector>
#include <iostream>

class Graph {
public:
    Graph(const unsigned &count) {
        variable_number = count;
        size = 2U * variable_number;
    }
    void initialization() {
        std::cin >> request_number;

        adjacency_lists.assign(size, std::vector<unsigned>());
        adjacency_lists_reverse.assign(size, std::vector<unsigned>());
        for (unsigned i = 0U; i < request_number; ++i) {
            unsigned x1, e1, x2, e2;
            std::cin >> x1 >> e1 >> x2 >> e2;

            addClauseOr(x1, e1, x2, e2);
        }
    }

    void solve() {
        initialization();
        findEndingVertices();
        findStronglyConnectedComponents();
        calcBitmask();
        printAnswer();
    }

private:
    void findEndingVertices() {
        is_visited.assign(size, false);
        for (unsigned i = 0U; i < size; ++i) {
            if (!is_visited[i]) {
                DFS1(i);
            }
        }
    }

    void DFS1(const unsigned& vertex) {
        is_visited[vertex] = true;
        for (const auto &to : adjacency_lists[vertex]) {
            if (!is_visited[to]) {
                DFS1(to);
            }
        }

        ending_vertices.emplace_back(vertex);
    }

    void findStronglyConnectedComponents() {
        color.assign(size, -1);
        component_number = 0U;

        for (auto it = ending_vertices.rbegin(); it < ending_vertices.rend(); ++it) {
            if (color[*it] == -1) {
                DFS2(*it, component_number++);
            }
        }
    }

    void DFS2(const unsigned &vertex, const unsigned &current_color) {
        color[vertex] = current_color;
        for (const auto &to : adjacency_lists_reverse[vertex]) {
            if (color[to] == -1) {
                DFS2(to, current_color);
            }
        }
    }

    void calcBitmask() {
        answer.assign(variable_number, 0U);
        for (unsigned i = 0U; i < variable_number; ++i) {
            /*if (color[i] == color[i + variable_number]) {

            }*/

            answer[i] = (color[i] > color[i + variable_number] ? 1U : 0U);
        }
    }

    void printAnswer() {
        for (const auto &bit : answer) {
            std::cout << bit;
        }

        std::cout << '\n';
    }

    void addEdge(const unsigned &u, const unsigned &v) {
        adjacency_lists[u].emplace_back(v);
        adjacency_lists_reverse[v].emplace_back(u);
    }

    void addClauseOr(const unsigned &x1, const unsigned &e1,
                     const unsigned &x2, const unsigned &e2) {
        addEdge(x1 + (e1 ? variable_number : 0U), x2 + (e2 ? 0U : variable_number));
        addEdge(x2 + (e2 ? variable_number : 0U), x1 + (e1 ? 0U : variable_number));
    }

    unsigned size;
    unsigned request_number;
    unsigned variable_number;
    unsigned component_number;

    std::vector<std::vector<unsigned>> adjacency_lists;
    std::vector<std::vector<unsigned>> adjacency_lists_reverse;

    std::vector<int> color;
    std::vector<bool> is_visited;
    std::vector<unsigned> ending_vertices;

    std::vector<unsigned> answer;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    unsigned variable_number;
    while (std::cin >> variable_number) {
        Graph D(variable_number);
        D.solve();
    }
}