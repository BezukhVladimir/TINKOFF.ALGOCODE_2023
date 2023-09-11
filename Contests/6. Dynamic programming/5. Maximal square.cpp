#include <vector>
#include <iostream>

typedef std::vector<std::vector<unsigned>> TMatrix;

unsigned maxSquare(TMatrix& m, unsigned& x, unsigned& y) {
    std::vector<unsigned> dp(m[0].size() + 1U, 0U);
    unsigned max = 0U;
    unsigned prev = 0U;
    for (unsigned i = 1U; i <= m.size(); ++i) {
        for (unsigned j = 1U; j <= m[0].size(); ++j) {
            unsigned temp = dp[j];
            if (m[i - 1U][j - 1U] == 1U) {
                dp[j] = std::min(std::min(dp[j - 1U], dp[j]), prev) + 1U;

                if (max <= dp[j]) {
                    max = dp[j];
                    x = i - (max - 1U);
                    y = j - (max - 1U);
                }
            } else {
                dp[j] = 0U;
            }

            prev = temp;
        }
    }

    return max;
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    
    unsigned n, m;
    std::cin >> n >> m;

    TMatrix input(n, std::vector<unsigned >(m));
    for (auto& row : input) {
        for (auto& bit : row) {
            std::cin >> bit;
        }
    }

    unsigned x, y;
    std::cout << maxSquare(input, x, y) << '\n'
              << x << ' ' << y;
}