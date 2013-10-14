#include <curl/curl.h>
#include "com.h"
#include <jsoncpp/json/json.h>
#include <algorithm>
#include <string.h>

static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata);

static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata){
    int i, bound;
    bound = std::min((int)size,BUFFERSIZE);
    printf("size %d, nmemb %d, ptr %x\n",size, nmemb, ptr);
    for (i=0;i<10;i++)
    {
        //userdata[i] = *(ptr++);
        //printf("printing %d %c\n",i, userdata[i]);
    }
    int count=0;
    int realsize = size*nmemb;
    userdata->append(ptr, realsize);
    printf("realise = %u * %u = %u\n", size, nmemb, realsize);
    return realsize;
}

void Com::init()
{
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.zuehlke.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeHandler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
}

void Com::getOrder(){
    CURLcode res = curl_easy_perform(curl);
    std::cout << buffer;
    return;
    if(res != CURLE_OK){
        //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    #ifndef DEBUG
    printf("quellcode %s\nFINISHED\n",(char*)res);
    #endif
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
   
   char res1[] = "{\
                \"encoding\" : \"UTF-8\",\
                            \"plug-ins\" : [\
                                    \"python\",\
                                            \"c++\",\
                                                    \"ruby\"\
                                                            ],\
                                                                            \"indent\" : { \"length\" : 3, \"use_space\": true }\
    }";

    bool parsingSuccessful = reader.parse( (char*)res1, root );
    
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse configuration\n"
        << reader.getFormattedErrorMessages();
        return;
    }
    #ifdef DEBUG
    printf("parsing passed\n")
    scanf();
    #endif
    // Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
    // such member.
    std::string encoding = root.get("encoding", "UTF-8" ).asString();
    // Get the value of the member of root named 'encoding', return a 'null' value if
    // there is no such member.
    const Json::Value plugins = root["plug-ins"];
}

void Com::respondDone(){
    
}

void Com::cleanup(){
    curl_easy_cleanup(curl);
}


