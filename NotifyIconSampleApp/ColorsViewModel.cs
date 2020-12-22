using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;

namespace NotifyIconSampleApp
{
    class ColorsViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        #region Commands

        public ICommand RedMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () => { MessageBox.Show("Red"); }
            };

        public ICommand GreenMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () => { MessageBox.Show("Green"); }
            };

        public ICommand BlueMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () => { MessageBox.Show("Blue"); }
            };

        public ICommand ClearMenuCommand =>
            new DelegateCommand
            {
                CommandAction = () => { MessageBox.Show("Clear Color"); }
            };

        public ICommand QuitCommand =>
            new DelegateCommand
            {
                CommandAction = () => { Application.Current.Shutdown(); }
            };

        #endregion Commands

    }
}
