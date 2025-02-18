#include "PathsMap.h"
#include "math.h"
#include <cmath>
#include <iostream>
#define DEFAULT_DISTANCE 1000
#define BUILDING_FACTOR 4
#define BUILDING_COEF 50
#define ENEMY_COEF 100
#define HQ_COEF 300

using namespace ai;
using namespace std;

std::vector<std::pair<int,int>> PathsMap::_map = {};

PathsMap::PathsMap (state::UnitInstance* unit, int height, int width)
{
    _unit = unit;
    _height = height;
    _width = width;
    _reachablePositions = {};
}

PathsMap::~PathsMap () {}


std::vector<Position> PathsMap::getReachablePositions ()
{
    Position unitPosition(_unit->getX(), _unit->getY());
    for (auto pairPosition : _map)
    {
        Position* pos = new Position(pairPosition.first, pairPosition.second);
        if (distance(*pos, unitPosition) <= _unit->getMoveRange())
        {
            _reachablePositions.push_back(*pos);
        }
        else
        {
            delete (pos);
        }
    }
    return _reachablePositions;
}

void PathsMap::spreadScore (Position p, int score){};

double PathsMap::distance (Position a, Position b) const
{
    return sqrt ( pow(a.getX() - b.getX(), 2) + pow(a.getY() - b.getY(),2) );
}
double PathsMap::distance (std::pair<int,int> a, std::pair<int,int> b) const
{
    return sqrt ( pow(a.first - b.first, 2) + pow(a.second - b.second,2) );
}

void PathsMap::init()
{
    for (size_t i = 0; i < _width - 1; i++)
        for (size_t j =0 ; j < _height - 1; j++)
        {
            _map.push_back(std::pair<int,int>(i, j));
        }
}

std::pair<int,int> PathsMap::findClosestEnemy(state::State& state)
{
    double minDist = DEFAULT_DISTANCE;
    std::pair<int,int> closestEnemyPosition = {-1, -1};
    std::pair<int,int> unitPosition = {_unit->getX(), _unit->getY()};
    for (auto enemyPosition : state.showEnemies())
    {
        if (double dist = distance(enemyPosition, unitPosition)<minDist)
        {
            minDist = dist;
            closestEnemyPosition = enemyPosition;
        }
    }
    return closestEnemyPosition;
}

Position PathsMap::bestPosition(state::State& state)
{
    Position bestPosition;
    std::pair<int,int> closest = findClosestEnemy(state);
    int value;
    if (state.getGI(closest.first, closest.second)->getTypeID() > 6)
    {
        value = ENEMY_COEF;
    }
    else if (state.getGI(closest.first, closest.second)->getTypeID() == state::GameInstanceTypeID::HEADQUARTER)
    {
        value = HQ_COEF;
    }
    else if (state.getGI(closest.first, closest.second)->getTypeID() == 4 || 5 || 6)
    {
        value = BUILDING_COEF;
    }
    
    int valueMax = 0;
    for (auto position : getReachablePositions())
    {
 
        if(double dist = distance (position, Position(closest.first, closest.second)) != 0)
        {
            position.setScore (value/dist);
        }    
        else
        {
            if(value == BUILDING_COEF || HQ_COEF)
            {
                position.setScore(value);
            }               
        }
        if(position.getScore()>valueMax)
            valueMax = position.getScore();
            bestPosition = position;
    }

    return bestPosition;

}