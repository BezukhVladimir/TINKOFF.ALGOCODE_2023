#include <cmath>
#include <vector>
#include <stdio.h>

int readInt()
{
    bool is_neg = false;
    int c = getchar_unlocked();
    while (true) {
        if (c == '-') {
            is_neg = true;
            c = getchar_unlocked();
            break;
        }
        
        if (c == ' ') { // c < '0' || '9' < c
            c = getchar_unlocked();
            continue;
        }
        
        break;
    }

    int input = 0;
    while ('0' <= c && c <= '9') {
        input = input * 10 + c - '0';
        c = getchar_unlocked();
    }
    
    return is_neg ? -input : input;
}

void writeInt(int n)
{
    if (n < 0) {
        putchar_unlocked('-');
        n = -n;
    }
    
    int i = 0;
    char reverse[10];
    
    do
        reverse[i++] = n % 10;
    while (n /= 10);
    
    while (i--) {
        char digit = reverse[i] + '0';
        putchar_unlocked(digit);
    }
}

void writelnInt(int n) {
    writeInt(n);
    putchar_unlocked('\n');
}

int main()
{
    int n = readInt();
    int k = readInt();
    
    std::vector<int> elements;
    for (int i = 0; i < n; ++i) {
        int element = readInt();
        elements.emplace_back(element);
    }
    
    int min = elements.front();
    int max = elements.back();
    for (int i = 0; i < k; ++i) {
        int number = readInt();
        
        if (number < min) {
            writeInt(min);
            putchar_unlocked('\n');
            continue;
        }
        
        if (number > max) {
            writeInt(max);
            putchar_unlocked('\n');
            continue;
        }
        
        int l = -1;
        int r = elements.size();
        while (l + 1 < r) {
            int m = (l + r) / 2;
            
            if (elements[m] < number)
                l = m;
            else if (elements[m] > number)
                r = m;
            else {
                r = m;
                break;
            }
        }
        
        int distance_l = abs(elements[l] - number);
        int distance_r = abs(elements[r] - number);
        
        if (distance_r == 0)
            writeInt(elements[r]);
        else if (distance_l == distance_r)
            writeInt(elements[l]);
        else if (distance_l < distance_r)
            writeInt(elements[l]);
        else
            writeInt(elements[r]);
            
        putchar_unlocked('\n');
    }
}