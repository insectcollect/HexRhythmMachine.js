// HexRhythmMachine.h based on http://the.mohayonao.com/6chars/

#include <regex>

typedef struct HexRhythmMachineInfo { int bd = 0; int sd = 0; int hh = 0; } t_HRMInfo;

class HexRhythmMachine {
	
	private :
		
		std::string pattern;
		
		double mBpm = 120;
		double mLength = 0;
		
		std::vector<char> BD;
		std::vector<char> SD;
		std::vector<char> HH;
		
		t_HRMInfo info;
		
		std::vector<std::string> search(std::string const &text, std::regex const &re) {
			std::vector< std::string > result;
			auto iter = text.cbegin();
			auto end = text.cend();
			std::smatch m ;
			while(std::regex_search(iter,end,m,re)) {
				result.push_back( m.str() ) ;
				iter = m[0].second ;
			}
			return result ;
		}

	public :
		
		static std::string make(int n=2,double bpm=0) {
			
			if(n<=2) n=2;
			
			std::string HEX = "0123456789abcdef";
			
			std::string SD1 = "002";
			std::string SD2 = "8899b";
			
			std::string BD1 = "8a";
			std::string BD2 = "288aab";
			
			std::string p = "";
            
            if(bpm==0) {
			
                switch((Rand::random()%4)) {
                    case 0 : p += "160;"; break;
                    case 1 : p += "140;"; break;
                    case 2 : p += "100;"; break;
                    default : break; // none 120
                }
            
            }
            else {
                
                p += std::to_string(bpm);
                p += ";";
                
            }
			
			for(int k=0; k<n; k++) {
				
				switch((Rand::random()%13)) {
					case 0 : p += "55"; break;
					case 1 : 
					case 2 :
						p += "88"; break;
					case 3 :
					case 4 :
					case 5 :
					case 6 :
						p += "aa"; break;
					case 7 :
					case 8 :
					case 9 :
						p += "ff"; break;
					case 10 :
					case 11 :
						p += "ae"; break;
					default : 
						p += HEX.substr(Rand::random()%HEX.size(),1);
						p += HEX.substr(Rand::random()%HEX.size(),1);
						break; // none
				}
			
				if(Rand::random()%6==5) {
					p += HEX.substr(Rand::random()%HEX.size(),1);
					p += HEX.substr(Rand::random()%HEX.size(),1);
				}
				else {
					p += SD1.substr(Rand::random()%SD1.size(),1);
					p += SD2.substr(Rand::random()%SD2.size(),1); 
				}
				
				if(Rand::random()%6==5) {
					p += HEX.substr(Rand::random()%HEX.size(),1);
					p += HEX.substr(Rand::random()%HEX.size(),1);
				}
				else {
					p += BD1.substr(Rand::random()%BD1.size(),1);
					p += BD2.substr(Rand::random()%BD2.size(),1); 
				}
				
				if(k!=n-1) p+=" "; 
				
			}
						
			return p;
		}
		
		int length() { return this->mLength; }
		double bpm() { return this->mBpm; }

		t_HRMInfo *get(int index) {
			
			if(index<-1) index = 0;
			
			index = index%this->length();
			
			info.bd = BD[index];
			info.sd = SD[index];
			info.hh = HH[index]; 
			
			return &info;
		}

		HexRhythmMachine(std::string pattern) {
			this->set(pattern);
		}
			
		void set(std::string pattern) {
			
			this->pattern = pattern;
			
			this->mBpm    = 0;
			this->mLength = 0;
			
			this->BD.clear();
			this->SD.clear();
			this->HH.clear();
		
			// var rePattern = /^(?:(\d+(?:\.\d+)?);)?(\s*(?:[0-9a-fA-F]{6})+)$/;
			std::regex re1(R"((\d+(?:\.\d+)?);)") ;
			std::regex re2(R"((?:[0-9a-fA-F]{6})+)") ;
			
			double bpm = 0;

			std::vector<std::string> result1 = search(pattern,re1) ;
			if(result1.size()>0) {
				result1[0].pop_back();
				
				bpm = std::stod(result1[0]);
			
				if(bpm<80)  bpm = 80;
				if(bpm>160) bpm = 160;
			}
			
			if(bpm<=0) bpm = 120;
				
			std::string p = "";
				
			std::vector<std::string> result2;
			
			unsigned long pos = pattern.find(";",0);
			result2 = search((pos!=std::string::npos)?pattern.substr(pos,pattern.size()-pos):pattern,re2);
			
			for(int k=0; k<result2.size(); k++) {
				p+=result2[k];
			}
			
			if(p.size()>5) {
								
				int len = (int)(p.size()/6.0);
				
				for(int k=0; k<len; k++) {
					int data = std::stoi(p.substr(6*k,6),nullptr,16);
						
					unsigned char bd = (data)&0xFF;
					unsigned char sd = (data>>8)&0xFF;
					unsigned char hh = (data>>16)&0xFF;
						
					int shift = 8;
					while(shift--) {
						this->BD.push_back(!!(bd&(1<<shift)));
						this->SD.push_back(!!(sd&(1<<shift)));
						this->HH.push_back(!!(hh&(1<<shift)));
					}
				}
				
				this->mBpm = bpm;
				this->mLength = len*8;
			}
		}
		
		std::string text() { return this->pattern; }
	
		void dump() {
			#ifdef __OBJC__
				printf("\"%s\"\n",this->pattern.c_str());
				printf("bpm : %f\n",this->bpm());
				printf("length : %d\n",this->length());
				printf("BD : ");
				for(int k=0; k<this->mLength; k++) printf("%d",this->BD[k]);
				printf("\nSD : ");
				for(int k=0; k<this->mLength; k++) printf("%d",this->SD[k]);
				printf("\nHH : ");
				for(int k=0; k<this->mLength; k++) printf("%d",this->HH[k]);
				printf("\n\n");
			#endif
		}
};