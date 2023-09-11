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

void printString(char * str)
{
    while (*str != '\0') {
        putchar_unlocked(*str);
        ++str;
    }
}

int main()
{
    int n = readInt();
    int k = readInt();
    
    std::vector<int> elements;
    elements.emplace_back(readInt());
    for (int i = 1; i < n; ++i) {
        int element = readInt();
        
        if (elements.back() == element)
            continue;
            
        elements.emplace_back(element);
    }
    
    int min = elements.front();
    int max = elements.back();
    for (int i = 0; i < k; ++i) {
        int number = readInt();
        
        if (number < min) {
            printString("NO\n");
            continue;
        }
        
        if (number > max) {
            printString("NO\n");
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
        
        if (elements[r] == number)
            printString("YES\n");
        else
            printString("NO\n");
    }
}