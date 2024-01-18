#include <vector>
#include <iostream>

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    unsigned n;
    std::cin >> n;

    std::vector<unsigned> result(n + 1);
    result[0] = 1U;
    result[1] = 3U;
    for (unsigned i = 2U; i <= n; ++i) {
        result[i] = 2U * (result[i - 1] + result[i - 2]);
    }

    std::cout << result[n];
}