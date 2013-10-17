/************************************************************
    Includes
*************************************************************/
#include "../inc/BottleManagement.h"

/************************************************************
    Local types
*************************************************************/

/************************************************************
     Local variables 
*************************************************************/

/************************************************************
    Local function prototypes
*************************************************************/

/************************************************************
    Public functions 
*************************************************************/
  
BottleManagement::BottleManagement( )
{
    for (int i = 0; i < E_LiquidDeliverySystemIndex_Invalid; i++)
    {
        lookUpTable.push_back( "" );
    }
}

LiquidDeliverySystemIndex_e BottleManagement::GetLiquidStationIndex(std::string liquidName)
{
    for (unsigned int i = 0u; i < lookUpTable.size( ); i++)
    {
        if (liquidName.compare(lookUpTable[i]) == 0)
        {
            return (LiquidDeliverySystemIndex_e)i;
        }
    }
    return E_LiquidDeliverySystemIndex_Invalid;
}

void BottleManagement::AssignBottleToLiquidStation(std::string liquidName, LiquidDeliverySystemIndex_e stationIndex)
{
    lookUpTable[stationIndex] = liquidName;
}
