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
    BottleData_t bd;
    for (int i = 0; i < E_LiquidDeliverySystemIndex_Invalid; i++)
    {
        bd.name = "";
        bd.fillLevel = 0u;
        lookUpTable.push_back( bd );
    }
}

LiquidDeliverySystemIndex_e BottleManagement::GetLiquidStationIndex(std::string liquidName)
{
    for (unsigned int i = 0u; i < lookUpTable.size( ); i++)
    {
        if (liquidName.compare(lookUpTable[i].name) == 0)
        {
            return (LiquidDeliverySystemIndex_e)i;
        }
    }
    return E_LiquidDeliverySystemIndex_Invalid;
}

void BottleManagement::AssignBottleToLiquidStation(std::string liquidName,
                                                   LiquidDeliverySystemIndex_e stationIndex,
                                                   unsigned int fillLevelInMl)
{
    lookUpTable[stationIndex].name = liquidName;
    lookUpTable[stationIndex].fillLevel = fillLevelInMl;
}

void BottleManagement::UpdateFillLevel(LiquidDeliverySystemIndex_e stationIndex,
                                       unsigned int deliveredLiquidInMl)
{
    if (lookUpTable[stationIndex].fillLevel > deliveredLiquidInMl)
    {
        lookUpTable[stationIndex].fillLevel -= deliveredLiquidInMl;
    }
}

bool BottleManagement::DeliveryPossible(LiquidDeliverySystemIndex_e stationIndex,
                                       unsigned int liquidInMlToDeliver)
{
    bool ret = false;
    if (lookUpTable[stationIndex].fillLevel > liquidInMlToDeliver)
    {
        ret = true;
    }
    return ret;
}
