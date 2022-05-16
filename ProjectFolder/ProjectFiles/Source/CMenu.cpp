#include "CMenu.h"

CMenu::CMenu(CoordinateInBlocks position)
{
	this->position = position;
	this->interfaceBlocks = {};
	this->canClickLeft = true;
	this->canClickRight = true;

	this->interfaceBlocks[0].info = mMenu1BlockID;
	this->interfaceBlocks[1].info = mMenu2BlockID;
	this->interfaceBlocks[2].info = mMenu3BlockID;
	this->interfaceBlocks[3].info = mMenu4BlockID;
	this->interfaceBlocks[4].info = mMenu5BlockID;
	this->interfaceBlocks[5].info = mMenu6BlockID;
	
	CoordinateInCentimeters playerLocation = GetPlayerLocation();
	CoordinateInCentimeters blockPositionCm = CoordinateInCentimeters(position);
	int64_t xDifference = std::abs(playerLocation.X - blockPositionCm.X);
	int64_t yDifference = std::abs(playerLocation.Y - blockPositionCm.Y);

	if (playerLocation.X < blockPositionCm.X - 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->direction = 1;

		this->interfaceBlocks[0].position = CoordinateInBlocks(0, -1, 3);
		this->interfaceBlocks[1].position = CoordinateInBlocks(0, 0, 3);
		this->interfaceBlocks[2].position = CoordinateInBlocks(0, 1, 3);
		this->interfaceBlocks[3].position = CoordinateInBlocks(0, -1, 2);
		this->interfaceBlocks[4].position = CoordinateInBlocks(0, 0, 2);
		this->interfaceBlocks[5].position = CoordinateInBlocks(0, 1, 2);
	}
	else if (playerLocation.X > blockPositionCm.X + 25 && playerLocation.Y < blockPositionCm.Y + xDifference && playerLocation.Y > blockPositionCm.Y - xDifference) {
		this->direction = 2;

		this->interfaceBlocks[0].position = CoordinateInBlocks(0, 1, 3);
		this->interfaceBlocks[1].position = CoordinateInBlocks(0, 0, 3);
		this->interfaceBlocks[2].position = CoordinateInBlocks(0, -1, 3);
		this->interfaceBlocks[3].position = CoordinateInBlocks(0, 1, 2);
		this->interfaceBlocks[4].position = CoordinateInBlocks(0, 0, 2);
		this->interfaceBlocks[5].position = CoordinateInBlocks(0, -1, 2);
	}
	else if (playerLocation.Y < blockPositionCm.Y - 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->direction = 3;

		this->interfaceBlocks[0].position = CoordinateInBlocks(1, 0, 3);
		this->interfaceBlocks[1].position = CoordinateInBlocks(0, 0, 3);
		this->interfaceBlocks[2].position = CoordinateInBlocks(-1, 0, 3);
		this->interfaceBlocks[3].position = CoordinateInBlocks(1, 0, 2);
		this->interfaceBlocks[4].position = CoordinateInBlocks(0, 0, 2);
		this->interfaceBlocks[5].position = CoordinateInBlocks(-1, 0, 2);
	}
	else if (playerLocation.Y > blockPositionCm.Y + 25 && playerLocation.X < blockPositionCm.X + yDifference && playerLocation.X > blockPositionCm.X - yDifference) {
		this->direction = 4;

		this->interfaceBlocks[0].position = CoordinateInBlocks(-1, 0, 3);
		this->interfaceBlocks[1].position = CoordinateInBlocks(0, 0, 3);
		this->interfaceBlocks[2].position = CoordinateInBlocks(1, 0, 3);
		this->interfaceBlocks[3].position = CoordinateInBlocks(-1, 0, 2);
		this->interfaceBlocks[4].position = CoordinateInBlocks(0, 0, 2);
		this->interfaceBlocks[5].position = CoordinateInBlocks(1, 0, 2);
	}
	
	this->currentPage = 0;

	setPage();
}

void CMenu::remove()
{
	removePage();
}

void CMenu::setPage()
{
	for (tempBlock &b : interfaceBlocks)
	{
		b.set(position);
	}
}

void CMenu::removePage()
{
	for (tempBlock &b : interfaceBlocks)
	{
		b.remove(position);
	}
}

void CMenu::changePage(int pageNumber)
{
	currentPage = pageNumber;
	updatePage();
}

void CMenu::updatePage()
{
	switch (currentPage) {
	case 0:
		interfaceBlocks[0].info = mMenu1BlockID;
		interfaceBlocks[1].info = mMenu2BlockID;
		interfaceBlocks[2].info = mMenu3BlockID;
		interfaceBlocks[3].info = mMenu4BlockID;
		interfaceBlocks[4].info = mMenu5BlockID;
		interfaceBlocks[5].info = mMenu6BlockID;
		break;
	case 2:
		interfaceBlocks[0].info = bMenu1BlockID;
		interfaceBlocks[1].info = bMenu2BlockID;
		interfaceBlocks[2].info = bMenu3BlockID;
		interfaceBlocks[3].info = bMenu4BlockID;
		interfaceBlocks[4].info = bMenu5BlockID;
		interfaceBlocks[5].info = bMenu6BlockID;
		break;
	}
	
	removePage();
	setPage();
}

void CMenu::runCheck()
{
	CoordinateInCentimeters fingerTipLocationLeft = GetIndexFingerTipLocation(true);
	CoordinateInCentimeters fingerTipLocationRight = GetIndexFingerTipLocation(false);
	CoordinateInCentimeters positionCm = CoordinateInCentimeters(position);

	clickCheck(fingerTipLocationLeft, canClickLeft, positionCm);
	clickCheck(fingerTipLocationRight, canClickRight, positionCm);
	
}

void CMenu::clickCheck(CoordinateInCentimeters fingerLocation, bool &canClick, CoordinateInCentimeters positionCm)
{
	if (canClick) {
		switch (direction) {
		case 1:
			if (fingerLocation.X > positionCm.X - 25) {
				clickRegister(fingerLocation);
				canClick = false;
			}
			break;
		case 2:
			if (fingerLocation.X < positionCm.X + 25) {
				clickRegister(fingerLocation);
				canClick = false;
			}
			break;
		case 3:
			if (fingerLocation.Y > positionCm.Y - 25) {
				clickRegister(fingerLocation);
				canClick = false;
			}
			break;
		case 4:
			if (fingerLocation.Y < positionCm.Y + 25) {
				clickRegister(fingerLocation);
				canClick = false;
			}
			break;
		}
	}
	else {
		switch (direction) {
		case 1:
			if (fingerLocation.X < positionCm.X - 25) {
				canClick = true;
			}
			break;
		case 2:
			if (fingerLocation.X > positionCm.X + 25) {
				canClick = true;
			}
			break;
		case 3:
			if (fingerLocation.Y < positionCm.Y - 25) {
				canClick = true;
			}
			break;
		case 4:
			if (fingerLocation.Y > positionCm.Y + 25) {
				canClick = true;
			}
			break;
		}
	}

	
}

void CMenu::clickRegister(CoordinateInCentimeters fingerLocation)
{
	switch (currentPage) {
	case 0: // Main menu
		if (isBetween(std::pair(512, 1280), std::pair(2047, 2815), fingerLocation)) {
			changePage(1);
		}
		else if (isBetween(std::pair(2304, 1081), std::pair(3840, 1984), fingerLocation)) {
			changePage(2);
		}
		break;
	case 1: // Items menu
		if (isBetween(std::pair(97, 72), std::pair(635, 610), fingerLocation)) {
			changePage(0);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Coal, 50);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Copper, 50);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(EBlockType::IngotIron, 50);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Gold, 50);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(EBlockType::DyeGreen, 50);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(EBlockType::DyeBlue, 50);
		}
		else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeRed, 50);
		}
		else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeWhite, 50);
		}
		else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeRainbow, 50);
		}
		else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
			AddToInventory(EBlockType::GlassIngot, 50);
		}
		break;
	case 2: // Block menu
		if (isBetween(std::pair(97, 72), std::pair(635, 610), fingerLocation)) {
			changePage(0);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(EBlockType::StoneMined, 50);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Grass, 50);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Dirt, 50);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Sand, 50);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(EBlockType::TreeWood, 50);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(EBlockType::TreeWoodBright, 50);
		}
		else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
			AddToInventory(EBlockType::WoodPlank, 50);
		}
		else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
			AddToInventory(EBlockType::WoodPlankBright, 50);
		}
		else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Ore_Coal, 50);
		}
		else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Ore_Copper, 50);
		}
		else if (isBetween(std::pair(3968, 1912), std::pair(4479, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Ore_Iron, 50);
		}
		else if (isBetween(std::pair(4736, 1912), std::pair(5247, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Ore_Gold, 50);
		}
		else if (isBetween(std::pair(896, 2680), std::pair(1407, 3191), fingerLocation)) {
			AddToInventory(EBlockType::CrystalBlock, 50);
		}
		else if (isBetween(std::pair(1664, 2680), std::pair(2175, 3191), fingerLocation)) {
			AddToInventory(EBlockType::Wallstone, 50);
		}
		else if (isBetween(std::pair(2432, 2680), std::pair(2943, 3191), fingerLocation)) {
			AddToInventory(EBlockType::Flagstone, 50);
		}
		else if (isBetween(std::pair(3200, 2680), std::pair(3711, 3191), fingerLocation)) {
			AddToInventory(EBlockType::WoodScaffolding, 50);
		}
		else if (isBetween(std::pair(3968, 2680), std::pair(4479, 3191), fingerLocation)) {
			AddToInventory(EBlockType::GlassBlock, 50);
		}
		break;
	}
}

CoordinateInCentimeters CMenu::getCorner()
{
	CoordinateInCentimeters corner;
	switch (direction) {
	case 1:
		corner = position + interfaceBlocks[0].position + CoordinateInCentimeters(0, -25, 25);
		break;
	case 2:
		corner = position + interfaceBlocks[0].position + CoordinateInCentimeters(0, 25, 25);
		break;
	case 3:
		corner = position + interfaceBlocks[0].position + CoordinateInCentimeters(25, 0, 25);
		break;
	case 4:
		corner = position + interfaceBlocks[0].position + CoordinateInCentimeters(-25, 0, 25);
		break;
	}
	return corner;
}

bool CMenu::isBetween(std::pair<int, int> topLeft, std::pair<int, int> bottomRight, CoordinateInCentimeters fingerPos)
{
	CoordinateInCentimeters corner = getCorner();
	int topLeftFirst = topLeft.first / 6144.0 * 150.0;
	int topLeftSecond = topLeft.second / 4096.0 * 100.0;
	int bottomRightFirst = bottomRight.first / 6144.0 * 150.0;
	int bottomRightSecond = bottomRight.second / 4096.0 * 100.0;
	Log(L"In isBetween"); 
	Log(L"Corner: " + std::to_wstring(corner.X) + L" " + std::to_wstring(corner.Y) + L" " + std::to_wstring(corner.Z));
	Log(L"Finger pos: " + std::to_wstring(fingerPos.X) + L" " + std::to_wstring(fingerPos.Y) + L" " + std::to_wstring(fingerPos.Z));
	Log(L"Selection: " + std::to_wstring(topLeftFirst) + L" " + std::to_wstring(topLeftSecond) + L" " + std::to_wstring(bottomRightFirst) + L" " + std::to_wstring(bottomRightSecond));
	
	bool result = false;

	switch (direction) {
	case 1:
		result = fingerPos.Y > corner.Y + topLeftFirst && fingerPos.Z < corner.Z - topLeftSecond && fingerPos.Y < corner.Y + bottomRightFirst && fingerPos.Z > corner.Z - bottomRightSecond;
		break;
	case 2:
		result = fingerPos.Y < corner.Y - topLeftFirst && fingerPos.Z < corner.Z - topLeftSecond && fingerPos.Y > corner.Y - bottomRightFirst && fingerPos.Z > corner.Z - bottomRightSecond;
		break;
	case 3:
		result = fingerPos.X < corner.X - topLeftFirst && fingerPos.Z < corner.Z - topLeftSecond && fingerPos.X > corner.X - bottomRightFirst && fingerPos.Z > corner.Z - bottomRightSecond;
		break;
	case 4:
		result = fingerPos.X > corner.X + topLeftFirst && fingerPos.Z < corner.Z - topLeftSecond && fingerPos.X < corner.X + bottomRightFirst && fingerPos.Z > corner.Z - bottomRightSecond;
		break;
	}
	Log(L"Result: " + std::to_wstring(result));
	return result;
}
