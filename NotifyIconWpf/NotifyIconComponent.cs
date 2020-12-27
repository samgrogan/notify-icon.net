using System;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;
using NotifyIcon.Win32;
using System.Diagnostics;
using System.Drawing;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable, INotifyPropertyChanged
    {
        private const string CategoryName = "NotifyIconWpf";

        #region Members

        // The win32 Notification Icon that this class wraps
        private NotificationAreaIcon _notifyIcon;

        // The GUID to uniquely identify this component
        private Guid _itemGuid;

        // The icon to display
        private Icon _icon;

        #endregion Members

        #region Properties

        // Icon property
        // An image source that provides the icon to display in the notification area
        public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
            nameof(Icon),
            typeof(ImageSource),
            typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(null, IconPropertyChanged)
        );

        public event PropertyChangedEventHandler PropertyChanged;

        [Category(CategoryName)]
        [Description("Sets the source of the notification tray icon.")]
        public ImageSource Icon
        {
            get => GetValue(IconProperty) as ImageSource;
            set
            {
                SetValue(IconProperty, value);
                _icon = value?.ToIcon();
                if (_notifyIcon != null)
                {
                    _notifyIcon.Icon = _icon?.Handle;
                }
            }
        }

        #endregion Properties

        #region Public Methods

        public NotifyIconComponent()
        {
            PropertyChanged = PropertyChangedEventHandler;
        }

        public void Dispose()
        {
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

        public static void PropertyChangedEventHandler(object sender, PropertyChangedEventArgs e)
        {
            Debug.Write(e.PropertyName);
        }

        public void OnNotificationIconEvent(object sender, NotifyIconEventArgs e)
        {
            Debug.Write(e.Type);
        }

        #endregion Internal Methods
    }
}
