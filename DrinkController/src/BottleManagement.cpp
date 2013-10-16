#include "../inc/BottleManagement.h"

  
BottleManagement::BottleManagement( )
{
  LiquidLookUpTable_t t;
  t.liquidName = "Vodka";
  t.index = E_LiquidDeliverySystemIndex_1;
  lookUpTable.push_back( t );
  t.liquidName = "Baccardi";
  t.index = E_LiquidDeliverySystemIndex_2;
  lookUpTable.push_back( t );
  t.liquidName = "Cola";
  t.index = E_LiquidDeliverySystemIndex_3;
  lookUpTable.push_back( t );
}

LiquidDeliverySystemIndex_e BottleManagement::GetLiquidStationIndex(std::string liquidName)
{
  for (unsigned int i = 0u; i < lookUpTable.size( ); i++)
    {
      if (liquidName.compare(lookUpTable[i].liquidName) == 0)
	{
	  return lookUpTable[i].index;
	}
    }
  return E_LiquidDeliverySystemIndex_Invalid;
}
