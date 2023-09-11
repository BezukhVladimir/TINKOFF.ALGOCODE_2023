using System;
using System.Text;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        while (true) {
            string data = Console.ReadLine();
            
            if (string.IsNullOrEmpty(data))
                break;
            
            string pattern     = @"\\circle\{\((\d+)\,(\d+)\)";
            string replacement = "\\circle{($2,$1)";
    
            data = Regex.Replace(data, pattern, replacement);
            Console.WriteLine(data);
        }
    }
}