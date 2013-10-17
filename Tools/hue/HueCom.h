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
        void writeHUE(int);
};

#endif
