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
            _notifyIcon = new NotificationAreaIcon(_itemGuid);
        }

        private static void OnIconChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
        }

        private static void OnGuidChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
        }

        #endregion Private Methods
    }
}
