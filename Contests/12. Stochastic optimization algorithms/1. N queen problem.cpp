#include <ctime>
#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>

int N;
int  * answer;
bool * main_diagonals;
bool * anti_diagonals;

int randNumber(const int& max = RAND_MAX) {
    static std::mt19937 gen(std::time(NULL));
    std::uniform_int_distribution<> uid(0, max);
    return uid(gen);
}

void clearDiagonals() {
    for (int i = 0; i < 2 * N - 1; ++i) {
        main_diagonals[i] = false;
        anti_diagonals[i] = false;
    }
}

bool isSafe(const int& row, const int& col) {
    return !(main_diagonals[col - row + N - 1] || anti_diagonals[col + row]);
}

int evaluate(int * state) {
    int correctly_set_queens = 1;
    main_diagonals[state[0] + N - 1] = true;
    anti_diagonals[state[0]]         = true;

    for (int i = 1; i < N; ++i) {
        if (isSafe(i, state[i])) {
            ++correctly_set_queens;
        }

        main_diagonals[state[i] - i + N - 1] = true;
        anti_diagonals[state[i] + i]         = true;
    }

    clearDiagonals();
    return correctly_set_queens;
}

void shuffle(int * arr, const int& n) {
    for (int i = n - 1; i >= 1; --i) {
        int j = randNumber(i);
        std::swap(arr[j], arr[i]);
    }
}

void initialization() {
    std::cin >> N;

    main_diagonals = new bool[2 * N - 1];
    anti_diagonals = new bool[2 * N - 1];
    clearDiagonals();

    answer = new int[N];
    for (int i = 0; i < N; ++i) {
        answer[i] = i + 1;
    }
    shuffle(answer, N);
}

void solveBySimulatedAnnealing() {
    const int iterations = 50000;
    int * new_state = new int[N];

    double t = 1;
    int current_evaluation = evaluate(answer);
    for (int i = 0; i < iterations && current_evaluation < N; ++i) {
        t *= 0.99;

        for (int j = 0; j < N; ++j) {
            new_state[j] = answer[j];
        }
        std::swap(new_state[randNumber() % N], new_state[randNumber() % N]);
        int new_evaluation = evaluate(new_state);

        if (new_evaluation > current_evaluation
            || (double(randNumber()) / RAND_MAX) < std::exp((new_evaluation - current_evaluation) / t)) {
            for (int j = 0; j < N; ++j) {
                answer[j] = new_state[j];
            }

            current_evaluation = new_evaluation;
        }
    }
    
    delete [] new_state;
}

void printAnswer() {
    for (int i = 0; i < N; ++i) {
        std::cout << answer[i] << ' ';
    }
}

void freeMemory() {
    delete [] main_diagonals;
    delete [] anti_diagonals;
    delete [] answer;
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    initialization();
    solveBySimulatedAnnealing();
    printAnswer();
    freeMemory();
}