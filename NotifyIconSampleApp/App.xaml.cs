﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using NotifyIcon.Wpf;

namespace NotifyIconSampleApp
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        #region Members

        private NotifyIconComponent _notifyIconComponent;

        #endregion

        #region Properties

        // Access the notify icon component owned by the application
        public NotifyIconComponent NotifyIcon => _notifyIconComponent;

        #endregion Properties

        #region Internal Methods

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            // Find the notify icon component
            _notifyIconComponent = FindResource("NotifyIconComponent") as NotifyIconComponent;
            _notifyIconComponent.ShowIcon();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            _notifyIconComponent?.Dispose();
            _notifyIconComponent = null;

            base.OnExit(e);
        }

        #endregion Internal Methods

    }
}
