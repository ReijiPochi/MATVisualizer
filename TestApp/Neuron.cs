using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    public delegate double OutputFunction(double u);

    public class Neuron
    {
        public Neuron()
        {
            Threshold = rand.NextDouble();
            F = SigmoidFunction;
        }

        public Neuron(NeuronInputPort input)
        {
            Threshold = rand.NextDouble();
            F = SigmoidFunction;
            Input.Add(input);
        }

        public List<NeuronInputPort> Input { get; private set; } = new List<NeuronInputPort>();
        public List<NeuronInputPort> OutputTo { get; private set; } = new List<NeuronInputPort>();
        public double OutputValue { get; set; }
        public OutputFunction F { get; set; }
        public double Threshold { get; set; }
        public bool IsConstant { get; set; }
        public static double alpha = 0.5;
        public static double beta = 0.5;

        private static Random rand = new Random();

        public double SigmoidFunction(double u)
        {
            return 1.0 / (1.0 + Math.Exp(-u));
        }

        public void DoWork()
        {
            if (IsConstant)
            {
                foreach (NeuronInputPort port in OutputTo)
                {
                    port.Input(OutputValue);
                }
            }
            else
            {
                double u = 0;

                foreach (NeuronInputPort port in Input)
                {
                    u += port.Value;
                }

                u -= Threshold;

                double y = F(u);
                OutputValue = y;

                foreach (NeuronInputPort port in OutputTo)
                {
                    port.Input(y);
                }
            }
        }


        public void BP_v(double error)
        {
            foreach(NeuronInputPort port in Input)
            {
                port.Wait += alpha * error * (1.0 - OutputValue) * OutputValue * port.Value;
            }
        }

        public void BP_w(double error, Neuron y)
        {
            foreach(NeuronInputPort port in Input)
            {
                port.Wait += beta * error * (1.0 - y.OutputValue) * y.OutputValue * Neuron.GetWait(this, y) * (1 - OutputValue) * OutputValue * port.Value;
            }
        }

        public void Connect(params Neuron[] n2)
        {
            foreach(Neuron n in n2)
            {
                NeuronInputPort port = new NeuronInputPort();
                OutputTo.Add(port);
                n.Input.Add(port);
            }
        }

        public static double GetWait(Neuron n1, Neuron n2)
        {
            foreach(NeuronInputPort port in n1.OutputTo)
            {
                foreach(NeuronInputPort target in n2.Input)
                {
                    if(port == target)
                    {
                        return target.Wait;
                    }
                }
            }

            return 0;
        }
    }
}
