#include <list>
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_map>

class Task {
public:
    Task() {
        std::cin >> _n >> _m >> _s >> _t;

        for (unsigned i = 0U; i < _m; ++i) {
            int weight;
            unsigned start, finish;
            
            std::cin >> start >> finish >> weight;
            graph[start].emplace_back(finish, weight);
        }

        distances.resize(_n + 1U, INF_WEIGHT);
        distances[_s] = 0;
    }

    void solve() {
        std::vector<unsigned> topological_sorted = topologicalSort();
        for (const auto& start : topological_sorted) {
            for (const auto& outedge : graph[start]) {
                if (distances[start] < INF_WEIGHT) {
                    if (distances[outedge.finish] > distances[start] + outedge.weight) {
                        distances[outedge.finish] = distances[start] + outedge.weight;
                    }
                }
            }
        }

        print();
    }

private:
    struct Outedge {
        unsigned finish;
        int weight;

        Outedge(const unsigned &f, const int &w) {
            finish = f;
            weight = w;
        }
    };
        
    std::vector<unsigned> topologicalSort() {
        std::vector<unsigned> topological_sorted;
        std::vector<unsigned> in_degree(_n + 1U, 0U);

        for (const auto& edges : graph) {
            for (const auto& outedge : edges.second) {
                ++in_degree[outedge.finish];
            }
        }

        std::queue<unsigned> q;
        for (unsigned i = 1U; i <= _n; ++i) {
            if (in_degree[i] == 0U) {
                q.push(i);
            }
        }

        while (!q.empty()) {
            unsigned start = q.front();
            q.pop();

            topological_sorted.emplace_back(start);
            for (const auto& outedge : graph[start]) {
                --in_degree[outedge.finish];

                if (in_degree[outedge.finish] == 0U) {
                    q.push(outedge.finish);
                }
            }
        }

        return topological_sorted;
    }

    void print() {
        if (distances[_t] == INF_WEIGHT) {
            std::cout << "Unreachable";
        } else {
            std::cout << distances[_t];
        }
    }

    std::vector<int> distances;
    std::unordered_map<unsigned, std::list<Outedge>> graph;
    unsigned _n, _m, _s, _t;
    const int INF_WEIGHT = 2147483647;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task E;
    E.solve();
}