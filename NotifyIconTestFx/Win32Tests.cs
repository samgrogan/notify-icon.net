using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using NotifyIcon.Win32;
using System.Drawing;
using System.Threading.Tasks;
using System.Threading;

namespace NotifyIcon.Test
{
    [TestClass]
    public class Win32Tests
    {
        public const string TEST_ICON = "Resources/Icons/test.ico";

        [TestMethod]
        public void SerialCreateDestroy()
        {
            using (NotificationAreaIcon notificationAreaIcon1 = new NotificationAreaIcon(Guid.NewGuid()))
            {
                Icon icon1 = new Icon(TEST_ICON);
                notificationAreaIcon1.Icon = icon1.Handle;
                notificationAreaIcon1.ToolTip = $"This is test #1";
                notificationAreaIcon1.ShowIcon();

                using (NotificationAreaIcon notificationAreaIcon2 = new NotificationAreaIcon(Guid.NewGuid()))
                {
                    Icon icon2 = new Icon(TEST_ICON);
                    notificationAreaIcon1.Icon = icon2.Handle;
                    notificationAreaIcon1.ToolTip = $"This is test #2";
                    notificationAreaIcon1.ShowIcon();

                    using (NotificationAreaIcon notificationAreaIcon3 = new NotificationAreaIcon(Guid.NewGuid()))
                    {
                        Icon icon3 = new Icon(TEST_ICON);
                        notificationAreaIcon1.Icon = icon3.Handle;
                        notificationAreaIcon1.ToolTip = $"This is test #2";
                        notificationAreaIcon1.ShowIcon();
                        Thread.Sleep(500);
                    }
                    Thread.Sleep(500);
                }
                Thread.Sleep(500);
            }
        }

        [TestMethod]
        public void ParallelCreateDestroy()
        {
            int runCount = 100;

            Parallel.For(0, runCount, run =>
            {
                using (NotificationAreaIcon notificationAreaIcon = new NotificationAreaIcon(Guid.NewGuid()))
                {
                    Icon icon = new Icon(TEST_ICON);
                    notificationAreaIcon.Icon = icon.Handle;
                    notificationAreaIcon.ToolTip = $"This is test #{run}";
                    notificationAreaIcon.ShowIcon();
                    Thread.Sleep(500);
                }
            });
        }

        [TestMethod]
        public void IconProperty()
        {
            using (NotificationAreaIcon notificationAreaIcon = new NotificationAreaIcon(Guid.NewGuid()))
            {
                Icon icon = new Icon(TEST_ICON);
                notificationAreaIcon.Icon = icon.Handle;
                notificationAreaIcon.ToolTip = $"This is test";
                notificationAreaIcon.ShowIcon();
                Thread.Sleep(500);

                notificationAreaIcon.Icon = null;
                Thread.Sleep(500);

                icon = new Icon(TEST_ICON);
                notificationAreaIcon.Icon = null;
                Thread.Sleep(500);
            }
        }

        [TestMethod]
        public void ToolTipProperty()
        {
            using (NotificationAreaIcon notificationAreaIcon = new NotificationAreaIcon(Guid.NewGuid()))
            {
                Icon icon = new Icon(TEST_ICON);
                notificationAreaIcon.Icon = icon.Handle;
                notificationAreaIcon.ToolTip = $"This is short tooltip";
                notificationAreaIcon.ShowIcon();
                Thread.Sleep(500);

                notificationAreaIcon.ToolTip = null;
                Thread.Sleep(500);

                notificationAreaIcon.ToolTip = String.Empty;
                Thread.Sleep(500);

                notificationAreaIcon.ToolTip = "This is a really long\nmulti-line tool-tip that is tool long to fit the tool tip property\n. This should be truncated and not cause an error.";
                Thread.Sleep(500);
            }
        }
    }
}
