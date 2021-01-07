using System;
using System.Drawing;
using System.Windows.Media;
using System.Windows;
using System.IO;

namespace NotifyIcon.Wpf
{
    internal static class Extensions
    {
        // Convert an ImageSource to an Icon
        public static Icon ToIcon(this ImageSource imageSource)
        {
            try {
                string imageSourcePath = imageSource?.ToString();
                if (imageSourcePath != null) {
                    Uri imageSourceUri = new Uri(imageSourcePath);
                    Stream iconStream = Application.GetResourceStream(imageSourceUri)?.Stream;
                    if (iconStream != null) {
                        return new Icon(iconStream);
                    }
                }
            }
            catch
            {
            }
            return null;
        }
    }
}
