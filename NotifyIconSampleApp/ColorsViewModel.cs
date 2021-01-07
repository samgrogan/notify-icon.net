using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using NotifyIcon.Wpf;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace NotifyIconSampleApp
{
    enum Color
    {
        NoColor,
        Red,
        Green,
        Blue
    }

    public class ColorsViewModel
    {
        #region Properties

        public string ColorGuid {
            get {
#if DEBUG
                return "253df4ea-bbe2-4e0d-81de-df4426d6d37d";
#else
                return "055cf532-a707-45fb-b0a9-930805704613";
#endif
            }
        }

#endregion Properties


#region Commands

        public ICommand RedMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () =>
                {
                    SetColor(Color.Red);
                }
            };

        public ICommand GreenMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () =>
                {
                    SetColor(Color.Green);
                }
            };

        public ICommand BlueMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () =>
                {
                    SetColor(Color.Blue);
                }
            };

        public ICommand ClearMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () =>
                {
                    SetColor(Color.NoColor);
                }
            };

        public ICommand QuitCommand =>
            new DelegateCommand
            {
                CommandAction = () =>
                {
                    Application.Current.Shutdown();
                }
            };

#endregion Commands

#region Internal Methods

        private void SetColor(Color color)
        {
            App app = App.Current as App;
            NotifyIconComponent notifyIcon = app?.NotifyIcon;

            string toolTip = Resources.Colors.NoColorToolTip;
            string iconName = "no-color.ico";

            switch (color)
            {
                case Color.Red:
                    toolTip = Resources.Colors.RedToolTip;
                    iconName = "red.ico";
                    break;
                case Color.Green:
                    toolTip = Resources.Colors.GreenToolTip;
                    iconName = "green.ico";
                    break;
                case Color.Blue:
                    toolTip = Resources.Colors.BlueToolTip;
                    iconName = "blue.ico";
                    break;
            }

            if (notifyIcon != null)
            {
                notifyIcon.ToolTip = toolTip;
                BitmapImage icon = new BitmapImage(new Uri($"pack://application:,,,/Resources/Icons/{iconName}", UriKind.Absolute)); ;
                notifyIcon.Icon = icon;
            }
        }

#endregion Internal Methods
    }
}
