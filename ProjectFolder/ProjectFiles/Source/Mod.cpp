#include "GameAPI.h"
#include "CMenu.h"
#include <fstream>
#include <iostream>

/************************************************************
	Custom Variables for the mod
*************************************************************/


const int CMenuBlockID = 298343567;

// Tick tracker to take care of saving blocks.
int tickNum = 0;
// Stores all CMenu blocks.
std::vector<CMenu> cMenuBlocks;
// The path to the saves folder.
std::wstring path;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { CMenuBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 10;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	switch (CustomBlockID) {
	case CMenuBlockID:
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
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	for (auto it = cMenuBlocks.begin(); it != cMenuBlocks.end(); it++) {
		if (it->position == At) {
			it->remove();
			cMenuBlocks.erase(it);
			break;
		}
	}
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName)
{

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
		writeBlocks<CMenu>(std::ofstream{ path + L"\\" + L"CMenuBlocks.txt" }, cMenuBlocks);

		tickNum = 0;
		break;
	default:
		tickNum++;
		break;
	}
}



// Run once when the world is loaded
void Event_OnLoad()
{
	path = GetThisModSaveFolderPath(L"CreativeMenu");
	CreateDirectoryW(path.c_str(), NULL);
	
	Log(L"Path: " + path);

	// Loads all previously placed CMenu blocks.
	cMenuBlocks = readBlocks<CMenu>(std::ifstream{ path + L"\\" + L"CMenuBlocks.txt"});
}

// Run once when the world is exited
void Event_OnExit()
{
	
	// Saves all placed CMenu blocks.
	writeBlocks<CMenu>(std::ofstream{ path + L"\\" + L"CMenuBlocks.txt" }, cMenuBlocks);
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
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName)
{

}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/