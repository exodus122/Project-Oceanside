#pragma once

#include <vector>

#include "./json.hpp"
#include "./Room.h"

class Scene
{
public:
	// Constructor
	Scene(char version, const std::string& sceneFile); // creates the scene object, which populates the rooms and transition actors data

	// Main functions
	void DumpSceneInfo() const;
	void ResetClearedActors(); // reset cleared actors in all rooms in the scene

	// Getters
	Room* GetRoom(int roomNumber) const;
	int NumberOfRooms() const;
	std::map<int, Node*> GetTransitionActors() const;
	int NumberOfTransitionActors() const;
	nlohmann::json GetActorJSON() const; // unused

private:
	// Constructor subroutines
	void ParseSceneJson(const std::string& filename);
	void ParseActorJson(char version);
	void LoadScene(); // creates rooms actors in the rooms
	void OutputJsonExceptionInformation(nlohmann::json::parse_error& error); // output json parsing exception info

	// Data
	std::vector<Room*> rooms;
	std::map<int, Node*> transitionActors;
	nlohmann::json actorJson;
	nlohmann::json sceneJson;
};

