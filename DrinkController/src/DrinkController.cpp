/************************************************************
    Includes
*************************************************************/
#include "../inc/DrinkController.h"
#include "../inc/BottleManagement.h"
#include "../inc/LiquidDeliverySystem.h"
#include "../inc/com.h"
#include "../inc/MicroControllerCommunication.h"
#include <stdio.h>
#include <unistd.h>

/** Local function prototype **/
static void* CommandManager(void *);
static bool HaveToDeliverIceCubes(Order_t* pOrder);


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
  LiquidDeliverySystem sps( "192.168.1.120", 200u );
  Com communicationInterface;
  Order_t order;
  
  communicationInterface.init( );
  bm.AssignBottleToLiquidStation( "Rum", E_LiquidDeliverySystemIndex_1 );
  bm.AssignBottleToLiquidStation( "Vodka", E_LiquidDeliverySystemIndex_2 );
  bm.AssignBottleToLiquidStation( "Cola", E_LiquidDeliverySystemIndex_3 );

  MicroControllerCommunication uCCom;
  //for (;;)
    {
      /* Poll order */
      order.orderId = 0;
      //communicationInterface.getOrder( &order );
      
      order.orderId = 1;
      order.ingredients.clear( );
      ingredient_item it;
      it.name = "Rum";
      it.amount = 20;
      order.ingredients.push_back( it );
      it.name = "Cola";
      it.amount = 250;
      order.ingredients.push_back( it );

      if (order.orderId != 0)
	{
	  fprintf( stderr, "New order:%d \n", order.orderId );

	  /* Ice necessary */
	  if (HaveToDeliverIceCubes( &order ))
	    {
	      uCCom.SendDeliverIceCube( 1u );
	      fprintf( stderr, "Order:%d with ice\n", order.orderId );
	    }
	  else
	    {
	      fprintf( stderr, "Order:%d no ice\n", order.orderId );
	    }
	  /* Deliver all liquids. */
	  for (unsigned int i = 0u; i < order.ingredients.size( ); i++)
	    {
	      /* Look up bottles */
	      liquidName = order.ingredients[i].name;
	      stationIndex = bm.GetLiquidStationIndex( liquidName );
	      fprintf( stderr, "Index:%d name:%s amount:%d\n", stationIndex, liquidName.c_str( ),
		       order.ingredients[i].amount );
	      
	      /* Deliver liquid*/
	      fprintf( stderr, "Delivery started.." );
		      
	      sps.DeliverVolume( stationIndex, order.ingredients[i].amount );
	      /* Wait delivery done */
	      
	      fprintf( stderr, "Delivery finished %d..", (int) sps.CheckDeliveryDoneSuccessfull( ));
	    }
	  /* Push finishing */
	  communicationInterface.respondDone( );
	}
      else
	{
	  sleep( 1u);
	  fprintf( stderr, "No new order \n"  );
	}
    }

    
  return NULL;
}

int DrinkController::WaitForKill( )
{
  return pthread_join( threadId, NULL ); 
}

static bool HaveToDeliverIceCubes(Order_t* pOrder)
{
  bool iceNecessary = false;
  for (unsigned int i = 0u; i < pOrder->ingredients.size( ); i++)
    {
      if (pOrder->ingredients[i].name == "IceCube")
	{
	  iceNecessary = true;
	  pOrder->ingredients.erase( pOrder->ingredients.begin() + i );
	  break;
	}
    }
  return iceNecessary;
}
