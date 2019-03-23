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
            if (e.PropertyName == nameof(IsActive))
            {
                if (this.IsActive)
                    this.Device.Subscribe(this.Service);
                else
                    this.Device.UnSubscribe(this.Service);
            }
        }

        #region IsActive
        private bool _IsActive;
        public bool IsActive
        {
            get { return this._IsActive; }
            set
            {
                if (this._IsActive != value)
                {
                    this._IsActive = value;
                    OnPropertyChanged(nameof(IsActive));
                }
            }
        }
        #endregion

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
