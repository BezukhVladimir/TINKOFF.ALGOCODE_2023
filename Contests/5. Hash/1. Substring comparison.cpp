#include <string>
#include <vector>
#include <iostream>

typedef unsigned long long ull;

class Task {
private:
    size_t m;
    std::string s;
    
    // details: http://e-maxx.ru/algo/string_hashes
    
    const ull P = 31ULL;
    std::vector<ull> p_pows;
    std::vector<ull> hashes;
    
    ull getHash(const ull& l, const ull& r) const {
        ull hash = hashes[r - 1];
        if (l != 1)
            hash -= hashes[l - 2];
            
        return hash;
    }

    bool isEqualSubstrings(const ull& l1, const ull& r1,
                           const ull& l2, const ull& r2) const {
        if (l1 == l2 && r1 == r2) {
            return true;
        }
        
        if (r1 - l1 != r2 - l2) {
            return false;
        }
            
        if (l1 == r1) {
            return s[l1 - 1] == s[l2 - 1];
        }
        
        ull h1 = getHash(l1, r1);
        ull h2 = getHash(l2, r2);
        
        if (l1 < l2) {
            return h1 * p_pows[l2 - l1] == h2;
        }
        else if (l1 > l2) {
            return h1 == h2 * p_pows[l1 - l2] ;
        }
    }

public:
    void solve() {
        std::cin >> s >> m;
        
        if (m != 0) {
            hashes.resize(s.length());
            p_pows.resize(s.length());
        
            p_pows[0] = 1ULL;
            hashes[0] = (s[0] - 'a' + 1);
            for (size_t i = 1; i < s.length(); ++i) {
                p_pows[i] = p_pows[i - 1] * P;
                hashes[i] = (s[i] - 'a' + 1) * p_pows[i];
                hashes[i] += hashes[i - 1];
            }
            
            for (size_t i = 0; i < m; ++i) {
                ull l1, r1, l2, r2;
                std::cin >> l1 >> r1 >> l2 >> r2;
                
                if (isEqualSubstrings(l1, r1, l2, r2)) {
                    std::cout << "Yes\n";
                } else {
                    std::cout << "No\n";
                }
            }
        }
    }
};

int main()
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Task A;
    A.solve();
}