#include <queue>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>

class Task {
public:
    Task() {
        std::cin >> _K;
        graph.resize(_K);
    }

    void solve() {
        minSumDigits();
    }

private:
    typedef unsigned T_u;
    typedef std::pair<T_u, T_u> T_outedge;
    void shortestPath() {
        std::priority_queue<T_outedge,
                            std::vector<T_outedge>,
                            std::greater<T_outedge>> min_heap;

        std::vector<unsigned> distance(_K, std::numeric_limits<unsigned>::max());
        min_heap.push(T_outedge (1U, 0U));
        distance[1U] = 0U;
        while (!min_heap.empty()) {
            int current = min_heap.top().first;
            min_heap.pop();

            for (const auto &outedge : graph[current]) {
                if (distance[outedge.first] > distance[current] + outedge.second) {
                    distance[outedge.first] = distance[current] + outedge.second;
                    min_heap.push(T_outedge(outedge.first, distance[outedge.first]));
                }
            }
        }

        std::cout << 1U + distance[0U];
    }

    void minSumDigits() {
        for (unsigned i = 0U; i < _K; ++i) {
            graph[i % _K].emplace_back(std::pair((i + 1U) % _K, 1U));
        }

        for (unsigned i = 0U; i < _K; ++i) {
            graph[i % _K].emplace_back(std::pair((i * 10U) % _K, 0U));
        }

        shortestPath();
    }

    unsigned _K;
    std::vector<std::vector<T_outedge>> graph;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task D;
    D.solve();
}