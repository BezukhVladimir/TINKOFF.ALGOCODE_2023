#include <cmath>
#include <iomanip>
#include <iostream>

double fun(const double &x) {
    return x * x + sqrt(x + 1);
}

int main() {
    double C;
    std::cin >> C;
    
    double l = 0.0;
    double r = C; 
    while (fabs(l - r) > 1e-6) {
        double m = (l + r) / 2.0;
        
        if (fun(m) < C)
            l = m;
        else
            r = m;
    }
    
    std::cout << std::setprecision(7) << r;
}