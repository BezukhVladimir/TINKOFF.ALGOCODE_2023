#include <ctime>
#include <cmath>
#include <random>
#include <vector>
#include <iostream>
#include <algorithm>

struct Coord {
    int x, y;
    int shift_x, shift_y;

    Coord (const int& x_, const int& y_, const int& shift_x_, const int& shift_y_)
            : x(x_), y(y_), shift_x(shift_x_), shift_y(shift_y_) {}
};

int N, M, C;
int ** answer;
std::vector<Coord> bad_rectangle_coord;

int randNumber(const int &min = 0, const int &max = RAND_MAX) {
    static std::mt19937 gen(std::time(NULL));
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}

void createState(int ** &state) {
    state = new int * [N];
    for (int i = 0; i < N; ++i) {
        state[i] = new int[M];
    }
}

void deleteState(int ** &state) {
    for (int i = 0; i < N; ++i) {
        delete[] state[i];
    }
    delete [] state;
}

void setRandomState(int ** &state) {
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
            state[x][y] = randNumber(1, C);
        }
    }
}

void copyState(int ** &from, int ** &to) {
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
            to[x][y] = from[x][y];
        }
    }
}

bool isBad(int ** &state, const int& x, const int& y, const int& shift_x, const int& shift_y) {
    return (state[x][y] == state[x][y + shift_y]) &&
           (state[x][y] == state[x + shift_x][y]) &&
           (state[x][y] == state[x + shift_x][y + shift_y]);
}

bool isBadState(const int& evaluation, const int& consecutive_evaluations) {
    return (evaluation > 50 && consecutive_evaluations > 3)
        || (evaluation > 30 && consecutive_evaluations > 15)
        || (evaluation > 15 && consecutive_evaluations > 30)
        || (evaluation > 5  && consecutive_evaluations > 40)
        || (consecutive_evaluations > 50);
}

void changeState(int ** &state) {
    int t   = bad_rectangle_coord.size() / 2;
    int max = bad_rectangle_coord.size() - 1;
    for (int i = 0; i < t; ++i) {
        Coord bad = bad_rectangle_coord[randNumber(0, max)];
        while (isBad(state, bad.x, bad.y, bad.shift_x, bad.shift_y)) {
            switch (randNumber(1, 4)) {
                case 1:
                    state[bad.x][bad.y] = randNumber(1, C);
                    break;
                case 2:
                    state[bad.x][bad.y + bad.shift_y] = randNumber(1, C);
                    break;
                case 3:
                    state[bad.x + bad.shift_x][bad.y] = randNumber(1, C);
                    break;
                case 4:
                    state[bad.x + bad.shift_x][bad.y + bad.shift_y] = randNumber(1, C);
                    break;
            }
        }
    }
}

int evaluate(int ** &state) {
    bad_rectangle_coord.clear();

    for (int x = 0; x < N - 1; ++x) {
        for (int y = 0; y < M - 1; ++y) {
            for (int shift_x = 1; x + shift_x < N; ++shift_x) {
                for (int shift_y = 1; y + shift_y < M; ++shift_y) {
                    if (isBad(state, x, y, shift_x, shift_y)) {
                        bad_rectangle_coord.emplace_back(Coord(x, y, shift_x, shift_y));
                    }
                }
            }
        }
    }

    return bad_rectangle_coord.size();
}

void initialization() {
    std::cin >> N >> M >> C;
    createState(answer);
    setRandomState(answer);
}

void solveBySimulatedAnnealing() {
    const int iterations = 1000000;
    int ** new_state;
    createState(new_state);

    int current_evaluation = evaluate(answer);
    while (current_evaluation != 0) {
        double t = 1;
        setRandomState(answer);
        current_evaluation = evaluate(answer);

        int consecutive_evaluations = 0;
        for (int i = 0; i < iterations && current_evaluation != 0; ++i) {
            if (isBadState(current_evaluation, consecutive_evaluations)) {
                break;
            }

            t *= 0.99;
            copyState(answer, new_state);
            changeState(new_state);
            int new_evaluation = evaluate(new_state);
            if (new_evaluation == current_evaluation) {
                ++consecutive_evaluations;
            }

            double t1 = (double(randNumber()) / RAND_MAX);
            double t2 = std::exp((current_evaluation - new_evaluation) / t);
            if (new_evaluation < current_evaluation || t1 < t2) {
                copyState(new_state, answer);

                if (current_evaluation != new_evaluation) {
                    consecutive_evaluations = 0;
                }

                current_evaluation = new_evaluation;
            }
        }
    }

    deleteState(new_state);
}

void printAnswer() {
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < M; ++y) {
            std::cout << answer[x][y] << ' ';
        }

        std::cout << '\n';
    }
}

void freeMemory() {
    deleteState(answer);
}

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    initialization();
    solveBySimulatedAnnealing();
    printAnswer();
    freeMemory();
}