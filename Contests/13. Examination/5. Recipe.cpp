#include <bits/stdc++.h>

const int MAX_INGREDIENTS = 101;
long long INF_COST = 0x1000000000000LL;
long long min_cost[MAX_INGREDIENTS];

int counter_id = 0;
std::map<std::string, int> ingredient_id;

std::vector<int> dish;
std::vector<std::vector<int>> ingredient_parts;

long long best_cost = 0;

long long dfs(const int& id) {
    long long cost = 0;

    for (const int& possible_parent : ingredient_parts[id]) {
        cost += dfs(possible_parent);
    }

    if (cost == 0) {
        cost = INF_COST;
    }

    min_cost[id] = std::min(min_cost[id], cost);
    return min_cost[id];
}

int getId(std::string name) {
    if (ingredient_id[name] == 0) {
        ingredient_id[name] = ++counter_id;
    }

    return ingredient_id[name];
}

void read() {
    int N;
    std::cin >> N;

    for (int i = 0; i < N; ++i) {
        std::string name;
        std::cin >> name;

        ingredient_id[name] = ++counter_id;
        dish.emplace_back(ingredient_id[name]);
    }

    for (int i = 0; i < MAX_INGREDIENTS; ++i) {
        min_cost[i] = INF_COST;
    }

    int M;
    std::cin >> M;

    for (int i = 0; i < M; ++i) {
        std::string name;
        int cost;
        std::cin >> name >> cost;

        min_cost[getId(name)] = cost;
    }

    ingredient_parts.resize(MAX_INGREDIENTS + 1);

    int K;
    std::cin >> K;

    for (int i = 0; i < K; ++i) {
        int C;
        std::string name;
        std::cin >> C >> name;

        getId(name);
        for (int j = 0; j < C; ++j) {
            std::string part;
            std::cin >> part;
            ingredient_parts[ingredient_id[name]].emplace_back(getId(part));
        }
    }
}

void solve() {
    for (int i = 1; i <= ingredient_id.size(); ++i) {
        dfs(i);
    }

    for (int i = 0; i < dish.size(); ++i) {
        best_cost += min_cost[dish[i]];
    }
}

void print() {
    if (best_cost < INF_COST) {
        std::cout << best_cost;
    } else {
        std::cout << -1;
    }
}

int main() {
    read();
    solve();
    print();
}