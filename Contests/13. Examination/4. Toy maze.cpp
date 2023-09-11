#include <bits/stdc++.h>

struct Position {
    int i;
    int j;

    Position(const int& i_, const int& j_)
        : i(i_), j(j_) {}
};

void solve() {
    int N, M;
    std::cin >> N >> M;

    std::vector<std::vector<int>> grid(N + 2, std::vector<int>(M + 2, 1));
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            std::cin >> grid[i][j];
        }
    }

    std::vector<std::vector<int>> len(N + 2, std::vector<int>(M + 2, -1));

    std::queue<Position> q;
    q.push((Position(1, 1)));
    len[1][1] = 0;

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        if (grid[current.i][current.j] == 2) {
            std::cout << len[current.i][current.j];
            return;
        }

        for (int shift_i = -1; shift_i <= 1; ++shift_i) {
            for (int shift_j = -1; shift_j <= 1; ++shift_j) {
                if (shift_i * shift_i + shift_j * shift_j == 1) {
                    Position new_pos(current.i, current.j);

                    while (grid[new_pos.i][new_pos.j] != 2
                        && grid[new_pos.i + shift_i][new_pos.j + shift_j] != 1) {

                        new_pos.i += shift_i;
                        new_pos.j += shift_j;
                    }

                    if (len[new_pos.i][new_pos.j] == -1) {
                        len[new_pos.i][new_pos.j] = len[current.i][current.j] + 1;
                        q.push(Position(new_pos.i, new_pos.j));
                    }
                }
            }
        }
    }
}

int main() {
    solve();
}