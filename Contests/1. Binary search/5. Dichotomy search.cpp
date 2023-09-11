#include <cmath>
#include <iomanip>
#include <iostream>

double a, b, c, d;

double fun(const double &x) {
    double xx = x * x;
    double xxx = x * xx;
    return a * xxx + b * xx + c * x + d;
}

int main() {
    std::cin >> a >> b >> c >> d;

    double r = 1.0;
    while (fun(r) * fun(-r) >= 0.0)
        r *= 2.0;

    double l = -r;

    while (r - l > 1e-6) {
        double m = (l + r) / 2.0;

        if (fun(m) * fun(r) > 0.0)
            r = m;
        else
            l = m;
    }
    
    std::cout << l;
}