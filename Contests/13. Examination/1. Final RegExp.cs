using System;
using System.Text;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        while (true) {
            string data = Console.ReadLine();
            
            if (string.IsNullOrEmpty(data))
                break;
            
            string pattern     = @"\\texttt\{([a-zA-Z]+)\}";
            string replacement = @"\begin{bfseries}$1\end{bfseries}";
    
            data = Regex.Replace(data, pattern, replacement);
            
            pattern = @"\\texttt\{([0-9]+)\}";
            
            data = Regex.Replace(data, pattern, replacement);
            Console.WriteLine(data);
        }
    }
}