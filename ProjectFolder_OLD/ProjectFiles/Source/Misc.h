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


template <typename T>
void writeBlocks(std::ostream& file, const std::vector<T>& blocks) {
	for (auto b : blocks) {
		file << b.position.X << ';' << b.position.Y << ';' << b.position.Z << ';' << b.direction << ';' << b.currentMenu << ';';
		for (const auto& i : b.interfaceBlocks) {
			file << i.position.X << ';' << i.position.Y << ';' << i.position.Z << ';' << i.info.CustomBlockID << ';';;
			if (i.prevInfo.Type == EBlockType::ModBlock) {
				file << 'C' << ';' << (int)i.prevInfo.CustomBlockID << ';';
			}
			else {
				file << 'N' << ';' << (int)i.prevInfo.Type << ';';
			}
		}
		file << b.canClickLeft << ';' << b.canClickRight << ';' << b.currentPage << ';';
		file << '\n';
	}
}

template <typename T>
void writeBlocks(std::ostream&& file, const std::vector<T>& blocks) {
	writeBlocks<T>(file, blocks);
}

template <typename T>
auto readBlocks(std::istream& file)->std::vector<T> {
	std::vector<T> blocks;
	auto line = std::string{};

	while (std::getline(file, line)) {

		CoordinateInBlocks position;
		size_t pos = line.find_first_of(';');
		position.X = std::stoi(std::string{ line, 0, pos });
		position.Y = std::stoi(std::string{ line, pos + 1 });
		pos = line.find_first_of(';', pos + 1);
		position.Z = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		int direction = std::stoi(std::string{ line, pos + 1 });
		
		pos = line.find_first_of(';', pos + 1);
		int currentMenu = std::stoi(std::string{ line, pos + 1 });
		

		std::array<tempBlock, 6> interfaceBlocks;
		for (auto& i : interfaceBlocks) {
			pos = line.find_first_of(';', pos + 1);
			i.position.X = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Y = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.position.Z = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			i.info = std::stoi(std::string{ line, pos + 1 });
			pos = line.find_first_of(';', pos + 1);
			if (std::string{ line, pos + 1 } == "C") {
				pos = line.find_first_of(';', pos + 1);
				i.prevInfo = std::stoi(std::string{ line, pos + 1 });
			}
			else {
				pos = line.find_first_of(';', pos + 1);
				i.prevInfo = (EBlockType)std::stoi(std::string{ line, pos + 1 });
			}
		}

		pos = line.find_first_of(';', pos + 1);
		bool canClickLeft = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		bool canClickRight = std::stoi(std::string{ line, pos + 1 });

		pos = line.find_first_of(';', pos + 1);
		int currentPage = std::stoi(std::string{ line, pos + 1 });

		blocks.push_back(T(position, direction, currentMenu, interfaceBlocks, canClickLeft, canClickRight, currentPage));
	}

	return blocks;
}

template <typename T>
auto readBlocks(std::istream&& file) -> std::vector<T> {
	return readBlocks<T>(file);
}