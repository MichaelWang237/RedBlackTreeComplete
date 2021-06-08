//RED-BLACK TREE
//Michael Wang
//5/30/2021 [part 2 - 6/7/2021]
//This project allows the user to create a self-balancing binary tree from a given file or through manual data entry.
//REFERENCES:
//  -Videos on Canvas
//  -Various youtube videos, including: https://www.youtube.com/watch?v=5IBxA-bZZH8&t=17s and https://www.youtube.com/watch?v=nMExd4DthdA and https://www.youtube.com/watch?v=v6eDztNiJwo
//  -https://www.geeksforgeeks.org/red-black-tree-set-1-introduction-2/
//  -https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html
//  -https://brilliant.org/wiki/red-black-tree/
//  -https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
//  -https://web.eecs.umich.edu/~sugih/courses/eecs281/f11/lectures/11-Redblack.pdf

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

//Each node contains a data value (int), a boolean value to keep track of color and is linked to its left and right children as well as its parent.
struct node{
    int data;
    bool isRed = true;
    node* left = NULL;
    node* right = NULL;
    node* parent = NULL;
};

//function prototypes below 
void print(node* root, int spacing);
void add(node* root, node* newNode);
void rotateL(node*&root, node* n);
void rotateR(node*&root, node* n);
void insert(node*&root, node* n);
//PART2
node* search(node* root, int a);
bool deleteNode(node*& root, node*n);
node* Successor(node* n);
node* Sibling(node* n);
void fix(node*&root, node* n);

//part 1 of the project is directly below, followed by MAIN. Immediately below MAIN are the part 2 functions -- search and delete -- as well as some associated functions.

//Prints tree through recursion
void print(node* root, int spacing){
    if(root == NULL){
        return;
    }
    spacing += 5;
    print(root->right, spacing);
    cout << endl;
    for(int i = 5; i < spacing; i++){
        cout << " ";
    }
    if(root->isRed){
        cout << "[r]";
    }
    else{
        cout << "[b]";
    }
    cout << root->data << '\n';
    print(root->left, spacing);
}

//This function is used by the "insert" function and simply adds the node to the correct position without doing anything to maintain the properties of a red-black tree.
void add(node* root, node* n){
    node* temp = root;
    while(1){

        if(n->data >= temp->data){
            if(temp->right == NULL){
                temp->right = n;
                n->parent = temp;
                return;
            }
            else{
                temp = temp->right;
            }
        }
        else if(n->data < temp->data){
            if(temp->left == NULL){
                temp->left = n;
                n->parent = temp;
                return;
            }
            else{
                temp = temp->left;
            }
        }
    }
}

void rotateL(node*& root, node* n){
    //function for left rotation around node n
    cout << "LR" << endl;
    node* right = n->right;
    node* temp = right->left;
    node* parent = n->parent;
    if(n == root){
        cout << "NOTE: root was affected by this rotation" << endl;
        root = right;    
    }
    else if(n == n->parent->left){
        n->parent->left = right;
    }
    else if(n == n->parent->right){
        n->parent->right = right;
    }
    n->parent = right;
    n->right = temp;
    if(temp != NULL) temp->parent = n;
    right->left = n;
    right->parent = parent;
}

void rotateR(node*&root, node* n){
    //function for right rotation around node n
    cout << "RR"<< endl;
    node* left = n->left;
    node* temp = left->right;
    node* parent = n->parent;
    if(n->parent == NULL){
        cout << "NOTE: root was affected by this rotation" << endl;
        root = left;
        root->parent = NULL;
    }
    else if(n == n->parent->left){
        n->parent->left = left;
    }
    else if(n == n->parent->right){
        n->parent->right = left;
    }
    n->parent = left;
    n->left = temp;
    if(temp != NULL) temp->parent = n;
    left->parent = parent;
    left->right = n;
    
}
//Using the "add" function, this function puts the node on the tree and does the necessary changes to ensure that the tree follows the rules of a red-black tree
//This is done by checking the new node's relatives against every case that violates the properties of a red-black tree.
void insert(node*& root, node* n){
    //If the node's parent is black and the node itself is red, then no changes need to be made. However, if the parent is red, then further analysis needs to be done.
    add(root, n);
    if(n->parent->isRed){
        node* uncle = NULL;
        bool isUncleR = false;
        while((n != root) && (n->parent->isRed)){ 
            if(n->parent != root && n != root){
                if(n->parent == n->parent->parent->left){
                    uncle = n->parent->parent->right;
                    isUncleR = true;
                }
                else{
                    uncle = n->parent->parent->left;
                    isUncleR = false;
                }
            }
            else if(n == root->left){
                isUncleR = true;
            }
            else if(n == root->right){
                isUncleR = false;
            }
            if((uncle != NULL) && (uncle->isRed)){
                n->parent->isRed = false;
                uncle->isRed = false;
                n->parent->parent->isRed = true;
                n = n->parent->parent;
            }
            else if((uncle == NULL) || (uncle->isRed == false)){
                if(((n == n->parent->right) && (n->parent == n->parent->parent->left)) || ((n == n->parent->left) && (n->parent == n->parent->parent->right))){
                    if(n == n->parent->left){
                        n = n->parent;
                        rotateR(root, n);
                    }
                    else{
                        n = n->parent;
                        rotateL(root, n);
                    }
                }
                n->parent->parent->isRed = true;
                n->parent->isRed = false;
                if(isUncleR){
                    rotateR(root, n->parent->parent);
                }
                else{
                    rotateL(root, n->parent->parent);
                }
            }
        }
    }
    root->isRed = false;
}

int main(){
    bool running = true;
    while(running){
        node* root;
        bool initRoot = false;
        bool sameTree = true;
        while(sameTree == true){
            cout << "Type 'add' to add a series of numbers, 'print' to display the tree, 'read' to create a tree from a file, 'search' to verify whether a given value is present, 'delete' to remove nodes of a certain value, and 'quit' to quit." << endl;
            int index = 0;
            int* array = new int[100];
            char* input = new char[10];
            bool addCommand = false;
            cin.clear();
            cin.getline(input, 10);
            
            //Implementation of ADD function in main
            if(strcmp(input, "add") == 0){
                char* temp = new char[500];
                cout << "Type integers below. Make sure to separate with spaces."  << endl;
                cin.getline(temp, 500);
                char* token = strtok(temp, " ");
                while(token != NULL){
                    array[index] = atoi(token);
                    token = strtok(NULL, " ");
                    index++;
                }
                addCommand = true;
            }

            //Implementation of READ function in main
            else if(strcmp(input, "read") == 0){
                cout << "Enter a file name, including the format, e.g. [fileName.txt]. Make sure the numbers in the file are separated by spaces." << endl;
                string fileName;
                cin.clear();
                cin >> fileName;
                fstream file;
                int temp = 0;
                file.open(fileName, ios::in);
                while(file >> temp){
                    array[index] = temp;
                    index++;
                }
                addCommand = true;
            }

            //Implementation of PRINT function in main
            else if(strcmp(input, "print") == 0){
                print(root, 0);
            }

            //Implementation of SEARCH function in main
            else if(strcmp(input, "search") == 0){
                int temp = 0;
                cout << "What is the value of the integer you would like to search for?" << endl;
                cin >> temp;
                node* n = search(root, temp);
                if(n == NULL){
                    cout << "Node could not be found" << endl;
                }
                else{
                    cout << "This node exists in the tree. Somewhere." << endl;
                }
            }

            //Implementation of DELETE function in main
            else if(strcmp(input, "delete") == 0){
                char* temp = new char[500];
                cout << "Separate each integer you want to delete with a space." << endl;
                cin.getline(temp, 500);
                char* token = strtok(temp, " ");
                while(token != NULL){
                    array[index] = atoi(token);
                    token = strtok(NULL, " ");
                    index++;
                }
                bool treeNotEmpty = true;
                int i = 0;
                while(i < index){
                    node* x = search(root, array[i]);
                    if(x != NULL){
                        treeNotEmpty = deleteNode(root, x); // <----- Delete function called here
                        if(treeNotEmpty == false){
                            sameTree = false;
                            break;
                        }
                    }
                    else{
                        cout << "couldn't find " << array[i] << endl;
                    }
                    i++;
                }
                delete [] array;
            }

            //Implementation of quit option
            else if(strcmp(input, "quit") == 0){
                cout << "quitting..." << endl;
                running  = false;
                sameTree = false;
                exit(0);
            }
            //If the input doesn't match any commands, the user is asked to input a valid command.
            else{
                cout << "Enter a valid command" << endl;
            }
            if(addCommand == true){
                int i = 0;
                //If the tree is empty, set root to black.
                if(initRoot == false){ 
                    root = new node;
                    root->data = array[0];
                    root->isRed = false;
                    i++;
                    initRoot = true;
                }
                while(i < index){
                    node* n = new node;
                    n->data = array[i];
                    insert(root, n);
                    i++;
                }
                delete [] array;
            }
        }
    }
    return 0;
}



node* search(node* root, int a){
    //function can be called to check the existence of a node in the tree based on its integer value
    //if the node exists, it is returned, if not, the function returns NULL
    node* n = new node;
    n = root;
    while(1){
        if(a > n->data){
            if(n->right == NULL){
                return NULL;
            }
            n = n->right;
        }
        else if(a < n->data){
            if(n->left == NULL){
                return NULL;
            }
            n = n->left;
        }
        else if(n->data == a){
            return n;
        }
    }
}

node* Successor(node* n){ //function to find in-order successor to a node
    node* temp = n;
    if(temp->left != NULL){
        temp = temp->left;
        while(temp->right != NULL){
            temp = temp->right;
        }
    }
    else if((temp->left == NULL) && (temp->right != NULL)){
        temp = temp->right;
        while(temp->left != NULL){
            temp = temp->left;
        }
    }
    return temp;
}

node* Sibling(node* n){ //finds the sibling of the given node
    if(n == n->parent->left){
        return n->parent->right;
    }
    else{
        return n->parent->left;
    }
}

//This function is ultimately responsible for deletion while maintaining the properties of a red black tree.
bool deleteNode(node*& root, node* n){ 
    if(n == root && root->right == NULL && root->left == NULL){ 
        root = NULL;
        delete root;
        return false;
    }
    if(n->left != NULL && n->right != NULL){
        cout << "finding successor" << endl;
        node*s = Successor(n);
        n->data = s->data;
        n = s;
    }

    //IF n is a black node... The code below handles each case.
    if(!n->isRed){ 
        if(n->left != NULL){
            n->data = n->left->data;
            n = n->left;
            deleteNode(root, n);
        }
        else if(n->right != NULL){

            n->data = n->right->data;
            n = n->right;
            deleteNode(root, n);
        }
        else if(n->right == NULL && n->left == NULL){
            node* sibling = Sibling(n);
            if(n == n->parent->left){
                if(sibling->isRed){

                    sibling->isRed = false;
                    rotateL(root, n->parent);
                    sibling = Sibling(n);
                    sibling->isRed = true;
                    if(n == n->parent->left){
                        n->parent->left = NULL;
                    }
                    else{
                        n->parent->right = NULL;
                    }
                    delete n;
                    return true;
                }
                if(!sibling->isRed){

                    if(sibling->right != NULL && sibling->right->isRed){
                        Sibling(n)->right->isRed = false;
                        rotateL(root, n->parent);
                        n->parent->left = NULL;
                        delete n;
                    }
                    else if(sibling->left != NULL && sibling->left->isRed){
                        Sibling(n)->left->isRed = false;
                        Sibling(n)->right->isRed = true;
                        rotateR(root, Sibling(n));
                        rotateL(root, n->parent);
                        n->parent->left = NULL;
                        delete n;
                    }
                    else{

                        sibling->isRed = true;
                        node* p = n->parent;
                        if(n == n->parent->left){
                            p->left = NULL;
                            delete n;
                        }
                        else if(n == n->parent->right){
                            p->right = NULL;
                            delete n;
                        }
                        fix(root, p);
                    }
                }
            }
            else{

                if(sibling->isRed){
                    sibling->isRed = false;
                    rotateR(root, n->parent);
                    sibling = Sibling(n);
                    sibling->isRed = true;
                    if(n == n->parent->left){
                        n->parent->left = NULL;
                    }
                    else{
                        n->parent->right = NULL;
                    }
                    delete n;
                    return true;
                }
                if(!sibling->isRed){

                    if(sibling->left != NULL && sibling->left->isRed){
                        Sibling(n)->left->isRed = false;
                        rotateR(root, n->parent);
                        n->parent->right = NULL;
                        delete n;
                    }
                    else if(sibling->left != NULL && sibling->right->isRed){
                        Sibling(n)->left->isRed = false;
                        Sibling(n)->right->isRed = true;
                        rotateL(root, Sibling(n));
                        rotateR(root, n->parent);
                        n->parent->right = NULL;
                        delete n;
                    }
                    else{
                        sibling->isRed = true;
                        node* p = n->parent;
                        if(n == n->parent->left){
                            p->left = NULL;
                            delete n;
                        }
                        else if(n == n->parent->right){
                            p->right = NULL;
                            delete n;
                        }
                        fix(root, p);
                    }
                }
            }
        }
    }
    //In case n is red, this section takes care of all 3 possibilities.
    else if(n->isRed){ 
        if(n->left != NULL){ 

            n->data = n->left->data;
            n->isRed = false;
            n->left = NULL;
        }
        else if(n->right != NULL){

            n->data = n->right->data;
            n->isRed = false;
            n->right = NULL;
        }
        else{

            if(n == n->parent->left){
                n->parent->left = NULL;
            }
            else if(n == n->parent->right){
                n->parent->right = NULL;
            }
            return true;
        }
    }
    return true;
}

//This function moves up the tree to take care of problems caused by other operations.
void fix(node*&root, node* n){ 
    while(n != root && !n->isRed){
        if(n == n->parent->left){
            node* sibling = n->parent->right;
            if(sibling->isRed){
                sibling->isRed = false;
                n->parent->isRed = true;
                rotateL(root, n->parent);
                sibling = n->parent->right;
            }
            if(( sibling->left == NULL || !sibling->left->isRed) && (sibling->right == NULL || !sibling->right->isRed)){
                sibling->isRed = true;
                n = n->parent;
            }
            else if(sibling->left->isRed){
                sibling->left->isRed = false;
                sibling->isRed = true;
                rotateR(root, sibling);
                sibling = n->parent->right;
            }
            if(sibling->right->isRed){
                sibling->isRed = n->parent->isRed;
                n->parent->isRed = false;
                sibling->right->isRed = false;
                rotateL(root, n->parent);
                break;
            }
        }
        else{
            node* sibling = n->parent->left;
            if(sibling->isRed){
                sibling->isRed = false;
                n->parent->isRed = true;
                rotateR(root, n->parent);
                sibling = n->parent->left;
            }
            if((sibling->left == NULL || !sibling->left->isRed) && (sibling->right == NULL || !sibling->right->isRed)){
                sibling->isRed = true;
                n = n->parent;
            }
            else if(sibling->right->isRed){
                sibling->left->isRed = false;
                sibling->isRed = true;
                rotateL(root, sibling);
                sibling = n->parent->left;
            }
            if(sibling->left->isRed){
                sibling->isRed = n->parent->isRed;
                n->parent->isRed = false;
                sibling->left->isRed = false;
                rotateR(root, n->parent);
                break;
            }
        }
    }
    n->isRed = false;
}