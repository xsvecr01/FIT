using System.Windows;
using WPFWeather.Services;
using WPFWeather.ViewModels;

namespace WPFWeather.Windows
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            // Set DataContext of this window to main ViewModel of application
            DataContext = new WeatherViewModel(new WeatherDiskService());
        }
    }
}