using System;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;
using NotifyIcon.Win32;
using System.Diagnostics;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable, INotifyPropertyChanged
    {
        #region Members

        // The win32 Notification Icon that this class wraps
        private NotificationAreaIcon _notifyIcon;
        private ImageSource _icon;
        private Guid _itemGuid;

        #endregion Members

        #region Properties

        // Icon property
        // An image source that provides the icon to display in the notification area
        public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
          "Icon",
          typeof(ImageSource),
          typeof(NotifyIconComponent),
          new FrameworkPropertyMetadata(null,
              FrameworkPropertyMetadataOptions.AffectsRender
          )
        );

        public event PropertyChangedEventHandler PropertyChanged;


        public ImageSource Icon
        {
            get { return _icon; }
            set {
                _icon = value;
 //               _icon?.ToIcon();
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
                ToolTip = this.ToolTip?.ToString()
            };
        }

        //private static void IconPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{
        //    NotifyIconComponent source = d as NotifyIconComponent;
        //    source?.OnIconChanged(d, e);
        //}

        //private void OnIconChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{
        //}

        public static void PropertyChangedEventHandler(object sender, PropertyChangedEventArgs e)
        {
            Debug.Write(e.PropertyName);
        }

        #endregion Internal Methods
    }
}
