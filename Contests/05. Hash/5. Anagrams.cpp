#include <map>
#include <random>
#include <vector>
#include <iostream>

typedef unsigned long long ull;
unsigned getHash() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> hash(1U, std::numeric_limits<unsigned>::max());
    return hash(rng);
}

int max_size, n, m;
std::vector<ull> first_sequence;
std::vector<ull> second_sequence;

void init() {
    unsigned value;
    std::map<unsigned, unsigned> numbers;

    std::cin >> n;
    first_sequence.resize(n + 1);

    std::cin >> value;
    numbers[value] = getHash();
    first_sequence[0] = 0;
    first_sequence[1] = numbers[value];

    for (size_t i = 2; i < n + 1; ++i) {
        std::cin >> value;
        if (numbers.find(value) == numbers.end()) {
            numbers[value] = getHash();
        }

        first_sequence[i] = first_sequence[i - 1] + numbers[value];
    }

    std::cin >> m;
    second_sequence.resize(m + 1);
    second_sequence[0] = 0;

    std::cin >> value;
    if (numbers.find(value) == numbers.end()) {
        numbers[value] = getHash();
    }

    second_sequence[1] = numbers[value];
    for (size_t i = 2; i < m + 1; ++i) {
        std::cin >> value;
        if (numbers.find(value) == numbers.end()) {
            numbers[value] = getHash();
        }

        second_sequence[i] = second_sequence[i - 1] + numbers[value];
    }
}

void solve() {
    max_size = n < m ? n : m;
    while (max_size > 0) {
        for (int i = 0; i < n - max_size + 1; ++i) {
            for (int j = 0; j < m - max_size + 1; ++j) {
                if ((first_sequence[n - i] + second_sequence[m - max_size - j])
                 == (second_sequence[m - j] + first_sequence[n - max_size - i])) {
                    return;
                }
            }
        }

        --max_size;
    }
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    init();
    solve();
    std::cout << max_size;
}