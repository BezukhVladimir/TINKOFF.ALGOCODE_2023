using System;
using System.Linq;
using T_Counters = System.Collections.Generic.SortedDictionary<string, int>;

class Program
{
    static string     leftHandFormula;
    static T_Counters LHF = new T_Counters();

    static int N;

    static bool IsDigit(string formula, int position) {
        return position < formula.Length && char.IsDigit(formula[position]);
    }

    static bool IsElementBegin(ref string formula, ref int position) {
        if (position < formula.Length) {
            char c = formula[position];
            return char.IsUpper(c) || c == '(';
        } else {
            return false;
        }
    }

    static void ReadNumber(ref string formula, ref int position, ref int number) {
        number = number * 10 + Convert.ToInt32(formula[position] - '0');
        ++position;

        if (IsDigit(formula, position)) {
            ReadNumber(ref formula, ref position, ref number);
        }
    }

    static void ReadChemicalElement(ref string formula, ref int position, ref string chemicalElement) {
        chemicalElement += formula[position++];
        if (position < formula.Length && char.IsLower(formula[position])) {
            chemicalElement += formula[position++];
        }
    }

    static void ReadElement(ref string formula, ref int position, ref int number, ref T_Counters subF) {
        if (formula[position] == '(') {
            ++position;
            ReadSequence(ref formula, ref position, ref number, ref subF);
            ++position;
        } else {
            string chemicalElement = "";
            ReadChemicalElement(ref formula, ref position, ref chemicalElement);
            if (subF.ContainsKey(chemicalElement)) {
                subF[chemicalElement] += 1;
            } else {
                subF.Add(chemicalElement, 1);
            }
        }
    }

    static void ReadSequence(ref string formula, ref int position, ref int number, ref T_Counters F) {
        while (IsElementBegin(ref formula, ref position)) {
            T_Counters subF = new T_Counters();
            ReadElement(ref formula, ref position, ref number, ref subF);
            int multiplier = 1;

            if (IsDigit(formula, position)) {
                multiplier = 0;
                ReadNumber(ref formula, ref position, ref multiplier);
            }

            foreach (var counter in subF) {
                if (F.ContainsKey(counter.Key)) {
                    F[counter.Key] += counter.Value * multiplier;
                } else {
                    F.Add(counter.Key, counter.Value * multiplier);
                }
            }
        }
    }

    static void ReadFormula(ref string formula, ref T_Counters F) {
        int position = 0;
        do {
            char c = formula[position];

            int multiplier = 1;
            if (char.IsDigit(c)) {
                multiplier = 0;
                ReadNumber(ref formula, ref position, ref multiplier);
            }

            T_Counters sequence = new T_Counters();
            if (IsElementBegin(ref formula, ref position)) {
                ReadSequence(ref formula, ref position, ref multiplier, ref sequence);
            }

            foreach (var counter in sequence) {
                if (F.ContainsKey(counter.Key)) {
                    F[counter.Key] += counter.Value * multiplier;
                } else {
                    F.Add(counter.Key, counter.Value * multiplier);
                }
            }
         } while (position < formula.Length && formula[position++] == '+');
    }

    static void Main() {
        Initialization();

        for (int i = 0; i < N; ++i) {
            string currentRightHandFormula = Console.ReadLine();
            T_Counters currentRHF = new T_Counters();

            ReadFormula(ref currentRightHandFormula, ref currentRHF);
            PrintCompare(currentRightHandFormula, currentRHF);
        }
    }   

    static void Initialization() {
        leftHandFormula = Console.ReadLine();
        N = Convert.ToInt32(Console.ReadLine());
        ReadFormula(ref leftHandFormula, ref LHF);
    }

    static void PrintCompare(string currentRightHandFormula, T_Counters currentRHF) {
        Console.Write(leftHandFormula);

        if (Enumerable.SequenceEqual(LHF, currentRHF)) {
            Console.Write("==");
        } else {
            Console.Write("!=");
        }

        Console.WriteLine(currentRightHandFormula);
    }
}