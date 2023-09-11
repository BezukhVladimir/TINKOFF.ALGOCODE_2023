#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Алгоритм Ландау-Вишкина (k несовпадений)
class FuzzyStringSearch {
    typedef std::vector<std::vector<int>> TMatrix;
public:
    FuzzyStringSearch() = delete;
    FuzzyStringSearch(std::string & text_, std::string & pattern_, size_t k_differences = 1) {
        text = text_;
        pattern = pattern_;

        n = text.length();
        m = pattern.length();

        k = k_differences;
        tm.resize(n - m + 1, std::vector<int>(k + 1));
        for (auto &i: tm) {
            for (auto &j: i) {
                j = m + 1;
            }
        }
    }

    void solve() {
        algorithmLandauViskin();

        size_t counter = 0;
        std::stringstream positions;
        for (int i = 0; i < n - m + 1; ++i) {
            if (tm[i][k] == m + 1) {
                ++counter;
                positions << i + 1 << ' ';
            }
        }

        std::cout << counter << '\n';
        std::cout << positions.str();
    }

private:
    size_t n, m, k;
    TMatrix tm;
    std::string text;
    std::string pattern;

    void algorithmLandauViskin() {
        for (int i = 0; i < n - m + 1; ++i) {
            int b = 0;

            for (int j = 0; j < m; ++j) {
                if (text[i + j] != pattern[j]) {
                    tm[i][b++] = j + 1;
                }

                if (b == k + 1)
                    break;
            }
        }
    }
};

int main() {
    std::string text;
    std::string pattern;

    std::cin >> pattern >> text;

    if (pattern.length() > text.length()) {
        std::cout << 0;
    } else {
        FuzzyStringSearch taskOrder(text, pattern);
        taskOrder.solve();
    }
}