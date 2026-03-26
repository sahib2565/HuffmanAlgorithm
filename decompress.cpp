#include <iostream>
#include <bits/stdc++.h>

struct Node {
    int freq;    // The frequency (used for sorting)
    int data;    // The character (use a special value like -1 for internal nodes)
    Node *left, *right;

    // Constructor for easy creation
    Node(int f, int d) : freq(f), data(d), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator() (Node* a,Node* b){
        return a->freq > b->freq;
    }
};


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
	//We populate the asciitable with the frequencies
	for(int i = 0;i < uniqueChars;i++){
		int charachter = 0;
		int freq = 0;
		inFile.read(reinterpret_cast<char*>(&charachter), sizeof(charachter));
		inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
		asciitable[charachter].second = freq;
	}
	// Building the tree
	std::priority_queue<Node*,std::vector<Node*>,CompareNodes> pq;
	for(auto i : asciitable){
		if(i.second > 0){
			Node* nodo = new Node(i.second,i.first);
			pq.push(nodo);
			
		}
	}
	// Building the tree
	while(pq.size() > 1){
		Node* temp1 = pq.top();
		pq.pop();
		Node* temp2 = pq.top();
		pq.pop();
		Node* nodo = new Node((temp1->freq + temp2->freq),-1);
		nodo->left = temp1;
		nodo->right = temp2;
		pq.push(nodo);
	}

	Node* HuffmanTree = pq.top();
	pq.pop();

	// Final step of decompressing the file
	std::ofstream OutFile("decompresssed_text.txt");

	unsigned char byte;
	bool eofFound = false;
	Node* temp = HuffmanTree;

	while(inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte)) && !eofFound){
		for(int i = 7;i >= 0;i--){
			// Extract the bit
			int bit = (byte >> i) & 1;
			if(!temp->right && !temp->left){
				int data = temp->data;
				if(data == 256){
					eofFound = true;
					break;
				}
				OutFile.put((char)(data));
				temp = HuffmanTree;
			}
			if(bit == 1){
				temp = temp->right;
			}
			if(bit == 0){
				temp = temp->left;
			}
		}
	}
	OutFile.close();
	inFile.close();

	return 0;

}

