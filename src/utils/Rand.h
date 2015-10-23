// http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c

class Rand {
    public :
        static unsigned long xorshf96(void) {
            static unsigned long x=123456789, y=362436069, z=521288629;
            unsigned long t;
            x ^= x << 16;
            x ^= x >> 5;
            x ^= x << 1;
            t = x;
            x = y;
            y = z;
            z = t^x^y;
            return z;
        }
        static unsigned long random(void) { return xorshf96(); }
        static float ranf(void) { return (float)random()/(float)(~0UL); }
        static float ranfu(void) { return (float) random()/((~0UL>>1)*1.0)-1.0; }
};