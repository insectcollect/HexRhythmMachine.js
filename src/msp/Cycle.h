
class Cycle {
	private :
		double phase1=0;
		double phase2=0;

		double freq1=440.;
		double freq2=0.;
	
	public :
		Cycle(double freq1=440.,double freq2=0.) {
			inlet1(freq1);
			inlet2(freq2);
		}
	
		void inlet1(double freq1) { this->freq1 = freq1; }
		void inlet2(double freq2) { this->freq2 = freq2; }
	
		double next() {
			
            double right = 0;
            
            // 2nd inlet
            if(this->freq2!=0) {
                this->phase2+=SAMPLING_PERIOD*this->freq2*POSITVE_EXPONENT;
                if(this->phase2<=-POSITVE_EXPONENT||this->phase2>=POSITVE_EXPONENT) this->phase2-=POSITVE_EXPONENT;
                right = sin(this->phase2*NEGATVE_EXPONENT*TWO_PI);
                right = (right-floor(right)); // 0.0 to 1.0
			}
            
            // 1st inlet
			this->phase1+=SAMPLING_PERIOD*this->freq1*POSITVE_EXPONENT;
			if(this->phase1<=-POSITVE_EXPONENT||this->phase1>=POSITVE_EXPONENT) this->phase1-=POSITVE_EXPONENT;
			
			double phase = this->phase1+(right*POSITVE_EXPONENT);
			
			return sin(phase*NEGATVE_EXPONENT*TWO_PI);
			
		}
};