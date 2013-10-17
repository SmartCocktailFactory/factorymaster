#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "HueCom.h"


static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata);
static size_t readHandler(char *ptr, size_t size, size_t nmemb, void *userdata);

static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata){
    int realsize = size*nmemb;
    userdata->append(ptr, realsize);
    return realsize;
}

static size_t readHandler(char *ptr, size_t size, size_t nmemb, void *userdata){
//    int realsize = size*nmemb;
//    userdata->append(ptr, realsize);
    size_t returncode;
    curl_off_t nread;
    printf("buffer = %s\n", ptr);
    memcpy(userdata,ptr,size);

    return returncode;
}
void HueCom::init(){
    curl = curl_easy_init();
    std::cout << "CURL configured\n";
}

void HueCom::writeHUE(int id){
     std::stringstream ss;//create a stringstream
     ss << id;//add number to the stream
     char buffer[] = "{\"on\":true,\"bri\":128,\"sat\":255,\"hue\":1212  }\0"; 
    std::string url = "http://192.168.1.101/api/smartcocktailfactory/lights/" + ss.str() + "/state"; 
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //curl_easy_setopt(curl, CURLOPT_READFUNCTION, &readHandler);
    //curl_easy_setopt(curl, CURLOPT_READDATA, buffer);
     //curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer); /* data goes here */
    
    CURLcode res;
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
       fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    std::cout << "FINISHED order" << std::endl<<  buffer << std::endl;    
}

void HueCom::cleanup(){
    curl_easy_cleanup(curl);
}
