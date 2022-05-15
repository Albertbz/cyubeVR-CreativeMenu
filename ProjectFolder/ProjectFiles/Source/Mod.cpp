#include "GameAPI.h"
#include "CMenu.h"

/************************************************************
	Custom Variables for the mod
*************************************************************/

const int CMenuBlockID = 298343567;

std::vector<CMenu> cMenuBlocks;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { CMenuBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 5;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

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
		it->checkAction();
	}
}



// Run once when the world is loaded
void Event_OnLoad()
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