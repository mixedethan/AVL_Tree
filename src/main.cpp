#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

/*
 *
 *
 *              AVLTree Project
 *          Designed by: Ethan Wilson
 *                Version 1.0
 *
 *
 */

/* Note:
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope.
*/

///Remember to use "condition ? expression_if_true : expression_if_false;"

struct Node // Basic Node class
{
    string name;
    int UFID;
    Node* left;
    Node* right;

    Node(string name, int UFID){
        this->name = name;
        this->UFID = UFID;
        this->left = nullptr;
        this->right = nullptr;
    };
};

class AVLTree{

private:
    Node* root;
    int nodeCount;

public:

//AVL Tree constructor
    AVLTree(){
        this->root = nullptr;
        this->nodeCount = 0;
    }

//AVL Tree destructor
    ~AVLTree() { //AVL Tree destructor
        delete root;
    }

//Root Getter
    Node* getRoot() {
        return root;
    }

//Root Setter
    void setRoot(Node* newRoot) {
        this->root = newRoot;
    }

// Left rotation
    Node* rotateLeft(Node *node) {
        Node* newParent = node->right;  // New root after rotation
        node->right = newParent->left;  // Update right child of old root
        newParent->left = node;         // Update left child of new root
        return newParent;               // Return new root
    }

// Right rotation
    Node* rotateRight(Node *node) {
        Node* newParent = node->left;  // New root after rotation
        node->left = newParent->right; // Update left child of old root
        newParent->right = node;       // Update right child of new root
        return newParent;              // Return new root
    }

// Right-Left rotation
    Node* rotateRightLeft(Node *node) {
        node->right = rotateRight(node->right); // First, right rotation on right child
        return rotateLeft(node);                // Then, left rotation on root
    }

// Left-Right rotation
    Node* rotateLeftRight(Node *node) {
        node->left = rotateLeft(node->left);  // First, left rotation on left child
        return rotateRight(node);             // Then, right rotation on root
    }

// Recursively finds the height of the tree
    int getHeight(Node* root)
    {
        return (root == nullptr) ? 0 : 1 + max(getHeight(root->left), getHeight(root->right));
    }

// Finds the balance factor of a node (height of left subtree - height of right subtree)
    int balanceFactor(Node* node) {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

// Helper function that balances the tree if necessary by making rotations
    Node* balanceAndRotate(Node* node, int ID) {
        int balFactor = balanceFactor(node);
        return (balFactor > 1 && ID < node->left->UFID) ? rotateRight(node) :
               (balFactor < -1 && ID > node->right->UFID) ? rotateLeft(node) :
               (balFactor > 1 && ID > node->left->UFID) ? rotateLeftRight(node) :
               (balFactor < -1 && ID < node->right->UFID) ? rotateRightLeft(node) : node;
    }

/// Required Functions

// Insert function that takes in root node & the new name & ID being inserted
    Node* insert(Node* node, string name, int ID) {
        // Base case: if node is null, create a new node with the given name & ID
        if (!node) {
            //cout << "successful" << endl; possible extra
            nodeCount++;
            node = new Node(name, ID);
            // Otherwise, recursively insert the new node into the left subtree
        } else if (node->UFID > ID) {
            node->left = insert(node->left, name, ID);
            // Otherwise, recursively insert the new node into the right subtree
        } else if (node->UFID < ID) {
            node->right = insert(node->right, name, ID);
            // Otherwise, the ID already exists in the tree
        } else {
            cout << "unsuccessful" << endl;
        }
        return balanceAndRotate(node, ID);
    }

// Removes a node when provided with an initial node & the UFID of the node to delete
    Node* remove(Node* node, int UFID){
        //If the initial node is null
        if(!node){
            return nullptr;
        }

        //Search for the node via UFID that must be deleted
        // Node is located in the right subtree
        if(UFID > node->UFID){
            node->right = remove(node->right, UFID);
        }
            //Node is located in the left subtree
        else if(UFID < node->UFID){
            node->left = remove(node->left, UFID);
        }
            // If the ID matches, reduce nodeCount & begin removal process by checking children
        else if(node->UFID == UFID) {
            nodeCount--;

            //If the node has 0 children, delete that sucker & set it to nullptr
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
                //If there is no left child
            else if(!node->left){
                Node* newNode = node->right;
                delete node;
                return newNode;
            }
                //If there is no right child
            else if(!node->right){
                Node* newNode = node->left;
                delete node;
                return newNode;
            }

                // If the node has two children
            else{
                Node* temp = node->right;
                //Checks all the left children until the smallest node is found
                while(temp->left){
                    temp = temp->left;
                }
                node->UFID = temp->UFID;
                node->name = temp->name;
                node->right = remove(node->right, temp->UFID);
            }
        }
        return node;
    }

// Searches for a match for a UFID, if so, it prints the corresponding name
    bool searchID(Node* node, int UFID){
        // If the node doesn't have any value
        if(node == nullptr){
            cout << "unsuccessful" << endl;
            return false;
        }
            //If the current node's UFID matches the target UFID
        else if(node->UFID == UFID){
            // Print the UFID's assigned name & return true that it was found
            cout << root->name << endl;
            return true;
        }
        else if(UFID < node->UFID){
            // If the target UFID is less than the current node's UFID
            // Recursive call to search to the left of the current node
            searchID(node->left, UFID);
        }
        else if(UFID > node->UFID){
            // If the target UFID is greater than the current node's UFID
            // Recursive call to search to the right of the current node
            searchID(node->right, UFID);
        }
    }

// Finds student with a specified name and prints the associated UFID.
    bool searchName(Node* root, string name){
        vector<Node*> preOrderNodes;
        vector<Node*> matchedNames;
        preOrder(root, preOrderNodes);
        ///  WIP ///
        // Search for matching name & print their UFIDs
        for(auto & preOrderNode : preOrderNodes){
            if(preOrderNode->name == name){
                matchedNames.push_back(preOrderNode);
                // Prints ID while only using 8 digits
                cout << setw(8) << setfill('0') << preOrderNode->UFID << endl;
            }
        }

        //Check if any names matched
        if(matchedNames.empty()){
            // If no matches are found
            cout << "unsuccessful" << endl;
            return false;
        }
        return true;
    }

    void printInOrder(Node* root){
        vector<Node*> nodes;
        inOrder(root, nodes);
        auto iter = nodes.begin();

        //Loop throught the vector & print the names
        while(iter != nodes.end()){
            cout << (*iter)->name;
            ++iter;

            //Add a comma if it's not the last element
            if(iter != nodes.end()){
                cout << ", ";
            }
        }
    }

    void printPreOrder(Node* root) {
        vector<Node*> nodes;
        preOrder(root, nodes);

        auto iter = nodes.begin();

        // Loop through the vector and print names
        while (iter != nodes.end()) {
            std::cout << (*iter)->name;
            ++iter;

            // Add a comma only if this is not the last element
            if (iter != nodes.end()) {
                std::cout << ", ";
            }
        }
    }

    void printPostOrder(Node* root) {
        vector<Node*> nodes;
        postOrder(root, nodes);

        auto it = nodes.begin();

        // Loop through the vector and print names
        while (it != nodes.end()) {
            cout << (*it)->name;
            ++it;

            // Add a comma only if this is not the last element
            if (it != nodes.end()) {
                cout << ", ";
            }
        }
    }

    void printLevelCount(Node* root) {
        // Calculate the height of the tree rooted at 'root' aka Level count
        int treeHeight = getHeight(root);
        // Output the height, which is also the level count
        cout << treeHeight << endl;
    }

    Node* removeInorder(Node* root, int n){

        // Vector to store nodes in inorder traversal
        vector<Node*> inorderNodes;
        inOrder(root, inorderNodes);

        // Check for empty tree or out-of-range index
        if (root == nullptr || n >= static_cast<int>(inorderNodes.size())) {
            cout << "unsuccessful" << endl;
            return root;
        }

        // Remove the node at the 'n'-th position in inorder traversal
        Node* updatedRoot = remove(root, inorderNodes[n]->UFID);
        cout << "successful" << endl;

        return updatedRoot;
    }

/// Helper Functions for AVLTree Class to make life easier

    // Adds the nodes of the tree into a referenced vector in the inOrder order
    void inOrder(Node* root, vector<Node*>& nodes) {
        stack<Node*> s;
        Node* curr = root;

        // Continue until the stack is empty or the current node is null
        while (!s.empty() || curr != nullptr) {
            // Reach the leftmost node of the current node
            while (curr != nullptr) {
                s.push(curr);
                curr = curr->left;
            }

            // Current must be null so pop the top node from the stack
            curr = s.top();
            s.pop();

            // Add popped node to the vector
            nodes.push_back(curr);

            // Check the right subtree
            curr = curr->right;
        }
    }

    // Adds the nodes of the tree into a referenced vector in the preOrder order
    void preOrder(Node* root, vector<Node*>& nodes) {

        if (!root) {
            return;
        }

        stack<Node*> s;
        s.push(root);
        //Continue while stack isn't empty
        while (!s.empty()) {
            //Pop the top node from the stack and add it to the vector
            Node* curr = s.top();
            s.pop();
            nodes.push_back(curr);
            //if the right child ecists add it to the stack
            if (curr->right) s.push(curr->right);
            //if the left child exists add it to the stack
            if (curr->left) s.push(curr->left);
        }
    }

    // Adds the nodes of the tree into a referenced vector in the postOrder order
    void postOrder(Node* root, vector<Node*>& nodes) {
        if (!root){
            return;
        }

        stack<Node*> s1;
        stack<Node*> s2;

        s1.push(root);

        // Continue as long as the first stack is not empty
        while (!s1.empty()) {
            // Pop the top node from the first stack & push it onto the second stack
            Node* curr = s1.top();
            s1.pop();
            s2.push(curr);

            // Push the left child onto the first stack if it exists
            if (curr->left) s1.push(curr->left);

            // Push the right child onto the first stack if it exists
            if (curr->right) s1.push(curr->right);
        }

        // Pop nodes from the second stack & add them to the nodes vector
        while (!s2.empty()) {
            nodes.push_back(s2.top());
            s2.pop();
        }
    }

    // Checks whether or not a UFID exists in the tree
    bool UFIDExists(Node* node, int UFID){
        if(node == nullptr){
            return false;
        }
        if(node->UFID == UFID){
            return true;
        }
        //Recursively checks both the left & right subtree for the UFID
        return UFIDExists(node->left, UFID) || UFIDExists(node->right, UFID);
    }
};

// UFID can only contain 8 digits & must be unique
bool validUFID(string UFID)
{
    //Checks UFID's length
    if(UFID.length() != 8){
        return false;
    }

    //Checks for only numbers in the UFID
    for(char c : UFID){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}

/// Main() Helper Functions

// Names must only include [a-z,A-Z, & spaces]
bool validName (string name) {
    bool validName = true;

    for(int i = 0; i < name.length(); i++){
        if(!isalpha(name.at(i)) && !isspace(name.at(i))){
            validName = false;
            break;
        }
    }
    return validName;
}

// Verifies that the command is in the correct format
bool isValidCommand(const string& command) {
    for(char c : command) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void handleInsert(string command, AVLTree &gatorTree) {
    int index = command.find_first_of('\"');
    string name = command.substr(0, index);
    string check = command.substr(index + 2);
    if (validUFID(check) && validName(name)) {
        int ID = stoi(check);
        Node* newRoot = gatorTree.insert(gatorTree.getRoot(), name, ID);
        gatorTree.setRoot(newRoot);
        cout << "successful" << endl;
    } else {
        cout << "unsuccessful" << endl;
    }
}

void handleRemove(string command, AVLTree &gatorTree) {
    if(validUFID(command)) {
        int ID = stoi(command);
        if(gatorTree.UFIDExists(gatorTree.getRoot(), ID)) {
            gatorTree.setRoot(gatorTree.remove(gatorTree.getRoot(), ID));
            cout << "successful" << endl;
        } else {
            cout << "unsuccessful" << endl;
        }
    } else {
        cout << "unsuccessful" << endl;
    }
}

void handleSearch(string command, AVLTree &gatorTree) {
    if (command[0] == '"') {
        string name = command.substr(1, command.length() - 2);
        if(validName(name)) {
            gatorTree.searchName(gatorTree.getRoot(), name);
        } else {
            cout << "unsuccessful" << endl;
        }
    } else {
        if (validUFID(command)) {
            int ID = stoi(command);
            gatorTree.searchID(gatorTree.getRoot(), ID);
        } else {
            cout << "unsuccessful" << endl;
        }
    }
}

void handleFunctions(string command, AVLTree &gatorTree) {
    // Takes in input e.g "insert "Brandon" 45679999"
    int index = command.find_first_of(' ');
    // Function hold the value of the first work e.g "insert"
    string function = command.substr(0, index);
    command = command.substr(index + 1);

    if (function == "insert") {
        command = command.substr(1);
        handleInsert(command, gatorTree);
    } else if (function == "remove") {
        handleRemove(command, gatorTree);
    } else if (function == "search") {
        handleSearch(command, gatorTree);
    } else if (function == "printInorder") {
        gatorTree.printInOrder(gatorTree.getRoot());
        cout << endl;
    } else if (function == "printPreorder") {
        gatorTree.printPreOrder(gatorTree.getRoot());
        cout << endl;
    } else if (function == "printPostorder") {
        gatorTree.printPostOrder(gatorTree.getRoot());
        cout << endl;
    } else if (function == "printLevelCount") {
        gatorTree.printLevelCount(gatorTree.getRoot());
    } else if (function == "removeInorder") {
        if(isValidCommand(command)) {
            int n = stoi(command);
            gatorTree.setRoot(gatorTree.removeInorder(gatorTree.getRoot(), n));
        } else {
            cout << "unsuccessful" << endl;
        }
    }
}

int main(){

    AVLTree tree;
    int cmdCount;
    string line;
    vector<string> cmds;

    cin >> cmdCount;

// Stores each command in a vector
    for(int i = 0; i < cmdCount + 1; i++){
        getline(cin, line);
        cmds.push_back(line);
    }

// Parses each command and calls the appropriate AVLTree method
    for(auto cmd : cmds){
        handleFunctions(cmd, tree);
    }

    return 0;
}

