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


        public void BP(double error)
        {
            foreach(NeuronInputPort port in Input)
            {

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
    }
}
