#include "GameAPI.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stack>
#include <map>

/************************************************************
	Custom Variables for the mod
*************************************************************/

int buildFileVersionNumber = 1;

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
const int Refresh = 1953081933;
const int Remove = 1136264579;
const int Open = 1926803967;
const int RegisterLoadID = 756713455;
const int SetLoad = 1836453716;
const int LoadRotationPositiveY = 2027769099;
const int LoadRotationNegativeX = 719875352;
const int LoadRotationNegativeY = 1299106903;
const int LoadRotationPositiveX = 1007662264;

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
int loadID = 0;
enum Rotation { PositiveY, NegativeX, NegativeY, PositiveX };
Rotation currentLoadRotation = PositiveY;
enum RotationDirection { Left, Right, Opposite };

int timesToIgnoreBlockPlacement = 0;
bool registerFillType = false;
bool registerReplaceType = false;
bool registerRemove = false;
bool registerLoad = false;

namespace fs = std::filesystem;

fs::path savedBuildsPath;

struct BuildInfo {
	fs::path path;
	std::wstring name;
	std::wstring timeOfCreation;
	int depth;
	int width;
	int height;
	std::map<EBlockType, int> nativeMaterials;
	std::map<UniqueID, int> customMaterials;
	std::vector<BlockInfoWithLocation> blocks;
};

std::map<int, BuildInfo> builds;

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
	if (location.Z < 1 || location.Z > 799) return;

	BlockInfo type = GetAndSetBlock(location, fillType);
	changedBlocks.push_back(BlockInfoWithLocation(type, location));
}

void replaceBlockAtLocationAndUpdateChangedBlocks(CoordinateInBlocks location, BlockInfo fillType, BlockInfo replaceType, std::vector<BlockInfoWithLocation>& changedBlocks) {
	if (location.Z < 1 || location.Z > 799) return;

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

	for (int64_t x = -ceilRadius; x <= ceilRadius; x++) {
		for (int64_t y = -ceilRadius; y <= ceilRadius; y++) {
			for (int16_t z = -ceilRadius; z <= ceilRadius; z++) {
				int64_t dSq = x * x + y * y + (int64_t) z * z;

				if (dSq > radiusSq) {
					continue;
				}

				if (!filled && dSq <= innerRadiusSq) {
					continue;
				}

				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);

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

std::vector<BlockInfoWithLocation> setCylinder(CoordinateInBlocks center, double radius, int16_t height, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;
	
	radius += 0.5;
	double radiusSq = radius * radius;
	int64_t ceilRadius = (int64_t) ceil(radius);

	double innerRadius = radius - 1;
	double innerRadiusSq = innerRadius * innerRadius;

	for (int64_t x = -ceilRadius; x <= ceilRadius; x++) {
		for (int64_t y = -ceilRadius; y <= ceilRadius; y++) {
			double dSq = (double) x * x + y * y;

			if (dSq > radiusSq) {
				continue;
			}

			for (int16_t z = 0; z <= height; z++) {
				if (!filled && dSq <= innerRadiusSq && !(z == 0 || z == height)) {
					continue;
				}

				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);

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

std::vector<BlockInfoWithLocation> setPyramid(CoordinateInBlocks center, int16_t levels, BlockInfo fillType, bool filled, bool replace, BlockInfo replaceType) {
	std::vector<BlockInfoWithLocation> changedBlocks;

	int16_t level = levels;

	for (int16_t z = 0; z <= levels; z++) {
		level--;
		for (int16_t x = -level; x <= level; x++) { 
			for (int16_t y = -level; y <= level; y++) {

				if (!filled && !(x == level || x == -level || y == level || y == -level || z == 0 || z == levels)) {
					continue;
				}

				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);

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
		for (int64_t x = -ceilRadius; x <= ceilRadius; x++) {
			int64_t xSq = x * x;
			for (int64_t y = -ceilRadius; y <= ceilRadius; y++) {
				int64_t dSq = xSq + y * y;
				
				if (dSq > radiusSq) {
					continue;
				}

				if (!filled && dSq <= innerRadiusSq && !(z == 0 || z == height) && dSq <= nextRadiusSq) {
					continue;
				}

				CoordinateInBlocks location = center + CoordinateInBlocks(x, y, z);

				if (replace) {
					replaceBlockAtLocationAndUpdateChangedBlocks(location, fillType, replaceType, changedBlocks);
				}
				else {
					setBlockAtLocationAndUpdateChangedBlocks(location, fillType, changedBlocks);
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

void newOperation(std::vector<BlockInfoWithLocation> changedBlocks) {
	undoOperations.push(changedBlocks);
	redoOperations = std::stack<std::vector<BlockInfoWithLocation>>(); // Empty the stack
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

	newOperation(changedBlocks);
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

void writeBuildToFile(BuildInfo buildInfo) {
	// Create file
	std::ofstream out(buildInfo.path, std::ios::binary);

	// Write the build file version number
	out.write(reinterpret_cast<char*>(&buildFileVersionNumber), sizeof buildFileVersionNumber);

	// Write the time of creation
	fs::path timeOfCreationAsPath = buildInfo.timeOfCreation;
	std::string timeOfCreationAsString = timeOfCreationAsPath.string();
	size_t timeOfCreationStringSize = timeOfCreationAsString.size();
	out.write(reinterpret_cast<char*>(&timeOfCreationStringSize), sizeof timeOfCreationStringSize);
	out.write((timeOfCreationAsString.c_str()), timeOfCreationStringSize);

	// Write the depth, width and height of the build
	out.write(reinterpret_cast<char*>(&buildInfo.depth), sizeof buildInfo.depth);
	out.write(reinterpret_cast<char*>(&buildInfo.width), sizeof buildInfo.width);
	out.write(reinterpret_cast<char*>(&buildInfo.height), sizeof buildInfo.height);

	// Write the size of the native materials map
	size_t nativeMaterialsMapSize = buildInfo.nativeMaterials.size();
	out.write(reinterpret_cast<char*>(&nativeMaterialsMapSize), sizeof nativeMaterialsMapSize);

	// Write the native materials map data
	std::map<EBlockType, int>::iterator itN;
	for (itN = buildInfo.nativeMaterials.begin(); itN != buildInfo.nativeMaterials.end(); itN++) {
		// Write the type of the native material
		EBlockType type = itN->first;
		out.write(reinterpret_cast<char*>(&type), sizeof type);

		// Write the amount of the native material
		out.write(reinterpret_cast<char*>(&itN->second), sizeof itN->second);
	}

	// Write the size of the custom materials map
	size_t customMaterialsMapSize = buildInfo.customMaterials.size();
	out.write(reinterpret_cast<char*>(&customMaterialsMapSize), sizeof customMaterialsMapSize);

	// Write the custom materials map data
	std::map<UniqueID, int>::iterator itC;
	for (itC = buildInfo.customMaterials.begin(); itC != buildInfo.customMaterials.end(); itC++) {
		// Write the ID of the custom material
		UniqueID uniqueID = itC->first;
		out.write(reinterpret_cast<char*>(&uniqueID), sizeof uniqueID);

		// Write the amount of the custom material
		out.write(reinterpret_cast<char*>(&itC->second), sizeof itC->second);
	}

	// Write the size of the blocks vector
	size_t blocksVectorSize = buildInfo.blocks.size();
	out.write(reinterpret_cast<char*>(&blocksVectorSize), sizeof blocksVectorSize);

	// Write the blocks vector data
	for (size_t i = 0; i < blocksVectorSize; i++) {
		// Write the location
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Location.X), sizeof buildInfo.blocks[i].Location.X);
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Location.Y), sizeof buildInfo.blocks[i].Location.Y);
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Location.Z), sizeof buildInfo.blocks[i].Location.Z);

		// Write the type
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Info.Type), sizeof buildInfo.blocks[i].Info.Type);
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Info.CustomBlockID), sizeof buildInfo.blocks[i].Info.CustomBlockID);
		out.write(reinterpret_cast<char*>(&buildInfo.blocks[i].Info.Rotation), sizeof buildInfo.blocks[i].Info.Rotation);
	}
}

BuildInfo readBuildFromFile(fs::path filePath) {
	std::ifstream in(filePath, std::ios::binary);

	// Read the build file version number
	int buildFileVersion = 0;
	in.read(reinterpret_cast<char*>(&buildFileVersion), sizeof buildFileVersion);

	// Variable to store time of creation
	std::string timeOfCreationAsString;

	// Variables to store depth, width and height
	int depth = 0, width = 0, height = 0;

	// Variables to store materials
	std::map<EBlockType, int> nativeMaterials;
	std::map<UniqueID, int> customMaterials;
	
	// Variable to store blocks
	std::vector<BlockInfoWithLocation> blocks;

	switch (buildFileVersion) {
	case 1:
		// Read the time of creation
		size_t timeOfCreationStringSize = 0;
		in.read(reinterpret_cast<char*>(&timeOfCreationStringSize), sizeof timeOfCreationStringSize);
		timeOfCreationAsString.resize(timeOfCreationStringSize);
		in.read(&timeOfCreationAsString[0], timeOfCreationStringSize);

		// Read the depth, width and height of the build
		in.read(reinterpret_cast<char*>(&depth), sizeof depth);
		in.read(reinterpret_cast<char*>(&width), sizeof width);
		in.read(reinterpret_cast<char*>(&height), sizeof height);

		// Read the size of the native materials map
		size_t nativeMaterialsMapSize = 0;
		in.read(reinterpret_cast<char*>(&nativeMaterialsMapSize), sizeof nativeMaterialsMapSize);

		// Read the native materials map data
		for (size_t i = 0; i < nativeMaterialsMapSize; i++) {
			// Read the type
			EBlockType type;
			in.read(reinterpret_cast<char*>(&type), sizeof type);
			
			// Read the amount
			int amount = 0;
			in.read(reinterpret_cast<char*>(&amount), sizeof amount);

			// Put into map
			nativeMaterials[type] = amount;
		}

		// Read the size of the custom materials map
		size_t customMaterialsMapSize = 0;
		in.read(reinterpret_cast<char*>(&customMaterialsMapSize), sizeof customMaterialsMapSize);

		// Read the custom materials map data
		for (size_t i = 0; i < customMaterialsMapSize; i++) {
			// Read the ID
			UniqueID uniqueID;
			in.read(reinterpret_cast<char*>(&uniqueID), sizeof uniqueID);

			// Read the amount
			int amount = 0;
			in.read(reinterpret_cast<char*>(&amount), sizeof amount);

			// Put into map
			customMaterials[uniqueID] = amount;
		}

		// Read the size of the blocks vector
		size_t blocksVectorSize = 0;
		in.read(reinterpret_cast<char*>(&blocksVectorSize), sizeof blocksVectorSize);

		// Read the blocks vector data
		for (size_t i = 0; i < blocksVectorSize; i++) {
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
			blocks.push_back(block);
		}
		break;
	}

	fs::path timeOfCreationAsPath = timeOfCreationAsString;
	std::wstring timeOfCreation = timeOfCreationAsPath.wstring();
	return BuildInfo(filePath, filePath.filename().replace_extension().wstring(), timeOfCreation, depth, width, height, nativeMaterials, customMaterials, blocks);
}

/*
	Write the names and IDs of builds to file and update the map of builds.
*/
void refreshBuilds() {
	std::wstring installFolderPath = GetThisModInstallFolderPath();
	std::wstring ue4ModsFolderPath = installFolderPath.substr(0, installFolderPath.find(L"APIMods")) + L"UE4Mods\\CreativeMenu__V1";

	std::ofstream file = std::ofstream{ ue4ModsFolderPath + L"\\Builds" };

	builds.clear();

	for (const auto& entry : fs::directory_iterator(savedBuildsPath)) {
		fs::path path = entry.path();

		// If extension is not correct, skip the file
		if (path.extension() != ".cyubeVRBuild") {
			continue;
		}

		BuildInfo buildInfo = readBuildFromFile(path);

		int id = GetRandomInt<1, 2147483646>();
		file << path.filename().replace_extension().string() << '|' << id << '|' << buildInfo.depth << '|' << buildInfo.width << '|' << buildInfo.height << '|';

		std::map<EBlockType, int>::iterator itN;
		for (itN = buildInfo.nativeMaterials.begin(); itN != buildInfo.nativeMaterials.end(); itN++) {
			file << (int)itN->first << '.' << itN->second << ',';
		}

		file << '-';

		std::map<UniqueID, int>::iterator itC;
		for (itC = buildInfo.customMaterials.begin(); itC != buildInfo.customMaterials.end(); itC++) {
			file << (int)itC->first << '.' << itC->second << ',';
		}

		fs::path timeOfCreationAsPath = buildInfo.timeOfCreation;
		std::string timeOfCreationAsString = timeOfCreationAsPath.string();
		file << '|' << timeOfCreationAsString << '\n';

		builds.insert({ id, buildInfo });
	}
	
	SpawnBPModActor(GetPlayerLocation(), L"CreativeMenu", L"RefreshBuilds");
}

void removeBuild(int id) {
	BuildInfo buildInfo = builds.find(id)->second;

	fs::remove(buildInfo.path);

	refreshBuilds();
}

void saveSelection(CoordinateInBlocks location1, CoordinateInBlocks location2) {
	int64_t xMin = min(location1.X, location2.X);
	int64_t xMax = max(location1.X, location2.X);
	int64_t yMin = min(location1.Y, location2.Y);
	int64_t yMax = max(location1.Y, location2.Y);
	int16_t zMin = min(location1.Z, location2.Z);
	int16_t zMax = max(location1.Z, location2.Z);

	CoordinateInBlocks startLocation = CoordinateInBlocks(xMin, yMin, zMin);

	CoordinateInBlocks location;
	BlockInfo type;
	std::vector<BlockInfoWithLocation> selection;
	std::map<EBlockType, int> nativeMaterials;
	std::map<UniqueID, int> customMaterials;
	for (int64_t x = xMin; x <= xMax; x++) {
		for (int64_t y = yMin; y <= yMax; y++) {
			for (int16_t z = zMin; z <= zMax; z++) {
				location = CoordinateInBlocks(x, y, z);
				type = GetBlock(location);

				// Add the type to the correct materials map
				if (type.Type == EBlockType::ModBlock) {
					customMaterials[type.CustomBlockID] += 1;
				}
				else {
					nativeMaterials[type.Type] += 1;
				}
				selection.push_back(BlockInfoWithLocation(type, location - startLocation));
			}
		}
	}

	// Remove the materials that should not be shown if they are present
	nativeMaterials.erase(EBlockType::Air);
	nativeMaterials.erase(EBlockType::Flower1);
	nativeMaterials.erase(EBlockType::Flower2);
	nativeMaterials.erase(EBlockType::Flower3);
	nativeMaterials.erase(EBlockType::Flower4);
	nativeMaterials.erase(EBlockType::FlowerRainbow);
	nativeMaterials.erase(EBlockType::GrassFoliage);

	std::time_t time = std::time(nullptr);
	std::tm timestamp;
	errno_t err = localtime_s(&timestamp, &time);
	std::wstringstream wss;
	wss << std::put_time(&timestamp, L"%d-%m-%y %H-%M-%S");
	std::wstring nameOfFile = GetWorldName() + L" " + wss.str() + L".cyubeVRBuild";

	fs::path filePath = savedBuildsPath / nameOfFile;


	std::wstringstream wssOther;
	wssOther << std::put_time(&timestamp, L"%d %B %Y at %H:%M:%S");
	BuildInfo buildInfo = BuildInfo(filePath, nameOfFile, wssOther.str(), (int)(xMax - xMin), (int)(yMax - yMin), zMax - zMin, nativeMaterials, customMaterials, selection);

	writeBuildToFile(buildInfo);

	refreshBuilds();
}

void openBuildsFolder() {
	ShellExecuteA(NULL, "open", savedBuildsPath.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

ERotation rotateBlockRotation(ERotation blockRotation, RotationDirection rotationDirection) {
	switch (blockRotation) {
	case ERotation::None:
		return ERotation::None;
	case ERotation::Left:
		switch (rotationDirection) {
		case Left:
			return ERotation::Backward;
		case Right:
			return ERotation::Forward;
		case Opposite:
			return ERotation::Right;
		}
	case ERotation::Right:
		switch (rotationDirection) {
		case Left:
			return ERotation::Forward;
		case Right:
			return ERotation::Backward;
		case Opposite:
			return ERotation::Left;
		}
	case ERotation::Forward:
		switch (rotationDirection) {
		case Left:
			return ERotation::Left;
		case Right:
			return ERotation::Right;
		case Opposite:
			return ERotation::Backward;
		}
	case ERotation::Backward:
		switch (rotationDirection) {
		case Left:
			return ERotation::Right;
		case Right:
			return ERotation::Left;
		case Opposite:
			return ERotation::Forward;
		}
	default:
		return blockRotation;
	}
}

BlockInfoWithLocation rotateBlockAroundOrigin(BlockInfoWithLocation block, Rotation rotation) {
	BlockInfoWithLocation rotatedBlock;
	rotatedBlock.Location = block.Location;
	rotatedBlock.Info = block.Info;
	switch (rotation) {
	case PositiveY:
		break;
	case NegativeX:
		rotatedBlock.Location = CoordinateInCentimeters(-block.Location.Y, block.Location.X, block.Location.Z);
		rotatedBlock.Info.Rotation = rotateBlockRotation(block.Info.Rotation, Right);
		break;
	case NegativeY:
		rotatedBlock.Location = CoordinateInCentimeters(-block.Location.X, -block.Location.Y, block.Location.Z);
		rotatedBlock.Info.Rotation = rotateBlockRotation(block.Info.Rotation, Opposite);
		break;
	case PositiveX:
		rotatedBlock.Location = CoordinateInCentimeters(block.Location.Y, -block.Location.X, block.Location.Z);
		rotatedBlock.Info.Rotation = rotateBlockRotation(block.Info.Rotation, Left);
		break;
	}
	return rotatedBlock;
}

void setBuild(CoordinateInBlocks location, int buildID, Rotation buildRotation) {
	std::vector<BlockInfoWithLocation> changedBlocks;

	BuildInfo buildInfo = builds.find(buildID)->second;

	BlockInfoWithLocation rotatedBlock;
	for (BlockInfoWithLocation block : buildInfo.blocks) {
		rotatedBlock = rotateBlockAroundOrigin(block, buildRotation);
		setBlockAtLocationAndUpdateChangedBlocks(location + rotatedBlock.Location, rotatedBlock.Info, changedBlocks);
	}

	newOperation(changedBlocks);
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
								Save, Refresh, Open, Remove, RegisterLoadID, SetLoad,
								LoadRotationPositiveY, LoadRotationNegativeX, LoadRotationNegativeY, LoadRotationPositiveX }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

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
	case Refresh:
		refreshBuilds();
		break;
	case Open:
		openBuildsFolder();
		break;
	case Remove:
		timesToIgnoreBlockPlacement = 2;
		registerRemove = true;
		break;
	case RegisterLoadID:
		timesToIgnoreBlockPlacement = 2;
		registerLoad = true;
		break;
	case SetLoad:
		setBuild(At, loadID, currentLoadRotation);
		break;
	case LoadRotationPositiveY:
		currentLoadRotation = PositiveY;
		break;
	case LoadRotationNegativeX:
		currentLoadRotation = NegativeX;
		break;
	case LoadRotationNegativeY:
		currentLoadRotation = NegativeY;
		break;
	case LoadRotationPositiveX:
		currentLoadRotation = PositiveX;
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
	refreshBuilds();
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
	else if (registerRemove) {
		registerRemove = false;
		removeBuild(Type.CustomBlockID);
	}
	else if (registerLoad) {
		registerLoad = false;
		loadID = Type.CustomBlockID;
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