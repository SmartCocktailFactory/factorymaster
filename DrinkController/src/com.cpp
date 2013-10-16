#include <curl/curl.h>
#include "../inc/com.h"
#include <jsoncpp/json/json.h>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata);

static size_t writeHandler(char *ptr, size_t size, size_t nmemb, std::string *userdata){
    int realsize = size*nmemb;
    userdata->append(ptr, realsize);
    return realsize;
}

void Com::init(){
    curl = curl_easy_init();
   // curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.40:12345/factory/orders/next");
   // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeHandler);
   // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
   // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); /* !!! */
    std::cout << "CURL configured\n";
}

void Com::getOrder(Order_t* order){
    HtmlPutRequestOrder();
    JsonParseOrder(order);
}

void Com::respondDone(int id){
     std::stringstream ss;//create a stringstream
     ss << id;//add number to the stream
    
    std::string s = "http://192.168.1.40:12345/factory/orders/" + ss.str() + "?status=completed"; 
    curl_easy_setopt(curl, CURLOPT_URL, s.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeHandler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */
    buffer = "";
    CURLcode res = curl_easy_perform(curl);
    std::cout << "FINISHED order" << std::endl<<  buffer << std::endl;    
}

void Com::cleanup(){
    curl_easy_cleanup(curl);
}

struct ingredient_item Com::makeIngredientItem(std::string name , int amount){
    struct ingredient_item a;
    a.name = name;
    a.amount = amount;
    return a;
}

CURLcode Com::HtmlPutRequestOrder()
{
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.40:12345/factory/orders/next");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeHandler);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); /* !!! */
    buffer = "";
    CURLcode res = curl_easy_perform(curl);
    std::cout << buffer;
    if(res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    return res;
}


void Com::JsonParseOrder(Order_t* order){
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
   
    bool parsingSuccessful = reader.parse( buffer, root );
    
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse configuration\n"
        << reader.getFormattedErrorMessages();
        return;
    }

    order->orderId = root["order_id"].asInt();

   for( Json::ValueIterator itr = root["recipe"].begin() ; itr != root["recipe"].end() ; itr++ ) {
#ifdef DEBUG
        std::cout << itr.key().asString() << " " << *itr;
#endif
        order->ingredients.push_back(makeIngredientItem(itr.key().asString(), (*itr).asInt()));
   }
#ifdef DEBUG
    printf("parsing passed\n");
#endif
}
