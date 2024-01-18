#include <map>
#include <queue>
#include <vector>
#include <iostream>

typedef std::pair<int, int> T_xy;

class Task {
public:
    Task() {
        std::cin >> _N 
                 >> _start.first >> _start.second
                 >> _end.first   >> _end.second;
    }

    void solve() {
        int length = findShortestPath(_N, _start, _end);
        std::cout << length << '\n';
        printPath();
    }

private:

    int findShortestPath(const unsigned &n, const T_xy& start, const T_xy& end) {
        std::queue<T_xy> q;
        q.push(start);
        
        is_visited[start] = std::pair(-1, -1);
        while (!q.empty()) {
            T_xy current = q.front();
            q.pop();
            
            if (current == end) {
                int length = 0;
                
                while (current != std::pair(-1, -1)) {
                    ++length;
                    current = is_visited[current];
                }
                
                return length - 1;
            }
            
            for (const auto& move : moves) {
                T_xy new_pos = { current.first + move.first, current.second + move.second };
                
                if (new_pos.first  > 0 && new_pos.first  <= n
                 && new_pos.second > 0 && new_pos.second <= n
                 && is_visited.find(new_pos) == is_visited.end()) {
                    q.push(new_pos);
                    is_visited[new_pos] = current;
                }
            }
        }
        
        return -1;
    }
    
    void printPath() {
        T_xy current = _end;
        std::vector<T_xy> path;
        
        while (current != std::pair(-1, -1)) {
            path.emplace_back(current);
            current = is_visited[current];
        }
        
        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            std::cout << it->first << ' ' << it->second << '\n';
        }
    }
    
    unsigned _N;
    T_xy _start, _end;
    std::map<T_xy, T_xy> is_visited;
    std::vector<T_xy> moves = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task C;
    C.solve();
}