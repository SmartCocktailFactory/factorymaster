
#ifndef COM_H
#define COM_H
#include <string>
#include <curl/curl.h>
#define BUFFERSIZE 5000000

class Com
{
    private:
        CURL *curl;
        //char buffer[BUFFERSIZE];
        std::string buffer;
        void HtmlPutRequestOrder();
        void JsonParseOrder();
    public:
        void init();
        void getOrder();
        void respondDone(); 
        void cleanup();
};
#endif

