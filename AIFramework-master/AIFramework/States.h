#pragma once

#include "StateMachine.h"

/*
	State machine is going to be
	used for the soldiers.
	- Looking for ammo - Default state
	- Shooting at enemy 
	- Taking cover 
*/

class StateMachine;

class Grid;

class States
{
public:
	virtual void OnEnter(StateMachine& Soldier) = 0;
	virtual void OnUpdate(StateMachine& Soldier, Grid* Grid,int AgentNumber) = 0;
	virtual void OnExit(StateMachine& Soldier) = 0;
	virtual ~States();

protected:
	void SetState(StateMachine& Soldier, States* state);
	void CheckState(StateMachine& Soldier);

};

class LookForAmmo : public States
{
	virtual void OnEnter(StateMachine& Soldier);
	virtual void OnUpdate(StateMachine& Soldier,Grid* Grid, int AgentNumber);
	virtual void OnExit(StateMachine& Soldier);
	virtual ~LookForAmmo();
};

class ShootAtEnemy : public States
{
	virtual void OnEnter(StateMachine& Soldier);
	virtual void OnUpdate(StateMachine& Soldier, Grid* Grid, int AgentNumber);
	virtual void OnExit(StateMachine& Soldier);
	virtual ~ShootAtEnemy();
};


class TakeCover : public States
{
public:
	virtual void OnEnter(StateMachine& Soldier);
	virtual void OnUpdate(StateMachine& Soldier, Grid* Grid, int AgentNumber);
	virtual void OnExit(StateMachine& Soldier);
	virtual ~TakeCover();
};


