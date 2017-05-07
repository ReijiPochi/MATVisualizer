using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestApp
{
    public class NeuronInputPort
    {
        public NeuronInputPort()
        {
            allPorts.Add(this);
            Wait = rand.NextDouble();
        }

        public static void ClearAllValue()
        {
            foreach(NeuronInputPort port in allPorts)
            {
                port.ClearValue();
            }
        }

        private static Random rand = new Random();
        private static List<NeuronInputPort> allPorts = new List<NeuronInputPort>();

        public double Wait { get; set; }
        public double Value { get; private set; }

        public void Input(double value)
        {
            Value += Wait * value;
        }

        public void ClearValue()
        {
            Value = 0;
        }
    }
}
