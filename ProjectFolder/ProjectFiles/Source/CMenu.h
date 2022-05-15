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
	// The current page that the player is on. 0=main, 1=blocks, 2=items, 3=tools.
	int currentPage;
	// An array with all of the menu blocks. 0-5 from top left to bottom right.
	std::array<tempBlock, 6> menuBlocks;
	// A bool to keep track of whether the player can click the interface with the left hand.
	bool canClickLeft;
	// A bool to keep track of whether the player can click the interface with the right hand.
	bool canClickRight;

	enum {
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
		bMenu6BlockID = 149771126
	};

	/**
	* Constructor for when placing a CMenu block.
	* 
	* @param position The position of the block.
	*/
	CMenu(CoordinateInBlocks position);

	// Removes the block.
	void remove();

	// Sets the blocks for the current page.
	void setPage();

	// Removes the blocks for the current page.
	void removePage();

	/**
	* Changes the page number and runs the proper methods
	* to update it.
	* 
	* @param pageNumber The page to change to.
	*/
	void changePage(int pageNumber);

	// Updates the page so the proper blocks are shown.
	void updatePage();

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
	void clickCheck(CoordinateInCentimeters fingerLocation, bool &canClick, CoordinateInCentimeters positionCm);

	/**
	* Acts on a click, i.e., checks what is being clicked and does what
	* is supposed to happen in accordance with chosen functionality.
	* 
	* @param fingerLocation The location of the given finger.
	*/
	void clickRegister(CoordinateInCentimeters fingerLocation);

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