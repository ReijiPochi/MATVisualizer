using MATVisualizer.Data;
using MATVisualizer.Graphics;
using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace TestApp
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Closed += MainWindow_Closed;
            Application.Current.Exit += Current_Exit;
        }

        private void MainWindow_Closed(object sender, EventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Current_Exit(object sender, ExitEventArgs e)
        {
            
        }

        UDCObject udcObj;
        Vector3 pos = new Vector3(0.05f, 0.05f, 0.05f);

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            UDC udc = UDCLoader.Load(@"AVS1.inp");
            //Object3D udcObj = udc.ToSolidObject();
            //Render.AddObject(udcObj);

            udcObj = new UDCObject(udc);
            udcObj.Slice(pos, new Vector3(1f, 1f, 1f));

            //NeuronInputPort u1 = new NeuronInputPort() { Wait = 1.0 };
            //NeuronInputPort u2 = new NeuronInputPort() { Wait = 1.0 };

            //Neuron Nu1 = new Neuron(u1);
            //Neuron Nu2 = new Neuron(u2);
            //Neuron Const1 = new Neuron() { IsConstant = true, OutputValue = 1.0 };

            //Neuron X1 = new Neuron();
            //Neuron X2 = new Neuron();
            //Neuron Const2 = new Neuron() { IsConstant = true, OutputValue = 1.0 };

            //Neuron Y1 = new Neuron();

            //Nu1.Connect(X1, X2);
            //Nu2.Connect(X1, X2);
            //Const1.Connect(X1, X2);

            //X1.Connect(Y1);
            //X2.Connect(Y1);
            //Const2.Connect(Y1);

            //double[] error = new double[4];
            //int errorCount = 1;

            //for(int study = 0; study < 100000; study++)
            //{
            //    double input1 = Math.Floor((study % 4) / 2.0);
            //    double input2 = study % 2;
            //    double t = Math.Abs(input1 - input2);

            //    NeuronInputPort.ClearAllValue();

            //    u1.Input(input1);
            //    u2.Input(input2);

            //    X1.DoWork();
            //    X2.DoWork();

            //    Y1.DoWork();

            //    error[0] = Y1.OutputValue - t;

            //    double E = 0;

            //    foreach(double ex in error)
            //    {
            //        E += ex;
            //    }

            //    E /= error.Length;

            //    if (errorCount == 4)
            //    {
            //        foreach (double ex in error)
            //        {
            //            E = 0;
            //        }
            //        errorCount = 1;
            //    }

            //    E = Y1.OutputValue - t;


            //    Y1.BP_v(E);
            //    Y1.Threshold = Neuron.GetWait(Const2, Y1);

            //    X1.BP_w(E, Y1);
            //    X1.Threshold = Neuron.GetWait(Const1, X1);
            //    X2.BP_w(E, Y1);
            //    X2.Threshold = Neuron.GetWait(Const1, X2);


            //    for(int i = 3; i > 0; i--)
            //    {
            //        error[i] = error[i - 1];
            //    }
            //}
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (udcObj != null)
            {
                if (e.Key == Key.Up)
                    pos += Vector3.One * 0.001f;
                else if (e.Key == Key.Down)
                    pos -= Vector3.One * 0.001f;

                udcObj.Slice(pos, new Vector3(1f, 1f, 1f));
            }
        }
    }
}
