class Snare {
    
    private :
    
        bool isPlay = false;

        Cycle *cycle1;
        Cycle *cycle2;
    
        Line *line1;
        Line *line2;
    
        double sig1 = 5;
        double sig2 = 32;
    
    public :
    
        Snare() {
            
            this->cycle1 = new Cycle(0);
            this->cycle2 = new Cycle(0);
            
            this->line1 = new Line({
                {0.0,12.0},{15.0,7.5},{50,0.0}
            });
            
            this->line2 = new Line({
                {0.0,1.0},{20.0,0.46},{40,0.0}
            });
            
        }
    
        void bang() {
            
            isPlay = true;
            
            sig2 = 10+Rand::random()%30;
            
            this->line1->bang();
            this->line2->bang();
        }
    
        double next() {
        
            double amp = line2->next();
        
            if(isPlay&&amp>=0.0) {
                double freq1 = sig1*sig2;
                cycle1->inlet1(freq1);
                double freq2 = sig1+cycle1->next()*freq1*line1->next();
                cycle2->inlet1(freq2);
            
                return cycle2->next()*amp;
            }
            else {
                return 0;
            }
        }
};