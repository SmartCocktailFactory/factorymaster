#ifndef COM_H
#define COM_H
#include <string>
#include <curl/curl.h>
#include <vector>
#define BUFFERSIZE 5000000

struct ingredient_item{
    std::string name;
    int amount;
};

typedef struct {
    int orderId;
    std::vector<ingredient_item> ingredients;
} Order_t;

class Com
{
    private:
        CURL *curl;
        //char buffer[BUFFERSIZE];
        std::string buffer;
        CURLcode HtmlPutRequestOrder();
        void JsonParseOrder(Order_t* order);
        struct ingredient_item makeIngredientItem(std::string, int);
    public:
        void init();
        void getOrder(Order_t* order);
        void respondDone(int id); 
        void cleanup();
};

#endif
