/************************************************************
    Includes
*************************************************************/
#include "../inc/DrinkController.h"
#include "../inc/BottleManagement.h"
#include "../inc/LiquidDeliverySystem.h"
#include "../inc/com.h"
#include "../inc/MicroControllerCommunication.h"
#include "../inc/UIController.h"
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
static void ProcessOrder(Order_t* pOrder,
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
    BottleManagement* pBm = BottleManagement::GetInstance( );
    UIController* pUIController = UIController::GetInstance( );
    
    Com communicationInterface;
    Order_t order;
    pUIController->UpdateGlobalState( E_UIState_Starting );
    communicationInterface.init( );
    pBm->AssignBottleToLiquidStation( "OrangeJuice", E_LiquidDeliverySystemIndex_3, 700u );
    pBm->AssignBottleToLiquidStation( "Bacardi", E_LiquidDeliverySystemIndex_2, 700u );
    pBm->AssignBottleToLiquidStation( "Vodka", E_LiquidDeliverySystemIndex_1, 700u );
    fprintf( stderr, "SCF starting... \n"  );
            
    for (;;)
    {
        /* Poll order */
        order.orderId = 0;
        order.ingredients.clear();
        communicationInterface.getOrder( &order );
        if (order.orderId != 0)
        {
            ProcessOrder( &order, &communicationInterface );
        }
        else
        {
            sleep( 1u);
            pUIController->UpdateGlobalState( E_UIState_WaitingForOrder );
            fprintf( stderr, "No new order \n"  );
        }
    }
    return NULL;
}

static void ProcessOrder(Order_t* pOrder, Com* pCom)
{
    BottleManagement* pBM = BottleManagement::GetInstance( );
    LiquidDeliverySystem* pLiquidDeliverySystem = LiquidDeliverySystem::GetInstance( "192.168.1.120", 200u );
    UIController* pUIController = UIController::GetInstance( );
    unsigned int numberIceCubes;
    MicroControllerCommunication uCCom;
    LiquidDeliverySystemIndex_e stationIndex;
    std::string liquidName; 
    
    fprintf( stderr, "New order:%d \n", pOrder->orderId );
    pUIController->UpdateGlobalState( E_UIState_Delivering );
    pLiquidDeliverySystem->WaitForGlass( );
    pUIController->UpdateGlobalState( E_UIState_Delivering );
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
    for (int j = E_LiquidDeliverySystemIndex_1;j<E_LiquidDeliverySystemIndex_Invalid;j++)
    {
	bool usedStation = true;
    	for (unsigned int i = 0u; i < pOrder->ingredients.size( ); i++)
    	{
            liquidName = pOrder->ingredients[i].name;
            stationIndex = pBM->GetLiquidStationIndex( liquidName );
	    if(stationIndex == (LiquidDeliverySystemIndex_e)j)
            {
		usedStation = false;
		break;
            }
	}
	if(usedStation == false)
	{
	    pUIController->UpdateBottleState( (LiquidDeliverySystemIndex_e)j, E_UIState_Bottle_DeliveringDone );
        }
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
            pUIController->UpdateBottleState( stationIndex, E_UIState_Bottle_Empty );
        }
        
        /* Deliver liquid*/
        fprintf( stderr, "Delivery started..\n" );
        pUIController->UpdateBottleState( stationIndex, E_UIState_Bottle_Delivering );
        pLiquidDeliverySystem->DeliverVolume( stationIndex, pOrder->ingredients[i].amount );
        pBM->UpdateFillLevel( stationIndex, pOrder->ingredients[i].amount );
        /* Wait delivery done */
        pUIController->UpdateBottleState( stationIndex, E_UIState_Bottle_DeliveringDone );
        fprintf( stderr, "Delivery %s finished %d..\n", liquidName.c_str( ),
                 (int) pLiquidDeliverySystem->CheckDeliveryDoneSuccessfull( ));
    }
    /* Push finishing */
    pUIController->UpdateGlobalState( E_UIState_DeliveringDone );
    pCom->respondDone( pOrder->orderId );
    /* Wait glass removed */
    pLiquidDeliverySystem->WaitClassRemoved( );
}
