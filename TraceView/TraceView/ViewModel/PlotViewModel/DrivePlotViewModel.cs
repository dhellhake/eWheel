using OxyPlot;
using OxyPlot.Series;
using System.Collections.ObjectModel;
using System.Windows.Threading;
using TracePersistence;

namespace TraceView
{
    public class DrivePlotViewModel : PlotViewModel
    {
        private ObservableCollection<DrivePackage> DataPackages { get; set; }

        private DrivePlotViewModel()
            : base(null, BLEService.None)
        { }
        public DrivePlotViewModel(BLEDevice device, BLEService service)
            : base(device, service)
        {
            this.DataPackages = new ObservableCollection<DrivePackage>();

            device.DrivePackageReceived += Device_DrivePackageReceived;
        }

        private void Plot()
        {
            PlotModel newPlot = new PlotModel();
            newPlot.Series.Add(new LineSeries());
            //newPlot.Series.Add(new LineSeries());
            for (int x = 0; x <= 200; x++)
            {
                if (x >= this.DataPackages.Count)
                    break;

                DrivePackage package = this.DataPackages[this.DataPackages.Count - x - 1];

                ((LineSeries)newPlot.Series[0]).Points.Add(new DataPoint(package.TimeStamp, package.DriveState));
            }
            this.PlotModel = newPlot;
        }

        private void Device_DrivePackageReceived(object sender, DrivePackageEventArgs e)
        {
            this.DataPackages.Add(e.DataPackage);

            if (this.IsActive)
                Dispatcher.CurrentDispatcher.Invoke(() => this.Plot());
        }
    }
}
