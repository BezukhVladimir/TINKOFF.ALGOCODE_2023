using System;
using System.Collections.Generic;

class Task
{
    private int N, M, K;
    private List<int>    A = new List<int>();
    private List<string> B = new List<string>();

    private List<char> _bestAnswer   = new List<char>();
    private long       _minimumPrize = long.MaxValue;
    
    public void Initialization() {
        string[] NMK = Console.ReadLine().Split();
        N = Convert.ToInt32(NMK[0]);
        M = Convert.ToInt32(NMK[1]);
        K = Convert.ToInt32(NMK[2]);

        string[] prizes = Console.ReadLine().Split();
        for (int i = 0; i < M; ++i) {
            int prize = Convert.ToInt32(prizes[i]);
            A.Add(prize);
        }

        for (int i = M - 1; i > 0; --i) {
            A[i] -= A[i - 1];
        }

        for (int i = 0; i < N; ++i) {
            string bet = Console.ReadLine();
            B.Add(bet);
        }

        for (int i = 0; i < M; ++i) {
            _bestAnswer.Add(' ');        
        }
    }

    public void Solve() {
        List<char> answer = new List<char>();
        for (int i = 0; i < M; ++i) {
            answer.Add(' ');
        }

        Find(0, 0, N, 0, answer);
    }

    public void PrintResult() {
        foreach (var digit in _bestAnswer) {
            Console.Write(digit);
        }

        Console.WriteLine();
        Console.Write(_minimumPrize);
    }

    private void Find(int i, int start, int shift, long prize, List<char> answer) {
        if (i == M) {
            Update(prize, answer);
            return;
        }

        int digit;
        int tempStart = start;
        int tempShift = start;
        for (digit = 0; digit < K; ++digit) {
            while (tempShift < shift && B[tempShift][i] == Normalize(digit)) {
                tempShift++;
            }

            answer[i] = Normalize(digit);
            Find(i + 1, tempStart, tempShift, prize + (long) A[i] * (tempShift - tempStart), answer);
            if (tempShift == tempStart) {
                Update(prize, answer);
                return;
            }

            tempStart = tempShift;
        }
    }

    private void Update(long prize, List<char> answer) {
        if (_minimumPrize > prize) {
            for (int d = 0; d < M; ++d) {
                _bestAnswer[d] = answer[d];
            }

            _minimumPrize = prize;
        }
    }

    private static char Normalize(int digit) {
        return (char)(digit + '0');
    } 
}

class Program {
    static void Main() {
        Task C11 = new Task();
        C11.Initialization();
        C11.Solve();
        C11.PrintResult();
    }
}