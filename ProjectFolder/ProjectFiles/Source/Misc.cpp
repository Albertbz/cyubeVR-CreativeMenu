#include "Misc.h"

void tempBlock::set(CoordinateInBlocks relativeTo)
{
	prevInfo = GetBlock(relativeTo + position);
	SetBlock(relativeTo + position, info);
}

void tempBlock::remove(CoordinateInBlocks relativeTo)
{
	SetBlock(relativeTo + position, prevInfo);
}
