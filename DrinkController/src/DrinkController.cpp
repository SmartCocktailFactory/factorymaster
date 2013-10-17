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
static unsigned int NumberIceCubesToDeliver(Order_t* pOrder);
static void ProcessOrder(Order_t* pOrder, BottleManagement* pBM,
                         Com* pCom);

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
static unsigned int NumberIceCubesToDeliver(Order_t* pOrder)
{
    unsigned int nr = 0u;
    for (unsigned int i = 0u; i < pOrder->ingredients.size( ); i++)
    {
        if (pOrder->ingredients[i].name == "IceCube")
        {
            nr = pOrder->ingredients[i].amount;
            pOrder->ingredients.erase( pOrder->ingredients.begin() + i );
            break;
        }
    }
    return nr;
}

static void* CommandManager(void *)
{
    BottleManagement bm;
    Com communicationInterface;
    Order_t order;
    
    communicationInterface.init( );
    bm.AssignBottleToLiquidStation( "OrangeJuice", E_LiquidDeliverySystemIndex_1, 700u );
    bm.AssignBottleToLiquidStation( "Bacardi", E_LiquidDeliverySystemIndex_2, 700u );
    bm.AssignBottleToLiquidStation( "Vodka", E_LiquidDeliverySystemIndex_3, 700u );

    for (;;)
    {
        /* Poll order */
        order.orderId = 0;
        order.ingredients.clear();
        communicationInterface.getOrder( &order );
        if (order.orderId != 0)
        {
            ProcessOrder( &order, &bm, &communicationInterface );
        }
        else
        {
            sleep( 1u);
            fprintf( stderr, "No new order \n"  );
        }
    }
    return NULL;
}

static void ProcessOrder(Order_t* pOrder, BottleManagement* pBM, Com* pCom)
{
    LiquidDeliverySystem sps( "192.168.1.120", 200u );
    unsigned int numberIceCubes;
    MicroControllerCommunication uCCom;
    LiquidDeliverySystemIndex_e stationIndex;
    std::string liquidName; 
    
    fprintf( stderr, "New order:%d \n", pOrder->orderId );

    /* Ice necessary */
    numberIceCubes = NumberIceCubesToDeliver( pOrder );
    if (numberIceCubes != 0u)
    {
        uCCom.SendDeliverIceCube( numberIceCubes );
        fprintf( stderr, "Order:%d with %d ice cubes\n", pOrder->orderId, numberIceCubes );
    }
    else
    {
        fprintf( stderr, "Order:%d no ice\n", pOrder->orderId );
    }
    /* Deliver all liquids. */
    for (unsigned int i = 0u; i < pOrder->ingredients.size( ); i++)
    {
        /* Look up bottles */
        liquidName = pOrder->ingredients[i].name;
        stationIndex = pBM->GetLiquidStationIndex( liquidName );
        fprintf( stderr, "Index:%d name:%s amount:%d\n", stationIndex, liquidName.c_str( ),
                 pOrder->ingredients[i].amount );
        
        /* Check delivering */
        if (!pBM->DeliveryPossible( stationIndex, pOrder->ingredients[i].amount ))
        {
            /* TODO: Wait for change */
            fprintf( stderr, "Please change bottle:%d %s \n", stationIndex,
                     pOrder->ingredients[i].name.c_str( ));
        }
        
        /* Deliver liquid*/
        fprintf( stderr, "Delivery started..\n" );
        
        sps.DeliverVolume( stationIndex, pOrder->ingredients[i].amount );
        pBM->UpdateFillLevel( stationIndex, pOrder->ingredients[i].amount );
        /* Wait delivery done */
        
        fprintf( stderr, "Delivery %s finished %d..\n", liquidName.c_str( ),
                 (int) sps.CheckDeliveryDoneSuccessfull( ));
    }
    /* Push finishing */
    pCom->respondDone( pOrder->orderId );
    /* Wait glass removed */
    sps.WaitClassRemoved( );
}
