using System;

namespace LicenseKey
{
    class Program
    {
        static bool CheckKey(string k)
        {
            int[] key = k.Select(c => (int)c).ToArray();
            if (key.Length != 6) { return false; }
            return (key[0] + 8 * key[1] - 9 * key[2] + 0 * key[3] + 0 * key[4] - 5 * key[5] == -626) && (key[0] - 2 * key[1] + 5 * key[2] + 10 * key[3] - 1 * key[4] - 3 * key[5] == 567) && (key[0] + 1 * key[1] - 0 * key[2] + 1 * key[3] + 4 * key[4] + 3 * key[5] == 917) && (key[0] + 6 * key[1] - 5 * key[2] - 5 * key[3] - 7 * key[4] + 10 * key[5] == 591) && (key[0] + 2 * key[1] + 2 * key[2] + 3 * key[3] - 2 * key[4] + 5 * key[5] == 1093) && (key[0] - 10 * key[1] - 6 * key[2] + 0 * key[3] + 5 * key[4] - 9 * key[5] == -1803);
        }
        static void Main(string[] args)
        {
            Console.WriteLine("[*] Hello World!");
            if (args.Length == 0)
            {
                Console.WriteLine("[!] Pass me a license key!");
                return;
            }
            if (CheckKey(args[0]))
            {
                Console.WriteLine("[+] Valid License key!");
                return;
            }
            Console.WriteLine("[!] Invalid License Key!");
        }
    }
}

