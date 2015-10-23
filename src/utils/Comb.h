class Comb {
	
	private :
		
        int head;
		int maxsize;
		int last;
        double delay;
		double *buffer;
        double wet;
    
			
	public :
    
		Comb(int maxsize=1000,int delay=300,double wet=0.5) {
			
			this->head = 0;
            
            if(delay>=maxsize) delay = maxsize;
			this->maxsize = maxsize;
			this->last = (int(SAMPLING_RATE*0.001*maxsize));
			this->delay = SAMPLING_RATE*0.001*delay;
            this->buffer = (double *)calloc(int(44.1*maxsize),sizeof(double));
            this->wet = wet;
            
		}
    
    
		double next(double src) {
        
            double playback = head-delay;
            if(playback<0) playback+=last;
            int frame = floor(playback);
            double delta = playback-frame;
            double grain = buffer[frame+1]*delta+buffer[frame]*(1-delta);
            
            if(head==last-1) {
                buffer[head] = buffer[0] = (src+grain)*this->wet;
                head = 0;
            }
            else buffer[head++] = (src+grain)*this->wet;
                    
            return grain;
            
		}
		
		~Comb() {
			free((void *)this->buffer);
		}
    
};