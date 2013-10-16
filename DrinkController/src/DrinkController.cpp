/************************************************************
    Includes
*************************************************************/
#include "../inc/DrinkController.h"
#include "../inc/BottleManagement.h"
#include "../inc/LiquidDeliverySystem.h"
#include <stdio.h>

/** Local function prototype **/
static void* CommandManager(void *);


/** Local functions **/
DrinkController::DrinkController()
{
  if (pthread_create( &threadId,
		      NULL,
		      &CommandManager,
		      NULL ) != 0)
    {
      fprintf( stderr, "Konnte Thread nicht erzeugen\n" );
    }
}

static void* CommandManager(void *)
{
  BottleManagement bm;
  LiquidDeliverySystemIndex_e stationIndex;
  std::string liquidName; 
  LiquidDeliverySystem sps( "127.0.0.1", 1999u );

  //  for (;;)
    {
      /* Poll order */

      /* Ice necessary */


      /* Look up bottles */
      liquidName = "Baccardi";
      stationIndex = bm.GetLiquidStationIndex( liquidName );
      fprintf( stderr, "Index:%d \n", stationIndex );

      /* Deliver liquid*/
      sps.DeliverVolume( stationIndex, 10u );
      /* Wait all done */

      /* Push finishing */

    }

    
  return NULL;
}

int DrinkController::WaitForKill( )
{
  return pthread_join( threadId, NULL ); 
}
