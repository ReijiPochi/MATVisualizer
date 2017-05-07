using MATVisualizer.Graphics;
using MATVisualizer.Graphics.Core;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Numerics;
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
using System.Windows.Threading;

namespace MATVisualizer
{
    public class ViewControl : Control
    {
        static ViewControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ViewControl), new FrameworkPropertyMetadata(typeof(ViewControl)));
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (DesignerProperties.GetIsInDesignMode(this))
                return;

            viewWindow = new Window()
            {
                Background = new SolidColorBrush(Color.FromRgb(20, 20, 20)),
                ResizeMode = ResizeMode.NoResize,
                ShowInTaskbar = false,
                Topmost = true,
                WindowStyle = WindowStyle.None
            };

            viewWindow.MouseWheel += ViewWindow_MouseWheel;
            viewWindow.MouseMove += ViewWindow_MouseMove;
            viewWindow.Activated += ViewWindow_Activated;

            viewWindow.Show();

            InitializeVisualizer();

            SetSize();
            SetPosition();

            ownerWindow = Window.GetWindow(this);
            if(ownerWindow != null)
            {
                ownerWindow.LocationChanged += Owner_LocationChanged;
                ownerWindow.StateChanged += Owner_StateChanged;
                ownerWindow.Activated += Owner_Activated;
                ownerWindow.Deactivated += Owner_Deactivated;
                ownerWindow.Closed += Owner_Closed;
            }

            SizeChanged += ViewControl_SizeChanged;
        }

        private Window viewWindow;
        private Window ownerWindow;
        private CameraPerspective camera;
        private Point previousMousePosition;

        private void ViewWindow_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            if (camera != null)
            {
                camera.Zoom(e.Delta / 2000.0);
                camera.UpdateCamera();
            }
        }

        private void ViewWindow_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.RightButton == MouseButtonState.Pressed)
            {
                Vector delta = e.GetPosition(this) - previousMousePosition;
                camera.Rotate(delta.X * 3.14 / viewWindow.ActualWidth, delta.Y * 3.14 / viewWindow.ActualHeight);
                camera.UpdateCamera();
            }

            if(e.MiddleButton== MouseButtonState.Pressed)
            {
                Vector delta = e.GetPosition(this) - previousMousePosition;
                camera.Move(delta.X * 3.14 / viewWindow.ActualWidth, delta.Y * 3.14 / viewWindow.ActualHeight);
                camera.UpdateCamera();
            }

            previousMousePosition = e.GetPosition(this);
        }

        private void ViewWindow_Activated(object sender, EventArgs e)
        {
            ownerWindow?.Activate();
        }

        private void Owner_Activated(object sender, EventArgs e)
        {
            viewWindow.Topmost = true;
        }

        private void Owner_Deactivated(object sender, EventArgs e)
        {
            viewWindow.Topmost = false;
        }

        private void Owner_LocationChanged(object sender, EventArgs e)
        {
            SetPosition();
        }

        private void Owner_StateChanged(object sender, EventArgs e)
        {
            SetSize();
        }

        private void ViewControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            SetSize();
        }

        private void Owner_Closed(object sender, EventArgs e)
        {
            Render.Render_Finalize();
        }

        private void InitializeVisualizer()
        {
            if (viewWindow == null) return;

            Render.Initialize(viewWindow);


            camera = new CameraPerspective()
            {
                Eye = new Vector3(0.5f, 0.5f, 0.5f),
                Target = Vector3.Zero,
                FieldOfView = 80.0
            };
            camera.UpdateCamera();

            Render.SetCamera(camera);

            Graphics.Grid grid = new Graphics.Grid(15, 0.1f);
            Render.AddObject(grid);
        }

        private void SetPosition()
        {
            if (viewWindow != null)
            {
                Point position = PointToScreen(new Point(0, 0));

                viewWindow.Left = position.X;
                viewWindow.Top = position.Y;
            }
        }

        private void SetSize()
        {
            if (viewWindow != null)
            {
                viewWindow.Width = ActualWidth;
                viewWindow.Height = ActualHeight;

                Render.Resize(viewWindow);
            }
        }
    }
}
