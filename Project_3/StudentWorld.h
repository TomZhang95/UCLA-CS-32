#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), m_tick(0)
	{}
    
    ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();
    
    /*------------ Type Determination ------------*/
    bool isFood(Actor* obj);
    bool isAntHill(Actor* obj);
    bool isInsect(Actor* obj);
    bool isAnt(Actor* obj);
	bool isPoison(Actor* obj);
    bool isWaterPool(Actor* obj);
    bool PebbleExist(int currX, int currY);
    bool FoodExist(int currX, int currY);
	bool dangerExist(Actor* obj, int currX, int currY);
	bool pheromoneExist(Actor* obj, int currX, int currY);
    bool isNotItSelf(Actor* itSelf, Actor* obj, int currX, int currY);
	bool isPheromon(Actor* obj);
	bool isDangerInFront(Actor* obj, int currX, int currY);
	bool isPheromoneInFront(Actor* obj, int currX, int currY);
	bool isStandingOnMyAntHill(Actor* obj, int currX, int currY);
    bool isStandingWithEnemy(Actor* obj, int currX, int currY);
    
    /*------------ Supporting Functions ------------*/
    int getTick();
    void updateDataStructure(Actor* currVec, int oldX, int oldY);
    int pickFood(Actor* obj, int amount, int currX, int currY);
    void addNewAnt(Actor* obj, Compiler* comp);
    void addNewAdultGrasshopper(int currX, int currY);
    void setAntNumbers(int colonyNumber, int num);
    void setAllInsectInPool(int currX, int currY);
    void setAllInsectPoisoned(Actor* currPoison, int currX, int currY);
    void checkAndAddFood(Actor* obj, int currX, int currY, int amount);
    bool checkStatusAndShouldDoSth(Actor* obj, int currX, int currY);
    int checkAndEatOrPickFood(int currX, int currY, int amount);
    bool bitOthers(Actor* itSelf, int currX, int currY, int amount);
    void jump(Actor* itSelf, int currX, int currY);
    bool initialAntHill(int colonyNumber, std::vector<std::string> fileNames, int currX, int currY);
    void addNewPheromone(Actor* obj, int currX, int currY);
    void setDisplayText();

private:
    std::map<int, std::list<Actor*>> arena;
    unsigned int m_tick, countCompiler, m_winningPlayer = 0;
    std::vector<int> m_antNumbers = {0, 0, 0, 0};
    std::vector<std::string> m_colonyName;
};

inline int StudentWorld::getTick() { return m_tick; }

#endif // STUDENTWORLD_H_
