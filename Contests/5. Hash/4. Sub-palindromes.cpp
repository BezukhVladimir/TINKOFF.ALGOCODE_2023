#include <vector>
#include <string>
#include <numeric>
#include <iostream>

typedef unsigned long long ull;

ull min(const ull& a, const ull& b) {
    return (a < b) ? a : b;
}

int main()
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
 
    std::string text;
    std::cin >> text;
	
    std::vector<ull> d1(text.size());
    int l = 0, r = -1;
    for (int i = 0; i < text.size(); ++i) {
        ull radius = i > r ? 1ULL : min(d1[l + r - i], r - i + 1);
        
        while (i + radius < text.size()
            && i - radius >= 0
            && text[i + radius] == text[i - radius]) {
            ++radius;
        }
        d1[i] = radius;
        
        if (i + radius - 1 > r) {
            l = i - radius + 1;
            r = i + radius - 1;
        }
    }
    
    std::vector<ull> d2(text.size());
	l = 0, r = -1;
    for (int i = 0; i < text.size(); ++i) {
        ull radius = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
        
        while (i + radius < text.size()
            && i - radius - 1 >= 0
            && text[i + radius] == text[i - radius - 1]) {
            ++radius;
        }
        d2[i] = radius;
        
        if (i + radius - 1 > r) {
            l = i - radius;
            r = i + radius - 1;
        }
    }
    
    std::cout << std::reduce(d1.begin(), d1.end()) + std::reduce(d2.begin(), d2.end());
}