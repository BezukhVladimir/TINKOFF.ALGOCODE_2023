#include <vector>
#include <iostream>

class Graph {
public:
    void initialization() {
        std::cin >> vertex_number >> edge_number;

        adjacency_lists.resize(vertex_number + 1U);
        adjacency_lists_reverse.resize(vertex_number + 1U);
        for (unsigned i = 0U; i < edge_number; ++i) {
            unsigned first_vertex, second_vertex;
            std::cin >> first_vertex >> second_vertex;

            adjacency_lists[first_vertex].emplace_back(second_vertex);
            adjacency_lists_reverse[second_vertex].emplace_back(first_vertex);
        }
    }

    void solve() {
        initialization();
        findEndingVertices();
        findStronglyConnectedComponents();
        calcMinAmbulanceStations();
        printAmbulanceStationsNumber();
    }

private:
    void DFS1(const unsigned& vertex) {
        is_visited[vertex] = true;
        for (const auto &to : adjacency_lists[vertex]) {
            if (!is_visited[to]) {
                DFS1(to);
            }
        }

        ending_vertices.emplace_back(vertex);
    }

    void DFS2(const unsigned &vertex, const unsigned &current_color) {
        color[vertex] = current_color;
        for (const auto &to : adjacency_lists_reverse[vertex]) {
            if (color[to] == -1) {
                DFS2(to, current_color);
            }
        }
    }

    void findEndingVertices() {
        is_visited.resize(vertex_number + 1U, false);
        for (unsigned i = 1U; i <= vertex_number; ++i) {
            if (!is_visited[i]) {
                DFS1(i);
            }
        }
    }

    void findStronglyConnectedComponents() {
        color.assign(vertex_number + 1U, -1);
        component_number = 0U;

        for (unsigned i = 1U; i <= vertex_number; ++i) {
            unsigned current_vertex = ending_vertices[vertex_number - i];
            if (color[current_vertex] == -1) {
                DFS2(current_vertex, component_number++);
            }
        }
    }

    void calcMinAmbulanceStations() {
        is_visited.assign(component_number, true);
        for (unsigned i = 1; i <= vertex_number; ++i) {
            for (const auto &to: adjacency_lists[i]) {
                if (color[i] != color[to]) {
                    is_visited[color[i]] = false;
                }
            }
        }

        ambulance_stations_number = 0U;
        for (const auto &should_be_built : is_visited) {
            if (should_be_built) {
                ++ambulance_stations_number;
            }
        }
    }

    void printAmbulanceStationsNumber() {
        std::cout << ambulance_stations_number;
    }

    unsigned edge_number;
    unsigned vertex_number;
    unsigned component_number;
    unsigned ambulance_stations_number;

    std::vector<std::vector<unsigned>> adjacency_lists;
    std::vector<std::vector<unsigned>> adjacency_lists_reverse;

    std::vector<int> color;
    std::vector<bool> is_visited;
    std::vector<unsigned> ending_vertices;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Graph A;
    A.solve();
}