#include <vector>
#include <iostream>

class Graph {
public:
    void initialization() {
        std::cin >> vertex_number;
        time.resize(vertex_number + 1U);

        for (unsigned i = 1U; i <= vertex_number; ++i) {
            std::cin >> time[i];
        }

        required_parts.resize(vertex_number + 1U);
        for (unsigned i = 1U; i <= vertex_number; ++i) {
            unsigned count;
            std::cin >> count;

            for (unsigned j = 0U; j < count; ++j) {
                unsigned part;
                std::cin >> part;

                required_parts[i].emplace_back(part);
            }
        }
    }


    void solve() {
        initialization();
        topologicalSort();
        print();
    }

private:
    void topologicalSort() {
        was_created.resize(vertex_number + 1U, false);
        createPart(1U);
    }

    void createPart(const unsigned &current_part) {
        if (was_created[current_part]) {
            return;
        }

        for (const auto &part : required_parts[current_part]) {
            createPart(part);
        }

        order.emplace_back(current_part);
        total_time += time[current_part];

        was_created[current_part] = true;
    }

    void print() {
        std::cout << total_time << ' ' << order.size() << '\n';
        for (const auto &part : order) {
            std::cout << part << ' ';
        }
    }

    unsigned vertex_number;
    std::vector<std::vector<unsigned>> required_parts;

    unsigned long long total_time = 0U;

    std::vector<unsigned> time;
    std::vector<unsigned> order;
    std::vector<bool> was_created;
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Graph C;
    C.solve();
}