#include <vector>
#include <iostream>
#include <algorithm>

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
        matrix(first.size() + 1U, std::vector<unsigned>(second.size() + 1U));

    matrix[0][0] = 0U;
    for (unsigned i = 1; i <= first.size(); ++i) {
        matrix[i][0] = i * delete_cost;
    }
    for (unsigned j = 1; j <= second.size(); ++j) {
        matrix[0][j] = j * insert_cost;
    }

    for (unsigned i = 1U; i <= first.size(); ++i) {
        for (unsigned j = 1U; j <= second.size(); ++j) {
            bool is_diff = (first[i - 1U] != second[j - 1U]);
            matrix[i][j]
                = std::min(matrix[i - 1U][j]      + insert_cost, std::min(
                           matrix[i][j - 1U]      + delete_cost,
                           matrix[i - 1U][j - 1U] + replace_cost * is_diff));

            if ((i > 1)
             && (j > 1)
             && (first[i - 1U] == second[j - 2U])
             && (first[i - 2U] == second[j - 1U])) {
                matrix[i][j]
                    = std::min(matrix[i][j],
                               matrix[i - 2U][j - 2U] + transpose_cost * is_diff);
            }
        }
    }

    return matrix[first.size()][second.size()];
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    std::string first, second;
    std::cin >> first >> second;

    std::cout << F(first, second) << std::endl;
}