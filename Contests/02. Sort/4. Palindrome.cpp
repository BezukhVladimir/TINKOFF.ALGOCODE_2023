#include <stdio.h>

const int ALPHABET_POWER = 26;

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

int main()
{
    int n = readInt();
    
    int chars[ALPHABET_POWER];
    for (int i = 0; i < ALPHABET_POWER; ++i) {
        chars[i] = 0;
    }
    
    for (int i = 0; i < n; ++i) {
        char c = getchar_unlocked();
        ++chars[c - 'A'];
    }
    
    int middle;
    bool is_found = false;
    
    int current = 0;
    for (; current < ALPHABET_POWER; ++current) {
        if (chars[current] != 0) {
            if (chars[current] == 1) {
                is_found = true;
                middle = current++;
                break;
            }
            
            int half = chars[current] / 2;
            for (int i = 0; i < half; ++i) {
                putchar_unlocked('A' + current);
            }
            
            if (chars[current] % 2) {
                is_found = true;
                middle = current++;
                break;
            }
        }
    }
    
    for (; current < ALPHABET_POWER; ++current) {
        if (chars[current] != 0
         && chars[current] != 1) {
            int half = chars[current] / 2;
            for (int i = 0; i < half; ++i) {
                putchar_unlocked('A' + current);
            }
        }
    }
    
    if (is_found) {
        putchar_unlocked('A' + middle);
    }
    
    for (--current; 0 <= current; --current) {
        if (chars[current] != 0
         && chars[current] != 1) {
            int half = chars[current] / 2;
            for (int i = 0; i < half; ++i) {
                putchar_unlocked('A' + current);
            }
        }
    }
}