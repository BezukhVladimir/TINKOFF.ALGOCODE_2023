#include <vector>
#include <iostream>
#include <algorithm>
 
typedef unsigned long long ull;

std::vector<ull> summands;
void solve(ull current_position,
           ull current_max_summand, ull number) {
    if (!number) {
        std::cout << summands[0];    
        for (ull i = 1ULL; i < current_position; ++i) {
            std::cout << ' ' << summands[i];
        }
        std::cout << '\n';
    } else {
        for (ull i = 1ULL; i <= std::min(current_max_summand, number); ++i) {
            summands[current_position] = i;
            solve(current_position + 1ULL, i, number - i);
        }
    }
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    
    ull n;
    std::cin >> n;
    summands.resize(n);
    solve(0ULL, n, n);
}