#include <vector>
#include <iostream>

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    unsigned n;
    std::cin >> n;

    std::vector<unsigned> result(n);

    for (unsigned i = 0; i < n; ++i) {
        if (i == 0 || i == 1) {
            std::cin >> result[i];
        } else {
            std::cin >> result[i];
            result[i] += std::min(result[i - 1], result[i - 2]);
        }
    }

    std::cout << result[n - 1];
}
