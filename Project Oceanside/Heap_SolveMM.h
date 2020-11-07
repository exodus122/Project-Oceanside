#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <direct.h>

#include "Heap.h"
#include "Room.h"
#include "Constants.h"

void Heap::SolveMM()
{
	unsigned int seed = time(NULL);
	srand(seed);

	uint64_t totalPermutations = 0;
	unsigned int totalSolutions = 0;

	std::vector<std::pair<int, int>> solution;

	std::cout << "Seed: " << seed << std::endl;
	std::cout << "Solving..." << std::endl;

	auto newContainerFolder = "./output/";
	auto newSubFolder = newContainerFolder + std::to_string(seed) + "\\";
	_mkdir(newContainerFolder);
	_mkdir(newSubFolder.c_str());

	while (true)
	{
		int roomLoads = (2 * (rand() % LOAD_MODIFIER));

		LoadInitialRoom(0);
		solution.push_back(std::make_pair(LOAD_INITIAL_ROOM, 0));

		if (breakRocks)
		{
			char rockRNG = rand() % 4;
			if (rockRNG == 0)
			{
				Deallocate(0x92, 0);
				solution.push_back(std::make_pair(0x92, 0));

			}
			else if (rockRNG == 1)
			{
				Deallocate(0x92, 1);
				solution.push_back(std::make_pair(0x92, 1));
			}
			else if (rockRNG == 2)
			{
				Deallocate(0x92, 0);
				Deallocate(0x92, 1);
				solution.push_back(std::make_pair(0x92, 0));
				solution.push_back(std::make_pair(0x92, 1));
			}
		}

		if (fins)
		{
			AllocateTemporaryActor(0x20);
			AllocateTemporaryActor(0x20);
			solution.push_back(std::make_pair(ALLOCATE, 0x20));
		}

		for (int i = 0; i < roomLoads; i++)
		{
			int deallocations = 0;
			int currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();
			if (currentRoomDeallocations)
			{
				deallocations = rand() % currentRoomDeallocations;
			}
			else
			{
				deallocations = 0;
			}

			for (int j = 0; j < deallocations; j++)
			{
				solution.push_back(DeallocateRandomActor());
			}

			if (madScrub)
			{
				char scrubRNG = rand() % 2;
				if (scrubRNG && currentRoomNumber != 0)
				{
					AllocateTemporaryActor(0x183);
					Deallocate(0x3B, 1);
					solution.push_back(std::make_pair(ALLOCATE, 0x183));
				}
			}

			if (smoke)
			{
				char smokeRNG = rand() % 2;
				if (smokeRNG)
				{
					AllocateTemporaryActor(0xA2);
					solution.push_back(std::make_pair(ALLOCATE, 0xA2));
				}
			}

			int allocations = 0;
			if (MAX_ALLOCATIONS_PER_STEP == 0)
			{
				allocations = 0;
			}
			else
			{
				allocations = rand() % MAX_ALLOCATIONS_PER_STEP;
			}

			for (int j = 0; j < allocations; j++)
			{
				auto allocate = AllocateRandomActor();
				solution.push_back(std::make_pair(ALLOCATE, allocate));
			}

			if (endAllocationStep)
			{
				char rng = rand() % 3;

				switch (rng)
				{
				case 0:
					break;
				case 1:
					AllocateTemporaryActor(0x3D);
					solution.push_back(std::make_pair(ALLOCATE, 0x3D));
					break;
				case 2:
					AllocateTemporaryActor(0x35);
					solution.push_back(std::make_pair(ALLOCATE, 0x35));
					break;
				default:
					break;
				}
			}

			int nextRoom;
			int nextPlane;
			//after all allocations are done, we need to change rooms.

			//if we are in room 2, the only plane we should use is plane 3
			if (currentRoomNumber == 2)
			{
				nextRoom = 0;
				nextPlane = 3;
			}
			//if we are in room 1, the only plane we should use is plane 2
			else if (currentRoomNumber == 1)
			{
				nextRoom = 0;
				nextPlane = 2;
			}

			//if we are currently in room 0, we need to randomly choose room 1 or 2 to go to
			else if (currentRoomNumber == 0)
			{
				nextRoom = 1;// (rand() > RAND_MAX / 2) ? 1 : 2;

				//if we're choosing to go to room 2, we need to use plane 3
				if (nextRoom == 2)
				{
					nextPlane = 3;
				}
				//if we're choosing to go to room 1, we need to use plane 2
				else if (nextRoom == 1)
				{
					nextPlane = 2;
				}
			}

			//also, should we allow the spawner to allocate or no?
			bool allocateSpawners = false;
			char spawnerRNG = rand() % 2;
			if (spawnerRNG)
			{
				allocateSpawners = true;
			}

			//actually perform room change using chosen room and plane
			ChangeRoom(nextRoom, nextPlane, nullptr, allocateSpawners);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, allocateSpawners));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}

		//we're now standing in room 0

		int allocations = 0;
		if (MAX_ALLOCATIONS_PER_STEP == 0)
		{
			allocations = 0;
		}
		else
		{
			allocations = rand() % MAX_ALLOCATIONS_PER_STEP;
		}

		for (int j = 0; j < allocations; j++)
		{
			auto allocate = AllocateRandomActor();
			solution.push_back(std::make_pair(ALLOCATE, allocate));
		}

		if (endAllocationStep)
		{
			char rng = rand() % 3;

			switch (rng)
			{
			case 0:
				break;
			case 1:
				AllocateTemporaryActor(0x3D);
				solution.push_back(std::make_pair(ALLOCATE, 0x3D));
				break;
			case 2:
				AllocateTemporaryActor(0x35);
				solution.push_back(std::make_pair(ALLOCATE, 0x35));
				break;
			default:
				break;
			}
		}

		//get back to pot room
		ChangeRoom(1, 2, nullptr, true);
		solution.push_back(std::make_pair(CHANGE_ROOM, 1));
		solution.push_back(std::make_pair(USE_PLANE, 2));

		char potDestroyRNG = rand() % 3;
		if (potDestroyRNG == 0)
		{
			Deallocate(0x82, 0);
			solution.push_back(std::make_pair(0x82, 0));
		}
		else if (potDestroyRNG == 1)
		{
			Deallocate(0x82, 1);
			solution.push_back(std::make_pair(0x82, 1));
		}

		//we're now standing in pot room
		std::vector<std::pair<int, int>> pots = GetAddressesAndPrioritiesOfType(0x82, 'A');

		AllocateTemporaryActor(0xA2);
		ChangeRoom(0, 0, nullptr, true);
		solution.push_back(std::make_pair(SUPERSLIDE, 0));

		//standing in main room now with SRM

		//now we need to randomly choose which room to end up in for guard SRM
		int nextRoom = (rand() > RAND_MAX / 2) ? 1 : 2;
		int nextPlaneRNG = rand() % 2;
		int nextPlane = 0;

		if (nextRoom == 1)
		{
			//if we randomly chose room 1 to finish in, we need to pick which plane to use
			if (nextPlaneRNG == 0)
			{
				nextPlane = 0;
			}
			else
			{
				nextPlane = 2;
			}

			ChangeRoom(1, nextPlane, nullptr, true);
			solution.push_back(std::make_pair(CHANGE_ROOM, 1));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}
		else if (nextRoom == 2)
		{
			//if we randomly chose room 2 to finish in, we need to pick which plane to use
			if (nextPlaneRNG == 0)
			{
				nextPlane = 1;
			}
			else
			{
				nextPlane = 3;
			}
			ChangeRoom(2, nextPlane, nullptr, true);
			solution.push_back(std::make_pair(CHANGE_ROOM, 2));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}

		if (postSSRoomChange)
		{
			char postSSRoomChangeRNG = rand() % 2;
			if (postSSRoomChangeRNG)
			{
				if (currentRoomNumber == 1)
				{
					ChangeRoom(0, nextPlane, nullptr, true); //go back through plane you just used back to room 0
					solution.push_back(std::make_pair(CHANGE_ROOM, 0));
					solution.push_back(std::make_pair(USE_PLANE, nextPlane));
					ChangeRoom(2, 3, nullptr, true); //go to room 2
					solution.push_back(std::make_pair(CHANGE_ROOM, 2));
					solution.push_back(std::make_pair(USE_PLANE, 3));
				}
				else if (currentRoomNumber == 2)
				{
					ChangeRoom(0, nextPlane, nullptr, true); //go back through plane you just used back to room 0
					solution.push_back(std::make_pair(CHANGE_ROOM, 0));
					solution.push_back(std::make_pair(USE_PLANE, nextPlane));
					ChangeRoom(1, 2, nullptr, true); //go to room 1
					solution.push_back(std::make_pair(CHANGE_ROOM, 1));
					solution.push_back(std::make_pair(USE_PLANE, 2));
				}
			}
		}

		std::vector<std::pair<int, int>> guards = GetAddressesAndPrioritiesOfType(0x17A, 'A');

		bool solutionFound = false;
		std::pair<std::pair<int, int>, std::pair<int, int>> solutionPair;

		for (auto guard : guards)
		{
			for (auto pot : pots)
			{
				if (pot.first - guard.first == 0x200 &&
					((currentRoomNumber == 2 && (guard.second == 2 || guard.second == 3 || guard.second == 1)) ||
					(currentRoomNumber == 1 && (guard.second == 2 || guard.second == 0))))
				{
					solutionFound = true;
					solutionPair = std::make_pair(pot, guard);

				}
			}

		}

		if (solutionFound)
		{
			std::cout << "SOLUTION FOUND\n";
			totalSolutions++;

			std::ofstream outputFile;
			std::string outputFilename = newSubFolder + "\\solution" + std::to_string(totalSolutions) + "_seed_" + std::to_string(seed) + ".txt";
			outputFile.open(outputFilename);

			outputFile << std::hex << "Pot Address | Priority: " << solutionPair.first.first << " | " << solutionPair.first.second <<
				" Guard Address | Priority: " << solutionPair.second.first << " | " << solutionPair.second.second << std::dec << std::endl;

			for (auto step : solution)
			{
				if (step.first == LOAD_INITIAL_ROOM)
				{
					outputFile << std::hex << "Load initial room: " << step.second << std::endl;
				}
				else if (step.first == CHANGE_ROOM)
				{
					outputFile << std::hex << "Load room: " << step.second;
				}
				else if (step.first == SPAWNERS)
				{
					outputFile << " | Spawners: " << step.second;
				}
				else if (step.first == USE_PLANE)
				{
					outputFile << " | Use plane: " << step.second << std::endl;
				}
				else if (step.first == ALLOCATE)
				{
					if (step.second != 0x0)
					{
						outputFile << std::hex << "Allocate: " << step.second << std::endl;
					}
				}
				else if (step.first == SUPERSLIDE)
				{
					outputFile << std::hex << "Superslide into room " << step.second << " with smoke still loaded using plane 0." << std::endl;
				}
				else if (step.first == 0)
				{
					;
				}
				else
				{
					outputFile << std::hex << "Dealloc: " << std::setw(2) << step.first << ", " << step.second << std::endl;
				}

			}
			outputFile.close();
		}

		ResetHeap();
		solution.clear();
		totalPermutations++;

		if (totalPermutations % 100000 == 0)
		{
			std::cout << std::dec << "Total permutations: " << totalPermutations << " | Total Solutions: " << totalSolutions << std::endl;
		}

	}
}

void Heap::SolveObservatory()
{
	unsigned int seed = time(NULL);
	srand(seed);

	uint64_t totalPermutations = 0;
	unsigned int totalSolutions = 0;

	std::vector<std::pair<int, int>> solution;

	std::cout << "Seed: " << seed << std::endl;
	std::cout << "Solving..." << std::endl;

	auto newContainerFolder = "./output/";
	auto newSubFolder = newContainerFolder + std::to_string(seed) + "\\";
	_mkdir(newContainerFolder);
	_mkdir(newSubFolder.c_str());

	while (true)
	{
		int roomLoads = (2 * (rand() % LOAD_MODIFIER));

		LoadInitialRoom(1);
		solution.push_back(std::make_pair(LOAD_INITIAL_ROOM, 1));

		for (int i = 0; i < roomLoads; i++)
		{
			int deallocations = 0;
			int currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();
			if (currentRoomDeallocations)
			{
				deallocations = rand() % currentRoomDeallocations;
			}
			else
			{
				deallocations = 0;
			}

			for (int j = 0; j < deallocations; j++)
			{
				auto dealloc = DeallocateRandomActor();
				solution.push_back(dealloc);

			}

			if (smoke)
			{
				char smokeRNG = rand() % 2;
				if (smokeRNG)
				{
					AllocateTemporaryActor(0xA2);
					solution.push_back(std::make_pair(ALLOCATE, 0xA2));
				}
			}

			int allocations = 0;
			if (MAX_ALLOCATIONS_PER_STEP == 0)
			{
				allocations = 0;
			}
			else
			{
				allocations = rand() % MAX_ALLOCATIONS_PER_STEP;
			}

			for (int j = 0; j < allocations; j++)
			{
				auto allocate = AllocateRandomActor();
				solution.push_back(std::make_pair(ALLOCATE, allocate));
			}

			if (endAllocationStep)
			{
				char rng = rand() % 2;

				switch (rng)
				{
				case 0:
					break;
					/*case 1:
						AllocateTemporaryActor(0x3D);
						solution.push_back(std::make_pair(ALLOCATE, 0x3D));
						break;*/
				case 1:
					AllocateTemporaryActor(0x35);
					solution.push_back(std::make_pair(ALLOCATE, 0x35));
					break;
				default:
					break;
				}
			}

			int nextRoom = 0;
			int nextPlane = 0;

			if (currentRoomNumber == 0)
			{
				nextRoom = 1;
				nextPlane = 0;
			}
			else if (currentRoomNumber == 1)
			{
				nextRoom = 0;
				nextPlane = 0;
			}

			//actually perform room change using chosen room and plane
			ChangeRoom(nextRoom, nextPlane, nullptr, false);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, false));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}

		//we're now standing in room 1

		char potDestroyRNG = rand() % 4;
		if (potDestroyRNG == 0)
		{
			Deallocate(0x82, 0);
			solution.push_back(std::make_pair(0x82, 0));

		}
		else if (potDestroyRNG == 1)
		{
			Deallocate(0x82, 1);
			solution.push_back(std::make_pair(0x82, 1));
		}
		else if (potDestroyRNG == 2)
		{
			Deallocate(0x82, 2);
			solution.push_back(std::make_pair(0x82, 2));
		}

		std::vector<std::pair<int, int>> pots = GetAddressesAndPrioritiesOfType(0x82, 'A');

		AllocateTemporaryActor(0xA2);
		ChangeRoom(0, 0, nullptr, false);
		solution.push_back(std::make_pair(SUPERSLIDE, 0));

		//standing in chest room now, but we need to swap rooms twice to have a chance of things lining up
		ChangeRoom(1, 0, nullptr, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 1));
		ChangeRoom(0, 0, nullptr, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 0));

		std::vector<std::pair<int, int>> chest = GetAddressesAndPrioritiesOfType(0x6, 'A');

		bool solutionFound = false;
		std::pair<std::pair<int, int>, std::pair<int, int>> solutionPair;

		for (auto c : chest)
		{
			for (auto pot : pots)
			{
				if (pot.first - c.first == 0x160)
				{
					solutionFound = true;
					solutionPair = std::make_pair(pot, c);
				}
			}

		}

		if (solutionFound)
		{
			std::cout << "SOLUTION FOUND\n";
			totalSolutions++;

			std::ofstream outputFile;
			std::string outputFilename = newSubFolder + "\\solution" + std::to_string(totalSolutions) + "_seed_" + std::to_string(seed) + ".txt";
			outputFile.open(outputFilename);

			outputFile << std::hex << "Pot Address | Priority: " << solutionPair.first.first << " | " << solutionPair.first.second <<
				" Guard Address | Priority: " << solutionPair.second.first << " | " << solutionPair.second.second << std::dec << std::endl;

			for (auto step : solution)
			{
				if (step.first == LOAD_INITIAL_ROOM)
				{
					outputFile << std::hex << "Load initial room: " << step.second << std::endl;
				}
				else if (step.first == CHANGE_ROOM)
				{
					outputFile << std::hex << "Load room: " << step.second;
				}
				else if (step.first == SPAWNERS)
				{
					outputFile << " | Spawners: " << step.second;
				}
				else if (step.first == USE_PLANE)
				{
					outputFile << " | Use plane: " << step.second << std::endl;
				}
				else if (step.first == ALLOCATE)
				{
					if (step.second != 0x0)
					{
						outputFile << std::hex << "Allocate: " << step.second << std::endl;
					}
				}
				else if (step.first == SUPERSLIDE)
				{
					outputFile << std::hex << "Superslide into room " << step.second << " with smoke still loaded using plane 0." << std::endl;
				}
				else if (step.first == 0)
				{
					;
				}
				else
				{
					outputFile << std::hex << "Dealloc: " << std::setw(2) << step.first << ", " << step.second << std::endl;
				}

			}
			outputFile.close();
		}

		ResetHeap();
		solution.clear();
		totalPermutations++;

		if (totalPermutations % 100000 == 0)
		{
			std::cout << std::dec << "Total permutations: " << totalPermutations << " | Total Solutions: " << totalSolutions << std::endl;
		}

	}
}

void Heap::SolveGraveyard()
{
	unsigned int seed = time(NULL);
	srand(seed);

	uint64_t totalPermutations = 0;
	unsigned int totalSolutions = 0;

	std::vector<std::pair<int, int>> solution;

	std::cout << "Seed: " << seed << std::endl;
	std::cout << "Solving..." << std::endl;

	auto newContainerFolder = "./output/";
	auto newSubFolder = newContainerFolder + std::to_string(seed) + "\\";
	_mkdir(newContainerFolder);
	_mkdir(newSubFolder.c_str());

	while (true)
	{
		int roomLoads = (2 * (rand() % LOAD_MODIFIER));

		LoadInitialRoom(1);
		solution.push_back(std::make_pair(LOAD_INITIAL_ROOM, 1));

		for (int i = 0; i < roomLoads; i++)
		{
			int deallocations = 0;
			int currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();
			if (currentRoomDeallocations)
			{
				deallocations = rand() % currentRoomDeallocations;
			}
			else
			{
				deallocations = 0;
			}

			for (int j = 0; j < deallocations; j++)
			{
				auto dealloc = DeallocateRandomActor();
				solution.push_back(dealloc);

			}

			if (smoke)
			{
				char smokeRNG = rand() % 2;
				if (smokeRNG)
				{
					AllocateTemporaryActor(0xA2);
					solution.push_back(std::make_pair(ALLOCATE, 0xA2));
				}
			}

			int allocations = 0;
			if (MAX_ALLOCATIONS_PER_STEP == 0)
			{
				allocations = 0;
			}
			else
			{
				allocations = rand() % MAX_ALLOCATIONS_PER_STEP;
			}

			for (int j = 0; j < allocations; j++)
			{
				auto allocate = AllocateRandomActor();
				solution.push_back(std::make_pair(ALLOCATE, allocate));
			}

			if (endAllocationStep)
			{
				char rng = rand() % 2;

				switch (rng)
				{
				case 0:
					break;
					/*case 1:
						AllocateTemporaryActor(0x3D);
						solution.push_back(std::make_pair(ALLOCATE, 0x3D));
						break;*/
				case 1:
					AllocateTemporaryActor(0x35);
					solution.push_back(std::make_pair(ALLOCATE, 0x35));
					break;
				default:
					break;
				}
			}

			int nextRoom = 0;
			int nextPlane = 0;

			if (currentRoomNumber == 0)
			{
				nextRoom = 1;
				nextPlane = 0;
			}
			else if (currentRoomNumber == 1)
			{
				nextRoom = 0;
				nextPlane = 0;
			}

			//actually perform room change using chosen room and plane
			ChangeRoom(nextRoom, nextPlane, nullptr, false);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, false));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}

		//we're now standing in room 1

		char potDestroyRNG = rand() % 4;
		if (potDestroyRNG == 0)
		{
			Deallocate(0x82, 0);
			solution.push_back(std::make_pair(0x82, 0));

		}
		else if (potDestroyRNG == 1)
		{
			Deallocate(0x82, 1);
			solution.push_back(std::make_pair(0x82, 1));
		}
		else if (potDestroyRNG == 2)
		{
			Deallocate(0x82, 2);
			solution.push_back(std::make_pair(0x82, 2));
		}

		std::vector<std::pair<int, int>> grass = GetAddressesAndPrioritiesOfType(0x90, 'A');
		std::vector<std::pair<int, int>> rocks = GetAddressesAndPrioritiesOfType(0xB0, 'A');


		AllocateTemporaryActor(0xA2);
		ChangeRoom(0, 0, nullptr, false);
		solution.push_back(std::make_pair(SUPERSLIDE, 0));

		//standing in chest room now, but we need to swap rooms twice to have a chance of things lining up
		ChangeRoom(1, 0, nullptr, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 1));
		ChangeRoom(0, 0, nullptr, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 0));

		std::vector<std::pair<int, int>> chest = GetAddressesAndPrioritiesOfType(0x6, 'A');

		bool solutionFound = false;
		std::pair<std::pair<int, int>, std::pair<int, int>> solutionPair;

		for (auto c : chest)
		{
			for (auto grasss : grass)
			{
				if (grasss.first - c.first == 0x160)
				{
					solutionFound = true;
					solutionPair = std::make_pair(grasss, c);
				}
			}

		}

		for (auto c : chest)
		{
			for (auto rock : rocks)
			{
				if (rock.first - c.first == 0x160 && !solutionFound)
				{
					solutionFound = true;
					solutionPair = std::make_pair(rock, c);
				}
			}

		}

		if (solutionFound)
		{
			std::cout << "SOLUTION FOUND\n";
			totalSolutions++;

			std::ofstream outputFile;
			std::string outputFilename = newSubFolder + "\\solution" + std::to_string(totalSolutions) + "_seed_" + std::to_string(seed) + ".txt";
			outputFile.open(outputFilename);

			outputFile << std::hex << "Pot Address | Priority: " << solutionPair.first.first << " | " << solutionPair.first.second <<
				" Guard Address | Priority: " << solutionPair.second.first << " | " << solutionPair.second.second << std::dec << std::endl;

			for (auto step : solution)
			{
				if (step.first == LOAD_INITIAL_ROOM)
				{
					outputFile << std::hex << "Load initial room: " << step.second << std::endl;
				}
				else if (step.first == CHANGE_ROOM)
				{
					outputFile << std::hex << "Load room: " << step.second;
				}
				else if (step.first == SPAWNERS)
				{
					outputFile << " | Spawners: " << step.second;
				}
				else if (step.first == USE_PLANE)
				{
					outputFile << " | Use plane: " << step.second << std::endl;
				}
				else if (step.first == ALLOCATE)
				{
					if (step.second != 0x0)
					{
						outputFile << std::hex << "Allocate: " << step.second << std::endl;
					}
				}
				else if (step.first == SUPERSLIDE)
				{
					outputFile << std::hex << "Superslide into room " << step.second << " with smoke still loaded using plane 0." << std::endl;
				}
				else if (step.first == 0)
				{
					;
				}
				else
				{
					outputFile << std::hex << "Dealloc: " << std::setw(2) << step.first << ", " << step.second << std::endl;
				}

			}
			outputFile.close();
		}

		ResetHeap();
		solution.clear();
		totalPermutations++;

		if (totalPermutations % 100000 == 0)
		{
			std::cout << std::dec << "Total permutations: " << totalPermutations << " | Total Solutions: " << totalSolutions << std::endl;
		}

	}
}

void Heap::SolveGrave()
{
	unsigned int seed = time(NULL);
	srand(seed);

	uint64_t totalPermutations = 0;
	unsigned int totalSolutions = 0;

	std::vector<std::pair<int, int>> solution;

	std::cout << "Seed: " << seed << std::endl;
	std::cout << "Solving..." << std::endl;

	auto newContainerFolder = "./output/";
	auto newSubFolder = newContainerFolder + std::to_string(seed) + "\\";
	_mkdir(newContainerFolder);
	_mkdir(newSubFolder.c_str());

	while (true)
	{
		int roomLoads = (2 * (rand() % LOAD_MODIFIER)) + 1;

		LoadInitialRoom(0);
		solution.push_back(std::make_pair(LOAD_INITIAL_ROOM, 0));

		bool bats = true;
		for (int i = 0; i < roomLoads; i++)
		{
			int deallocations = 0;
			int currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();
			if (currentRoomDeallocations)
			{
				deallocations = rand() % currentRoomDeallocations;
			}
			else
			{
				deallocations = 0;
			}

			for (int j = 0; j < deallocations; j++)
			{
				auto dealloc = DeallocateRandomActor();
				solution.push_back(dealloc);
			}

			if (currentRoomNumber == 1 && bats)
			{
				auto batts = GetAllActorsOfID(0x015B);
				for (auto bat : batts)
				{
					Deallocate(bat);
					scene->GetRoom(currentRoomNumber)->ClearActor(bat);
				}
				bats = false;
			}

			if (smoke)
			{
				char smokeRNG = rand() % 2;
				if (smokeRNG)
				{
					AllocateTemporaryActor(0xA2);
					solution.push_back(std::make_pair(ALLOCATE, 0xA2));
				}
			}

			int allocations = 0;
			if (MAX_ALLOCATIONS_PER_STEP == 0)
			{
				allocations = 0;
			}
			else
			{
				allocations = rand() % MAX_ALLOCATIONS_PER_STEP;
			}

			for (int j = 0; j < allocations; j++)
			{
				auto allocate = AllocateRandomActor();
				solution.push_back(std::make_pair(ALLOCATE, allocate));
			}

			if (endAllocationStep)
			{
				char rng = rand() % 3;

				switch (rng)
				{
				case 0:
					break;
				case 1:
					AllocateTemporaryActor(0x3D);
					solution.push_back(std::make_pair(ALLOCATE, 0x3D));
					break;
				case 2:
					AllocateTemporaryActor(0x35);
					solution.push_back(std::make_pair(ALLOCATE, 0x35));
					break;
				default:
					break;
				}
			}

			int nextRoom = 0;
			int nextPlane = 0;

			if (currentRoomNumber == 0)
			{
				nextRoom = 1;
				nextPlane = 0;
			}
			else if (currentRoomNumber == 1)
			{
				nextRoom = 0;
				nextPlane = 0;
			}

			//also, should we allow the spawner to allocate or no?
			bool allocateSpawners = false;
			char spawnerRNG = rand() % 2;
			if (spawnerRNG)
			{
				allocateSpawners = true;
			}

			//actually perform room change using chosen room and plane
			ChangeRoom(nextRoom, nextPlane, nullptr, allocateSpawners);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, allocateSpawners));
			solution.push_back(std::make_pair(USE_PLANE, nextPlane));
		}

		//we're now standing in room 1 (pot room)

		char potDestroyRNG = rand() % 4;
		if (potDestroyRNG == 0)
		{
			Deallocate(0x82, 0);
			solution.push_back(std::make_pair(0x82, 0));

		}
		else if (potDestroyRNG == 1)
		{
			Deallocate(0x82, 1);
			solution.push_back(std::make_pair(0x82, 1));
		}
		else if (potDestroyRNG == 2)
		{
			Deallocate(0x82, 2);
			solution.push_back(std::make_pair(0x82, 2));
		}

		std::vector<std::pair<int, int>> pots = GetAddressesAndPrioritiesOfType(0x82, 'A');

		//also, should we allow the spawner to allocate or no?
		bool allocateSpawners = false;
		char spawnerRNG = rand() % 2;
		if (spawnerRNG)
		{
			allocateSpawners = true;
		}

		AllocateTemporaryActor(0xA2);
		ChangeRoom(0, 0, nullptr, allocateSpawners);
		solution.push_back(std::make_pair(SUPERSLIDE, 0));
		solution.push_back(std::make_pair(SPAWNERS, allocateSpawners));

		ChangeRoom(1, 0, nullptr, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 1));

		std::vector<std::pair<int, int>> chest = GetAddressesAndPrioritiesOfType(0x6, 'A');

		bool solutionFound = false;
		std::pair<std::pair<int, int>, std::pair<int, int>> solutionPair;

		for (auto c : chest)
		{
			for (auto pot : pots)
			{
				if (pot.first - c.first == 0x160)
				{
					solutionFound = true;
					solutionPair = std::make_pair(pot, c);
				}
			}

		}

		if (solutionFound)
		{
			std::cout << "SOLUTION FOUND\n";
			totalSolutions++;

			std::ofstream outputFile;
			std::string outputFilename = newSubFolder + "\\solution" + std::to_string(totalSolutions) + "_seed_" + std::to_string(seed) + ".txt";
			outputFile.open(outputFilename);

			outputFile << std::hex << "Pot Address | Priority: " << solutionPair.first.first << " | " << solutionPair.first.second <<
				" Guard Address | Priority: " << solutionPair.second.first << " | " << solutionPair.second.second << std::dec << std::endl;

			for (auto step : solution)
			{
				if (step.first == LOAD_INITIAL_ROOM)
				{
					outputFile << std::hex << "Load initial room: " << step.second << std::endl;
				}
				else if (step.first == CHANGE_ROOM)
				{
					outputFile << std::hex << "Load room: " << step.second;
				}
				else if (step.first == SPAWNERS)
				{
					outputFile << " | Spawners: " << step.second;
				}
				else if (step.first == USE_PLANE)
				{
					outputFile << " | Use plane: " << step.second << std::endl;
				}
				else if (step.first == ALLOCATE)
				{
					if (step.second != 0x0)
					{
						outputFile << std::hex << "Allocate: " << step.second << std::endl;
					}
				}
				else if (step.first == SUPERSLIDE)
				{
					outputFile << std::hex << "Superslide into room " << step.second << " with smoke still loaded using plane 0." << std::endl;
				}
				else if (step.first == 0)
				{
					;
				}
				else
				{
					outputFile << std::hex << "Dealloc: " << std::setw(2) << step.first << ", " << step.second << std::endl;
				}

			}
			outputFile.close();
		}

		ResetHeap();
		solution.clear();
		totalPermutations++;

		if (totalPermutations % 100000 == 0)
		{
			std::cout << std::dec << "Total permutations: " << totalPermutations << " | Total Solutions: " << totalSolutions << std::endl;
		}

	}
}