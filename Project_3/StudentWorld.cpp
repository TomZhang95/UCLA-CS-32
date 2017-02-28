
#include "StudentWorld.h"
#include "Actor.h"
#include "Field.h"
#include "Compiler.h"
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
	Field f;
	string fieldFileName = getFieldFilename();
	int loadReturn = f.loadField(fieldFileName);
	if (loadReturn != 0)
		return GWSTATUS_LEVEL_ERROR;
    
    vector<string> fileNames = getFilenamesOfAntPrograms();
    
	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 64; y++)
		{
            int pos = (x*100) + y;
            if (f.getContentsOf(x, y) == Field::FieldItem::anthill0 && (0 < fileNames.size()))
            {
                if(!initialAntHill(0, fileNames, x, y))
                    return GWSTATUS_LEVEL_ERROR;
            }
            else if (f.getContentsOf(x, y) == Field::FieldItem::anthill1 && (1 < fileNames.size()))
            {
                if(!initialAntHill(1, fileNames, x, y))
                return GWSTATUS_LEVEL_ERROR;
            }
            else if (f.getContentsOf(x, y) == Field::FieldItem::anthill2 && (2 < fileNames.size()))
            {
                if(!initialAntHill(2, fileNames, x, y))
                    return GWSTATUS_LEVEL_ERROR;
            }
            if (f.getContentsOf(x, y) == Field::FieldItem::anthill3 && (3 < fileNames.size()))
            {
                if(!initialAntHill(3, fileNames, x, y))
                    return GWSTATUS_LEVEL_ERROR;
            }
			else if (f.getContentsOf(x, y) == Field::FieldItem::food)
				arena[pos].push_back(new Food(IID_FOOD, x, y, this));
			else if (f.getContentsOf(x, y) == Field::FieldItem::grasshopper)
				arena[pos].push_back(new BabyGrasshopper(IID_BABY_GRASSHOPPER, x, y, this));
			else if (f.getContentsOf(x, y) == Field::FieldItem::poison)
                arena[pos].push_back(new Poison(IID_POISON, x, y, this));
			else if (f.getContentsOf(x, y) == Field::FieldItem::rock)
				arena[pos].push_back(new Pebble(IID_ROCK, x, y, this));
			else if (f.getContentsOf(x, y) == Field::FieldItem::water)
				arena[pos].push_back(new PoolofWater(IID_WATER_POOL, x, y, this));
		}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_tick++;
    for (map<int, list<Actor*>>::iterator iter = arena.begin(); iter != arena.end(); iter++)
    {
        for (list<Actor*>::iterator iter_list = iter->second.begin(); iter_list != iter->second.end();)
        {
            int oldX = (*iter_list)->getX();
            int oldY = (*iter_list)->getY();
            if (!(*iter_list)->isDead())
            {
                (*iter_list)->doSomething();
                if ((*iter_list)->getX() != oldX || (*iter_list)->getY() != oldY)
                {
					(*iter_list)->setWasInPool(false);
                    updateDataStructure((*iter_list++), oldX, oldY);
                }
				else
					iter_list++;
            }
            else
            {
                delete *iter_list;
                iter->second.erase(iter_list++);
            }
        }
    }
    setDisplayText();
    if (m_tick == 2000)
    {
        if (m_antNumbers[m_winningPlayer] <= 5)
        return GWSTATUS_NO_WINNER;
        else
            return GWSTATUS_PLAYER_WON;
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (map<int, list<Actor*>>::iterator iter = arena.begin(); iter != arena.end(); iter++)
    {
        for(list<Actor*>::iterator iter_list = iter->second.begin(); iter_list != iter->second.end() ;)
        {
            delete (*iter_list);
            iter->second.erase(iter_list++);
        }
    }
}

/*------------------------- Type Determination -------------------------*/

bool StudentWorld::isFood(Actor* obj)
{
    Actor* tmp = dynamic_cast<Food*>(obj);
    if (tmp != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::isAntHill(Actor* obj)
{
    Actor* tmp = dynamic_cast<AntHill*>(obj);
    if (tmp != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::isInsect(Actor* obj)
{
    Ant* ant = dynamic_cast<Ant*>(obj);
    BabyGrasshopper* bg = dynamic_cast<BabyGrasshopper*>(obj);
    AdultGrasshopper* ag = dynamic_cast<AdultGrasshopper*>(obj);
    if (ant != nullptr || bg != nullptr || ag != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::isAnt(Actor* obj)
{
    Ant* ant = dynamic_cast<Ant*>(obj);
    if (ant != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::isPoison(Actor * obj)
{
	Poison* poison = dynamic_cast<Poison*>(obj);
	if (poison != nullptr)
		return true;
	else
		return false;
}

bool StudentWorld::isWaterPool(Actor* obj)
{
    PoolofWater* water = dynamic_cast<PoolofWater*>(obj);
    if (water != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::PebbleExist(int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
	if (currList.size() == 0)
		return false;
    Actor* tmp = dynamic_cast<Pebble*>((*currList.begin()));
    if (tmp != nullptr)
        return true;
    else
        return false;
}

bool StudentWorld::FoodExist(int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
	if (currList.size() == 0)
		return false;
    for (auto iter_list = currList.begin(); iter_list != currList.end(); iter_list++)
    {
        if(isFood(*iter_list))
            return true;
    }
    return false;
}

bool StudentWorld::dangerExist(Actor* obj, int currX, int currY)
{
	list<Actor*> currList;
	currList = arena[currX * 100 + currY];
	if (currList.size() == 0)
		return false;
	for (auto iter_list = currList.begin(); iter_list != currList.end(); iter_list++)
	{
		if ((isInsect(*iter_list) && (*iter_list)->getColonyNumber() != obj->getColonyNumber()) 
			|| isPoison(*iter_list) || isWaterPool(*iter_list))
			return true;
	}
	return false;
}

bool StudentWorld::pheromoneExist(Actor * obj, int currX, int currY)
{
	list<Actor*> currList;
	currList = arena[currX * 100 + currY];
	if (currList.size() == 0)
		return false;
	for (auto iter_list = currList.begin(); iter_list != currList.end(); iter_list++)
	{
		if (isPheromon(*iter_list))
			return true;
	}
	return false;
}

bool StudentWorld::isNotItSelf(Actor* itSelf, Actor* obj, int currX, int currY)
{
    if (obj != itSelf)
        return true;
    else
        return false;
}

bool StudentWorld::isPheromon(Actor* obj)
{
	Pheromone* tmp = dynamic_cast<Pheromone*>(obj);
	if (tmp != nullptr)
		return true;
	else
		return false;
}

bool StudentWorld::isDangerInFront(Actor* obj, int currX, int currY)
{
	switch (obj->getDirection())
	{
	case Actor::up:
		currY += 1;
		break;
	case Actor::right:
		currX += 1;
		break;
	case Actor::down:
		currY -= 1;
		break;
	case Actor::left:
		currX -= 1;
		break;
	default:
		break;
	};

	if (dangerExist(obj, currX, currY))
		return true;
	else
		return false;
	
}

bool StudentWorld::isPheromoneInFront(Actor * obj, int currX, int currY)
{
	switch (obj->getDirection())
	{
	case Actor::up:
		currY += 1;
		break;
	case Actor::right:
		currX += 1;
		break;
	case Actor::down:
		currY -= 1;
		break;
	case Actor::left:
		currX -= 1;
		break;
	default:
		break;
	};

	if (pheromoneExist(obj, currX, currY))
		return true;
	else
		return false;
}

bool StudentWorld::isStandingOnMyAntHill(Actor * obj, int currX, int currY)
{
	list<Actor*> currList;
	currList = arena[currX * 100 + currY];
	if (currList.size() == 0)
		return false;
	for (auto iter_list = currList.begin(); iter_list != currList.end(); iter_list++)
	{
		if (isAntHill(*iter_list))
			if((*iter_list)->getColonyNumber() == obj->getColonyNumber())
				return true;
	}
	return false;
}

bool StudentWorld::isStandingWithEnemy(Actor* obj, int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX * 100 + currY];
    if (currList.size() == 0)
        return false;
    for (auto iter_list = currList.begin(); iter_list != currList.end(); iter_list++)
    {
        if (isInsect(*iter_list))
            if((*iter_list)->getColonyNumber() == obj->getColonyNumber())
                return true;
    }
    return false;
}


/*------------------------- Supporting Functions -------------------------*/

void StudentWorld::updateDataStructure(Actor* currObj, int oldX, int oldY)
{
    int newX = currObj->getX();
    int newY = currObj->getY();
    arena[100*newX + newY].push_back(currObj);
    for (list<Actor*>::iterator iter_list = arena[100*oldX+ oldY].begin();
         iter_list != arena[100*oldX + oldY].end();)
    {
        if((*iter_list)->getX() == newX && (*iter_list)->getY() == newY)
            arena[100*oldX+ oldY].erase(iter_list++);
        else
            iter_list++;
    }
}

int StudentWorld::pickFood(Actor* obj, int amount, int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
	if (currList.size() == 0)
		return 0;
    auto iter_list = currList.begin();
    while(iter_list != currList.end())
    {
        if (isFood(*iter_list))
            break;
        else
            iter_list++;
    }
    
    Actor* currActor;
    currActor = (*iter_list);
	if (isAnt(obj))
		if (obj->getFood() + 400 > 1800)
			amount = 1800 - obj->getFood();
    if (currActor->getHp() <= amount)
    {
        amount = currActor->getHp();
        currActor->setHp(0);
        return amount;
    }
    else
    {
        currActor->setHp(currActor->getHp() - amount);
        return amount;
    }
}

void StudentWorld::addNewAnt(Actor* obj, Compiler* comp)
{
    switch (obj->getColonyNumber())
    {
        case 0:
            arena[(obj->getX())*100 + obj->getY()].push_back(new Ant(IID_ANT_TYPE0, obj->getX(), obj->getY(), this, 1500, Ant::Direction::right, 0, 0, comp));
            m_colonyName.push_back(comp->getColonyName());
            break;
        case 1:
            arena[(obj->getX())*100 + obj->getY()].push_back(new Ant(IID_ANT_TYPE1, obj->getX(), obj->getY(), this, 1500, Ant::Direction::right, 0, 1, comp));
            m_colonyName.push_back(comp->getColonyName());
            break;
        case 2:
            arena[(obj->getX())*100 + obj->getY()].push_back(new Ant(IID_ANT_TYPE2, obj->getX(), obj->getY(), this, 1500, Ant::Direction::right, 0, 2, comp));
            m_colonyName.push_back(comp->getColonyName());
            break;
        case 3:
            arena[(obj->getX())*100 + obj->getY()].push_back(new Ant(IID_ANT_TYPE3, obj->getX(), obj->getY(), this, 1500, Ant::Direction::right, 0, 3, comp));
            m_colonyName.push_back(comp->getColonyName());
            break;
        default:
            break;
    }
}

void StudentWorld::addNewAdultGrasshopper(int currX, int currY)
{
    arena[100*currX + currY].push_back(new AdultGrasshopper(IID_ADULT_GRASSHOPPER,
                                                            currX, currY, this));
    
}

void StudentWorld::setAntNumbers(int colonyNumber, int num)
{
    switch (colonyNumber)
    {
        case 0:
            m_antNumbers[0] += num;
            break;
        case 1:
            m_antNumbers[1] += num;
            break;
        case 2:
            m_antNumbers[2] += num;
            break;
        case 3:
            m_antNumbers[3] += num;
            break;
    };
}

void StudentWorld::setAllInsectInPool(int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
    auto iter_list = currList.begin();
    while(iter_list != currList.end())
    {
        if (isInsect(*iter_list) && (*iter_list)->getWasInPool() == false)
        {
            (*iter_list)->setWasInPool(true);
            (*iter_list)->setStunned((*iter_list)->getStunned() + 2);
            iter_list++;
        }
        else
            iter_list++;
    }

}

void StudentWorld::setAllInsectPoisoned(Actor* currPoison, int currX, int currY)
{
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
    auto iter_list = currList.begin();
	Actor* obj = (*iter_list);
    while(iter_list != currList.end())
    {
        AdultGrasshopper* ag = dynamic_cast<AdultGrasshopper*>(obj);
        if (isInsect(*iter_list) && (ag == nullptr) &&
            (currPoison->getCountTick() > (*iter_list)->getCountTick()))
        {
            int amount = 150;
            if ((*iter_list)->getHp() < 150)
                amount = (*iter_list)->getHp();
            (*iter_list)->setHp((*iter_list)->getHp() - amount);
            iter_list++;
        }
        else
            iter_list++;
    }
}

void StudentWorld::checkAndAddFood(Actor* obj, int currX, int currY, int amount)
{
    if (!FoodExist(currX, currY))
    {
        arena[100*currX + currY].push_back(new Food(IID_FOOD, currX, currY, this, amount));
        if (isAnt(obj))
            obj->setFood(0);
    }
    else
    {
        list<Actor*> currList;
        currList = arena[currX*100 + currY];
        auto iter_list = currList.begin();
        while(iter_list != currList.end())
        {
            if (isFood(*iter_list))
            {
                (*iter_list)->setHp((*iter_list)->getHp() + amount);
                if (isAnt(obj))
                    obj->setFood(0);
                return;
            }
            iter_list++;
        }

    }
}

bool StudentWorld::checkStatusAndShouldDoSth(Actor* obj, int currX, int currY)
{
    if (obj->isCalled(obj))
        return false;
    else
        obj->setCountTick(obj->getCountTick() + 1);
    
    obj->setHp(obj->getHp() - 1);
    if (obj->isDead())
    {
        checkAndAddFood(obj, currX, currY, 100);
        if (isAnt(obj))
            setAntNumbers(obj->getColonyNumber(), -1);
        return false;
    }
    if (obj->getStunned() != 0)
    {
        obj->setStunned(obj->getStunned() - 1);
        return false;
    }
    return true;
}

int StudentWorld::checkAndEatOrPickFood(int currX, int currY, int amount)
{
    if (FoodExist(currX, currY))
    {
        list<Actor*> currList;
        currList = arena[currX*100 + currY];
        auto iter_list = currList.begin();
        while(iter_list != currList.end())
        {
            if (isFood(*iter_list))
                break;
            iter_list++;
        }
        if (amount > (*iter_list)->getHp())
            amount = (*iter_list)->getHp();
        (*iter_list)->setHp((*iter_list)->getHp() - amount);
        return amount;
    }
    else
        return 0;
}

bool StudentWorld::bitOthers(Actor* itSelf, int currX, int currY, int amount)
{
    bool Ant = isAnt(itSelf);
    int count = 0;
    int whichToBit;
    list<Actor*> currList;
    currList = arena[currX*100 + currY];
    auto iter_list = currList.begin();
    while(iter_list != currList.end())
    {
        if (isNotItSelf(itSelf, (*iter_list), currX, currY) && isInsect((*iter_list)))
            if((*iter_list)->getHp() != 0)
            {
                if (!Ant)
                    count++;
                else
                {
                    if(itSelf->getColonyNumber() != (*iter_list)->getColonyNumber())
                        count++;
                }
            }
        iter_list++;
    }
    
    if ( count == 0)
        return false;
    
    whichToBit = randInt(1, count);
    iter_list = currList.begin();
    int i = 0;
    while (i != whichToBit)
    {
        if (isNotItSelf(itSelf, (*iter_list), currX, currY) && isInsect((*iter_list)))
            if((*iter_list)->getHp() != 0)
            {
                if (!Ant)
                    i++;
                else
                {
                    if((itSelf->getColonyNumber() != (*iter_list)->getColonyNumber()))
                        i++;
                }
            }
        if (i == whichToBit)
        {
            (*iter_list)->setHp((*iter_list)->getHp() - amount);
            (*iter_list)->setWasBit(true);
			return true;
        }
        iter_list++;
    }
	return false;
}

void StudentWorld::jump(Actor* itSelf, int currX, int currY)
{
	int counter = 0;
    double r;
	int x, y, newX = currX, newY = currY;
    do
    {
		if (counter > 1000)
			break;
        x = randInt(-10, 10);
        y = randInt(-10, 10);
		newX = currX + x;
		newY = currY + y;
        r = pow(x, 2) + pow(y, 2);
		counter++;
    }while (r > 100 || newX < 0 || newX > 63 ||
		newY < 0 || newY > 63 || (PebbleExist(currX + x, currY + y)));
	itSelf->moveTo(newX, newY);
}

bool StudentWorld::initialAntHill(int colonyNumber, vector<string> fileNames,
                                  int currX, int currY)
{
    Compiler *compilerForEntrant0, *compilerForEntrant1,
    *compilerForEntrant2, *compilerForEntrant3;
    Compiler* currComp = nullptr;
    string error;
    if (colonyNumber == 0)
    {
        compilerForEntrant0 = new Compiler;
        currComp = compilerForEntrant0;
    }
    if (colonyNumber == 1)
    {
        compilerForEntrant1 = new Compiler;
        currComp = compilerForEntrant1;
    }
    if (colonyNumber == 2)
    {
        compilerForEntrant2 = new Compiler;
        currComp = compilerForEntrant2;
    }
    if (colonyNumber == 3)
    {
        compilerForEntrant3 = new Compiler;
        currComp = compilerForEntrant3;
    }
    if ( !compilerForEntrant0->compile(fileNames[colonyNumber], error) )
    {
        setError(fileNames[colonyNumber] + " " + error);
        return false;
    }
    list<Actor*> tmp;
    arena[currX*100 + currY].push_back(new AntHill(IID_ANT_HILL, currX, currY, this, colonyNumber, currComp));
    return true;
}

void StudentWorld::addNewPheromone(Actor* obj, int currX, int currY)
{
	list<Actor*> currList;
	currList = arena[currX * 100 + currY];
	auto iter_list = currList.begin();
	while (iter_list != currList.end())
	{
		if (isPheromon(*iter_list))
		{
			int currC = (*iter_list)->getColonyNumber();
			int objC = obj->getColonyNumber();
			if ( currC == objC )
			{
				(*iter_list)->setHp((*iter_list)->getHp() + 256);
				return;
			}
		}
		iter_list++;
	}
    if (obj->getColonyNumber() == 0)
        arena[currX*100 + currY].push_back(new Pheromone(IID_PHEROMONE_TYPE0, currX, currY, this, 0));
    else if (obj->getColonyNumber() == 1)
        arena[currX*100 + currY].push_back(new Pheromone(IID_PHEROMONE_TYPE1, currX, currY, this, 1));
    else if (obj->getColonyNumber() == 2)
        arena[currX*100 + currY].push_back(new Pheromone(IID_PHEROMONE_TYPE2, currX, currY, this, 2));
    else if (obj->getColonyNumber() == 3)
        arena[currX*100 + currY].push_back(new Pheromone(IID_PHEROMONE_TYPE3, currX, currY, this, 3));
}

void StudentWorld::setDisplayText()
{
    int winningAntAmounts = m_antNumbers[0];
    string s;
    ostringstream oss;
    if (m_colonyName.size() < 4)
    {
        unsigned long size = m_colonyName.size();
        for (int i = 0; i < (4 - size); i++)
            m_colonyName.push_back("-----");
    }
    
    vector<string> tmp(m_colonyName);
    
    if (winningAntAmounts < m_antNumbers[1])
    {
        m_winningPlayer = 1;
        winningAntAmounts = m_antNumbers[1];
    }
    if (winningAntAmounts < m_antNumbers[2])
    {
        m_winningPlayer = 2;
        winningAntAmounts = m_antNumbers[2];
    }
    if (winningAntAmounts < m_antNumbers[3])
    {
        m_winningPlayer = 3;
        winningAntAmounts = m_antNumbers[3];
    }
    setWinner(tmp[m_winningPlayer]);
    tmp[m_winningPlayer] += '*';
        oss << "Ticks:" << setw(5) << m_tick << " -  ";
    oss.fill('0');
    oss << tmp[0] << ": " << setw(2) << m_antNumbers[0] << "  "
    << tmp[1] << ": " << setw(2) << m_antNumbers[1] << "  "
    << tmp[2] << ": " << setw(2) << m_antNumbers[2] << "  "
    << tmp[3] << ": " << setw(2) << m_antNumbers[3] << "  ";
    s = oss.str();
    setGameStatText(s);
}
