﻿using System;
using System.Diagnostics;

namespace MaxMix.Framework
{
    internal static class AppLogging
    {
        [Conditional("DEBUG")]
        public static void DebugLog(string area, params string[] msgs)
        {
            Console.WriteLine($"{area}\t{string.Join("\t", msgs)}");
        }

        [Conditional("DEBUG")]
        public static void DebugLogException(string area, Exception e)
        {
            DebugLog(area, e.GetType().Name, e.Message);
        }
    }
}
