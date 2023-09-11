using System;
using System.Text;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        string data = Console.ReadLine();
        
        data = Regex.Replace(data, @"\b(a|A)\b", "");
        data = Regex.Replace(data, @"\b(an|An)\b", "");
        data = Regex.Replace(data, @"\b(the|The)\b", "");
        data = Regex.Replace(data, @"( )\1+", "$1");
        
        data = Regex.Replace(data, @"Ce", "Se");
        data = Regex.Replace(data, @"Ci", "Si");
        data = Regex.Replace(data, @"ce", "se");
        data = Regex.Replace(data, @"ci", "si");
        data = Regex.Replace(data, @"Ck", "K");
        data = Regex.Replace(data, @"ck", "k");
        data = Regex.Replace(data, @"C", "K");
        data = Regex.Replace(data, @"c", "k");
        
        data = Regex.Replace(data, @"Ee", "I");
        data = Regex.Replace(data, @"Oo", "U");
        data = Regex.Replace(data, @"ee", "i");
        data = Regex.Replace(data, @"oo", "u");
        data = Regex.Replace(data, @"([a-z])\1+", "$1", RegexOptions.IgnoreCase);
        
        data = Regex.Replace(data, @"([a-zA-Z])e\b", "$1");
        
        Console.WriteLine(data.Trim());
    }
}