using OxyPlot;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using TracePersistence;

namespace TraceView
{
    public class VESCPlotViewModel : PlotViewModel
    {
        private ObservableCollection<VESCPackage> DataPackages { get; set; }

        private VESCPlotViewModel()
            : base(null, BLEService.None)
        { }
        public VESCPlotViewModel(BLEDevice device, BLEService service)
            : base(device, service)
        {
            this.DataPackages = new ObservableCollection<VESCPackage>();

            device.VESCPackageReceived += Device_VESCPackageReceived;
        }
        
        private void Plot()
        {
            PlotModel newPlot = new PlotModel();
            newPlot.Series.Add(new LineSeries());
            for (int x = 0; x <= 200; x++)
            {
                if (x >= this.DataPackages.Count)
                    break;

                VESCPackage package = this.DataPackages[this.DataPackages.Count - x - 1];

                //((LineSeries)newPlot.Series[0]).Points.Add(new DataPoint(package.TimeStamp, package.Avl_RPM));
                //((LineSeries)newPlot.Series[1]).Points.Add(new DataPoint(package.TimeStamp, package.Avl_Duty));
                //((LineSeries)newPlot.Series[2]).Points.Add(new DataPoint(package.TimeStamp, package.Avl_TempFET));
                ((LineSeries)newPlot.Series[0]).Points.Add(new DataPoint(package.TimeStamp, package.Avl_PIDPosNow));
            }
            this.PlotModel = newPlot;
        }

        private void Device_VESCPackageReceived(object sender, VESCEventArgs e)
        {
            this.DataPackages.Add(e.DataPackage);

            if (this.IsActive)
                Dispatcher.CurrentDispatcher.Invoke(() => this.Plot());
        }
    }
}
