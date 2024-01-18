#include <string>
#include <iostream>

int main()
{
    int n;
    std::cin >> n;
    
    int l = 1;
    int r = n;
    std::string hint; 
    while (l + 1 < r) {
        int m = (l + r) / 2;
        
        std::cout << m << std::endl << std::flush;
        
        std::cin >> hint;
        if (hint[0] == '<')
            r = m - 1;
        else
            l = m + 1;
    }
    
    std::cout << l << std::endl << std::flush;

    std::cin >> hint;
        
    if (hint[0] == '>') {
        std::cout << r << std::endl << std::flush;
        
        std::cin >> hint;
        
        if (hint[0] == '>')
            std::cout << "! " << r;
        else
            std::cout << "! " << l;
        }
    else 
        std::cout << "! " << l - 1;
}