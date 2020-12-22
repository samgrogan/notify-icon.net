using System;
using System.Windows;
using System.Windows.Media;
using NotifyIcon.Win32;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable
    {
        #region Members

        // The win32 Notification Icon that this class wraps
        private NotificationAreaIcon _notifyIcon;

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
              FrameworkPropertyMetadataOptions.AffectsRender,
              new PropertyChangedCallback(OnIconChanged)
          )
        );

        public ImageSource Icon
        {
            get { return (ImageSource)GetValue(IconProperty); }
            set { SetValue(IconProperty, value); }
        }

        #endregion Properties

        #region Public Methods

        public NotifyIconComponent()
        {
            _itemGuid = new Guid(this.Uid);
            _notifyIcon = new NotificationAreaIcon(_itemGuid);
        }

        public void Dispose()
        {
        }

        #endregion Public Methods

        #region Private Methods

        private static void OnIconChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
        }

        private static void OnGuidChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
        }

        #endregion Private Methods
    }
}
