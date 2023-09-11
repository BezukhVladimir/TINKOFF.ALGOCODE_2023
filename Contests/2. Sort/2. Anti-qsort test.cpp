#include <stdio.h>
#include <algorithm>

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

int main()
{
    int n = readInt();
    int array[n];
    
    for (int i = 0; i < n; ++i) {
        array[i] = i + 1;
    }
    
    for (int i = 2; i < n; ++i) {
        std::swap(array[i], array[i / 2]);
    }
        
    for (int i = 0; i < n; ++i) {
        writeInt(array[i]);
        putchar_unlocked(' ');
    }
}