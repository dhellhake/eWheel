using OxyPlot;
using OxyPlot.Series;
using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows.Threading;
using TracePersistence;

namespace TraceView
{
    public class ChassisPlotViewModel : PlotViewModel
    {
        private ObservableCollection<ChassisPackage> DataPackages { get; set; }

        private ChassisPlotViewModel()
            : base(null, BLEService.None)
        { }
        public ChassisPlotViewModel(BLEDevice device, BLEService service)
            : base(device, service)
        {
            this.DataPackages = new ObservableCollection<ChassisPackage>();

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

                ChassisPackage package = this.DataPackages[this.DataPackages.Count - x - 1];

                ((LineSeries)newPlot.Series[0]).Points.Add(new DataPoint(package.TimeStamp, package.Chassis_Roll));
                ((LineSeries)newPlot.Series[1]).Points.Add(new DataPoint(package.TimeStamp, package.Chassis_Pitch));
            }
            this.PlotModel = newPlot;
        }
        
        private void Device_OrientationPackageReceived(object sender, ChassisEventArgs e)
        {
            this.DataPackages.Add(e.DataPackage);
            Dispatcher.CurrentDispatcher.Invoke(() => this.Plot());
        }
    }
}
