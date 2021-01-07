using System;
using System.Windows;
using System.Windows.Media;
using System.Drawing;
using NotifyIcon.Win32;
using System.Windows.Controls.Primitives;
using System.Windows.Interop;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable
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
            // Register a listener for the tool tip property
            ToolTipProperty.OverrideMetadata(typeof(NotifyIconComponent), new FrameworkPropertyMetadata(ToolTipPropertyChanged));

            // Register a listener for the data context property
            DataContextProperty.OverrideMetadata(typeof(NotifyIconComponent), new FrameworkPropertyMetadata(DataContextPropertyChanged));
        }

        // Called when a window even occurs in the notification area icon
        public void OnNotificationIconEvent(object sender, NotifyIconEventArgs e)
        {
            // Convert from the Win32 event type to the wpf event type
            if (e != null)
            {
                EventType eventType = (EventType)e.Type;

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

        // Make the notification icon visible
        public void ShowIcon()
        {
            // Create the Win32 notify icon, if needed
            if (_notifyIcon == null)
            {
                // Try to convert the Uid to a GUID, or create a new guid
                if (!Guid.TryParse(this.IconGuid, out _itemGuid))
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
            }

            // Show the icon
            _notifyIcon.ShowIcon();
        }

        // Hide the notification icon
        public void HideIcon() {
            _notifyIcon?.HideIcon();
        }


        #endregion Public Methods

        #region Internal Methods

        private static void IconPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NotifyIconComponent source = d as NotifyIconComponent;
            source?.OnIconChanged(e);
        }

        private void OnIconChanged(DependencyPropertyChangedEventArgs e)
        {
            Icon = e.NewValue as ImageSource;
        }

        private static void IconGuidPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NotifyIconComponent source = d as NotifyIconComponent;
            source?.OnIconGuidChanged(e);
        }

        private void OnIconGuidChanged(DependencyPropertyChangedEventArgs e) {
            this.IconGuid = e.NewValue?.ToString();
        }

        // Forward events to any registered commands
        protected void ForwardEventToCommand(EventType eventType)
        {
            switch (eventType)
            {
                case EventType.LeftButtonDoubleClick:
                case EventType.MiddleButtonDoubleClick:
                case EventType.RightButtonDoubleClick:
                    // Handle double click
                    if (DoubleClickCommand?.CanExecute(DoubleClickCommandParameter) == true)
                    {
                        DoubleClickCommand?.Execute(DoubleClickCommandParameter);
                    }
                    break;
            }
        }

        // Handle the display of the command menu, based on the event type
        protected void HandleCommandMenu(EventType eventType, int cursorX, int cursorY)
        {
            if (ContextMenu != null)
            {
                // Should we 
                if (ShouldShowCommandMenu(eventType))
                {
                    // Set the position of the menu
                    if (ContextMenu != null) {
                        ContextMenu.Placement = PlacementMode.AbsolutePoint;
                        ContextMenu.HorizontalOffset = cursorX;
                        ContextMenu.VerticalOffset = cursorY;
                        ContextMenu.IsOpen = true;

                        // Show the window
                        if (PresentationSource.FromVisual(ContextMenu) is HwndSource hwndSource) {
                            NotificationAreaIcon.SetForegroundWindow(hwndSource.Handle);
                        }
                    }
                }
            }
        }

        // Should the command menu be shown based on the event type?
        protected bool ShouldShowCommandMenu(EventType eventType)
        {
            // Check for single event types
            if (MenuActivation == eventType)
            {
                return true;
            }

            // Check for compound types
            switch (MenuActivation)
            {
                case EventType.Any:
                    return true;
                case EventType.DoubleClick:
                    if (eventType == EventType.LeftButtonDoubleClick ||
                        eventType == EventType.MiddleButtonDoubleClick ||
                        eventType == EventType.RightButtonDoubleClick)
                    {
                        return true;
                    }
                    break;
                case EventType.SingleClick:
                    if (eventType == EventType.LeftButtonSingleClick ||
                        eventType == EventType.MiddleButtonSingleClick ||
                        eventType == EventType.RightButtonSingleClick)
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
