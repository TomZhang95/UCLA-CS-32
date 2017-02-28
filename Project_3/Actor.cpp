#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <sstream>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/*--------------------------------------- Actor ---------------------------------------*/
Actor::Actor(int imageID, int startX, int startY, StudentWorld* sw,
             unsigned int depth, int hp, Direction dir, unsigned int currFood,
             unsigned int distance, unsigned int stunned,
	bool wasBit, bool wasBlocked, int lastRandomNumber, unsigned int countTick,
             bool wasInPool)
	:GraphObject(imageID, startX, startY, dir, depth), m_hp(hp), m_food(currFood),
	m_stunned(stunned), m_wasBit(wasBit), m_wasBlocked(wasBlocked), m_lastRandomNumber(lastRandomNumber), m_distance(distance), m_world(sw), m_countTick(countTick),
m_wasInPool(wasInPool)
{
}

Actor::~Actor()
{}

GraphObject::Direction Actor::randomDir()
{
	switch (randInt(1, 4))
	{
	case 1: return up;
	case 2: return right;
	case 3: return down;
	case 4: return left;
	}
	return none;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

bool Actor::isDead()
{
    return m_hp == 0;
}

bool Actor::isCalled(Actor* currObj)
{
    if (currObj->getCountTick() != getWorld()->getTick())
        return true;
    else
        return false;
}

void Actor::checkPebbleAndMove()
{
    if (getDistance() == 0)
    {
        setDirection(randomDir());
        setDistance(randInt(2,10));
    }
    
    int currX = getX();
    int currY = getY();

    switch (getDirection())
    {
        case up:
            if (getWorld()->PebbleExist(currX, currY + 1))
                setDistance(0);
            else
            {
                setDistance(getDistance() - 1);
                moveTo(currX, currY + 1);
            }
            break;
        case right:
            if (getWorld()->PebbleExist(currX + 1, currY))
                setDistance(0);
            else
            {
                setDistance(getDistance() - 1);
                moveTo(currX + 1, currY);
            }
            break;
        case down:
            if (getWorld()->PebbleExist(currX, currY - 1))
                setDistance(0);
            else
            {
                setDistance(getDistance() - 1);
                moveTo(currX, currY - 1);
            }
            break;
        case left:
            if (getWorld()->PebbleExist(currX - 1, currY))
                setDistance(0);
            else
            {
                setDistance(getDistance() - 1);
                moveTo(currX - 1, currY);
            }
            break;
        default:
            break;
    }
}

int Actor::StringToInt(string s)
{
    stringstream ss;
    int i;
    ss << s;
    ss >> i;
    return i;
}

void Actor::rotateClockwise()
{
    switch (getDirection())
    {
        case up:
            setDirection(right);
            break;
        case right:
            setDirection(down);
            break;
        case down:
            setDirection(left);
            break;
        case left:
            setDirection(up);
            break;
        default:
            randomDir();
    }
}

void Actor::rotateCounterClockwise()
{
    switch (getDirection())
    {
        case up:
            setDirection(left);
            break;
        case right:
            setDirection(up);
            break;
        case down:
            setDirection(right);
            break;
        case left:
            setDirection(down);
            break;
        default:
            randomDir();
    }
}

void Actor::moveForward()
{
    int currX = getX();
    int currY = getY();
    
    switch (getDirection())
    {
        case up:
            if (getWorld()->PebbleExist(currX, currY + 1))
            {
                setWasBlocked(true);
                break;
            }
            moveTo(currX, currY + 1);
            setWasBlocked(false);
            break;
        case right:
            if (getWorld()->PebbleExist(currX + 1, currY))
            {
                setWasBlocked(true);
                break;
            }
            moveTo(currX + 1, currY);
            setWasBlocked(false);
            break;
        case down:
            if (getWorld()->PebbleExist(currX, currY - 1))
            {
                setWasBlocked(true);
                break;
            }
            moveTo(currX, currY - 1);
            setWasBlocked(false);
            break;
        case left:
            if (getWorld()->PebbleExist(currX - 1, currY))
            {
                setWasBlocked(true);
                break;
            }
            moveTo(currX - 1, currY);
            setWasBlocked(false);
            break;
        default:
            break;
    }
}


/*---------------------------------------- Ant ----------------------------------------*/

Ant::Ant(int imageID, int startX, int startY, StudentWorld* sw,
	int hp, Direction dir, unsigned int currFood, unsigned int colonyNumber,
         Compiler* compilerObject,
	unsigned int distance, unsigned int stunned, bool wasBit,
	bool wasBlocked, int lastRandomNumber, unsigned int  instructionCounter,
         unsigned int depth)
	:Actor(imageID, startX, startY, sw, depth,
		hp, dir, currFood, distance, stunned, wasBit, wasBlocked, lastRandomNumber), 
	m_instructionCounter(0), m_colonyNumber(colonyNumber), m_compilerObject(compilerObject)
{
    setDirection(randomDir());
    setCountTick(getWorld()->getTick() - 1);
    setCountTick(getWorld()->getTick());
}

Ant::~Ant()
{
}

void Ant::doSomething()
{
    if (!getWorld()->checkStatusAndShouldDoSth(this, getX(), getY()))
        return;
    if(!interpreter())
        return;
}

bool Ant::interpreter()
{
    Compiler::Command cmd;
    int i = 0;
    while (i < 10)
    {
		if (!(m_compilerObject->getCommand(getIC(), cmd)))
			return false;
        int op1 = StringToInt(cmd.operand1);
        int op2 = StringToInt(cmd.operand2);
		Compiler::Opcode currCode = cmd.opcode;
        switch (currCode)
        {
            case Compiler::invalid:
                setIC(getIC() + 1);
                break;
            case Compiler::label:
                setIC(getIC() + 1);
                break;
            case Compiler::goto_command:
                setIC(op1);
                i++;
                break;
            case Compiler::emitPheromone:
                getWorld()->addNewPheromone(this, getX(), getY());
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::faceRandomDirection:
                setDirection(randomDir());
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::rotateClockwise:
                rotateClockwise();
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::rotateCounterClockwise:
                rotateCounterClockwise();
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::moveForward:
                moveForward();
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::bite:
                getWorld()->bitOthers(this, getX(), getY(), 15);
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::pickupFood:
				if (getFood() < 1800)
					setFood(getFood() + getWorld()->pickFood(this, 400, getX(), getY()));
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::dropFood:
                getWorld()->checkAndAddFood(this, getX(), getY(), getFood());
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::eatFood:
                if (getFood() < 100)
                {
                    setHp(getHp() + getFood());
                    setFood(0);
                }
                else
                {
                    setHp(getHp() + 100);
                    setFood(getHp() - 100);
                }
                setIC(getIC() + 1);
                i++;
                return true;
            case Compiler::generateRandomNumber:
                setLastRandomNumber(randInt(0, op1 - 1));
                setIC(getIC() + 1);
                i++;
                break;
			case Compiler::if_command:
				if (op1 == 0)
				{
					if (getWorld()->isDangerInFront(this, getX(), getY()))
                        setIC(op2);
					else
					{
						setIC(getIC() + 1);
						i++;
					}
				}
				else if (op1 == 1)
				{
					if (getWorld()->isPheromoneInFront(this, getX(), getY()))
						setIC(op2);
					else
					{
						setIC(getIC() + 1);
						i++;
					}
				}
				else if (op1 == 2)
                {
                    if (getWasBit())
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
				else if (op1 == 3)
                {
                    if (getFood() != 0)
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
				else if (op1 == 4)
                {
                    if (getHp() <= 25)
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
				else if (op1 == 5)
                {
                    if (getWorld()->isStandingOnMyAntHill(this, getX(), getY()))
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
                else if (op1 == 6)
                {
                    if (getWorld()->FoodExist(getX(), getY()))
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
                else if (op1 == 7)
                {
                    if(getWorld()->isStandingOnMyAntHill(this, getX(), getY()))
                       setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
                else if (op1 == 8)
                {
                    if (getWasBlocked())
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
                else if (op1 == 9)
                {
                    if (getLastRandomNumber())
                        setIC(op2);
                    else
                    {
                        setIC(getIC() + 1);
                        i++;
                    }
                }
				else
				{
					setIC(getIC() + 1);
					i++;
				}
				break;
            default:
                setIC(getIC() + 1);
                break;
                
        };
    }
        return true;
}

/*---------------------------------------- Pebble ----------------------------------------*/

Pebble::Pebble(int imageID, int startX, int startY, StudentWorld* sw)
	:Actor(imageID, startX, startY, sw)
{
}

Pebble::~Pebble()
{
}

void Pebble::doSomething()
{
}

/*---------------------------------------- Food ----------------------------------------*/

Food::Food(int imageID, int startX, int startY, StudentWorld* sw, int hp, 
           Direction dir, int currFood, unsigned int depth)
	:Actor(imageID, startX, startY, sw, depth, hp, dir, currFood)
{
}

Food::~Food()
{}

void Food::doSomething()
{
}

/*---------------------------------------- Pheromone ----------------------------------------*/

Pheromone::Pheromone(int imageID, int startX, int startY, StudentWorld* sw, int num,
                     unsigned int depth, int hp)
	:Actor(imageID, startX, startY, sw, depth, hp), m_num(num)
{
}

Pheromone::~Pheromone()
{}

void Pheromone::doSomething()
{
    setHp(getHp()-1);
}

/*---------------------------------------- AntHill ----------------------------------------*/

AntHill::AntHill(int imageID, int startX, int startY, StudentWorld* sw,
                 int colonyNumber, Compiler* compilerObject,
	unsigned int depth, int hp)
	:Actor(imageID, startX, startY, sw, depth, hp), m_colonyNumber(colonyNumber),
	m_compilerObject(compilerObject)
{
}

AntHill::~AntHill()
{}

void AntHill::doSomething()
{
    setHp(getHp()-1);
    if (getWorld()->FoodExist(getX(), getY()))
    {
        int foodPicked = getWorld()->pickFood(this, 10000, getX(), getY());
        setHp(getHp() + foodPicked);
        return;
    }
    if (getHp() >= 2000)
    {
        getWorld()->addNewAnt(this, getCompilerObject());
        setHp(getHp() - 1500);
        getWorld()->setAntNumbers(getColonyNumber(), 1);
    }

}

/*---------------------------------- PoolofWater -----------------------------------*/

PoolofWater::PoolofWater(int imageID, int startX, int startY, StudentWorld* sw,
                         unsigned int depth)
	:Actor(imageID, startX, startY, sw, depth)
{
}

PoolofWater::~PoolofWater()
{
}

void PoolofWater::doSomething()
{
    getWorld()->setAllInsectInPool(getX(), getY());
}

/*---------------------------------------- Poison ----------------------------------------*/

Poison::Poison(int imageID, int startX, int startY, StudentWorld* sw, unsigned int depth)
	:Actor(imageID, startX, startY, sw, depth)
{
}

Poison::~Poison()
{
}

void Poison::doSomething()
{
    getWorld()->setAllInsectPoisoned(this, getX(), getY());
}

/*---------------------------------- BabyGrasshopper -----------------------------------*/

BabyGrasshopper::BabyGrasshopper(int imageID, int startX, int startY, StudentWorld* sw,
                                 unsigned int depth, int hp, Direction dir,
                                 int currFood, unsigned int distance)
	:Actor(imageID, startX, startY, sw, depth, hp, dir, currFood, distance)
{
    setDirection(randomDir());
    setDistance(randInt(2,10));
}

BabyGrasshopper::~BabyGrasshopper()
{
}

void BabyGrasshopper::doSomething()
{
    if ((getWorld()->checkStatusAndShouldDoSth(this, getX(), getY())) == false)
        return;
    if (getHp() >= 1600)
    {
        getWorld()->addNewAdultGrasshopper(getX(), getY());
        setHp(0);
        getWorld()->checkAndAddFood(this, getX(), getY(), 100);
        return;
    }
    int foodAte = getWorld()->checkAndEatOrPickFood(getX(), getY(), 200);
    if (foodAte != 0)
    {
        setHp(getHp() + foodAte);
        if(randInt(0, 1))
        {
            setStunned(2);
            return;
        }
    }
    
    checkPebbleAndMove();
    setStunned(2);
    
}

/*---------------------------------- AdultGrasshopper -----------------------------------*/

AdultGrasshopper::AdultGrasshopper(int imageID, int startX, int startY, StudentWorld* sw,
                                   unsigned int depth, int hp, Direction dir,
                                   int currFood, unsigned int distance)
	:Actor(imageID, startX, startY, sw, depth, hp, dir, currFood, distance)
{
    setDirection(randomDir());
    setDistance(randInt(2,10));
    setCountTick(getWorld()->getTick());
}

AdultGrasshopper::~AdultGrasshopper()
{
}

void AdultGrasshopper::doSomething()
{
    if (getWorld()->checkStatusAndShouldDoSth(this, getX(), getY()) == false)
        return;
    if (randInt(1, 3) == 1)
    {
        if(getWorld()->bitOthers(this, getX(), getY(), 50))
		{
			setStunned(2);
			return;
		}
    }
    if (randInt(1, 10) == 1)
        getWorld()->jump(this, getX(), getY());
    int foodAte = getWorld()->checkAndEatOrPickFood(getX(), getY(), 200);
    if (foodAte != 0)
    {
        setHp(getHp() + foodAte);
        if(randInt(0, 1))
        {
            setStunned(2);
            return;
        }
    }
    checkPebbleAndMove();
	setWasBit(false);
    setStunned(2);
    
}
