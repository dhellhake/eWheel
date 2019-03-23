using OxyPlot;
using OxyPlot.Series;
using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows.Threading;
using TracePersistence;

namespace TraceView
{
    public class OrientationPlotViewModel : PlotViewModel
    {
        private ObservableCollection<OrientationPackage> DataPackages { get; set; }

        private OrientationPlotViewModel()
            : base(null, BLEService.None)
        { }
        public OrientationPlotViewModel(BLEDevice device, BLEService service)
            : base(device, service)
        {
            this.DataPackages = new ObservableCollection<OrientationPackage>();

            device.OrientationPackageReceived += Device_OrientationPackageReceived;
        }
        
        private void Plot()
        {
            PlotModel newPlot = new PlotModel();
            newPlot.Series.Add(new LineSeries());
            newPlot.Series.Add(new LineSeries());
            for (int x = 0; x <= 200; x++)
            {
                if (x >= this.DataPackages.Count)
                    break;

                OrientationPackage package = this.DataPackages[this.DataPackages.Count - x - 1];

                ((LineSeries)newPlot.Series[0]).Points.Add(new DataPoint(package.TimeStamp, package.Roll));
                ((LineSeries)newPlot.Series[1]).Points.Add(new DataPoint(package.TimeStamp, package.Pitch));
            }
            this.PlotModel = newPlot;
        }
        
        private void Device_OrientationPackageReceived(object sender, OrientationEventArgs e)
        {
            this.DataPackages.Add(e.DataPackage);
            Dispatcher.CurrentDispatcher.Invoke(() => this.Plot());
        }
    }
}
