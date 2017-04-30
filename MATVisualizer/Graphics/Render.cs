using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MATVisualizer.Graphics.Core;
using System.Threading;
using System.Windows;
using System.Windows.Interop;

namespace MATVisualizer.Graphics
{
    public class Render
    {
        public static double ViewPortWidth { get; protected set; }
        public static double ViewPortHeight { get; protected set; }
        public static double ZoomDispX { get; protected set; }
        public static double ZoomDispY { get; protected set; }
        public static Camera ActiveCamera { get; set; }
        private static Thread graphicsThread;

        public static void Initialize(Window viewWindow)
        {
            PresentationSource s = PresentationSource.FromVisual(viewWindow);
            ZoomDispX = s.CompositionTarget.TransformToDevice.M11;
            ZoomDispY = s.CompositionTarget.TransformToDevice.M22;

            HwndSource source = (HwndSource)HwndSource.FromVisual(viewWindow);
            GraphicsCoreDescription desc = new GraphicsCoreDescription();
            desc.handle = source.Handle;
            desc.width = (int)(viewWindow.ActualWidth * ZoomDispX);
            desc.height = (int)(viewWindow.ActualHeight * ZoomDispY);

            graphicsThread = new Thread(GraphicsWork);
            graphicsThread.Start(desc);
        }

        public static void Render_Finalize()
        {
            GraphicsCore.GraphicsCore_Finalize();
            graphicsThread?.Join();
        }

        private static void GraphicsWork(object desc)
        {
            GraphicsCore.Initialize((GraphicsCoreDescription)desc);
        }

        public static void AddObject(Object3D obj)
        {
            GraphicsCore.AddToRenderingList(obj.objectHandle);
        }

        public static void SetCamera(Camera camera)
        {
            ActiveCamera = camera;
            GraphicsCore.SetCamera(camera.cameraHandle);
        }

        public static void Resize(Window viewWindow)
        {
            ViewPortWidth = (int)(viewWindow.ActualWidth * ZoomDispX);
            ViewPortHeight = (int)(viewWindow.ActualHeight * ZoomDispY);

            ActiveCamera.UpdateCamera();

            GraphicsCore.Resize((int)ViewPortWidth, (int)ViewPortHeight);
        }
    }
}
