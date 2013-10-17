#ifndef _BOTTLE_MANAGEMENT_H_
#define _BOTTLE_MANAGEMENT_H_

/************************************************************
    Includes
*************************************************************/

#include <vector>
#include <string>
#include "../inc/LiquidDeliverySystem.h"

/************************************************************
    Class definition
*************************************************************/

class BottleManagement
{
 public:
    BottleManagement( );
    LiquidDeliverySystemIndex_e GetLiquidStationIndex(std::string liquidName);
    void AssignBottleToLiquidStation(std::string liquidName, LiquidDeliverySystemIndex_e stationIndex);

 private:
    std::vector<std::string> lookUpTable;
};

#endif
