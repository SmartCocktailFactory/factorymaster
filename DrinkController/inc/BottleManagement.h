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

 private:
  struct LiquidLookUpTable_t
  {
    std::string liquidName;
    LiquidDeliverySystemIndex_e index;
  };
  std::vector<LiquidLookUpTable_t> lookUpTable;

  
};

#endif
