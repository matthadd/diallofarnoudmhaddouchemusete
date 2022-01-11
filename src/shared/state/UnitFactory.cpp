#include "UnitFactory.h"

using namespace std;

namespace state{

    UnitFactory::UnitFactory (){}

    UnitFactory::~UnitFactory(){}

    GameInstance* UnitFactory::createGI(GameInstanceTypeID typeID, int playerID)
    {
        if(typeID == GameInstanceTypeID::BAT){
            return (GameInstance*) new UnitInstance(typeID, playerID, 10, 10, 4, 10, 20); 
        }
        else if(typeID == GameInstanceTypeID::CYCLOPS)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 100, 100, 1, 1, 200);
        }
        else if(typeID == GameInstanceTypeID::WITCH)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 30, 40, 2, 1,70);
        }
        else if(typeID == GameInstanceTypeID::DWARF)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 50, 50, 1, 2, 50); 
        }
        else
        {
            return NULL;
        }
    }

    GameInstance* UnitFactory::createGIat(GameInstanceTypeID typeID, int playerID, int x, int y)
    {
        if(typeID == GameInstanceTypeID::BAT){
            return (GameInstance*) new UnitInstance(typeID, playerID, 10, 10, 4, 10, 20, x, y); 
        }
        else if(typeID == GameInstanceTypeID::CYCLOPS)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 100, 100, 1, 1, 200, x, y);
        }
        else if(typeID == GameInstanceTypeID::WITCH)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 30, 40, 2, 1,70, x, y);
        }
        else if(typeID == GameInstanceTypeID::DWARF)
        {
            return (GameInstance*) new UnitInstance(typeID, playerID, 50, 50, 1, 2, 50, x, y); 
        }
        else
        {
            return NULL;
        }
    }
}