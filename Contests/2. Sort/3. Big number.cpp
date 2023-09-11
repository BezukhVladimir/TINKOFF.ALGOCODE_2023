#include <stdio.h>
#include <string.h>
#include <algorithm>

class ThirdTask
{
private:
    char ** digits_sequences = nullptr;
    size_t sequences_counter = 0;
    
    const size_t MAX_SEQUENCE_LENGTH  = 100;
    const size_t MAX_SEQUENCES_NUMBER = 10000;
    
public:

    ThirdTask() {
        digits_sequences = readStrings(); // sequences_counter
    }
    
    void solve() {
        mergeSort(digits_sequences, sequences_counter);
        writeStrings(digits_sequences);
    }
    
    ~ThirdTask() {
        deleteStrings(digits_sequences);
    }
    
private:
    char * readString()
    {
        char buffer[MAX_SEQUENCE_LENGTH];
        size_t i = 0;
        size_t chars_read = 0;
        
        int c;
        while ((c = getchar_unlocked()) != '\n' && c != EOF) {
            if (i < MAX_SEQUENCE_LENGTH) {
                buffer[i++] = (char) c;
            }
            
            ++chars_read;
        }
        
        // if (chars_read > MAX_SEQUENCE_LENGTH) {
        //     error: truncation occured
        // }
        
        char * string = new char[chars_read + 1];
        for (size_t i = 0; i < chars_read; ++i)
            string[i] = buffer[i];
            
        string[chars_read] = '\0';
        
        return string;
    }
    
    char ** readStrings()
    {
        char * buffer[MAX_SEQUENCES_NUMBER];
        size_t i = 0;
        size_t strings_read = 0;
        
        char * string;
        while (*(string = readString()) != '\0') {
            if (i < MAX_SEQUENCES_NUMBER) {
                buffer[i++] = string;
            }
            
            ++strings_read;
        }
        
        // if (strings_read > MAX_SEQUENCES_NUMBER) {
        //     error: truncation occured
        // }
        
        char ** strings = new char * [strings_read + 1];
        for (size_t i = 0; i < strings_read; ++i)
            strings[i] = buffer[i];
            
        strings[strings_read] = nullptr;
        
        sequences_counter = strings_read;
        return strings;
    }
    
    void writeString(char * string)
    {
        size_t j = 0;
        char c;
        while ((c = string[j++]) != '\0') {
            putchar_unlocked(c);
        }
    }
    
    void writeStrings(char ** strings)
    {
        size_t i = 0;
        char * string;
        while ((string = strings[i++]) != nullptr)
        {
            writeString(string);
            // putchar_unlocked('\n');
        }
    }
    
    void deleteStrings(char ** strings)
    {
        size_t i = 0;
        char * string;
        while ((string = strings[i++]) != nullptr) {
            delete [] string;
        }
        
        delete [] strings;
    }

    int compare(char * lhs, char * rhs)
    {
        int lhs_size = strlen(lhs);
        int rhs_size = strlen(rhs);
        int concat_size = lhs_size + rhs_size;
        
        char * lhs_rhs = new char[concat_size + 1];
        char * rhs_lhs = new char[concat_size + 1];
        
        for (int i = 0; i < lhs_size; ++i) {
            lhs_rhs[i]            = lhs[i];
            rhs_lhs[rhs_size + i] = lhs[i];
        } 
        
        for (int i = 0; i < rhs_size; ++i) {
            lhs_rhs[lhs_size + i] = rhs[i];
            rhs_lhs[i]            = rhs[i];
        }
        
        lhs_rhs[concat_size] = '\0';
        rhs_lhs[concat_size] = '\0';
    
        return strcmp(lhs_rhs, rhs_lhs);
    }
    
    void merge(char ** a, int l, int m, int r)
    {
        int iter_l = l;
        int iter_r = m;
        int size = r - l;
        char * result[size];
        int current_result = 0;
        
        while (iter_l < m && iter_r < r) {
            if (compare(a[iter_l], a[iter_r]) >= 0) {
                result[current_result++] = a[iter_l++];
            } else {
                result[current_result++] = a[iter_r++];
            }
        }
        
        while (iter_l < m) {
            result[current_result++] = a[iter_l++];
        }
      
        while (iter_r < r) {
            result[current_result++] = a[iter_r++];
        }
      
        for (int i = 0; i < size; ++i) {
            a[l + i] = result[i];
        }
    }
    
    void mergeSort(char ** a, int n) {
        for (int i = 1; i < n; i <<= 1) {
            for (int j = 0; j < n - i; j += (i << 1)) {
                merge(a, j, j + i, std::min(j + (i << 1), n));
            }
        }
    }
};

int main()
{
    ThirdTask task;
    task.solve();
}