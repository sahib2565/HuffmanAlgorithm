#include <fstream>
#include <bits/stdc++.h>

// Implementing a Huffleman tree, which is basically a binary tree

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

// Print out the tree
void dfsPreOrder(Node* root) {
    if (root == nullptr) return;

    // Process the current node
    if(root->data != -1){
        std::cout << char(root->data) << " " << root-> freq << std::endl;
    }else{
        std::cout << root->data << " " << root-> freq << std::endl;
    }

    // Recur on left and right children
    dfsPreOrder(root->left);
    dfsPreOrder(root->right);
}


// Creating a table to encode our string
// Pass by pointer to the root
void buildNewTable(Node* root, std::string currentCode, std::vector<std::string>& table) {
    if (!root) return;

    // If it's a leaf node, store the code
    if (!root->left && !root->right) {
        table[root->data] = currentCode;
        return;
    }

    // Traverse left (add '0') and right (add '1')
    buildNewTable(root->left, currentCode + "0", table);
    buildNewTable(root->right, currentCode + "1", table);
}


int main() {
    bool debug = 0;
    std::vector <std::pair<int, int>> asciitable;

    // Initalizing the vector
    for(int i = 0;i < 256;i++){
        asciitable.push_back(std::pair(i,0));
    }

    //Last character, to tell decompresser when to stop
    asciitable.push_back(std::pair(256,1));

    std::string filename;
    std::cout << "Enter file path: ";
    std::cin >> filename;
    std::ifstream myFile(filename);
    char text;

    while(myFile.get(text)){
        asciitable[(unsigned char)(text)].second++;
    }


    if(debug == 1){
        for(auto i : asciitable){
            if(i.second > 0){
                std::cout << char(i.first) << ": " << i.second << std::endl;
            }
        }
    }

    // Building the priority queue

    std::priority_queue<Node*,std::vector<Node*>, CompareNodes> pq;


    // Building the leaf nodes
    for(auto i : asciitable){
        if(i.second > 0){
            Node* nodo = new Node(i.second, i.first);
            pq.push(nodo);
        }
    }

    // Building the tree

    while(pq.size() > 1){
        Node* temp1 = pq.top();
        pq.pop();
        Node*temp2 = pq.top();
        pq.pop();
        Node* nodo = new Node((temp1->freq + temp2->freq),-1);
        nodo->left = temp1;
        nodo->right= temp2;
        pq.push(nodo);
    }

    // Compressing the file

    Node* HufflemanTree = pq.top();
    pq.pop();

    // Build the new code table
    // 1. Prepare a vector for all 256 ASCII characters
    std::vector<std::string> huffmanCodes(257, "");

    // 2. Call the function starting with an empty string ""
    buildNewTable(HufflemanTree, "", huffmanCodes);

    // 3. Now huffmanCodes is filled!
    if(debug == 1){
        for(int i = 0;i < 256;i++){
            if(huffmanCodes[i] != ""){
                std::cout << char(i) << " = " << huffmanCodes[i] << std::endl;
            }
        }
    }


    // Creating our compressed file

    unsigned char bucket = 0;
    int counter = 0;
    myFile.clear();
    myFile.seekg(0,myFile.beg);


    // Output file
    std::ofstream outFile(filename + ".bin",std::ios::binary);

    // Writing the header
    // 1. Count how many unique characters actually appeared
    int uniqueChars = 0;
    for(int i = 0; i < 257; i++){
        if(asciitable[i].second > 0){
            uniqueChars++;
        }
    }

    // 2. Write the total count of unique characters (4 bytes)
    outFile.write(reinterpret_cast<const char*>(&uniqueChars), sizeof(uniqueChars));

    // 3. Write the frequency table
    for(int i = 0; i < 257; i++){
        if(asciitable[i].second > 0){
            // Write the character index (we cast it to an int so we can store '256' for EOF)
            int charIndex = i;
            outFile.write(reinterpret_cast<const char*>(&charIndex), sizeof(charIndex));
            
            // Write the frequency count
            int freq = asciitable[i].second;
            outFile.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
        }
    }

    // Compressing the file
    while(myFile.get(text)){
        std::string code = huffmanCodes[(unsigned char)(text)];
        for(int i = 0;i < code.size();i++){
            bucket = bucket << 1;
            if(code[i] == '1'){
                bucket = bucket | 1;
            }

            counter++;
            if(counter == 8){
                outFile.put(bucket);

                // Reset for the next batch
                bucket = 0;
                counter = 0;
            }
        }
    }

    myFile.close();

    // EOF

    std::string code = huffmanCodes[256];
    for(int i = 0;i < code.size();i++){
        bucket = bucket << 1;
        if(code[i] == '1'){
            bucket = bucket | 1;
        }

        counter++;
        if(counter == 8){
            outFile.put(bucket);

            // Reset for the next batch
            bucket = 0;
            counter = 0;
        }
    }
    if(counter > 0){
        while(counter < 8){
            bucket = bucket << 1;
            counter++;
        }
        outFile.put(bucket);
    }

    outFile.close();

    return 0;
}