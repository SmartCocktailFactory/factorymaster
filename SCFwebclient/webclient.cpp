#include <cstdio>
#include <cstdlib>
#include "com.h"

int main()
{
    Com* com = new Com();
    com->init();
    com->getOrder();
    return 1;
    com->cleanup();
    delete(com);
    return 0;
}

