#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <direct.h>

#include "Heap.h"
#include "Room.h"
#include "Constants.h"

/* Rooms */
enum rooms {
	Kokiri_Forest_Square = 0,
	Skull_Kid_Minigame_Meadow = 1,
	Goron_City_Square = 2,
	Zoras_River_Square = 3,
	Blank_Square = 4,
	Forest_Bridge = 5,
	Forest_Stage_Meadow = 6,
	Boulder_and_Grass_Square = 7,
	Sacred_Forest_Square = 8,
	Skull_Kid_Stump = 9
};

/* transition ids:
0 = 0, 1
1 = 0, 9
2 = 1, 2
3 = 2, 3
4 = 3, 4
5 = 4, 6
6 = 4, 7
7 = 5, 9
8 = 7, 8
*/

void Heap::SolveOoT3D_LostWoods() {

	LoadInitialRoom(Sacred_Forest_Square);
	PrintHeap(0);

	ChangeRoom(Boulder_and_Grass_Square, 8, nullptr, false); // 1 is the room to load, 0 is the transition actor id you are using to load that room
	PrintHeap(0);

	AllocateTemporaryActor(0x57);

	ChangeRoom(Blank_Square, 6, nullptr, false); // 1 is the room to load, 0 is the transition actor id you are using to load that room
	PrintHeap(0);

	/*LoadInitialRoom(Kokiri_Forest_Square);
	PrintHeap(0);
	
	ChangeRoom(Skull_Kid_Minigame_Meadow, 0, nullptr, false); // 1 is the room to load, 0 is the transition actor id you are using to load that room
	PrintHeap(0);

	ChangeRoom(Goron_City_Square, 2, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Zoras_River_Square, 3, nullptr, false);
	PrintHeap(0);*/
	
	/*ChangeRoom(Blank_Square, 4, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Forest_Stage_Meadow, 5, nullptr, true);
	PrintHeap(0);

	ChangeRoom(Blank_Square, 5, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Boulder_and_Grass_Square, 6, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Sacred_Forest_Square, 8, nullptr, false);
	PrintHeap(0);*/

	/*ChangeRoom(Skull_Kid_Stump, 1, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Forest_Bridge, 7, nullptr, false);
	PrintHeap(0);

	ChangeRoom(Skull_Kid_Stump, 7, nullptr, false);
	PrintHeap(0);*/

}


