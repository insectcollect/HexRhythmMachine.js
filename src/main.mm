#ifdef EMSCRIPTEN
    #include <emscripten.h>
    #include <math.h>
#endif

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

#define TWO_PI 3.14159265358979323846*2.0
#define BUFFER_SIZE 1<<10
#define SAMPLING_RATE 44100.
#define SAMPLING_PERIOD 1.0/SAMPLING_RATE

#define POSITVE_EXPONENT 1000000
#define NEGATVE_EXPONENT 0.000001


#ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #import <Cocoa/Cocoa.h>
    #import "Audio.h"
#endif

#include "Rand.h"

#include "Cycle.h"
#include "Line.h"
#include "Noise.h"

#include "Comb.h"

#include "BPM.h"

#include "Kick.h"
#include "Snare.h"
#include "Hat.h"

#include "HexRhythmMachine.h"

class App {
    
    private :
    
        #ifdef __OBJC__
        float *bL = (float *)malloc((BUFFER_SIZE)*sizeof(float *));
        float *bR = (float *)malloc((BUFFER_SIZE)*sizeof(float *));
        Audio *au;
        #endif
    
        int timer = 0;
    
        BPM *bpm;
    
        Kick *kick;
        Snare *snare;
        Hat *hat;
    
        HexRhythmMachine *hrm;
    
        std::vector<Comb *> comb;

    
    public :
    
        void rand() {
            this->hrm->set(HexRhythmMachine::make(2,100));
        }
    
        void next(float *L,float *R,int size) {
            
            for(int k=0; k<size; k++) {
                
                this->bpm->next();
                 
                double kick  = this->kick->next();
                double snare = this->snare->next();
                double hat   = this->hat->next();
                
                double wav = kick*0.25 + snare*0.6 + hat;
                
                L[k] = wav*0.2 +this->comb[0]->next(wav)*0.1;
                R[k] = wav*0.2 +this->comb[1]->next(wav)*0.1;
                    
            }
        }
    
        App() {
        
            
            
            this->comb.push_back(new Comb(1000,20,0.1));
            this->comb.push_back(new Comb(1000,30,0.1));
        
            this->kick  = new Kick();
            this->snare = new Snare();
            this->hat   = new Hat();
        
            this->hrm = new HexRhythmMachine(HexRhythmMachine::make(2,100));
            #ifdef __OBJC__
            this->hrm->dump();
            #endif
            
            this->bpm = new BPM(true,100,2*8,[this](int cnt) { // 16
                
                t_HRMInfo *info = this->hrm->get(cnt); // 2*8
                
                if(info->bd==1) this->kick->bang();
                if(info->sd==1) this->snare->bang();
                if(info->hh==1) this->hat->bang();
                
                #ifdef __OBJC__
                // random
                if(cnt==(2*8-1)) {
                    if(Rand::random()%10==0) {
                        this->hrm->set(HexRhythmMachine::make(2,120));
                        this->hrm->dump();
                    }
                }
                #endif
                
            });
            
            #ifdef __OBJC__
            this->au = (new Audio(0,2))->on("AudioOutput",^(float *output,int size,int ch) {
                if(size<=BUFFER_SIZE&&ch==2) {
                    for(int k=0; k<size; k++) {
                        this->bL[k] = output[k*ch+0];
                        this->bR[k] = output[k*ch+1];
                    }
                    this->next(this->bL,this->bR,size);
                    for(int k=0; k<size; k++) {
                        output[k*ch+0] = this->bL[k];
                        output[k*ch+1] = this->bR[k];
                    }
               }
            })->start();
            #endif
        }
    
        ~App() {
            #ifdef __OBJC__
            this->au->stop();
            #endif
        }
};
#ifdef EMSCRIPTEN
App *app = NULL;
extern "C" {
    void setup(int uid) { for(int k=0; k<uid; k++) Rand::random(); app = new App(); }
    void next(float *L, float *R,int size) { if(app) app->next(L,R,size); }
    void randamize() { if(app) app->rand(); }
}
#elif __OBJC__
#pragma mark AppDelegate
@interface AppDelegate:NSObject <NSApplicationDelegate> { App *m; } @end
@implementation AppDelegate
-(void)applicationDidFinishLaunching:(NSNotification*)aNotification { m = new App(); }
-(void)applicationWillTerminate:(NSNotification *)aNotification { if(m) delete m; }
@end
int main (int argc, const char * argv[]) {
    srand(CFAbsoluteTimeGetCurrent());
    srandom(CFAbsoluteTimeGetCurrent());
    id app = [NSApplication sharedApplication];
    id delegat = [AppDelegate alloc];
    [app setDelegate:delegat];
    id menu = [[NSMenu alloc] init];
    id rootMenuItem = [[NSMenuItem alloc] init];
    [menu addItem:rootMenuItem];
    id appMenu = [[NSMenu alloc] init];
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];
    [rootMenuItem setSubmenu:appMenu];
    [NSApp setMainMenu:menu];
    [app run];
    return 0;
}
#endif