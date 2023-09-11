using System;
using System.IO;
using System.Collections.Generic;

namespace AhoCorasickAlgorithm
{
    using T_Result = Dictionary<string, List<int>>;

    public class DictionaryMatching
    {
        static readonly char[] alphabet =
        { 'a', 'b', 'c', 'd', 'e',
          'f', 'g', 'h', 'i', 'j',
          'k', 'l', 'm', 'n', 'o',
          'p', 'q', 'r', 's', 't',
          'u', 'v', 'w', 'x', 'y',
          'z' }; // sorted in ascending order
        static readonly char minimumCharOfAlphabet = 'a';
        private static int NormalizeChar(char c) {
            return c - minimumCharOfAlphabet;
        }

        private class TrieNode
        {
            public int[] childIndices         = new int[alphabet.Length];
            public bool  isTerminal           = false;
            public int   patternIndex         = -1;
            public int   normalizedParentChar = -1;
            public int   parentIndex          = -1;
            public int   suffLinkIndex        = -1;
            public int   goodSuffLinkIndex    = -1;
            public int[] transitionIndices    = new int[alphabet.Length];

            public TrieNode(int p = -1, int c = -1) {
                for (int i = 0; i < alphabet.Length; ++i) {
                    childIndices[i] = -1;
                    transitionIndices[i] = -1;
                }

                parentIndex = p;
                normalizedParentChar = c;
            }
        }

        private List<TrieNode> _trie     = new List<TrieNode>();
        private List<string>   _patterns = new List<string>();
        private T_Result       _allMatches;

        public T_Result GetMatches {
            get {
                return _allMatches;
            }
        }

        public DictionaryMatching() {
            _trie.Add(new TrieNode());
        }

        public void Add(string pattern) {
            int currentNodeIndex = 0;

            foreach (char c in pattern) {
                int normalizedChar = NormalizeChar(c);

                if (_trie[currentNodeIndex].childIndices[normalizedChar] == -1) {
                    _trie.Add(new TrieNode(currentNodeIndex, normalizedChar));
                    _trie[currentNodeIndex].childIndices[normalizedChar] = _trie.Count - 1;
                }

                currentNodeIndex = _trie[currentNodeIndex].childIndices[normalizedChar];
            }

            _trie[currentNodeIndex].isTerminal = true;
            _patterns.Add(pattern);
            _trie[currentNodeIndex].patternIndex = _patterns.Count - 1;
        }

        public int GetSufflink(int nodeIndex) {
            if (_trie[nodeIndex].suffLinkIndex == -1) {
                if (nodeIndex == 0 || _trie[nodeIndex].parentIndex == 0) {
                    _trie[nodeIndex].suffLinkIndex = 0;
                } else {
                    _trie[nodeIndex].suffLinkIndex =
                        GetTransition(GetSufflink(_trie[nodeIndex].parentIndex), _trie[nodeIndex].normalizedParentChar);
                }
            }

            return _trie[nodeIndex].suffLinkIndex;
        }

        public int GetGoodSufflink(int nodeIndex) {
            if (_trie[nodeIndex].goodSuffLinkIndex == -1) {
                int sufflink = GetSufflink(nodeIndex);

                if (sufflink == 0) {
                    _trie[nodeIndex].goodSuffLinkIndex = sufflink;
                } else {
                    _trie[nodeIndex].goodSuffLinkIndex =
                        _trie[sufflink].isTerminal ? sufflink : GetGoodSufflink(sufflink);
                }
            }

            return _trie[nodeIndex].goodSuffLinkIndex;
        }

        public int GetTransition(int nodeIndex, int normalizedChar) {
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

        public T_Result FindAll(string text) {
            AllMatchesInitialization();

            int currentState = 0;
            for (int i = 0; i < text.Length; ++i) {
                currentState = GetTransition(currentState, NormalizeChar(text[i]));
                Check(currentState, i);
            }

            return GetMatches;
        }

        private void Check(int state, int index) {
            for (int currentState = state; currentState != 0;) {
                if (_trie[currentState].isTerminal) {
                    string pattern = _patterns[_trie[currentState].patternIndex];
                    AddMatch(pattern, index - pattern.Length + 1);
                }

                currentState = GetGoodSufflink(currentState);
            }
        }

        private void AddMatch(string pattern, int index) {
            _allMatches[pattern].Add(index);
        }

        private void AllMatchesInitialization() {
            _allMatches = new T_Result();
            foreach (var pattern in _patterns) {
                if (!_allMatches.ContainsKey(pattern)) {
                    _allMatches.Add(pattern, new List<int>());
                }
            }
        }
    }
}

class Task
{
    private string _text;
    private List<string> _patterns = new List<string>();
    private AhoCorasickAlgorithm.DictionaryMatching _finiteStateMachine = new AhoCorasickAlgorithm.DictionaryMatching();
    private Dictionary<string, List<int>> _allMatches;

    public void Initialization() {
        using (StreamReader inputik = new StreamReader("inputik.txt")) {
            _text = inputik.ReadLine();

            int patternNumber = Convert.ToInt32(inputik.ReadLine());
            for (int i = 0; i < patternNumber; ++i) {
                string pattern = inputik.ReadLine();
                _patterns.Add(pattern);
                _finiteStateMachine.Add(pattern);
            }
        }
    }

    public void Solve() {
        _allMatches = _finiteStateMachine.FindAll(_text);
    }

    public void PrintResult() {
        using (StreamWriter outputik = new StreamWriter("outputik.txt")) {
            foreach (var pattern in _patterns) {
                outputik.Write(_allMatches[pattern].Count);

                if (_allMatches[pattern].Count > 0) {
                    foreach (int index in _allMatches[pattern]) {
                        outputik.Write($" {index + 1}");
                    }
                }

                outputik.WriteLine();
            }
        }
    }
}

class Program {
    static void Main() {
        Task B11 = new Task();
        B11.Initialization();
        B11.Solve();
        B11.PrintResult();
    }
}