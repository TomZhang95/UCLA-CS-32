#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

/*--------------------------------------- Actor ---------------------------------------*/
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* sw, unsigned int depth = 1,
		int hp = 1, Direction dir = right, unsigned int currFood = 0, unsigned int distance = 2,
		unsigned int stunned = 0, bool wasBit = false,
		bool wasBlocked = false, int lastRandomNumber = 0, unsigned int countTick = 1,
          bool wasInPool = false);
	
    virtual ~Actor();
	Direction randomDir();
    virtual void doSomething() = 0;
	StudentWorld* getWorld();
    
    bool isDead();
    bool isCalled(Actor* currObj);
    void checkPebbleAndMove();
    int StringToInt(std::string s);
    void rotateClockwise();
    void rotateCounterClockwise();
    void moveForward();
    
    void setHp(int hp);
	void setFood(int food);
	void setDistance(int distance);
	void setStunned(int stunned);
	void setWasBit(bool bitten);
	void setWasBlocked(bool blocked);
	void setLastRandomNumber(int num);
    void setCountTick(int num);
    void setWasInPool(bool status);

	int getHp() const;
	int getFood() const;
	int getStunned() const;
	int getLastRandomNumber() const;
	unsigned int getDistance() const;
	bool getWasBit() const;
	bool getWasBlocked() const;
    unsigned int getCountTick() const;
    bool getWasInPool() const;
    
    //AntHill/And virtual functions
    virtual int getColonyNumber() const { return -1; }
    virtual Compiler* getCompilerObject() const { return nullptr; }
    virtual int getIC() const { return -1; }
    virtual void setIC(int ic) {}

private:
	int m_hp, m_food, m_stunned, m_lastRandomNumber;
    unsigned int m_distance, m_countTick;
    bool m_wasBit, m_wasBlocked, m_wasInPool;
    StudentWorld* m_world;

};

inline int Actor::getHp() const                       { return m_hp; }
inline int Actor::getFood() const                     { return m_food; }
inline int Actor::getStunned() const                  { return m_stunned; }
inline int Actor::getLastRandomNumber() const         { return m_lastRandomNumber; }
inline unsigned int Actor::getDistance() const       { return m_distance; }
inline bool Actor::getWasBit() const                  { return m_wasBit; }
inline bool Actor::getWasBlocked() const              { return m_wasBlocked; }
inline unsigned int Actor::getCountTick() const      { return m_countTick; }
inline bool Actor::getWasInPool() const               { return m_wasInPool; }

inline void Actor::setHp(int hp)                { m_hp = hp; }
inline void Actor::setFood(int food)            { m_food = food; }
inline void Actor::setDistance(int distance)    { m_distance = distance; }
inline void Actor::setStunned(int stunned)      { m_stunned = stunned; }
inline void Actor::setWasBit(bool bitten)       { m_wasBit = bitten; }
inline void Actor::setWasBlocked(bool blocked)  { m_wasBlocked = blocked; }
inline void Actor::setLastRandomNumber(int num) { m_lastRandomNumber = num; }
inline void Actor::setCountTick(int num)        { m_countTick = num; }
inline void Actor::setWasInPool(bool status)    { m_wasInPool = status; }

/*---------------------------------------- Ant ----------------------------------------*/
class Ant : public Actor
{
public:
	Ant(int imageID, int startX, int startY, StudentWorld* sw,
		int hp = 1500, Direction dir = right, 
		unsigned int currFood = 0, unsigned int colonyNumber = 5, Compiler* compilerObject = nullptr,
		unsigned int distance = 2, unsigned int stunned = 0, bool wasBit = false,
		bool wasBlocked = false, int lastRandomNumber = 0, unsigned int  instructionCounter = 0, unsigned int depth = 1);
	~Ant();
    virtual void doSomething();
    virtual Compiler* getCompilerObject() const { return m_compilerObject; }
	virtual int getColonyNumber() const { return m_colonyNumber; }
    virtual int getIC() const { return m_instructionCounter; }
    virtual void setIC(int ic) { m_instructionCounter = ic ;}
private:
	unsigned int m_colonyNumber, m_instructionCounter;
    Compiler* m_compilerObject;
    bool interpreter();
};

/*---------------------------------------- Pebble ----------------------------------------*/
class Pebble : public Actor
{
public:
	Pebble(int imageID, int startX, int startY, StudentWorld* sw);
	~Pebble();
    virtual void doSomething();
private:
};

/*---------------------------------------- Food ----------------------------------------*/
class Food : public Actor
{
public:
	Food(int imageID, int startX, int startY, StudentWorld* sw,
         int hp = 6000, Direction dir = right,
		int currFood = 0, unsigned int depth = 2);
	~Food();
    virtual void doSomething();
};

/*---------------------------------------- Pheromone ----------------------------------------*/
class Pheromone : public Actor
{
public:
	Pheromone(int imageID, int startX, int startY, StudentWorld* sw, int num,
              unsigned int depth = 2, int hp = 256);
	~Pheromone();
    virtual void doSomething();
	virtual int getColonyNumber() const { return m_num; }
private:
	int m_num;
};

/*---------------------------------------- AntHill ----------------------------------------*/
class AntHill : public Actor
{
public:
	AntHill(int imageID, int startX, int startY, StudentWorld* sw,
            int colonyNumber = 0, Compiler* compilerObject = nullptr,
		unsigned int depth = 2, int hp = 8999);
	~AntHill();
    virtual void doSomething();
    
    virtual int getColonyNumber() const;
    virtual Compiler* getCompilerObject() const;
private:
    int m_colonyNumber;
    Compiler* m_compilerObject;
};

inline int AntHill::getColonyNumber() const { return m_colonyNumber; }
inline Compiler* AntHill::getCompilerObject() const { return m_compilerObject; }

/*---------------------------------- PoolofWater -----------------------------------*/
class PoolofWater : public Actor
{
public:
	PoolofWater(int imageID, int startX, int startY, StudentWorld* sw,
                unsigned int depth = 2);
	~PoolofWater();
    virtual void doSomething();
};

/*---------------------------------------- Poison ----------------------------------------*/
class Poison : public Actor
{
public:
	Poison(int imageID, int startX, int startY, StudentWorld* sw,
           unsigned int depth = 2);
	~Poison();
    virtual void doSomething();
};

/*---------------------------------- BabyGrasshopper -----------------------------------*/
class BabyGrasshopper : public Actor
{
public:
	BabyGrasshopper(int imageID, int startX, int startY, StudentWorld* sw,
                    unsigned int depth = 1, int hp = 500, Direction dir = right,
                    int currFood = 0, unsigned int distance = 2);
	~BabyGrasshopper();
    virtual void doSomething();
private:
};

/*---------------------------------- AdultGrasshopper -----------------------------------*/
class AdultGrasshopper : public Actor
{
public:
	AdultGrasshopper(int imageID, int startX, int startY, StudentWorld* sw,
                     unsigned int depth = 1, int hp = 1600, Direction dir = right,
                     int currFood = 0, unsigned int distance = 2);
	~AdultGrasshopper();
    virtual void doSomething();
};
#endif // ACTOR_H_
