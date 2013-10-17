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


/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/

/************************************************************
    Local function prototypes
*************************************************************/
static void* CommandManager(void *);
static bool HaveToDeliverIceCubes(Order_t* pOrder);

/************************************************************
    Public functions 
*************************************************************/

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


int DrinkController::WaitForKill( )
{
    return pthread_join( threadId, NULL ); 
}

/************************************************************
    Local functions
*************************************************************/

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
    for (;;)
    {
        /* Poll order */
        order.orderId = 0;
        communicationInterface.getOrder( &order );
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
            communicationInterface.respondDone(order.orderId );
            /* Wait glass removed */
            sps.WaitClassRemoved( );
        }
        else
        {
            sleep( 1u);
            fprintf( stderr, "No new order \n"  );
        }
    }
    return NULL;
}
