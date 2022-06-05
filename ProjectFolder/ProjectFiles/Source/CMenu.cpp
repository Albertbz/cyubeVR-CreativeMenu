#include "CMenu.h"

CMenu::CMenu(CoordinateInBlocks position)
{
	this->position = position;
	this->interfaceBlocks = {};
	this->canClickLeft = true;
	this->canClickRight = true;
	this->currentPage = 1;
	this->currentMenu = 0;

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

	setMenu();
}

CMenu::CMenu(CoordinateInBlocks position, int direction, int currentMenu, std::array<tempBlock, 6> interfaceBlocks, bool canClickLeft, bool canClickRight, int currentPage)
{
	this->position = position;
	this->direction = direction;
	this->currentMenu = currentMenu;
	this->interfaceBlocks = interfaceBlocks;
	this->canClickLeft = canClickLeft;
	this->canClickRight = canClickRight;
	this->currentPage = currentPage;
}

void CMenu::remove()
{
	removeMenu();
}

void CMenu::setMenu()
{
	for (tempBlock &b : interfaceBlocks)
	{
		b.set(position);
	}
}

void CMenu::removeMenu()
{
	for (tempBlock &b : interfaceBlocks)
	{
		b.remove(position);
	}
}

void CMenu::changeMenu(int menuNumber)
{
	currentMenu = menuNumber;
	currentPage = 1;
	updateMenu();
}

void CMenu::updateMenu()
{
	switch (currentMenu) {
	case 0:
		interfaceBlocks[0].info = mMenu1BlockID;
		interfaceBlocks[1].info = mMenu2BlockID;
		interfaceBlocks[2].info = mMenu3BlockID;
		interfaceBlocks[3].info = mMenu4BlockID;
		interfaceBlocks[4].info = mMenu5BlockID;
		interfaceBlocks[5].info = mMenu6BlockID;
		break;
	case 1:
		interfaceBlocks[0].info = iMenu1BlockID;
		interfaceBlocks[1].info = iMenu2BlockID;
		interfaceBlocks[2].info = iMenu3BlockID;
		interfaceBlocks[3].info = iMenu4BlockID;
		interfaceBlocks[4].info = iMenu5BlockID;
		interfaceBlocks[5].info = iMenu6BlockID;
		break;
	case 2:
		interfaceBlocks[0].info = bMenu1BlockID;
		interfaceBlocks[1].info = bMenu2BlockID;
		interfaceBlocks[2].info = bMenu3BlockID;
		interfaceBlocks[3].info = bMenu4BlockID;
		interfaceBlocks[4].info = bMenu5BlockID;
		interfaceBlocks[5].info = bMenu6BlockID;
		break;
	case 3:
		switch (currentPage) {
		case 1:
			interfaceBlocks[0].info = oMenu11BlockID;
			interfaceBlocks[1].info = oMenu12BlockID;
			interfaceBlocks[2].info = oMenu13BlockID;
			interfaceBlocks[3].info = oMenu14BlockID;
			interfaceBlocks[4].info = oMenu15BlockID;
			interfaceBlocks[5].info = oMenu16BlockID;
			break;
		case 2:
			interfaceBlocks[0].info = oMenu21BlockID;
			interfaceBlocks[1].info = oMenu22BlockID;
			interfaceBlocks[2].info = oMenu23BlockID;
			interfaceBlocks[3].info = oMenu24BlockID;
			interfaceBlocks[4].info = oMenu25BlockID;
			interfaceBlocks[5].info = oMenu26BlockID;
			break;
		}
		break;
	case 4:
		interfaceBlocks[0].info = fMenu1BlockID;
		interfaceBlocks[1].info = fMenu2BlockID;
		interfaceBlocks[2].info = fMenu3BlockID;
		interfaceBlocks[3].info = fMenu4BlockID;
		interfaceBlocks[4].info = fMenu5BlockID;
		interfaceBlocks[5].info = fMenu6BlockID;
		break;
	case 5:
		interfaceBlocks[0].info = tMenu1BlockID;
		interfaceBlocks[1].info = tMenu2BlockID;
		interfaceBlocks[2].info = tMenu3BlockID;
		interfaceBlocks[3].info = tMenu4BlockID;
		interfaceBlocks[4].info = tMenu5BlockID;
		interfaceBlocks[5].info = tMenu6BlockID;
		break;
	}
	
	removeMenu();
	setMenu();
}

void CMenu::runCheck()
{
	CoordinateInCentimeters fingerTipLocationLeft = GetIndexFingerTipLocation(true);
	CoordinateInCentimeters fingerTipLocationRight = GetIndexFingerTipLocation(false);
	CoordinateInCentimeters positionCm = CoordinateInCentimeters(position);

	clickCheck(fingerTipLocationLeft, canClickLeft, positionCm, true);
	clickCheck(fingerTipLocationRight, canClickRight, positionCm, false);
	
}

void CMenu::clickCheck(CoordinateInCentimeters fingerLocation, bool &canClick, CoordinateInCentimeters positionCm, bool leftHand)
{
	if (canClick) {
		switch (direction) {
		case 1:
			if (fingerLocation.X > positionCm.X - 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 2:
			if (fingerLocation.X < positionCm.X + 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 3:
			if (fingerLocation.Y > positionCm.Y - 25) {
				clickRegister(fingerLocation, leftHand);
				canClick = false;
			}
			break;
		case 4:
			if (fingerLocation.Y < positionCm.Y + 25) {
				clickRegister(fingerLocation, leftHand);
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

void CMenu::clickRegister(CoordinateInCentimeters fingerLocation, bool leftHand)
{
	switch (currentMenu) {
	case 0: // Main menu
		if (isBetween(std::pair(512, 1280), std::pair(2047, 2815), fingerLocation)) {
			changeMenu(1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2304, 1081), std::pair(3840, 1984), fingerLocation)) {
			changeMenu(2);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2184, 2105), std::pair(2951, 3013), fingerLocation)) {
			changeMenu(3);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3192, 2105), std::pair(3959, 3013), fingerLocation)) {
			changeMenu(4);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4096, 1280), std::pair(5631, 2815), fingerLocation)) {
			changeMenu(5);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 1: // Items menu
		if (isBetween(std::pair(70, 70), std::pair(608, 608), fingerLocation)) {
			changeMenu(0);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Coal, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Copper, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(EBlockType::IngotIron, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Nugget_Gold, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(EBlockType::DyeGreen, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(EBlockType::DyeBlue, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeRed, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeWhite, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DyeRainbow, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
			AddToInventory(EBlockType::GlassIngot, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1912), std::pair(4479, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Crystal, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1912), std::pair(5247, 2423), fingerLocation)) {
			AddToInventory(EBlockType::DryGrass, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 2: // Blocks menu
		if (isBetween(std::pair(70, 70), std::pair(608, 608), fingerLocation)) {
			changeMenu(0);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2430, 3642), std::pair(3712, 4037), fingerLocation)) {
			BlockInfo blockBelow = GetBlock(position + CoordinateInBlocks(0, 0, 1));
			if (blockBelow.Type != EBlockType::Air) {
				AddToInventory(blockBelow, 50);
			}
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(EBlockType::StoneMined, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Grass, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Dirt, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(EBlockType::Sand, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(EBlockType::TreeWood, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(EBlockType::TreeWoodBright, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
			AddToInventory(EBlockType::WoodPlank, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
			AddToInventory(EBlockType::WoodPlankBright, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
			AddToInventory(pCoalBlockID, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
			AddToInventory(pCopperBlockID, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1912), std::pair(4479, 2423), fingerLocation)) {
			AddToInventory(pIronBlockID, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1912), std::pair(5247, 2423), fingerLocation)) {
			AddToInventory(pGoldBlockID, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 2680), std::pair(1407, 3191), fingerLocation)) {
			AddToInventory(pCrystalBlockID, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 2680), std::pair(2175, 3191), fingerLocation)) {
			AddToInventory(EBlockType::Wallstone, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 2680), std::pair(2943, 3191), fingerLocation)) {
			AddToInventory(EBlockType::Flagstone, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 2680), std::pair(3711, 3191), fingerLocation)) {
			AddToInventory(EBlockType::WoodScaffolding, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 2680), std::pair(4479, 3191), fingerLocation)) {
			AddToInventory(EBlockType::GlassBlock, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 3: // Objects menu
		if (isBetween(std::pair(70, 70), std::pair(608, 608), fingerLocation)) {
			changeMenu(0);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else {
			switch (currentPage) {
			case 1: // Page 1
				if (isBetween(std::pair(5663, 2718), std::pair(6098, 3153), fingerLocation)) {
					currentPage++;
					updateMenu();
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
					AddToInventory(EBlockType::Chest1, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
					AddToInventory(EBlockType::Chair1, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
					AddToInventory(EBlockType::Chair1Birch, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
					AddToInventory(EBlockType::WoodStool, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
					AddToInventory(EBlockType::WoodTable1, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
					AddToInventory(EBlockType::WoodTable2, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
					AddToInventory(EBlockType::WoodCarafe, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
					AddToInventory(EBlockType::WoodBench1, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
					AddToInventory(EBlockType::WoodBench2, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
					AddToInventory(EBlockType::WoodBench3, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3968, 1912), std::pair(4479, 2423), fingerLocation)) {
					AddToInventory(EBlockType::WoodBarrel, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(4736, 1912), std::pair(5247, 2423), fingerLocation)) {
					AddToInventory(EBlockType::MetalPod, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(896, 2680), std::pair(1407, 3191), fingerLocation)) {
					AddToInventory(EBlockType::WallmountCopper, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(1664, 2680), std::pair(2175, 3191), fingerLocation)) {
					AddToInventory(EBlockType::FrameWood, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(2432, 2680), std::pair(2943, 3191), fingerLocation)) {
					AddToInventory(EBlockType::FrameCopper, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3200, 2680), std::pair(3711, 3191), fingerLocation)) {
					AddToInventory(EBlockType::FrameGold, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3968, 2680), std::pair(4479, 3191), fingerLocation)) {
					AddToInventory(EBlockType::Furnace, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(4736, 2680), std::pair(5247, 3191), fingerLocation)) {
					AddToInventory(EBlockType::FurnaceMoldIron, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				break;
			case 2: // Page 2
				if (isBetween(std::pair(5664, 1182), std::pair(6099, 1617), fingerLocation)) {
					currentPage--;
					updateMenu();
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
					AddToInventory(EBlockType::SandbagPile, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
					AddToInventory(EBlockType::Torch, 50);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
					AddToInventory(EBlockType::TorchBlue, 50);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
					AddToInventory(EBlockType::TorchGreen, 50);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
					AddToInventory(EBlockType::TorchRed, 50);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
					AddToInventory(EBlockType::TorchRainbow, 12);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
					AddToInventory(EBlockType::RespawnTorch, 1);
					PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
				}
				break;
			}
		}
		break;
	case 4: // Foliage menu
		if (isBetween(std::pair(70, 70), std::pair(608, 608), fingerLocation)) {
			changeMenu(0);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(3001, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(3000, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(3002, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(3004, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(3005, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(3003, 50);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		break;
	case 5: // Tools menu
		if (isBetween(std::pair(70, 70), std::pair(608, 608), fingerLocation)) {
			changeMenu(0);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1144), std::pair(1407, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_PickAxe_Stone, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1144), std::pair(2175, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_Axe_Stone, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1144), std::pair(2943, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_Shovel_Stone, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1144), std::pair(3711, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_PickAxe_Copper, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1144), std::pair(4479, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_Axe_Copper, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1144), std::pair(5247, 1655), fingerLocation)) {
			AddToInventory(EBlockType::T_Shovel_Copper, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(896, 1912), std::pair(1407, 2423), fingerLocation)) {
			AddToInventory(EBlockType::T_Sledgehammer_Copper, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(1664, 1912), std::pair(2175, 2423), fingerLocation)) {
			AddToInventory(EBlockType::T_PickAxe_Iron, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(2432, 1912), std::pair(2943, 2423), fingerLocation)) {
			AddToInventory(EBlockType::T_Axe_Iron, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3200, 1912), std::pair(3711, 2423), fingerLocation)) {
			AddToInventory(EBlockType::T_Shovel_Iron, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(3968, 1912), std::pair(4479, 2423), fingerLocation)) {
			AddToInventory(EBlockType::T_Sledgehammer_Iron, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		}
		else if (isBetween(std::pair(4736, 1912), std::pair(5247, 2423), fingerLocation)) {
			AddToInventory(EBlockType::Compass, 1);
			PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
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
	return result;
}
