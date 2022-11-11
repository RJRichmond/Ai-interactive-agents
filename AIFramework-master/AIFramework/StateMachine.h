#pragma once

#include "States.h"
#include <string>


class Grid;

class StateMachine
{
	friend class States;
	

public:
	StateMachine();
	~StateMachine();
	void OnEnter();
	void OnUpdate(Grid* Grid,int AgentNumber);
	void OnExit();

	std::string StateString;

	std::string CurrentAction;
	bool IsTargetted;
	bool ShouldFire;
	int Ammo;
	int Health;
	int TimesTakenCover;

	int GridPosX;
	int GridPosY;


private:
	States* CurrentState;
	
};
