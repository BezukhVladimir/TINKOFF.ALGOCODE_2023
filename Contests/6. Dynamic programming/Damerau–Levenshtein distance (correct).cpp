#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

unsigned F(const std::string& first,
           const std::string& second,
           const unsigned& insert_cost = 1U,
           const unsigned& delete_cost = 1U,
           const unsigned& replace_cost = 1U,
           const unsigned& transpose_cost = 1U) {
    if (first.empty()) {
        if (second.empty()) {
            return 0U;
        } else {
            return second.size() * delete_cost;
        }
    } else if (second.empty()) {
        return first.size() * insert_cost;
    }

    std::vector<std::vector<unsigned>>
        matrix(first.size() + 2U, std::vector<unsigned>(second.size() + 2U));
    unsigned big = (first.size() + second.size())
                   * std::max(std::max(insert_cost, delete_cost),
                              std::max(replace_cost, transpose_cost));

    matrix[0][0] = big;
    for (unsigned i = 0; i <= first.size(); ++i) {
        matrix[i + 1U][0] = big;
        matrix[i + 1U][1] = i * delete_cost;
    }
    for (unsigned j = 0; j <= second.size(); ++j) {
        matrix[0][j + 1U] = big;
        matrix[1][j + 1U] = j * insert_cost;
    }

    std::unordered_map<char, unsigned> last_positions;
    for (const auto& c : first) {
        last_positions[c] = 0U;
    }
    for (const auto& c : second) {
        last_positions[c] = 0U;
    }

    for (unsigned i1 = 1U; i1 <= first.size(); ++i1) {
        unsigned last = 0U;
        for (unsigned j1 = 1U; j1 <= second.size(); ++j1) {
            unsigned i2 = last_positions[second[j1 - 1U]];
            unsigned j2 = last;

            if (first[i1 - 1U] == second[j1 - 1U]) {
                matrix[i1 + 1U][j1 + 1U] = matrix[i1][j1];
                last = j1;
            } else {
                matrix[i1 + 1U][j1 + 1U]
                    = std::min(std::min(matrix[i1][j1]      + replace_cost,
                                        matrix[i1 + 1U][j1] + insert_cost),
                                        matrix[i1][j1 + 1U] + delete_cost);
            }

            matrix[i1 + 1U][j1 + 1U]
                = std::min(matrix[i1 + 1U][j1 + 1U],
                           matrix[i2][j2] + (i1 - i2 - 1U) * delete_cost
                                          + transpose_cost
                                          + (j1 - j2 - 1U) * insert_cost);
        }

        last_positions[first[i1 - 1U]] = i1;
    }

    return matrix[first.size() + 1U][second.size() + 1U];
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    std::string first, second;
    std::cin >> first >> second;

    std::cout << F(first, second);
}