#ifndef _BOTTLE_MANAGEMENT_H_
#define _BOTTLE_MANAGEMENT_H_

#include <vector>
#include <string>
#include "../inc/LiquidDeliverySystem.h"

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
