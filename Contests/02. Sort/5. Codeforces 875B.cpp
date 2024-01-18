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

int main()
{
    int n = readInt();
    
    int bits[n];
    for (int i = 0; i < n; ++i) {
        bits[i] = 0;
    }
    
    int rightmost_zero_index = n - 1; 
            
    putchar_unlocked('1');
    putchar_unlocked(' ');
    for (int ones_number = 1;
         ones_number < n;
         ++ones_number) {
        bits[readInt() - 1] = 1;
        
        while (bits[rightmost_zero_index]) {
            --rightmost_zero_index;
        }
        
        int swaps_counter
            = ones_number
              - (n - rightmost_zero_index - 2);
              
        writeInt(swaps_counter);
        putchar_unlocked(' ');
    }
    
    putchar_unlocked('1');
}