#pragma once

#include "Scene.h"
#include "Node.h"

class Heap
{
public:
	// Constructor and Destructor
	Heap(Scene* scene, char version, int start, int linkSize);
	~Heap();

	// Allocate/Deallocate Actions
	void Allocate(Node* node);
	Node* AllocateTemporaryActor(int actorID);
	int AllocateRandomActor();
	void Deallocate(int actorID, int priority);
	void Deallocate(Node* node);
	void DeallocateTemporaryActor(int actorID);
	std::pair<int, int> DeallocateRandomActor();
	void ClearTemporaryActors();
	std::pair<int, int> ClearRandomActor();
	Node* FindSuitableGap(Node* newNode) const;
	void Insert(Node* newNode, Node* oldNode);

	// Room Actions
	void LoadInitialRoom(int roomNumber);
	void UnloadRoom(Room& room, int transitionActorID, Node* carryActor);
	void ChangeRoom(int newRoomNumber, int transitionActorID, Node* carryActor, bool spawners);

	// Other Actions
	void PrintHeap(char setting) const;
	void PrintCurrentActorCount() const;
	void ResetHeap();
	void ResetLeaks();
	void DeleteHeap();
	
	// Solve Functions
	void SolveOoT3D_LostWoods();
	void SolveMM();
	void SolveObservatory();
	void SolveGrave();
	void SolveGraveyard();

	// Getters
	Node* GetHead() const;
	Node* GetTail() const;
	int GetRoomNumber() const;
	int GetCurrentRoomNumber() const;
	std::vector<std::pair<int, int>> GetAddressesAndPrioritiesOfType(int actorID, char type);
	std::vector<std::pair<int, int>> GetAllAddresses(char type);
	int GetOverlayAddress(int actorID);
	std::vector<Node*> GetAllActorsOfID(int actorID);
	
private:
	// Private funcs
	void AllocateNewRoom(Room& newRoom, Room& oldRoom, int transitionActorID);
	void DeallocateClearedActors();
	void AllocateSpawnerOffspring();
	void DeallocateReallocatingActors();

	// Heap info
	char version = 0;
	Scene* scene = nullptr;
	Node* head = nullptr;
	Node* tail = nullptr;
	bool initialLoad = true;
	Room* currentRoom;
	int currentRoomNumber = 0;
	int initiallyLoadedRoomNumber = 0;
	const int start_address = 0x0;
	const int END_ADDRESS = 0x99499E0;
	const int linkSize = 0x0;
	const int LINK_ID = 0xffff;
	const char LINK_TYPE = 'L';
	const char OVERLAY_TYPE = 'O';

	std::map<int, int> currentActorCount;
	std::vector<Node*> temporaryActors;
	std::vector<Node*> offspringToAllocate;

	std::vector<Node*> leaks;
	std::vector<Node*> singletons;

	std::vector<Node*> singletonsAttemptingToReallocate;
	std::vector<Node*> reallocatingTransitionActors;

	Node* carryActor = nullptr;
	std::pair<Node*, int> heldActor;

	// Constants
	const int MAX_ALLOCATIONS_PER_STEP = 9;
	const int LOAD_MODIFIER = 4;

	const int MAX_EXPLOSIVES_PER_ROOM = 3;
	const int MAX_ARROWS_PER_ROOM = 3;
	const int MAX_CHUS = 40;
	const int MAX_ISOT = 50;
	const int MAX_BOMBS = 40;

	int allocatedExplosiveCount;
	int allocatedArrowCount;
	int allocatedChuCount;
	int allocatedISoTCount;
	int allocatedBombCount;

	// Scene-specific flags
	const bool smoke = false;
	const bool endAllocationStep = false;
	const bool postSSRoomChange = false;
	const bool breakRocks = false;
	const bool fins = false;
	const bool nut = false;
	const bool madScrub = false;
};

