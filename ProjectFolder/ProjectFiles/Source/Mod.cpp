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
BlockInfo fillType = BlockInfo(EBlockType::Air);
std::stack<std::vector<BlockInfoLocation>> operations;

int timesToIgnoreBlockPlacement = 0;
bool registerFillType = false;

/************************************************************
	Custom Functions for the mod
*************************************************************/

void setCuboid(CoordinateInBlocks location1, CoordinateInBlocks location2, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;

	int64_t xMin = min(location1.X, location2.X);
	int64_t xMax = max(location1.X, location2.X);
	int64_t yMin = min(location1.Y, location2.Y);
	int64_t yMax = max(location1.Y, location2.Y);
	int16_t zMin = min(location1.Z, location2.Z);
	int16_t zMax = max(location1.Z, location2.Z);


	for (int64_t x = xMin; x <= xMax; x++) {
		for (int64_t y = yMin; y <= yMax; y++) {
			for (int16_t z = zMin; z <= zMax; z++) {
				CoordinateInBlocks location = CoordinateInBlocks(x, y, z);
				BlockInfo type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));
			}
		}
	}

	operations.push(operation);
}

void setSphere(CoordinateInBlocks center, double radius, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int16_t ceilRadius = (int16_t) ceil(radius);

	for (int16_t x = 0; x <= ceilRadius; x++) {
		for (int16_t y = 0; y <= ceilRadius; y++) {
			for (int16_t z = 0; z <= ceilRadius; z++) {
				int64_t dSq = x * x + y * y + z * z;

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

void setCylinder(CoordinateInBlocks center, double radius, int16_t height, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int64_t ceilRadius = (int64_t) ceil(radius);

	for (int64_t x = 0; x <= ceilRadius; x++) {
		for (int64_t y = 0; y <= ceilRadius; y++) {
			double dSq = (double) x * x + y * y;

			if (dSq > radiusSq) {
				continue;
			}

			for (int16_t z = 0; z <= height; z++) {
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

void setPyramid(CoordinateInBlocks center, int16_t levels, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;

	int16_t level = levels;

	for (int16_t z = 0; z <= levels; z++) {
		level--;
		for (int16_t x = 0; x <= level; x++) {
			for (int16_t y = 0; y <= level; y++) {
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

void setCone(CoordinateInCentimeters center, double radius, int16_t height, BlockInfo fillType) {
	std::vector<BlockInfoLocation> operation;

	Log(L"Original radius: " + std::to_wstring(radius));
	Log(L"Height: " + std::to_wstring(height));
	double deltaRadius = 0;
	if (height > 0) {
		deltaRadius = radius / height;
	}
	Log(L"deltaRadius " + std::to_wstring(deltaRadius));
	
	radius += 0.5;
	double radiusSq = radius * radius;
	
	int64_t ceilRadius = (int64_t) ceil(radius);

	for (int z = 0; z <= height; z++) {
		Log(L"New radius: " + std::to_wstring(radius));
		Log(L"Radius squared: " + std::to_wstring(radiusSq));
		Log(L"Radius ceil: " + std::to_wstring(ceilRadius));

		Log(L"Z: " + std::to_wstring(z));
		for (int x = 0; x <= ceilRadius; x++) {
			double xSq = x * x;
			for (int y = 0; y <= ceilRadius; y++) {

				double dSq = xSq + y * y;
				
				Log(L"Distance squared: " + std::to_wstring(dSq));
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

				location = center + CoordinateInBlocks(-x, -y, z);
				type = GetAndSetBlock(location, fillType);
				operation.push_back(BlockInfoLocation(type, location));

			}
		}

		radius -= deltaRadius;
		radiusSq = radius * radius;
		ceilRadius = (int64_t) ceil(radius);
	}

	operations.push(operation);
}

void setShape(Shape shape, BlockInfo fillType) {
	switch (shape)
	{
	case cuboid:
		setCuboid(location1, location2, fillType);
		break;
	case sphere:
	{
		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;
		int16_t zDiff = location1.Z - location2.Z;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

		setSphere(location1, radius, fillType);
		break;
	}
	case cylinder:
	{
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, min(location1.Z, location2.Z));

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		int16_t height = abs(location1.Z - location2.Z);

		setCylinder(center, radius, height, fillType);
		break;
	}
	case pyramid:
	{
		CoordinateInBlocks diff = location1 - location2;
		int16_t levels = (int16_t) max(abs(diff.X), max(abs(diff.Y), abs(diff.Z))) + 1;
		
		setPyramid(location1, levels, fillType);
		break;
	}
	case cone:
	{
		int height = abs(location1.Z - location2.Z);
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, location1.Z - height);

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		setCone(center, radius, height, fillType);
		break;
	}
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