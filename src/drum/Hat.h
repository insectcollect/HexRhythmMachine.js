class Hat {
    
    private :
    
        bool isPlay = false;

        Noise *noise;
        Line *line;
    
    public :
    
        Hat() {
            this->noise = new Noise();
            this->line = new Line({
                {0.0,1.0},{1.0+(Rand::random()%4),0.0}
            });
        }
    
        void bang() {
            isPlay = true;
            this->line->set({
                {0.0,1.0},{1.0+(Rand::random()%4),0.0}
            });
        }
    
        double next() {
        
            double amp = line->next();
            
            if(isPlay&&amp>0.0) {
                return noise->next()*amp;
            }
            else {
                return 0;
            }
        }
};