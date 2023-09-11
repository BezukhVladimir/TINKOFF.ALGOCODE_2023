#include <cmath>
#include <stdio.h>

long int inversions_counter = 0;

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

void merge(int a[], int l, int m, int r)
{
    int iter_l = l;
    int iter_r = m;
    int size = r - l;
    int result[size];
    int current_result = 0;
    
    while (iter_l < m || iter_r < r) {
        if (iter_l == m) {
            result[current_result++] = a[iter_r++];
        }
        else if (iter_r == r) {
            result[current_result++] = a[iter_l++];
        }
        else if (a[iter_l] <= a[iter_r]) {
            result[current_result++] = a[iter_l++];
        } else {
            result[current_result++] = a[iter_r++];
            inversions_counter += m - iter_l;
        }
    }
  
    for (int i = 0; i < size; ++i) {
        a[l + i] = result[i];
    }
}

void mergeSort(int a[], int n) {
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n - i; j += (i << 1)) {
            merge(a, j, j + i, std::min(j + (i << 1), n));
        }
    }
}

int main()
{
    int n = readInt();
    int array[n];
    
    for (int i = 0; i < n; ++i) {
        array[i] = readInt();
    }
    
    mergeSort(array, n);
    
    writelnInt(inversions_counter);
    
    for (int i = 0; i < n; ++i) {
        writeInt(array[i]);
        putchar_unlocked(' ');
    }
}