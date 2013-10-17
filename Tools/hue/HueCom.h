#ifndef HUE_COM_H
#define HUE_COM_H
#include <string>
#include <curl/curl.h>
#include <vector>
#define BUFFERSIZE 5000000

class HueCom
{
    private:
        CURL *curl;
    public:
        void init();
        void cleanup();
        void writeHUE(int, int, int, int);

        void Farbkreis(int hue, int steps);
        void blink(int hue,int period, int up, int low)
};

#endif
