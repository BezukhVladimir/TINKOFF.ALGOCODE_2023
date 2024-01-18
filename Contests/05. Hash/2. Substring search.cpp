#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

// good suffix heuristic
std::vector<int> prefix_func(const std::string& text) {
    std::vector<int> prefix(text.length());

    int k = 0;
    prefix[0] = 0;
    for (size_t i = 1; i != text.length(); ++i)
    {
        while (k > 0 && text[k] != text[i])
            k = prefix[k - 1];

        if (text[k] == text[i])
            ++k;

        prefix[i] = k;
    }

    return prefix;
}

std::vector<int> find(std::string& text, std::string& pattern) {
    if (text.length() < pattern.length())
        return std::vector<int>(1, -1);

    typedef std::unordered_map<char, int> TStopTable;
    typedef std::unordered_map<int, int> TSufficsTable;

    TStopTable stop_table;
    TSufficsTable suffics_table;

    // bad character heuristic
    for (int i = 0; i != static_cast<int>(pattern.length()); ++i) {
        stop_table[pattern[i]] = i;
    }

    std::string reverse_pattern(pattern.rbegin(), pattern.rend());
    std::vector<int> prefix = prefix_func(pattern);
    std::vector<int> reverse_prefix = prefix_func(reverse_pattern);
    for (int i = 0; i != pattern.length() + 1; ++i) {
        suffics_table[i] = pattern.length() - prefix.back();
    }

    for (int i = 1; i != pattern.length(); ++i) {
        int j = reverse_prefix[i];
        suffics_table[j] = std::min(suffics_table[j], i - reverse_prefix[i] + 1);
    }

    std::vector<int> shifts;
    for (int shift = 0; shift <= static_cast<int>(text.length()) - static_cast<int>(pattern.length());) {
        unsigned position = pattern.length() - 1;

        while (pattern[position] == text[position + shift]) {
            if (position == 0) {
                shifts.emplace_back(shift);
                break;
            }

            --position;
        }

        if (pattern.length() != 1 && position == pattern.length() - 1) {
            TStopTable::const_iterator stop_symbol = stop_table.find(text[position + shift]);
            int stop_symbol_additional = position - (stop_symbol != stop_table.end() ? stop_symbol->second : -1);
            shift += stop_symbol_additional;
        } else {
            shift += suffics_table[pattern.length() - 1 - position];
        }
    }

    if (!shifts.empty())
        return shifts;

    return std::vector<int>(1, -1);
}

int main()
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    std::string text;
    std::string pattern;

    std::cin >> text >> pattern;

    std::vector<int> shifts = find(text, pattern);
    if (shifts[0] != -1) {
        for (const auto& shift : shifts) {
            std::cout << shift << ' ';
        }
    }
}