#include <iostream>
#include <bits/stdc++.h>

int main(){
	std::cout << "This program for decompressing" << std::endl;
	std::string filename = "";
	std::cout << "Enter filename: ";
	std::cin >> filename;

	// Opening compressed file
	std::ifstream inFile(filename, std::ios::binary);

	if(!inFile.is_open()){
		std::cout << "File does not exist!\n";
		return 1;
	}

	// Reading the header of the file
	int uniqueChars = 0;
	inFile.read(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
	
	// Initializing the vector
	std::vector<std::pair<int,int>> asciitable;
	for(int i = 0;i < 257;i++){
		asciitable.push_back(std::pair(i,0));
	}
	


	for(int i = 0;i < uniqueChars;i++){
		int charIndex = 0;
		int freq = 0;
		
		inFile.read(reinterpret_cast<char*>(&charIndex),sizeof(charIndex));
		inFile.read(reinterpret_cast<char*>(&freq),sizeof(freq));

		asciitable[charIndex].second = freq;
	}
	
	// Now that we have built our frequency ascii table we have to build the Huffman tree

	
}
