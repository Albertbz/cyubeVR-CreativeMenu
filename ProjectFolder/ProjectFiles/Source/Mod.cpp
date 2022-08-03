#include "GameAPI.h"
#include "CMenu.h"
#include <fstream>
#include <iostream>
#include <filesystem>

/************************************************************
	Custom Variables for the mod
*************************************************************/




// Tick tracker to take care of saving blocks.
int tickNum = 0;
// Stores all CMenu blocks.
std::vector<CMenu> cMenuBlocks;
// The path to the saves folder.
std::wstring path;

// Custom blocks

const int CMenuBlockID = 298343567;
const int PlaceableCoalBlockID = 1169799691;
const int PlaceableCopperBlockID = 1169799692;
const int PlaceableCrystalBlockID = 1169799693;
const int PlaceableGoldBlockID = 1169799694;
const int PlaceableIronBlockID = 1169799695;

const int mMenu1BlockID = 149771111;
const int mMenu2BlockID = 149771112;
const int mMenu3BlockID = 149771113;
const int mMenu4BlockID = 149771114;
const int mMenu5BlockID = 149771115;
const int mMenu6BlockID = 149771116;
const int bMenu1BlockID = 149771121;
const int bMenu2BlockID = 149771122;
const int bMenu3BlockID = 149771123;
const int bMenu4BlockID = 149771124;
const int bMenu5BlockID = 149771125;
const int bMenu6BlockID = 149771126;
const int iMenu1BlockID = 149771131;
const int iMenu2BlockID = 149771132;
const int iMenu3BlockID = 149771133;
const int iMenu4BlockID = 149771134;
const int iMenu5BlockID = 149771135;
const int iMenu6BlockID = 149771136;
const int tMenu1BlockID = 149771141;
const int tMenu2BlockID = 149771142;
const int tMenu3BlockID = 149771143;
const int tMenu4BlockID = 149771144;
const int tMenu5BlockID = 149771145;
const int tMenu6BlockID = 149771146;
const int fMenu1BlockID = 149771151;
const int fMenu2BlockID = 149771152;
const int fMenu3BlockID = 149771153;
const int fMenu4BlockID = 149771154;
const int fMenu5BlockID = 149771155;
const int fMenu6BlockID = 149771156;
const int oMenu11BlockID = 149771161;
const int oMenu12BlockID = 149771162;
const int oMenu13BlockID = 149771163;
const int oMenu14BlockID = 149771164;
const int oMenu15BlockID = 149771165;
const int oMenu16BlockID = 149771166;
const int oMenu21BlockID = 149771171;
const int oMenu22BlockID = 149771172;
const int oMenu23BlockID = 149771173;
const int oMenu24BlockID = 149771174;
const int oMenu25BlockID = 149771175;
const int oMenu26BlockID = 149771176;


/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { CMenuBlockID, PlaceableCoalBlockID, PlaceableCopperBlockID, PlaceableCrystalBlockID,
								PlaceableGoldBlockID, PlaceableIronBlockID, 
								mMenu1BlockID, mMenu2BlockID, mMenu3BlockID, mMenu4BlockID, mMenu5BlockID, mMenu6BlockID,
								bMenu1BlockID, bMenu2BlockID, bMenu3BlockID, bMenu4BlockID, bMenu5BlockID, bMenu6BlockID,
								iMenu1BlockID, iMenu2BlockID, iMenu3BlockID, iMenu4BlockID, iMenu5BlockID, iMenu6BlockID,
								tMenu1BlockID, tMenu2BlockID, tMenu3BlockID, tMenu4BlockID, tMenu5BlockID, tMenu6BlockID,
								fMenu1BlockID, fMenu2BlockID, fMenu3BlockID, fMenu4BlockID, fMenu5BlockID, fMenu6BlockID,
								oMenu11BlockID, oMenu12BlockID, oMenu13BlockID, oMenu14BlockID, oMenu15BlockID, oMenu16BlockID,
								oMenu21BlockID, oMenu22BlockID, oMenu23BlockID, oMenu24BlockID, oMenu25BlockID, oMenu26BlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 10;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	switch (CustomBlockID) {
	case CMenuBlockID:
		{
			bool newBlock = true;
			for (auto it = cMenuBlocks.begin(); it != cMenuBlocks.end(); it++) {
				if (it->position == At) {
					newBlock = false;
					break;
				}
			}
			if (newBlock) {
				cMenuBlocks.push_back(CMenu(At));
			}
		}
		break;
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
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	switch (CustomBlockID) {
	case CMenuBlockID:
		for (auto it = cMenuBlocks.begin(); it != cMenuBlocks.end(); it++) {
			if (it->position == At) {
				it->remove();
				cMenuBlocks.erase(it);
				break;
			}
		}
		break;
	}

}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{
	switch (CustomBlockID) {
	case mMenu2BlockID:
	case mMenu1BlockID:
	case mMenu3BlockID:
	case mMenu4BlockID:
	case mMenu5BlockID:
	case mMenu6BlockID:
	case bMenu1BlockID:
	case bMenu2BlockID:
	case bMenu3BlockID:
	case bMenu4BlockID:
	case bMenu5BlockID:
	case bMenu6BlockID:
	case iMenu1BlockID:
	case iMenu2BlockID:
	case iMenu3BlockID:
	case iMenu4BlockID:
	case iMenu5BlockID:
	case iMenu6BlockID:
	case tMenu1BlockID:
	case tMenu2BlockID:
	case tMenu3BlockID:
	case tMenu4BlockID:
	case tMenu5BlockID:
	case tMenu6BlockID:
	case fMenu1BlockID:
	case fMenu2BlockID:
	case fMenu3BlockID:
	case fMenu4BlockID:
	case fMenu5BlockID:
	case fMenu6BlockID:
	case oMenu11BlockID:
	case oMenu12BlockID:
	case oMenu13BlockID:
	case oMenu14BlockID:
	case oMenu15BlockID:
	case oMenu16BlockID:
	case oMenu21BlockID:
	case oMenu22BlockID:
	case oMenu23BlockID:
	case oMenu24BlockID:
	case oMenu25BlockID:
	case oMenu26BlockID:
		bool remove = true;
		for (auto it = cMenuBlocks.begin(); it != cMenuBlocks.end(); it++) {
			if (it->hasOriginBlock(At)) {
				remove = false;
				break;
			}
		}

		if (remove) {
			SetBlock(At, EBlockType::Air);
		}
		break;
	}

}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	for (auto it = cMenuBlocks.begin(); it != cMenuBlocks.end(); it++) {
		it->runCheck();
	}

	switch (tickNum) {
	case 100:
		// Saves all CMenu blocks to a file for later loading.
		writeBlocks<CMenu>(std::ofstream{ path + L"CMenuBlocks.txt" }, cMenuBlocks);

		tickNum = 0;
		break;
	default:
		tickNum++;
		break;
	}
}



// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	path = GetThisModSaveFolderPath(L"CreativeMenu");

	// Loads all previously placed CMenu blocks.
	cMenuBlocks = readBlocks<CMenu>(std::ifstream{ path + L"CMenuBlocks.txt"});
}

// Run once when the world is exited
void Event_OnExit()
{
	
	// Saves all placed CMenu blocks.
	writeBlocks<CMenu>(std::ofstream{ path + L"CMenuBlocks.txt" }, cMenuBlocks);
}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

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