using OxyPlot;
using OxyPlot.Series;
using System;
using TracePersistence;

namespace TraceView
{
    public class PlotViewModel : Bindable
    {        public BLEDevice Device { get; private set; }        public BLEService Service { get; private set; }
        public string Title { get; private set; }


        private PlotViewModel() { }
        protected PlotViewModel(BLEDevice device, BLEService service)
        {
            this.Device = device;
            this.Service = service;
            this.Title = service.ToString();
                                 
            this.PropertyChanged += PlotViewModel_PropertyChanged;
        }

        private void PlotViewModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
        }

        #region PlotModel
        private PlotModel _PlotModel;
        public PlotModel PlotModel
        {
            get { return this._PlotModel; }
            set
            {
                if (this._PlotModel != value)
                {
                    this._PlotModel = value;
                    OnPropertyChanged(nameof(PlotModel));
                }
            }
        }
        #endregion
    }
}
