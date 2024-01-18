#include <vector>
#include <iostream>

typedef unsigned long long ull;

const ull FIB_SIZE = 88ULL; // FIB[88] <= 10^18, FIB[89] > 10^18
const std::vector<ull> FIB = { 1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL, 21ULL, 34ULL, 55ULL, 89ULL, 144ULL, 233ULL, 377ULL, 610ULL, 987ULL, 1597ULL, 2584ULL, 4181ULL, 6765ULL, 10946ULL, 17711ULL, 28657ULL, 46368ULL, 75025ULL, 121393ULL, 196418ULL, 317811ULL, 514229ULL, 832040ULL, 1346269ULL, 2178309ULL, 3524578ULL, 5702887ULL, 9227465ULL, 14930352ULL, 24157817ULL, 39088169ULL, 63245986ULL, 102334155ULL, 165580141ULL, 267914296ULL, 433494437ULL, 701408733ULL, 1134903170ULL, 1836311903ULL, 2971215073ULL, 4807526976ULL, 7778742049ULL, 12586269025ULL, 20365011074ULL, 32951280099ULL, 53316291173ULL, 86267571272ULL, 139583862445ULL, 225851433717ULL, 365435296162ULL, 591286729879ULL, 956722026041ULL, 1548008755920ULL, 2504730781961ULL, 4052739537881ULL, 6557470319842ULL, 10610209857723ULL, 17167680177565ULL, 27777890035288ULL, 44945570212853ULL, 72723460248141ULL, 117669030460994ULL, 190392490709135ULL, 308061521170129ULL, 498454011879264ULL, 806515533049393ULL, 1304969544928657ULL, 2111485077978050ULL, 3416454622906707ULL, 5527939700884757ULL, 8944394323791464ULL, 14472334024676221ULL, 23416728348467685ULL, 37889062373143906ULL, 61305790721611591ULL, 99194853094755497ULL, 160500643816367088ULL, 259695496911122585ULL, 420196140727489673ULL, 679891637638612258ULL };

ull backtracking(ull n, ull current_fib_index) {
    if (n == 1ULL) {
        return 1ULL;
    }
    if (current_fib_index >= FIB_SIZE) {
        return 0ULL;
    }
    if (n < FIB[current_fib_index]) {
        return 0ULL;
    }
    
    ull products_number = backtracking(n, current_fib_index + 1ULL);
    if (n % FIB[current_fib_index] == 0ULL) {
        products_number += backtracking(n / FIB[current_fib_index], current_fib_index);
    }

    return products_number;
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    
    ull n;
    std::cin >> n;
    
    for (ull i = 0; i < n; ++i) {
        ull x;
        std::cin >> x;
        
        std::cout << backtracking(x, 2ULL) << '\n';
    }
}