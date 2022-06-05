#include "Misc.h"

void tempBlock::set(CoordinateInBlocks relativeTo)
{
	prevInfo = GetAndSetBlock(relativeTo + position, info);
}

void tempBlock::remove(CoordinateInBlocks relativeTo)
{
	SetBlock(relativeTo + position, prevInfo);
}
