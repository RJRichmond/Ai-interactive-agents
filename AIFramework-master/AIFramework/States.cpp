#include "States.h"
#include <iostream>
#include "Grid.h"

States::~States()
{
}

void States::SetState(StateMachine& Soldier, States* state)
{
	if (Soldier.CurrentState == nullptr) return;

	Soldier.CurrentState = state;
}

void States::CheckState(StateMachine& Soldier)
{
	//Check which state the soldier needs to be in

	if (Soldier.IsTargetted == true) 
	{
		SetState(Soldier, new TakeCover());
	}
	else if (Soldier.Ammo <= 0) SetState(Soldier, new LookForAmmo());
	
	else SetState(Soldier, new ShootAtEnemy());

}

void LookForAmmo::OnEnter(StateMachine& Soldier)
{
	
	
	Soldier.StateString = "Looking for Ammo";
	
	std::cout << "This wil find the grid tile for the ammo\n";
}

void LookForAmmo::OnUpdate(StateMachine& Soldier, Grid* Grid, int AgentNumber)
{
	// This is using A* pathfinding to these tiles.
	
	Grid->AStarPathFinding(Grid->GetNode(Soldier.GridPosX, Soldier.GridPosY), Grid->AmmoTiles[AgentNumber]);
	//LevelGrid.AStarPathFinding(LevelGrid.GetNode(GetNode(Soldier.GridPosX,Soldier.GridPosY)), LevelGrid.AmmoTiles[0]);
	Soldier.CurrentAction = "Pathfinding to ammo tile";

	std::cout << "Pathfind to the ammo tile\n";
}

void LookForAmmo::OnExit(StateMachine& Soldier)
{
	
	Soldier.CurrentAction = "Got ammo, need to decide new state";
	Soldier.Ammo = 5;
	//Soldier.IsTargetted = true; // For the sake of testing the FSM
	CheckState(Soldier);
}

LookForAmmo::~LookForAmmo()
{
}

void ShootAtEnemy::OnEnter(StateMachine& Soldier)
{
	
	std::cout << "Looking for enemy to fire at\n";
	Soldier.StateString = "Shoot at \nthe enemy";
}

void ShootAtEnemy::OnUpdate(StateMachine& Soldier, Grid* Grid, int AgentNumber)
{
	//Pathfind to the location depending on the number of the agent.
	if (AgentNumber == 0) 
	{
		Grid->AStarPathFinding(Grid->GetNode(Soldier.GridPosX, Soldier.GridPosY), Grid->AgentLocation[1]);
	}
	else 
	{
		Grid->AStarPathFinding(Grid->GetNode(Soldier.GridPosX, Soldier.GridPosY), Grid->AgentLocation[0]);
	}
	Soldier.CurrentAction = "Pathfinding to shoot\n at the enemy agent";
	std::cout << "Bang\n";
}

void ShootAtEnemy::OnExit(StateMachine& Soldier)
{
	Soldier.ShouldFire = true;
	Soldier.Ammo -= 1;
	Soldier.IsTargetted = true;
	Soldier.CurrentAction = "I've shot at the enemy,\nI need to check my state";
	CheckState(Soldier);
}

ShootAtEnemy::~ShootAtEnemy()
{
}


void TakeCover::OnEnter(StateMachine& Soldier)
{
	std::cout << "Taking cover\n";
	Soldier.StateString = "Taking cover";
	
}

void TakeCover::OnUpdate(StateMachine& Soldier, Grid* Grid, int AgentNumber)
{
	std::cout << "Pathfind to cover\n";
	
	Grid->AStarPathFinding(Grid->GetNode(Soldier.GridPosX, Soldier.GridPosY), Grid->HideLocations[Soldier.TimesTakenCover]);
	
	Soldier.CurrentAction = "I need to pathfind to\ncover to be safe";
	
}

void TakeCover::OnExit(StateMachine& Soldier)
{
	std::cout << "Change state\n";
	Soldier.TimesTakenCover += 1;
	if (Soldier.TimesTakenCover >= 4)
	{
		Soldier.TimesTakenCover = 0;
	}
	Soldier.IsTargetted = false;
	Soldier.CurrentAction = "I've taken cover, I \nneed to check my state";
	CheckState(Soldier);
}

TakeCover::~TakeCover()
{
}
