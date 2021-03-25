using NeuroLibNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    class Program
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class FindData
        {
            public int fileAttributes = 0;
            // creationTime was an embedded FILETIME structure.
            public int creationTime_lowDateTime = 0;
            public int creationTime_highDateTime = 0;
            // lastAccessTime was an embedded FILETIME structure.
            public int lastAccessTime_lowDateTime = 0;
            public int lastAccessTime_highDateTime = 0;
            // lastWriteTime was an embedded FILETIME structure.
            public int lastWriteTime_lowDateTime = 0;
            public int lastWriteTime_highDateTime = 0;
            public int nFileSizeHigh = 0;
            public int nFileSizeLow = 0;
            public int dwReserved0 = 0;
            public int dwReserved1 = 0;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string fileName = null;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 14)]
            public string alternateFileName = null;
        }
         
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class CTest
        {
            int i = 10;
            int j = 20;
        }

        [DllImport(@"c:\all\Work\neuro\neuro\out\NeuroLib.dll", EntryPoint = "createNeuro")]
        static extern IntPtr createNeuro(fMathFunc a, fMathFunc b, double d);

        [DllImport("Kernel32.dll", CharSet = CharSet.Auto)]
        internal static extern IntPtr FindFirstFile(
        string fileName, [In, Out] FindData findFileData);

        delegate double fMathFunc(double x);

        static double foo(double x)
        {
            return x;
        }

        static void Main(string[] args)
        {
            Neuro neuro= new Neuro(Neuro.Sigmoid, Neuro.SigmoidDx, 0.2);
            neuro.addLayerNeuro(12);

            neuro.addLayerNeuro(6);
            neuro.randomWeightNeuro(0.2,0.8);

            double[] input = new double[12];
            double[] output = new double[6];
            neuro.calcNeuro(input, 12,output, (uint)output.Length);
        }
    }
}
