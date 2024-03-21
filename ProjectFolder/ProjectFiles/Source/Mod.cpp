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
const int CuboidShape = 1919599248;
const int SphereShape = 272127303;
const int CylinderShape = 1174910537;
const int PyramidShape = 210618550;
const int ConeShape = 909836507;
const int RegisterFillType = 1004850721;
const int ToggleFilled = 1343228703;
const int Set = 666103118;
const int Undo = 205046945;

enum Shape { cuboid, sphere, cylinder, pyramid, cone };

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
Shape shape = cuboid;
bool filled = true;
BlockInfo fillType = BlockInfo(EBlockType::Air);
std::stack<std::vector<BlockInfoLocation>> operations;

int timesToIgnoreBlockPlacement = 0;
bool registerFillType = false;


/************************************************************
	Custom Functions for the mod
*************************************************************/

std::vector<BlockInfoLocation> setCuboid(CoordinateInBlocks location1, CoordinateInBlocks location2, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;

	int64_t xMin = min(location1.X, location2.X);
	int64_t xMax = max(location1.X, location2.X);
	int64_t yMin = min(location1.Y, location2.Y);
	int64_t yMax = max(location1.Y, location2.Y);
	int16_t zMin = min(location1.Z, location2.Z);
	int16_t zMax = max(location1.Z, location2.Z);

	CoordinateInBlocks location;
	for (int64_t x = xMin; x <= xMax; x++) {
		for (int64_t y = yMin; y <= yMax; y++) {
			for (int16_t z = zMin; z <= zMax; z++) {
				location = CoordinateInBlocks(x, y, z);

				if (!filled && !(x == xMin || x == xMax || y == yMin || y == yMax || z == zMin || z == zMax)) {
					continue;
				}
				BlockInfo type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoLocation> setSphere(CoordinateInBlocks center, double radius, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int16_t ceilRadius = (int16_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

	CoordinateInBlocks location;
	for (int16_t x = 0; x <= ceilRadius; x++) {
		for (int16_t y = 0; y <= ceilRadius; y++) {
			for (int16_t z = 0; z <= ceilRadius; z++) {
				int64_t dSq = x * x + y * y + z * z;

				if (dSq > radiusSq) {
					continue;
				}

				if (!filled && dSq <= innerRadiusSq) {
					continue;
				}

				location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
				
				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, y, -z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, -z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, -z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, -z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoLocation> setCylinder(CoordinateInBlocks center, double radius, int16_t height, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int64_t ceilRadius = (int64_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

	CoordinateInBlocks location;
	for (int64_t x = 0; x <= ceilRadius; x++) {
		for (int64_t y = 0; y <= ceilRadius; y++) {
			double dSq = (double) x * x + y * y;

			if (dSq > radiusSq) {
				continue;
			}

			for (int16_t z = 0; z <= height; z++) {
				if (!filled && dSq <= innerRadiusSq && !(z == 0 || z == height)) {
					continue;
				}

				location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoLocation> setPyramid(CoordinateInBlocks center, int16_t levels, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;

	int16_t level = levels;

	CoordinateInBlocks location;
	for (int16_t z = 0; z <= levels; z++) {
		level--;
		for (int16_t x = 0; x <= level; x++) {
			for (int16_t y = 0; y <= level; y++) {

				if (!filled && !(x == level || y == level || z == 0 || z == levels)) {
					continue;
				}

				location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoLocation> setCone(CoordinateInCentimeters center, double radius, int16_t height, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;

	double deltaRadius = 0;
	if (height > 0) {
		deltaRadius = radius / height;
	}
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int64_t ceilRadius = (int64_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

	double nextRadius = radius - deltaRadius;
	double nextRadiusSq = nextRadius * nextRadius;
	
	CoordinateInBlocks location;
	for (int z = 0; z <= height; z++) {
		for (int x = 0; x <= ceilRadius; x++) {
			double xSq = x * x;
			for (int y = 0; y <= ceilRadius; y++) {
				double dSq = xSq + y * y;
				
				if (dSq > radiusSq) {
					continue;
				}

				if (!filled && dSq <= innerRadiusSq && !(z == 0 || z == height) && dSq <= nextRadiusSq) {
					continue;
				}

				location = center + CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				changedBlocks.push_back(BlockInfoLocation(type, location));
			}
		}

		radius = nextRadius;
		radiusSq = nextRadiusSq;
		ceilRadius = (int64_t) ceil(radius);

		innerRadius = radius - 1;
		innerRadiusSq = innerRadius * innerRadius;

		nextRadius = radius - deltaRadius;
		nextRadiusSq = nextRadius * nextRadius;
	}

	return changedBlocks;
}

void setShape(Shape shape, BlockInfo fillType, bool filled) {
	std::vector<BlockInfoLocation> changedBlocks;

	switch (shape)
	{
	case cuboid:
		changedBlocks = setCuboid(location1, location2, fillType, filled);
		break;
	case sphere:
	{
		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;
		int16_t zDiff = location1.Z - location2.Z;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

		changedBlocks = setSphere(location1, radius, fillType, filled);
		break;
	}
	case cylinder:
	{
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, min(location1.Z, location2.Z));

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		int16_t height = abs(location1.Z - location2.Z);

		changedBlocks = setCylinder(center, radius, height, fillType, filled);
		break;
	}
	case pyramid:
	{
		CoordinateInBlocks diff = location1 - location2;
		int16_t levels = (int16_t) max(abs(diff.X), max(abs(diff.Y), abs(diff.Z))) + 1;
		
		changedBlocks = setPyramid(location1, levels, fillType, filled);
		break;
	}
	case cone:
	{
		int height = abs(location1.Z - location2.Z);
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, location1.Z - height);

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		changedBlocks = setCone(center, radius, height, fillType, filled);
		break;
	}
	default:
		break;
	}

	operations.push(changedBlocks);
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
								ToggleFilled,
								Location1, Location2,
								CuboidShape, SphereShape, CylinderShape, PyramidShape, ConeShape,
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
	case ToggleFilled:
		filled = !filled;
		break;
	case CuboidShape:
		shape = cuboid;
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
	case ConeShape:
		shape = cone;
		break;
	case RegisterFillType:
		timesToIgnoreBlockPlacement = 2;
		registerFillType = true;
		break;
	case Set:
		setShape(shape, fillType, filled);
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