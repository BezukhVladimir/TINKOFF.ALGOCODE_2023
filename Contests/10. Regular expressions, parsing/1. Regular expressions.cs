using System;
using System.Text;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        while (true) {
            string data = Console.ReadLine();
            
            if (string.IsNullOrEmpty(data))
                break;
            
            string pattern     = @"([a]+[b]{2,}[c]+)";
            string replacement = "QQQ";
    
            data = Regex.Replace(data, pattern, replacement);
            Console.WriteLine(data);
        }
    }
}