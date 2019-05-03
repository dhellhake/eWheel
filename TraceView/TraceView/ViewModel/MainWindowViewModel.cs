using System;
using System.Collections.Generic;
using TracePersistence;

namespace TraceView
{
    public class MainWindowViewModel : Bindable
    {
        public MainWindow MainWindow { get; set; }        public BLEDevice eWheel { get; private set; }        
        public MainWindowViewModel()
        {
            this.eWheel = new BLEDevice("CC41-A");

            List<PlotViewModel> models = new List<PlotViewModel>();
            
            foreach (BLEService service in (BLEService[])Enum.GetValues(typeof(BLEService)))
            {
                switch(service)
                {
                    case BLEService.Orientation:
                        models.Add(new ChassisPlotViewModel(this.eWheel, service));
                        break;
                    case BLEService.VESC:
                        models.Add(new VESCPlotViewModel(this.eWheel, service));
                        break;
                }
            }
            this.PlotViewModels = models;
        }
        
        #region PlotViewModels
        private List<PlotViewModel> _PlotViewModels;
        public List<PlotViewModel> PlotViewModels
        {
            get { return this._PlotViewModels; }
            set
            {
                if (this._PlotViewModels != value)
                {
                    this._PlotViewModels = value;
                    OnPropertyChanged(nameof(PlotViewModels));
                }
            }
        }
        #endregion
    }
}
