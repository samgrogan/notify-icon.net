using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using System.Windows;
using System.IO;

namespace NotifyIcon.Wpf
{
    internal static class Extensions
    {
        // Convert an ImageSource to an Icon
        public static Icon ToIcon(this ImageSource imageSource)
        {
            try
            {
                Stream iconStream = Application.GetResourceStream(new Uri(imageSource?.ToString()))?.Stream;
                return new Icon(iconStream);
            }
            catch
            {
                return null;
            }
        }
    }
}
