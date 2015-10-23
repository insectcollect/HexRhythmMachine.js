class Line {
	private :
		
		long length;
		double first;
		double last;
		long seek = 0;
		double position = 0;
		double p[2] = {0,0};
		double q[2] = {0,0};
		
		double weight = 0.0;
		
		std::vector<std::array<double,2>> envelope;
			
	public :
		
        void bang() {
            this->seek = 0;
			this->position = 0;
        }
    
        void bang(double d) {
            if(this->envelope.size()>0) {
                this->envelope[0][1] = d;
                this->first = this->envelope[0][1];
            }
            this->bang();
        }
    
		void set(std::vector<std::array<double,2>> data) {
			
			if(envelope.size()>0) {
                envelope.clear();
			}
            
			std::sort(data.begin(),data.end(),[](std::array<double,2>x,std::array<double,2>y) -> int { return (x[0]<y[0]); });
			
			double then = data[0][0]-1;
			
			std::vector<std::array<double,2>>::iterator it;
			it = data.begin();
			while(it!=data.end()) {
				
				if((*it)[0]<0||(*it)[0]==then) {
					it = data.erase(it);
				}
				else {
					then = (*it)[0];
					it++;
				}
			}
			
			if(data.size()==0) data.push_back({0,0});
			if(data[0][0]!=0) data.insert(data.begin(),{0,data[0][1]});
			
			this->envelope = data;
			
			this->seek = 0;
			this->position = 0;
			
			this->length = this->envelope.size();
			this->first = this->envelope[0][1];
			this->last  = this->envelope[this->length-1][1];
            
		}
		
		Line(std::vector<std::array<double,2>> data) {
			this->set(data);
		}		
		
    
		void update() { this->position+=0.001*SAMPLING_RATE*0.001; }
		
		double next() {
			
			for(long k=this->seek; k<this->length; k++) {
				if(this->position<=this->envelope[k][0]) {
					if(this->seek!=k) {
						this->seek = k;
						p[0] = this->envelope[this->seek-1][0];
						p[1] = this->envelope[this->seek-1][1];
						q[0] = this->envelope[this->seek][0];
						q[1] = this->envelope[this->seek][1];
						this->weight = 1.0/(q[0]-p[0]);
					}
					break;
				}
				else if(k==length-1) {
					seek++;
				}
			}
		
			if(this->seek==0) {
				this->update();
				return this->first;
			}
			else if(this->seek>=this->length) {								
				this->update();
				return this->last;
			}
			else {
			
				double wet = (this->position-p[0])*this->weight;
				double dry = 1-wet;
												
				this->update();

				return this->p[1]*dry+this->q[1]*wet;
			}

			return 0;
		}
};