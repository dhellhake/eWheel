using System.Windows;
using System.Windows.Media.Imaging;

namespace DiagMonitor
{
    public class MainViewModel : Bindable
    {
        
        public MainViewModel()
        {
            this.DiagnosisPort = new DiagnosisPort();
            this.DiagnosisPort.PropertyChanged += DiagnosisPort_PropertyChanged;

            this.PitchRaw = -42.45f;
        }

        private void DiagnosisPort_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Pitch")
                this.PitchRaw = this.DiagnosisPort.Pitch;
            else if (e.PropertyName == "SysTick")
                this.SysTick = this.DiagnosisPort.SysTick;
        }

        #region SysTick
        private int _SysTick;
        public int SysTick
        {
            get { return this._SysTick; }
            set
            {
                if (this._SysTick != value)
                {
                    this._SysTick = value;
                    OnPropertyChanged("SysTick");
                }
            }
        }
        #endregion

        #region PitchRaw
        private float _PitchRaw;
        public float PitchRaw
        {
            get { return this._PitchRaw; }
            set
            {
                if (this._PitchRaw != value)
                {
                    this._PitchRaw = value;
                    OnPropertyChanged("PitchRaw");
                }
            }
        }
        #endregion

        #region DiagnosisPort
        private DiagnosisPort _DiagnosisPort;
        public DiagnosisPort DiagnosisPort
        {
            get { return this._DiagnosisPort; }
            set
            {
                if (this._DiagnosisPort != value)
                {
                    this._DiagnosisPort = value;
                    OnPropertyChanged("DiagnosisPort");
                }
            }
        }
        #endregion
    }
}
