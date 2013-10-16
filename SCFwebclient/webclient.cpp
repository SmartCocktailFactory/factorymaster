#include <cstdio>
#include <cstdlib>
#include "com.h"
#include <iostream>
int main()
{
    Com* com = new Com();
    Order_t order;
    com->init();
    com->getOrder(&order);

    //std::cout << "New order\nOrder id = " <<  order.orderId << "\n";
    //std::cout <<  order.ingredients[0].name<< order.ingredients[0].amount<<"\n";
    //std::cout <<  order.ingredients[1].name<< order.ingredients[1].amount<<"\n";
    //std::cout <<  order.ingredients[2].name<< order.ingredients[2].amount<<"\n";
    //std::cout <<  order.ingredients[3].name<< order.ingredients[3].amount<<"\n";
    //std::cout << "Prost!\n";  

    com->cleanup();
    delete(com);
    return 0;
}

