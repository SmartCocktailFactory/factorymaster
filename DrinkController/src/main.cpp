/** Main of the drink controller */  

/** Includes **/
#include "../inc/DrinkController.h"

int main()
{
  int ret;
  DrinkController* dc = new DrinkController( );
  ret = dc->WaitForKill( );
  delete dc;
  return ret;
}
