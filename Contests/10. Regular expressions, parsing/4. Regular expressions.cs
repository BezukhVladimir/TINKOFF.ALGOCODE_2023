using System;
using System.Text;
using System.Text.RegularExpressions;

class Program {
    static void Main() {
        while (true) {
            string data = Console.ReadLine();
            
            if (string.IsNullOrEmpty(data))
                break;
            
            string pattern = @"\$v_(\w|\{\w+\})\$";
            data = Regex.Replace(data, pattern, match => {
                string value = match.Groups[1].Value;
                if (value[0] == '{') {
                    return "v[" + value.Substring(1, value.Length - 2) + ']';
                } else {
                    return "v[" + value + ']';
                }
            });
            
            Console.WriteLine(data);
        }
    }
}