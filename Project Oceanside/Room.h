#pragma once

#include <vector>
#include "Node.h"	

class Room
{
public:

	// Constructor
	Room(int roomNumber);

	// Main functions
	void AddActor(Node* actor);
	void AddCurrentlyLoadedActor(Node* actor);
	void RemoveCurrentlyLoadedActor(Node* node);
	void ResetCurrentlyLoadedActors();
	void ClearActor(Node* actor);
	void ResetClearedActors();
	void AddRandomAllocatableActor(int timesCanAllocate, Node* actor);
	void AddDeallocatableActor(Node* actor);
	void AddTransitionActor(Node* actor);
	void DeallocateActor(Node* actor);
	void ReplenishDeallocatableActors();
	void DumpRoomInfo() const; // print room info

	// Getters
	int GetRoomNumber() const;
	std::vector<Node*> GetAllActors() const;
	std::vector<Node*> GetCurrentlyLoadedActors() const;
	std::vector<Node*> GetDeallocatableActors() const;
	std::vector<Node*> GetClearableActors() const;
	std::vector<Node*> GetClearedActors() const;
	std::map<int, Node*> GetTransitionActors() const;
	std::map<int, std::pair<int, Node*>> GetPossibleTemporaryActors() const;
	std::vector<int> GetPossibleTemporaryActorsIDs() const;
	
private:
	int roomNumber;

	// Actor Lists
	std::vector<Node*> allActors;
	std::vector<Node*> currentlyLoadedActors;
	std::vector<Node*> deallocatableActors;
	std::vector<Node*> clearableActors;
	std::vector<Node*> clearedActors;
	std::vector<Node*> currentlyDeallocatedActors;
	std::map<int, Node*> transitionActors;
	std::map<int, std::pair<int, Node*>> possibleTemporaryActors;
	std::vector<int> possibleTemporaryActorsIDs;
};

