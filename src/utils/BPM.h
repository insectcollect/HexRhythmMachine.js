class BPM {
	
	private :
    
        bool isPlay = false;
	
		std::function<void(int)> callback;
	
		int counter;
	
		int cnt;
		int max;
		int bpm;
			
	public :
		
		BPM(bool isPlay,int bpm,int max,std::function<void(int)> callback) {
            this->cnt = 0;
			inlet01(isPlay);
            inlet02(bpm);
            inlet03(max);
            outlet01(callback);
		}
		
        void inlet01(bool in1) {
            this->isPlay = (in1)?true:false;
        }
    
        void inlet02(double in2) {
            this->bpm = in2;
            this->counter = (SAMPLING_RATE*(((60.0/this->bpm)*1000.)/4.0)*0.001);
        }
    
        void inlet03(int in3) {
            this->max = in3;
        }
    
        void outlet01(std::function<void(int)> callback) {
			this->callback = callback;
		}
    
		void next() {
            if(this->isPlay) {
                if((--this->counter)==0) {
                    if(this->callback) this->callback(this->cnt);
                    if((++this->cnt)>=this->max) this->cnt = 0;
                    inlet02(this->bpm);
                }
            }
        }
};