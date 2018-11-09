using System.Windows;

namespace DiagMonitor
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
        private void StartUp(object sender, StartupEventArgs e)
        {
            MainViewModel viewModel = new MainViewModel();
            MainWindow mainView = new MainWindow(viewModel);

            mainView.Show();
        }

    }
}
