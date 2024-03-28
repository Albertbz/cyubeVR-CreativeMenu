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
const int ToggleFilled = 1343228703;
const int ToggleReplace = 1961506553;
const int RegisterFillType = 1004850721;
const int RegisterReplaceType = 1172271389;
const int Set = 666103118;
const int Undo = 205046945;
const int Redo = 861326958;
const int Save = 1564423633;

enum Shape { cuboid, sphere, cylinder, pyramid, cone };

CoordinateInBlocks location1 = CoordinateInBlocks(0, 0, 0);
CoordinateInBlocks location2 = CoordinateInBlocks(0, 0, 0);
Shape shape = cuboid;
bool filled = true;
bool replace = false;
BlockInfo fillType = BlockInfo(EBlockType::Air);
BlockInfo replaceType = BlockInfo(EBlockType::Air);
std::stack<std::vector<BlockInfoWithLocation>> undoOperations;
std::stack<std::vector<BlockInfoWithLocation>> redoOperations;
void* hintText;

int timesToIgnoreBlockPlacement = 0;
bool registerFillType = false;
bool registerReplaceType = false;

namespace fs = std::filesystem;

fs::path savedBuildsPath;

/************************************************************
	Custom Functions for the mod
*************************************************************/

bool isSameType(BlockInfo type1, BlockInfo type2) {
	if (type1.Type == type2.Type) {
		if (type1.Type == EBlockType::ModBlock) {
			return type1.CustomBlockID == type2.CustomBlockID ? true : false;
		}
		
		return true;
	}
	else if ((type1.Type == EBlockType::Stone && type2.Type == EBlockType::StoneMined) || (type2.Type == EBlockType::Stone && type1.Type == EBlockType::StoneMined)) {
		return true;
	}
	
	return false;
}

void setBlockAtLocationAndUpdateChangedBlocks(CoordinateInBlocks location, BlockInfo fillType, std::vector<BlockInfoWithLocation>& changedBlocks) {
	BlockInfo type = GetAndSetBlock(location, fillType);
	changedBlocks.push_back(BlockInfoWithLocation(type, location));
}

void replaceBlockAtLocationAndUpdateChangedBlocks(CoordinateInBlocks location, BlockInfo fillType, BlockInfo replaceType, std::vector<BlockInfoWithLocation>& changedBlocks) {
	BlockInfo type = GetBlock(location);

	if (isSameType(type, replaceType)) {
		SetBlock(location, fillType);
		changedBlocks.push_back(BlockInfoWithLocation(type, location));
	}
}

std::vector<BlockInfoWithLocation> setCuboid(CoordinateInBlocks location1, CoordinateInBlocks location2, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;

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

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(location, fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(location, fillType, changedBlocks);
				}
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoWithLocation> setSphere(CoordinateInBlocks center, double radius, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int16_t ceilRadius = (int16_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

	for (int64_t x = 0; x <= ceilRadius; x++) {
		for (int64_t y = 0; y <= ceilRadius; y++) {
			for (int16_t z = 0; z <= ceilRadius; z++) {
				int64_t dSq = x * x + y * y + (int64_t) z * z;

				if (dSq > radiusSq) {
					continue;
				}

				if (!filled && dSq <= innerRadiusSq) {
					continue;
				}

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, -z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, -z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, -z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, -z), fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, -z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, -z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, -z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, -z), fillType, changedBlocks);
				}
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoWithLocation> setCylinder(CoordinateInBlocks center, double radius, int16_t height, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int64_t ceilRadius = (int64_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

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

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, changedBlocks);
				}
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoWithLocation> setPyramid(CoordinateInBlocks center, int16_t levels, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;

	int16_t level = levels;

	for (int16_t z = 0; z <= levels; z++) {
		level--;
		for (int16_t x = 0; x <= level; x++) {
			for (int16_t y = 0; y <= level; y++) {

				if (!filled && !(x == level || y == level || z == 0 || z == levels)) {
					continue;
				}

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, changedBlocks);
				}
			}
		}
	}

	return changedBlocks;
}

std::vector<BlockInfoWithLocation> setCone(CoordinateInCentimeters center, double radius, int16_t height, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;

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

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, replaceType, changedBlocks);
					replaceBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(x, -y, z), fillType, changedBlocks);
					setBlockAtLocationAndUpdateChangedBlocks(center + CoordinateInBlocks(-x, -y, z), fillType, changedBlocks);
				}
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

void setShape(Shape shape, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;

	switch (shape)
	{
	case cuboid:
		changedBlocks = setCuboid(location1, location2, fillType, filled, replace, replaceType);
		break;
	case sphere:
	{
		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;
		int16_t zDiff = location1.Z - location2.Z;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

		changedBlocks = setSphere(location1, radius, fillType, filled, replace, replaceType);
		break;
	}
	case cylinder:
	{
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, min(location1.Z, location2.Z));

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		int16_t height = abs(location1.Z - location2.Z);

		changedBlocks = setCylinder(center, radius, height, fillType, filled, replace, replaceType);
		break;
	}
	case pyramid:
	{
		CoordinateInBlocks diff = location1 - location2;
		int16_t levels = (int16_t) max(abs(diff.X), max(abs(diff.Y), abs(diff.Z))) + 1;
		
		changedBlocks = setPyramid(location1, levels, fillType, filled, replace, replaceType);
		break;
	}
	case cone:
	{
		int height = abs(location1.Z - location2.Z);
		CoordinateInBlocks center = CoordinateInBlocks(location1.X, location1.Y, location1.Z - height);

		int64_t xDiff = location1.X - location2.X;
		int64_t yDiff = location1.Y - location2.Y;

		double radius = sqrt(xDiff * xDiff + yDiff * yDiff);

		changedBlocks = setCone(center, radius, height, fillType, filled, replace, replaceType);
		break;
	}
	default:
		break;
	}

	size_t amountOfChangedBlocks = changedBlocks.size();
	
	DestroyHintText(hintText);
	std::wstring text = amountOfChangedBlocks == 1 ? L"1 block was changed." : std::to_wstring(amountOfChangedBlocks) + L" blocks were changed.";
	hintText = SpawnHintTextAdvanced(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, text, 5, 0.5);

	undoOperations.push(changedBlocks);
	redoOperations = std::stack<std::vector<BlockInfoWithLocation>>(); // Empty the stack
}

void DoOperationAndUpdateOther(std::stack<std::vector<BlockInfoWithLocation>>& toDo, std::stack<std::vector<BlockInfoWithLocation>>& toUpdate) {
	std::vector<BlockInfoWithLocation> operation = toDo.top();
	std::vector<BlockInfoWithLocation> newOperation;


	for (auto& block : operation) {
		BlockInfo type = GetAndSetBlock(block.Location, block.Info);
		newOperation.push_back(BlockInfoWithLocation(type, block.Location));
	}

	toUpdate.push(newOperation);
	toDo.pop();
}

void undoOperation() {
	DoOperationAndUpdateOther(undoOperations, redoOperations);
}

void redoOperation() {
	DoOperationAndUpdateOther(redoOperations, undoOperations);
}

void writeBuildToFile(std::vector<BlockInfoWithLocation> build, fs::path filePath) {
	// Create file
	std::ofstream out(filePath, std::ios::binary);

	// Write the size of the vector
	size_t vectorSize = build.size();
	out.write(reinterpret_cast<char*>(&vectorSize), sizeof vectorSize);

	// Write the vector data
	for (size_t i = 0; i < vectorSize; i++) {
		// Write the location
		out.write(reinterpret_cast<char*>(&build[i].Location.X), sizeof build[i].Location.X);
		out.write(reinterpret_cast<char*>(&build[i].Location.Y), sizeof build[i].Location.Y);
		out.write(reinterpret_cast<char*>(&build[i].Location.Z), sizeof build[i].Location.Z);

		// Write the type
		out.write(reinterpret_cast<char*>(&build[i].Info.Type), sizeof build[i].Info.Type);
		out.write(reinterpret_cast<char*>(&build[i].Info.CustomBlockID), sizeof build[i].Info.CustomBlockID);
		out.write(reinterpret_cast<char*>(&build[i].Info.Rotation), sizeof build[i].Info.Rotation);
	}
}

std::vector<BlockInfoWithLocation> readBuildFromFile(fs::path filePath) {
	std::vector<BlockInfoWithLocation> build;

	std::ifstream in(filePath, std::ios::binary);

	// Read the size of the vector
	size_t vectorSize = 0;
	in.read(reinterpret_cast<char*>(&vectorSize), sizeof vectorSize);

	// Read the vector data
	for (size_t i = 0; i < vectorSize; i++) {
		BlockInfoWithLocation block;

		// Read the location
		in.read(reinterpret_cast<char*>(&block.Location.X), sizeof block.Location.X);
		in.read(reinterpret_cast<char*>(&block.Location.Y), sizeof block.Location.Y);
		in.read(reinterpret_cast<char*>(&block.Location.Z), sizeof block.Location.Z);

		// Read the type
		in.read(reinterpret_cast<char*>(&block.Info.Type), sizeof block.Info.Type);
		in.read(reinterpret_cast<char*>(&block.Info.CustomBlockID), sizeof block.Info.CustomBlockID);
		in.read(reinterpret_cast<char*>(&block.Info.Rotation), sizeof block.Info.Rotation);
		
		// Add to the vector
		build.push_back(block);
	}

	return build;
}

void saveSelection(CoordinateInBlocks location1, CoordinateInBlocks location2) {
	std::vector<BlockInfoWithLocation> selection;

	int64_t xMin = min(location1.X, location2.X);
	int64_t xMax = max(location1.X, location2.X);
	int64_t yMin = min(location1.Y, location2.Y);
	int64_t yMax = max(location1.Y, location2.Y);
	int16_t zMin = min(location1.Z, location2.Z);
	int16_t zMax = max(location1.Z, location2.Z);

	CoordinateInBlocks startLocation = CoordinateInBlocks(xMin, yMin, zMin);

	CoordinateInBlocks location;
	BlockInfo type;
	for (int64_t x = xMin; x <= xMax; x++) {
		for (int64_t y = yMin; y <= yMax; y++) {
			for (int16_t z = zMin; z <= zMax; z++) {
				location = CoordinateInBlocks(x, y, z);
				type = GetBlock(location);
				selection.push_back(BlockInfoWithLocation(type, location - startLocation));
			}
		}
	}

	std::time_t time = std::time(nullptr);
	std::tm timestamp;
	errno_t err = localtime_s(&timestamp, &time);
	std::wstringstream wss;
	wss << std::put_time(&timestamp, L"%F %H-%M-%S");
	std::wstring nameOfFile = GetWorldName() + L" " + wss.str();

	fs::path filePath = savedBuildsPath / nameOfFile;

	writeBuildToFile(selection, filePath);
}

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { PlaceableCoalBlockID, PlaceableCopperBlockID, PlaceableCrystalBlockID,
								PlaceableGoldBlockID, PlaceableIronBlockID,
								ToggleFilled, ToggleReplace,
								Location1, Location2,
								CuboidShape, SphereShape, CylinderShape, PyramidShape, ConeShape,
								RegisterFillType, RegisterReplaceType,
								Set, Undo, Redo,
								Save}; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

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
	case ToggleReplace:
		replace = !replace;
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
	case RegisterReplaceType:
		timesToIgnoreBlockPlacement = 2;
		registerReplaceType = true;
		break;
	case Set:
		setShape(shape, fillType, filled, replace, replaceType);
		break;
	case Undo:
		undoOperation();
		break;
	case Redo:
		redoOperation();
		break;
	case Save:
		saveSelection(location1, location2);
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
	// Create the SavedBuilds folder and save the path
	std::wstring globalModFolderPath = GetThisModGlobalSaveFolderPath(L"CreativeMenu");
	savedBuildsPath = fs::path::path(globalModFolderPath) / "SavedBuilds";
	bool createdDirectory = fs::create_directory(savedBuildsPath);
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
	else if (registerReplaceType) {
		registerReplaceType = false;
		replaceType = Type;
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