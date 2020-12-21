using System;
using System.Windows;
using System.Windows.Media;

namespace NotifyIcon.Wpf
{
    public partial class Proxy : FrameworkElement, IDisposable
    {
        #region Properties

        public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
          "AquariumGraphic",
          typeof(ImageSource),
          typeof(Proxy),
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

        public void Dispose()
        {
        }

        #endregion Public Methods

        #region Private Methods

        private static void OnIconChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
        }

        #endregion Private Methods
    }
}
