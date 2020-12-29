using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent : FrameworkElement, IDisposable, INotifyPropertyChanged
    {
        private const string CategoryName = "NotifyIconWpf";

        public event PropertyChangedEventHandler PropertyChanged;

        #region MenuActivation property

        // Menu Activation property
        // What event(s) should show the context menu for the icon
        public static readonly DependencyProperty MenuActivationProperty = DependencyProperty.Register(
            nameof(MenuActivation),
            typeof(EventType),
            typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(EventType.SingleClick)
        );

        [Category(CategoryName)]
        [Description("What event(s) should show the context menu for the icon.")]
        public EventType MenuActivation
        {
            get => (EventType)GetValue(MenuActivationProperty);
            set => SetValue(MenuActivationProperty, value);
        }

        #endregion MenuActivation Property

        #region Icon property

        // Icon property
        // An image source that provides the icon to display in the notification area
        public static readonly DependencyProperty IconProperty = DependencyProperty.Register(
            nameof(Icon),
            typeof(ImageSource),
            typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(null, IconPropertyChanged)
        );

        [Category(CategoryName)]
        [Description("An image source that provides the icon to display in the notification area.")]
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

        #endregion Icon property

        #region DoubleClickCommand properties

        // DoubleClickCommand property
        // Invoked when a double click event is raised from notification area icon
        public static readonly DependencyProperty DoubleClickCommandProperty =
            DependencyProperty.Register(nameof(DoubleClickCommand),
                typeof(ICommand),
                typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(null));

        [Category(CategoryName)]
        [Description("Invoked when a double click event is raised from notification area icon.")]
        public ICommand DoubleClickCommand
        {
            get => (ICommand)GetValue(DoubleClickCommandProperty);
            set => SetValue(DoubleClickCommandProperty, value);
        }

        // Parameter to pass to the double click command, if any
        public static readonly DependencyProperty DoubleClickCommandParameterProperty =
            DependencyProperty.Register(nameof(DoubleClickCommandParameter),
                typeof(object),
                typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(null));

        [Category(CategoryName)]
        [Description("Parameter to pass to the double click command, if any.")]
        public object DoubleClickCommandParameter
        {
            get => GetValue(DoubleClickCommandParameterProperty);
            set => SetValue(DoubleClickCommandParameterProperty, value);
        }

        #endregion DoubleClickCommand properties
    }
}
