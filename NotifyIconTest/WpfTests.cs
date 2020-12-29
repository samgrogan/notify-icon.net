using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using NotifyIcon.Wpf;
using NotifyIcon.Win32;

namespace NotifyIcon.Test
{
    [TestClass]
    public class WpfTests
    {
        public const string TEST_ICON = "Resources/Icons/test.ico";


        [TestMethod]
        public void SimpleCreateDestory()
        {
            NotifyIconComponent notifyIconComponent = new NotifyIconComponent
            {
                Icon = new BitmapImage(new Uri(TEST_ICON, UriKind.Relative)),
                ToolTip = "This is a test",
                MenuActivation = NotifyIconEventType.SingleClick
            };
        }
    }
}
