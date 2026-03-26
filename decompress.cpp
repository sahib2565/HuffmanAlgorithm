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

void decompressFile(Node* root,std::string code,std::ofstream &myFile){
	if(!root) return;

	if(!root->left && !root->right){
		myFile << char(root->data);
		return;
	}

	if(code[0] == '1'){
		decompressFile(root->right,code.erase(0,1),myFile);
	}else{
		decompressFile(root->left,code.erase(0,1),myFile);
	}
}


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
}

