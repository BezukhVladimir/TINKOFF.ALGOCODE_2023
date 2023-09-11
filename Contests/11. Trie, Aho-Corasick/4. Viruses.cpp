#include <vector>
#include <string>
#include <iostream>

class DictionaryMatching {
private:
    static const std::vector<char> alphabet;
    static const char minimumCharOfAlphabet;

    struct TrieNode {
        std::vector<int> childIndices = std::vector<int>(alphabet.size(), -1);
        bool isTerminal           = false;
        int  patternIndex         = -1;
        int  normalizedParentChar = -1;
        int  parentIndex          = -1;
        int  suffLinkIndex        = -1;
        int  goodSuffLinkIndex    = -1;
        int  closestTerminalIndex = -1;
        int  color                = -1;
        std::vector<int> transitionIndices = std::vector<int>(alphabet.size(), -1);

        TrieNode(int p = -1, int c = -1)
            : parentIndex(p), normalizedParentChar(c) {}
    };

    std::vector<TrieNode>    _trie = { TrieNode() };
    std::vector<std::string> _patterns;

    static int NormalizeChar(char c) {
        return c - minimumCharOfAlphabet;
    }

public:
    DictionaryMatching() {}
    void Add(std::string pattern);
    int  GetSufflink(int nodeIndex);
    int  GetGoodSufflink(int nodeIndex);
    int  GetTransition(int nodeIndex, int normalizedChar);
    int  GetTerminal(int nodeIndex);
    bool IsLoop(int nodeIndex);
};

const std::vector<char> DictionaryMatching::alphabet = {'0', '1'};
const char DictionaryMatching::minimumCharOfAlphabet = '0';

void DictionaryMatching::Add(std::string pattern) {
    int currentNodeIndex = 0;
    for (char c: pattern) {
        int normalizedChar = NormalizeChar(c);

        if (_trie[currentNodeIndex].childIndices[normalizedChar] == -1) {
            _trie.emplace_back(currentNodeIndex, normalizedChar);
            _trie[currentNodeIndex].childIndices[normalizedChar] = _trie.size() - 1;
        }

        currentNodeIndex = _trie[currentNodeIndex].childIndices[normalizedChar];
    }

    _trie[currentNodeIndex].isTerminal = true;
    _patterns.emplace_back(pattern);
    _trie[currentNodeIndex].patternIndex = _patterns.size() - 1;
}

int DictionaryMatching::GetSufflink(int nodeIndex) {
    if (_trie[nodeIndex].suffLinkIndex == -1) {
        if (nodeIndex == 0 || _trie[nodeIndex].parentIndex == 0) {
            _trie[nodeIndex].suffLinkIndex = 0;
        } else {
            _trie[nodeIndex].suffLinkIndex
                = GetTransition(GetSufflink(_trie[nodeIndex].parentIndex), _trie[nodeIndex].normalizedParentChar);
        }
    }

    return _trie[nodeIndex].suffLinkIndex;
}

int DictionaryMatching::GetGoodSufflink(int nodeIndex) {
    if (_trie[nodeIndex].goodSuffLinkIndex == -1) {
        int sufflink = GetSufflink(nodeIndex);

        if (sufflink == 0) {
            _trie[nodeIndex].goodSuffLinkIndex = sufflink;
        } else {
            _trie[nodeIndex].goodSuffLinkIndex = _trie[sufflink].isTerminal ? sufflink : GetGoodSufflink(sufflink);
        }
    }

    return _trie[nodeIndex].goodSuffLinkIndex;
}

int DictionaryMatching::GetTransition(int nodeIndex, int normalizedChar) {
    if (_trie[nodeIndex].transitionIndices[normalizedChar] == -1) {
        if (_trie[nodeIndex].childIndices[normalizedChar] != -1) {
            _trie[nodeIndex].transitionIndices[normalizedChar] = _trie[nodeIndex].childIndices[normalizedChar];
        } else {
            _trie[nodeIndex].transitionIndices[normalizedChar] =
                    (nodeIndex == 0) ? 0 : GetTransition(GetSufflink(nodeIndex), normalizedChar);
        }
    }

    return _trie[nodeIndex].transitionIndices[normalizedChar];
}

int DictionaryMatching::GetTerminal(int nodeIndex) {
    if (_trie[nodeIndex].closestTerminalIndex == -1) {
        int currentIndex = GetGoodSufflink(nodeIndex);

        while (_trie[currentIndex].parentIndex != -1 && !_trie[currentIndex].isTerminal) {
            currentIndex = GetGoodSufflink(currentIndex);
        }

        _trie[nodeIndex].closestTerminalIndex = currentIndex;
    }

    return _trie[nodeIndex].closestTerminalIndex;
}

bool DictionaryMatching::IsLoop(int nodeIndex) {
    bool result;

    if (_trie[nodeIndex].color == 0) {
        _trie[nodeIndex].color = -1;
        return true;
    }

    if (_trie[nodeIndex].color == 1
        || _trie[nodeIndex].isTerminal
        || _trie[GetTerminal(nodeIndex)].parentIndex != -1) {
        return false;
    }

    _trie[nodeIndex].color = 0;

    result = IsLoop(GetTransition(nodeIndex, NormalizeChar('0')))
             || IsLoop(GetTransition(nodeIndex, NormalizeChar('1')));

    _trie[nodeIndex].color = 1;

    return result;
}

class Task {
private:
    std::string              _text;
    std::vector<std::string> _patterns;
    DictionaryMatching       _finiteStateMachine;
    bool                     _isTAK = false;

public:
    void Initialization() {
        int patternNumber;
        std::cin >> patternNumber;

        for (int i = 0; i < patternNumber; ++i) {
            std::string pattern;
            std::cin >> pattern;
            _patterns.push_back(pattern);
            _finiteStateMachine.Add(pattern);
        }
    }

    void Solve() {
        _isTAK = _finiteStateMachine.IsLoop(0);
    }

    void PrintResult() {
        if (_isTAK) {
            std::cout << "TAK";
        } else {
            std::cout << "NIE";
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    Task D11;

    D11.Initialization();
    D11.Solve();
    D11.PrintResult();
}