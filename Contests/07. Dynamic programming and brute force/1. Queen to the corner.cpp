#include <cmath>
#include <vector>
#include <utility>
#include <iostream>

bool checkVerticalPositions(const int &i, const std::vector<std::pair<int, int>> &p) {
    for (const auto &yx : p) {
        if (i == yx.first) {
            return true;
        }
    }
    
    return false;
}

bool checkHorizontalPositions(const int &j, const std::vector<std::pair<int, int>> &p) {
    for (const auto &yx : p) {
        if (j == yx.second) {
            return true;
        }
    }
    
    return false;
}

bool checkDiagonalPositions(const int &i, const int &j, const std::vector<std::pair<int, int>> &p) {
    for (const auto &yx : p) {
        if ((std::abs(j - yx.second) == i - yx.first) && (j < yx.second)) {
            return true;
        }
    }
    
    return false;
}

int main() {
    int m, n;
    std::cin >> m >> n;
    std::vector<std::vector<bool>> dp(m, std::vector<bool>(n));
    
    std::vector<std::pair<int, int>> p(1, std::pair<int, int>(0, n - 1)); // positions of moves that do not result in defeat
    for (int i = 0; i < m; ++i) {
        for (int j = n - 1; j >= 0; --j) {
            if (i == 0 && j == n - 1) {
                dp[i][j] = false;
            } else if (checkVerticalPositions(i, p)) {
                dp[i][j] = true;
            } else if (checkHorizontalPositions(j, p)) {
                dp[i][j] = true;
            } else if (checkDiagonalPositions(i, j, p)) {
                dp[i][j] = true;
            } else {
                if (dp[i - 1][j] && dp[i - 1][j + 1] && dp[i][j + 1]) {
                    dp[i][j] = false;
                    p.emplace_back(std::pair<int, int>(i, j));
                } else {
                    dp[i][j] = true;
                }
            }
            
        }
    }
    
    std::cout << (dp[m - 1][0] ? 1 : 2);
}