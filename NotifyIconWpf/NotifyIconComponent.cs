using System;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;
using System.Drawing;
using NotifyIcon.Win32;
using System.Windows.Controls.Primitives;
using System.Windows.Interop;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable, INotifyPropertyChanged
    {
        #region Members

        // The win32 Notification Icon that this class wraps
        private NotificationAreaIcon _notifyIcon;

        // The GUID to uniquely identify this component
        private Guid _itemGuid;

        // The icon to display
        private Icon _icon;

        #endregion Members

        #region Public Methods

        public NotifyIconComponent()
        {
            PropertyChanged = PropertyChangedEventHandler;
        }

        // Called when a window even occurs in the notification area icon
        public void OnNotificationIconEvent(object sender, NotifyIconEventArgs e)
        {
            // Convert from the Win32 event type to the wpf event type
            if (e != null)
            {
                Wpf.EventType eventType = (Wpf.EventType)e.Type;

                // Forward the event to any commands
                ForwardEventToCommand(eventType);

                // Handle the content menu
                HandleCommandMenu(eventType, e.CursorX, e.CursorY);
            }
        }

        public void Dispose()
        {
            if (_notifyIcon != null)
            {
                _notifyIcon.Dispose();
                _notifyIcon = null;
            }
        }

        #endregion Public Methods

        #region Internal Methods

        protected override void OnInitialized(EventArgs e)
        {
            base.OnInitialized(e);

            // Try to convert the Uid to a GUID, or create a new guid
            if (!Guid.TryParse(this.Uid, out _itemGuid))
            {
                _itemGuid = Guid.NewGuid();
            }

            // Create the Win32 Notification Area Icon
            _notifyIcon = new NotificationAreaIcon(_itemGuid)
            {
                ToolTip = this.ToolTip?.ToString(),
                Icon = this._icon?.Handle
            };
            _notifyIcon.NotificationIconEventHandler += OnNotificationIconEvent;

            // Show the icon
            _notifyIcon.ShowIcon();
        }

        private static void IconPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NotifyIconComponent source = d as NotifyIconComponent;
            source?.OnIconChanged(d, e);
        }

        private void OnIconChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Icon = e.NewValue as ImageSource;
        }

        protected static void PropertyChangedEventHandler(object sender, PropertyChangedEventArgs e)
        {
        }


        // Forward events to any registed commands
        protected void ForwardEventToCommand(Wpf.EventType eventType)
        {
            switch (eventType)
            {
                case Wpf.EventType.LeftButtonDoubleClick:
                case Wpf.EventType.MiddleButtonDoubleClick:
                case Wpf.EventType.RightButtonDoubleClick:
                    // Handle double click
                    if (DoubleClickCommand?.CanExecute(DoubleClickCommandParameter) == true)
                    {
                        DoubleClickCommand?.Execute(DoubleClickCommandParameter);
                    }
                    break;
            }
        }

        // Handle the display of the command menu, based on the event type
        protected void HandleCommandMenu(Wpf.EventType eventType, int cursorX, int cursorY)
        {
            if (ContextMenu != null)
            {
                // Should we 
                if (ShouldShowCommandMenu(eventType))
                {
                    // Set the position of the menu
                    ContextMenu.Placement = PlacementMode.AbsolutePoint;
                    ContextMenu.HorizontalOffset = cursorX;
                    ContextMenu.VerticalOffset = cursorY;
                    ContextMenu.IsOpen = true;

                    // Show the window
                    HwndSource hwndSource = PresentationSource.FromVisual(ContextMenu) as HwndSource;
                    if (hwndSource != null)
                    {
                        _notifyIcon.SetForegroundWindow(hwndSource.Handle);
                    }

                }
            }
        }

        // Should the command menu be shown based on the event type?
        protected bool ShouldShowCommandMenu(Wpf.EventType eventType)
        {
            // Check for single event types
            if (MenuActivation == eventType)
            {
                return true;
            }

            // Check for compound types
            switch (MenuActivation)
            {
                case Wpf.EventType.Any:
                    return true;
                case Wpf.EventType.DoubleClick:
                    if (eventType == Wpf.EventType.LeftButtonDoubleClick ||
                        eventType == Wpf.EventType.MiddleButtonDoubleClick ||
                        eventType == Wpf.EventType.RightButtonDoubleClick)
                    {
                        return true;
                    }
                    break;
                case Wpf.EventType.SingleClick:
                    if (eventType == Wpf.EventType.LeftButtonSingleClick ||
                        eventType == Wpf.EventType.MiddleButtonSingleClick ||
                        eventType == Wpf.EventType.RightButtonSingleClick)
                    {
                        return true;
                    }
                    break;
            }

            return false;
        }

        #endregion Internal Methods
    }
}
