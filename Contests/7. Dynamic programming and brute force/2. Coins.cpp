#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>

typedef unsigned long long ull;
typedef std::vector<ull> TVull;

void pay(ull N, ull total_amount_of_money, const TVull &A, ull coin_copies,
         ull &coin_counter, TVull &coin_copies_counters, bool &did_he_pay,
         ull total_coin_counter = 0, TVull all_coin_copies_counters = {}) {
    if  (N == 0) {
        bool total_counter_is_valid
            = !coin_counter || (total_coin_counter < coin_counter);
 
        if  (!did_he_pay || total_counter_is_valid) {
            coin_counter = total_coin_counter;
            coin_copies_counters = all_coin_copies_counters;
            did_he_pay = true;
        }
    } else if (total_amount_of_money >= N) {
        size_t index = A.size() - 1 - all_coin_copies_counters.size();
        ull current_coin = A[index];
        ull new_total_amount_of_money = total_amount_of_money - current_coin * coin_copies;
        ull current_coin_copies = std::min(coin_copies, N / current_coin);
 
        for (int copies = current_coin_copies; copies >= 0; --copies) {
            ull new_N = N - current_coin * copies;
            ull new_total_coin_counter = total_coin_counter + copies;
            TVull new_all_coin_copies_counters = all_coin_copies_counters;
            new_all_coin_copies_counters.emplace_back(copies);
 
            pay(new_N, new_total_amount_of_money, A, coin_copies,
                coin_counter, coin_copies_counters, did_he_pay,
                new_total_coin_counter, new_all_coin_copies_counters);
        }
    }
}

void solve(ull N, std::vector<ull> A, ull coin_copies) {
    std::sort(A.begin(), A.end());
    ull coin_counter = 0ULL;
    TVull coin_copies_counters;
    ull total_amount_of_money
        = std::accumulate(A.begin(), A.end(), 0ULL) * coin_copies;
 
    if (N > total_amount_of_money) {
        std::cout << -1;
    } else {
        bool did_he_pay;
 
        pay(N, total_amount_of_money, A, coin_copies,
            coin_counter, coin_copies_counters, did_he_pay);
 
        if (did_he_pay) {
            std::cout << coin_counter << '\n';
 
            for (ull i = 0ULL; i < coin_copies_counters.size(); ++i) {
                for (ull counter = 0ULL; counter < coin_copies_counters[i]; ++counter) {
                    std::cout << A[A.size() - 1 - i] << ' ';
                }
            }
        } else {
            std::cout << 0;
        }
    }
}

int main() {
    const ull COIN_COPIES = 2ULL;
    ull N, M;
    std::cin >> N >> M;

    TVull A(M);
    for (ull &denomination : A) {
        std::cin >> denomination;
    }

    solve(N, A, COIN_COPIES);
}