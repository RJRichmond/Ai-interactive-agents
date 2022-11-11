#include "StateMachine.h"

StateMachine::StateMachine()
{
	CurrentState = new LookForAmmo();
	Ammo = 0;
	Health = 3;
	IsTargetted = false;
	StateString = "Looking For Ammo";
}

StateMachine::~StateMachine()
{
	delete CurrentState;
	CurrentState = nullptr;
}

void StateMachine::OnEnter()
{
	CurrentState->OnEnter(*this);
}

void StateMachine::OnUpdate(Grid* Grid, int AgentNumber)
{
	CurrentState->OnUpdate(*this, Grid, AgentNumber);
}

void StateMachine::OnExit()
{
	CurrentState->OnExit(*this);
}
