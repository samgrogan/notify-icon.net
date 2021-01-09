using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using NotifyIcon.Win32;

namespace NotifyIcon.Wpf
{
    public partial class NotifyIconComponent {
        private const string CategoryName = "NotifyIconWpf";

        #region MenuActivation property

        // Menu Activation property
        // What event(s) should show the context menu for the icon
        public static readonly DependencyProperty MenuActivationProperty = DependencyProperty.Register(
            nameof(MenuActivation),
            typeof(EventType),
            typeof(NotifyIconComponent),
                new FrameworkPropertyMetadata(EventType.ContextMenu)
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

        #region IconGuid property

        // Icon Guid property
        // Uniquely identify the notification icon to the system
        public static readonly DependencyProperty GuidProperty = DependencyProperty.Register(
            nameof(IconGuid),
            typeof(string),
            typeof(NotifyIconComponent),
            new FrameworkPropertyMetadata(null, IconGuidPropertyChanged)
        );

        [Category(CategoryName)]
        [Description("Uniquely identify the notification icon to the system.")]
        public string IconGuid
        {
            get => GetValue(GuidProperty) as string;
            set => SetValue(GuidProperty, value);
        }

        #endregion Icon Guid property

        #region Tool Tip property

        private static void ToolTipPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NotifyIconComponent owner = d as NotifyIconComponent;
            owner?.OnToolTipPropertyChanged(e);
        }

        private void OnToolTipPropertyChanged(DependencyPropertyChangedEventArgs e)
        {
            // Update the icon with the new tooltip
            if (_notifyIcon != null)
            { 
                _notifyIcon.ToolTip = e.NewValue?.ToString();
            }
        }

        #endregion Data Context property

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

        #region Data Context property

        private static void DataContextPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            NotifyIconComponent owner = d as NotifyIconComponent;
            owner?.OnDataContextPropertyChanged(e);
        }

        private void OnDataContextPropertyChanged(DependencyPropertyChangedEventArgs e)
        {
            // Update the data context so that commands are properly routed
            if (ContextMenu != null)
            {
                ContextMenu.DataContext = e.NewValue;
            }
        }

        #endregion Data Context property
    }
}
