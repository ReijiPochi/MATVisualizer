using MATVisualizer.Graphics;
using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.Linq;
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
            GraphicsCore.GraphicsCore_Finalize();
            graphicsThread?.Join();
        }

        Thread graphicsThread;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Window graphicsWindow = new Window();
            graphicsWindow.Show();

            HwndSource source = (HwndSource)HwndSource.FromVisual(graphicsWindow);

            graphicsThread = new Thread(Initialize);
            graphicsThread.Start(source.Handle);

            GraphicsObjectDescription desc = new GraphicsObjectDescription()
            {
                vertexType = VertexType.ShapeAndValue,
                primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY.TRIANGLELIST
            };

            Thread.Sleep(1000);

            //Object3D obj1 = new Object3D(desc);

            //MATVisualizer.Data.UDCLoader.Load(@"AVS1.inp");
        }

        private static void Initialize(object handle)
        {
            GraphicsCore.Initialize((IntPtr)handle);
        }
    }
}
