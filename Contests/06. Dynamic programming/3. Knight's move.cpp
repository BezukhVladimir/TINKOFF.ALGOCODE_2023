#include <vector>
#include <iostream>

unsigned F(const unsigned& n, const unsigned& m) {
    std::vector<std::vector<unsigned>>
        grid(n + 3U, std::vector<unsigned>(m + 3U, 0U));

    grid[2][2] = 1U;
    unsigned current_diagonal_i = 2U;
    unsigned current_diagonal_j = 2U;
    while ((current_diagonal_i < n + 1U)
        || (current_diagonal_j < m + 1U)) {
        if (current_diagonal_j == m + 1U) {
            ++current_diagonal_i;
        } else {
            ++current_diagonal_j;
        }

        unsigned i = current_diagonal_i;
        unsigned j = current_diagonal_j;
        while ((i <= n + 1U) && j >= 2U) {
            grid[i][j] = grid[i + 1][j - 2] + grid[i - 1][j - 2] + grid[i - 2][j - 1] + grid[i - 2][j + 1];
            ++i;
            --j;
        }
    }

    return grid[n + 1][m + 1];
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    unsigned n, m;
    std::cin >> n >> m;

    std::cout << F(n, m);
}