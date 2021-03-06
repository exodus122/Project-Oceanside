#pragma once

const int MM_JP1_START = 0x40B670;
const int MM_US0_START = 0x40b140;
const int MM_JP0_START = 0x40B3B0;
const int MM_JP_GC_START = 0x3A5870;
const int OOT3D_START = 0x9907450;

const int MM_JP0_LINK_SIZE = 0x30;
const int MM_JP1_LINK_SIZE = 0x30;
const int MM_US0_LINK_SIZE = 0x10;
const int OOT3D_LINK_SIZE = 0x10;

enum SolverTypes {
	SuccessiveActorSolver,
	RandomAssortment,
	Test,
	DFSRM,
	OneRandomPerm,
	nop
};

enum Version
{
	OOT,
	OOT_3D,
	MM_US,
	MM_JP,
	MM_JP_GC
};

enum SolverStep
{
	ALLOCATE = 0xdddd,
	DEALLOCATE,
	CHANGE_ROOM,
	SUPERSLIDE,
	USE_PLANE,
	LOAD_INITIAL_ROOM,
	CARRY,
	SPAWNERS
};