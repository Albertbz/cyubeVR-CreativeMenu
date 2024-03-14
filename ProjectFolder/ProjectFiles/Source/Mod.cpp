#include "GameAPI.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stack>

/************************************************************
	Custom Variables for the mod
*************************************************************/

const int PlaceableCoalBlockID = 1169799691;
const int PlaceableCopperBlockID = 1169799692;
const int PlaceableCrystalBlockID = 1169799693;
const int PlaceableGoldBlockID = 1169799694;
const int PlaceableIronBlockID = 1169799695;

const int Location1 = 1066913798;
const int Location2 = 1823382047;
const int CubeShape = 1919599248;
const int SphereShape = 272127303;
const int CylinderShape = 1174910537;
const int PyramidShape = 210618550;
const int RegisterFillType = 1004850721;
const int Set = 666103118;
const int Undo = 205046945;

enum Shape { cube, sphere, cylinder, pyramid };

struct BlockInfoLocation {
	BlockInfo type;
	CoordinateInBlocks location;

	BlockInfoLocation(BlockInfo type, CoordinateInBlocks location) {
		this->type = type;
		this->location = location;
	}
};

CoordinateInBlocks location1 = CoordinateInBlocks(0, 0, 0);
CoordinateInBlocks location2 = CoordinateInBlocks(0, 0, 0);
Shape shape = cube;
BlockInfo fillType = BlockInfo(EBlockType::Air);
std::stack<std::vector<BlockInfoLocation>> operations;

int timesToIgnoreBlockPlacement = 0;
bool registerFillType = false;

/************************************************************
	Custom Functions for the mod
*************************************************************/

void setCube(CoordinateInBlocks location1, CoordinateInBlocks location2, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;

	int xMin = min(location1.X, location2.X);
	int xMax = max(location1.X, location2.X);
	int yMin = min(location1.Y, location2.Y);
	int yMax = max(location1.Y, location2.Y);
	int zMin = min(location1.Z, location2.Z);
	int zMax = max(location1.Z, location2.Z);


	for (int x = xMin; x <= xMax; x++) {
		for (int y = yMin; y <= yMax; y++) {
			for (int z = zMin; z <= zMax; z++) {
				CoordinateInBlocks location = CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	operations.push(operation);
}

void setSphere(CoordinateInBlocks center, double radius, BlockInfo fillType) {
	radius += 0.5;
	double radiusSq = radius * radius;
	int ceilRadius = ceil(radius);

	std::vector<BlockInfoLocation> operation;

	for (int x = 0; x <= ceilRadius; x++) {
		for (int y = 0; y <= ceilRadius; y++) {
			for (int z = 0; z <= ceilRadius; z++) {
				double dSq = x * x + y * y + z * z;

				if (dSq > radiusSq) {
					continue;
				}

				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));
				
				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, y, -z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, -z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, -z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, -z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	operations.push(operation);
}

void setCylinder(CoordinateInBlocks center, double radius, int height, BlockInfo fillType) {
	radius += 0.5;
	double radiusSq = radius * radius;
	int ceilRadius = ceil(radius);

	std::vector<BlockInfoLocation> operation;

	for (int x = 0; x <= ceilRadius; x++) {
		for (int y = 0; y <= ceilRadius; y++) {
			double dSq = x * x + y * y;

			if (dSq > radiusSq) {
				continue;
			}

			for (int z = 0; z <= height; z++) {
				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	operations.push(operation);
}

void setShape(Shape shape, BlockInfo fillType) {
	switch (shape)
	{
	case cube:
		setCube(location1, location2, fillType);
		break;
	case sphere:
	{
		int xDiff = location1.X - location2.X;
		int yDiff = location1.Y - location2.Y;
		int zDiff = location1.Z - location2.Z;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
		setSphere(location1, radius, fillType);
		break;
	}
	case cylinder:
	{
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, min(location1.Z, location2.Z));

		int xDiff = location1.X - location2.X;
		int yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		int height = abs(location1.Z - location2.Z);
		setCylinder(center, radius, height, fillType);
		break;
	}
	case pyramid:
		break;
	default:
		break;
	}
}

void undoOperation() {
	std::vector<BlockInfoLocation> operation = operations.top();

	for (auto& block : operation) {
		SetBlock(block.location, block.type);
	}

	operations.pop();
}

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { PlaceableCoalBlockID, PlaceableCopperBlockID, PlaceableCrystalBlockID,
								PlaceableGoldBlockID, PlaceableIronBlockID,
								Location1, Location2,
								CubeShape, SphereShape, CylinderShape, PyramidShape,
								RegisterFillType,
								Set, Undo}; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 0;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	switch (CustomBlockID) {
	case PlaceableCoalBlockID:
		SetBlock(At, EBlockType::Ore_Coal);
		break;
	case PlaceableCopperBlockID:
		SetBlock(At, EBlockType::Ore_Copper);
		break;
	case PlaceableCrystalBlockID:
		SetBlock(At, EBlockType::CrystalBlock);
		break;
	case PlaceableGoldBlockID:
		SetBlock(At, EBlockType::Ore_Gold);
		break;
	case PlaceableIronBlockID:
		SetBlock(At, EBlockType::Ore_Iron);
		break;
	case Location1:
		location1 = At;
		break;
	case Location2:
		location2 = At;
		break;
	case CubeShape:
		shape = cube;
		break;
	case SphereShape:
		shape = sphere;
		break;
	case CylinderShape:
		shape = cylinder;
		break;
	case PyramidShape:
		shape = pyramid;
		break;
	case RegisterFillType:
		timesToIgnoreBlockPlacement = 2;
		registerFillType = true;
		break;
	case Set:
		setShape(shape, fillType);
		break;
	case Undo:
		undoOperation();
		break;
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{
	
}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	
}



// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	
}

// Run once when the world is exited
void Event_OnExit()
{
	
}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{
	if (timesToIgnoreBlockPlacement != 0) {
		timesToIgnoreBlockPlacement--;
	}
	else if (registerFillType) {
		registerFillType = false;
		fillType = Type;
	}
}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is hit by a tool
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/