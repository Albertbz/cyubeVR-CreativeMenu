#pragma once
#include "GameAPI.h"
#include "Misc.h"
#include <array>

class CMenu {
public:
	// The position of the block.
	CoordinateInBlocks position;
	// The direction the player was when placing the block. 1=-x, 2=+x, 3=-y, 4=+y.
	int direction;
	// The current menu that the player is on. 0=main, 1=items, 2=blocks, 3=objects, 4=foliage, 5=tools.
	int currentMenu;
	// An array with all of the interface blocks. 0-5 from top left to bottom right.
	std::array<tempBlock, 6> interfaceBlocks;
	// A bool to keep track of whether the player can click the interface with the left hand.
	bool canClickLeft;
	// A bool to keep track of whether the player can click the interface with the right hand.
	bool canClickRight;
	// The current page of the menu.
	int currentPage;

	enum {
		pCoalBlockID = 1169799691,
		pCopperBlockID = 1169799692,
		pCrystalBlockID = 1169799693,
		pGoldBlockID = 1169799694,
		pIronBlockID = 1169799695,
		mMenu1BlockID = 149771111,
		mMenu2BlockID = 149771112,
		mMenu3BlockID = 149771113,
		mMenu4BlockID = 149771114,
		mMenu5BlockID = 149771115,
		mMenu6BlockID = 149771116,
		bMenu1BlockID = 149771121,
		bMenu2BlockID = 149771122,
		bMenu3BlockID = 149771123,
		bMenu4BlockID = 149771124,
		bMenu5BlockID = 149771125,
		bMenu6BlockID = 149771126,
		iMenu1BlockID = 149771131,
		iMenu2BlockID = 149771132,
		iMenu3BlockID = 149771133,
		iMenu4BlockID = 149771134,
		iMenu5BlockID = 149771135,
		iMenu6BlockID = 149771136,
		tMenu1BlockID = 149771141,
		tMenu2BlockID = 149771142,
		tMenu3BlockID = 149771143,
		tMenu4BlockID = 149771144,
		tMenu5BlockID = 149771145,
		tMenu6BlockID = 149771146,
		fMenu1BlockID = 149771151,
		fMenu2BlockID = 149771152,
		fMenu3BlockID = 149771153,
		fMenu4BlockID = 149771154,
		fMenu5BlockID = 149771155,
		fMenu6BlockID = 149771156,
		oMenu11BlockID = 149771161,
		oMenu12BlockID = 149771162,
		oMenu13BlockID = 149771163,
		oMenu14BlockID = 149771164,
		oMenu15BlockID = 149771165,
		oMenu16BlockID = 149771166,
		oMenu21BlockID = 149771171,
		oMenu22BlockID = 149771172,
		oMenu23BlockID = 149771173,
		oMenu24BlockID = 149771174,
		oMenu25BlockID = 149771175,
		oMenu26BlockID = 149771176
	};

	/**
	* Constructor for when placing a CMenu block.
	* 
	* @param position The position of the block.
	*/
	CMenu(CoordinateInBlocks position);

	/**
	* Constructor for when loading a CMenu block.
	* 
	* @param position The position of the block.
	* @param direction The direction the player was looking.
	* @param currentMenu The current menu.
	* @param interfaceBlocks The blocks for the interface.
	* @param canClickLeft Whether it is possible to click with left hand.
	* @param canClickRight Whether it is possible to click with right hand.
	*/
	CMenu(CoordinateInBlocks position, int direction, int currentMenu, std::array<tempBlock, 6> interfaceBlocks,
		bool canClickLeft, bool canClickRight, int currentPage);

	// Removes the block.
	void remove();

	// Sets the blocks for the current menu.
	void setMenu();

	// Removes the blocks for the current menu.
	void removeMenu();

	/**
	* Changes the menu number and runs the proper methods
	* to update it.
	* 
	* @param menuNumber The menu to change to.
	*/
	void changeMenu(int menuNumber);

	// Updates the menu so the proper blocks are shown.
	void updateMenu();

	// Checks for any action by the player.
	void runCheck();

	// Checks if anything on the interface is being clicked.
	/**
	* Checks if anything on the interface is being clicked and acts on it
	* if need be.
	* 
	* @param fingerLocation The location of the given finger.
	* @param canClick The canClick field that corresponds to given finger.
	* @param positionCm The position of the main block in centimeters.
	*/
	void clickCheck(CoordinateInCentimeters fingerLocation, bool &canClick, CoordinateInCentimeters positionCm, bool leftHand);

	/**
	* Acts on a click, i.e., checks what is being clicked and does what
	* is supposed to happen in accordance with chosen functionality.
	* 
	* @param fingerLocation The location of the given finger.
	*/
	void clickRegister(CoordinateInCentimeters fingerLocation, bool leftHand);

	/**
	* Calculates the top left corner of the current interface.
	* 
	* @return The top left corner of the current interface.
	*/
	CoordinateInCentimeters getCorner();

	/**
	* Checks whether the given finger position is between the two
	* specified corners on the interface.
	* 
	* @param topLeft The coordinates for the top left corner in pixels.
	* @param bottomRight The coordinates for the bottom right corner in pixels.
	* @param fingerPos The position of the finger.
	* 
	* @return Whether the given finger is between the two given corners.
	*/
	bool isBetween(std::pair<int, int> topLeft, std::pair<int, int> bottomRight, CoordinateInCentimeters fingerPos);

};