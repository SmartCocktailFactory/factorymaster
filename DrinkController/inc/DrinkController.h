#ifndef _DRINK_CONTROLLER_H_
#define _DRINK_CONTROLLER_H_

/************************************************************
    Includes
*************************************************************/
#include <pthread.h>

/************************************************************
    Class definition
*************************************************************/

class DrinkController
{
 public:
    DrinkController( );
    int WaitForKill( );
  
 private:
    /* Variables */
    pthread_t threadId;

    /* Functions */
};

#endif
