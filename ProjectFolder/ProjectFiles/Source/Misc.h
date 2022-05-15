#pragma once
#include "GameAPI.h"

struct tempBlock {
	// The position of the block.
	CoordinateInBlocks position = CoordinateInBlocks(0, 0, 0);
	// Info about the block.
	BlockInfo info = BlockInfo();
	// Info about the block that used to be where the block now is.
	BlockInfo prevInfo = BlockInfo();

	/**
	* Sets the block at its position and updates prevInfo.
	* 
	* @param relativeTo Where to set the block relative to (typically some "main" block).
	*/
	void set(CoordinateInBlocks relativeTo);

	/**
	* Removes the block and replaces it with the block that used to be there.
	*
	* @param relativeTo Where to remove the block relative to (typically some "main" block).
	*/
	void remove(CoordinateInBlocks relativeTo);

};