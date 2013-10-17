#ifndef _BOTTLE_MANAGEMENT_H_
#define _BOTTLE_MANAGEMENT_H_

/************************************************************
    Includes
*************************************************************/
#include <vector>
#include <string>
#include "../inc/LiquidDeliverySystem.h"

/************************************************************
    Types
*************************************************************/
typedef struct
{
    std::string name;
    unsigned int fillLevel;
} BottleData_t;

/************************************************************
    Class definition
*************************************************************/

class BottleManagement
{
 public:
    static BottleManagement* GetInstance()
    {
        if (instanz == NULL)
        {
            instanz = new BottleManagement( );
        }
        return instanz;
    }
    LiquidDeliverySystemIndex_e GetLiquidStationIndex(std::string liquidName);
    void AssignBottleToLiquidStation(std::string liquidName, LiquidDeliverySystemIndex_e stationIndex,
                                     unsigned int fillLevelInMl);
    void UpdateFillLevel(LiquidDeliverySystemIndex_e stationIndex, unsigned int deliveredLiquidInMl);
    bool DeliveryPossible(LiquidDeliverySystemIndex_e stationIndex, unsigned int liquidInMlToDeliver);

 protected:
    BottleManagement( );
    
 private:
    std::vector<BottleData_t> lookUpTable;
    static BottleManagement *instanz;
};

#endif
