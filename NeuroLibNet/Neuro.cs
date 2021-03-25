using System;
using System.Runtime.InteropServices;

namespace NeuroLibNet
{
    public class Neuro
    {
        private IntPtr neuroPtr;

        private const string dllName = @"NeuroLib.dll";

        public delegate double fMathFunc(double x);

        [DllImport(dllName, EntryPoint = "Sigmoid")]
        public static extern double Sigmoid(
            double x);

        [DllImport(dllName, EntryPoint = "SigmoidDx")]
        public static extern double SigmoidDx(
           double x);

        [DllImport(dllName, EntryPoint = "Liner_01")]
        public static extern double Liner_01(
           double x);

        [DllImport(dllName, EntryPoint = "Liner_01Dx")]
        public static extern double Liner_01Dx(
           double x);

        [DllImport(dllName, EntryPoint = "Bent_identity")]
        public static extern double Bent_identity(
         double x);

        [DllImport(dllName, EntryPoint = "Bent_identityDx")]
        public static extern double Bent_identityDx(
           double x);

        [DllImport(dllName, EntryPoint = "Identical")]
        public static extern double Identical(
        double x);

        [DllImport(dllName, EntryPoint = "IdenticalDx")]
        public static extern double IdenticalDx(
           double x);

        [DllImport(dllName, EntryPoint = "createNeuro")]
        public static extern IntPtr createNeuro_native(
            fMathFunc fActivation,
            fMathFunc fCorrercion,
            double fKf);

        [DllImport(dllName, EntryPoint = "deleteNeuro")]
        public static extern void deleteNeuro_native(
            IntPtr neuroPtr);

        [DllImport(dllName, EntryPoint = "addLayerNeuro")]
        public static extern uint addLayerNeuro_native(
           IntPtr neuroPtr,
           uint count,
           bool bShift,
           fMathFunc fActivation,
           fMathFunc fCorrercion);

        [DllImport(dllName, EntryPoint = "randomWeightNeuro")]
        public static extern bool randomWeightNeuro_native(
           IntPtr neuroPtr,
           double fMin,
           double fMax);

        [DllImport(dllName, EntryPoint = "calcNeuro")]
        public static extern IntPtr calcNeuro_native(
           IntPtr neuroPtr,
           double[] input,
           uint countInput,
           double[] output,
           uint countOutput);

        [DllImport(dllName, EntryPoint = "calcErrorNeuro")]
        public static extern IntPtr calcErrorNeuro_native(
           IntPtr neuroPtr,
           bool bCorrect,
           double[] input,
           uint countInput,
           double[] output,
           uint countOutput);

        [DllImport(dllName, EntryPoint = "correctWeightNeuro")]
        public static extern bool correctWeightNeuro_native(
           IntPtr neuroPtr);

        [DllImport(dllName, EntryPoint = "getErrorNeuro")]
        public static extern double getErrorNeuro_native(
           IntPtr neuroPtr);

        [DllImport(dllName, EntryPoint = "getFullErrorNeuro")]
        public static extern double getFullErrorNeuro_native(
           IntPtr neuroPtr);

        [DllImport("kernel32.dll")]
        static extern uint GetLastError();

        public Neuro(
            fMathFunc fActivation,
            fMathFunc fCorrercion,
            double fKf)
        {
            neuroPtr = createNeuro_native(fActivation, fCorrercion, fKf);

            if(neuroPtr == (IntPtr)0)
                throw new Exception("Last Error = " + GetLastError());
        }

        ~Neuro()
        {
            deleteNeuro_native(neuroPtr);
        }

        public uint addLayerNeuro(
           uint count,
           bool bShift = false,
           fMathFunc fActivation = null,
           fMathFunc fCorrercion = null)
        {
            var res = addLayerNeuro_native(neuroPtr, count, bShift, fActivation, fCorrercion);
            if(res < 0 )
                throw new Exception("Last Error = " + GetLastError());

            return res;
        }

        public void randomWeightNeuro(
           double fMin,
           double fMax)
        {
            if (!randomWeightNeuro_native(neuroPtr, fMin, fMax))
                throw new Exception("Last Error = " + GetLastError());
        }

        public IntPtr calcNeuro(
           double[] input,
           uint countInput,
           double[] ioutput,
           uint countOutput)
        {
            var res = calcNeuro_native(neuroPtr, input, countInput, ioutput, countOutput);

            if (res == (IntPtr)0)
                throw new Exception("Last Error = " + GetLastError());
        
            return res;
        }
        public IntPtr calcErrorNeuro(
           bool bCorrect,
           double[] input,
           uint countInput,
           double[] ioutput,
           uint countOutput)
        {
            var res = calcErrorNeuro_native(neuroPtr, bCorrect, input, countInput, ioutput, countOutput);

            if (res == (IntPtr)0)
                throw new Exception("Last Error = " + GetLastError());

            return res;
        }

        public double getErrorNeuro()
        {
            var res = getErrorNeuro_native(neuroPtr);

            if (res < 0)
                throw new Exception("Last Error = " + GetLastError());

            return res;
        }

        public double getFullErrorNeuro()
        {
            var res = getFullErrorNeuro_native(neuroPtr);

            if (res < 0)
                throw new Exception("Last Error = " + GetLastError());

            return res;
        }
    }
}
