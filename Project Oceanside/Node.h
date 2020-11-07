#pragma once

#include <string>
#include <map>
#include <string>

#include "./json.hpp"

class Node
{
public:
	// Constructors, going to handle this without a factory pattern for now
	Node();
	Node(int actorID, std::string s_actorID, nlohmann::json& actorInfo, nlohmann::json& actorParameters, int priority);
	Node(int actorID, nlohmann::json& actorInfo, int priority);
	Node(const Node& copy); // used to make a copy of an existing node
	Node(int size, int ID, char type, Node* overlay);
	Node(int address, int size, Node* prev, Node* next, char type, int ID);

	// Setters
	void SetAddress(int address); // used
	void SetSize(int size); // unused
	void SetNext(Node* next); // used
	void SetPrev(Node* prev); // used
	void SetID(int ID); // unused
	void SetType(char type);
	void SetSpawnerOffspring(Node* node);
	void SetCleared(bool clearStatus);

	// Node Info Getters
	int GetAddress() const; // used
	int GetSize() const; // used
	Node* GetNext() const; // used
	Node* GetPrev() const; // used
	Node* GetOverlay() const; // used
	int GetID() const; // used
	char GetType() const;
	int GetPriority() const;
	
	// Actor Property Getters
	std::string GetDescription() const;
	bool IsDeallocatable() const; // if is an actor that always is loaded, but can be deloaded by choice (i.e. a pot)
	bool IsClearable() const; // if is an actor that can be permanantly deallocated every time you enter the room (i.e. deku palace rupees)
	bool CanStartCleared() const; // if the actor can be cleared the first time the room is loaded???
	bool StartCleared() const; // unused
	bool ConsiderForSRM() const; // unused
	bool ReallocateOnRoomChange() const; // mm only maybe? for clock and dampe. maybe this is for non-transition actors which can change rooms
	bool IsSingleton() const; // seems to include the reallocate on room change actors and some(?) transition actors?
	bool IsSpawner() const; // true if the actor has offspring. false if not
	bool IsTransitionActor() const; // is a door, loading plane, etc.

	// Offspring Data Getters
	std::vector<Node*> GetOffspring();
	char GetNumberOfOffspring() const; // for clusters of actors, such as rupees, grass, rocks
	int GetOffspringActorID() const; // the actor id of the offspring
	
	// Transition Actor Data Getters
	int GetSceneTransitionID() const; // get the transition id of the transition actor
	std::pair<int,int> GetRoomsConnectedByTransition() const; // get the pair of two rooms connected by transition actor. unused

private:
	// General Node info
	int address = 0;
	int size = 0;
	Node* next = nullptr;
	Node* prev = nullptr;
	int ID = 0xFFFF; // actor id
	char type = 'X';
	Node* overlay = nullptr;
	int priority = 0;
	std::string description;
	
	// Node Properties
	bool isDeallocatable;
	bool isClearable;
	bool canStartCleared;
	bool startCleared;
	bool cleared; // unused
	bool considerForSRM;
	bool reallocateOnRoomChange;
	bool isSingleton;
	bool isSpawner;
	bool isTransitionActor = false;

	// Offspring Data
	std::vector<Node*> spawnerOffspring;
	int numberOfOffspring;
	int offspringActorID;

	// Transition Actor Data
	std::pair<int, int> roomsConnectedByTransition;
	int sceneTransitionID = -1;
};



