#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;

/*Binary Tree Nodes*/
struct Node{
	long int data;
	Node *parent;	/*Points the node's parent. It is necessary to print the best paths*/
	Node *left;
	Node *right;
};

/*I used a dynamic array to create binary tree, This array holds the nodes of the tree.*/
/*Also I will use another array to keep nodes belongs to path and to print them*/
struct Array{
	Node *binary;		/*Points the binary nodes*/
};

struct BinaryTree{
	Node *root;
	Array *numberArray;		/*My dynamic array to construct binary tree by using given method*/
	Array *pathArray;		/*Keeps the nodes belongs to path and to print them*/
	long int nodecount;		/*Holds number of nodes my tree has*/
	long int target;		/*Holds target number*/
	bool found;				/*Holds a boolean value for the path process, if a path was found:true, if not:false*/
	Node *last_path_node;	/*Points the last node of the found path beacuse my recursive function finds last node belongs to the best path*/

	FILE *file;
	void create();						/*Creates the binary tree*/
	void ReadFile(char *);				/*Reads the file*/
	void Find_Paths(Node*, int);		/*Finds the best paths (Recursive process)*/	
	void print_path(Node*);				/*Sends the nodes to recursive funtion and prints the paths*/
	void close();						/*Gives back all allocated spaces to the memory*/
	void emptytree(Node *);

};

void BinaryTree::create(){

	root=numberArray[1].binary;		/*Root is first index of array*/
	root->parent=NULL;				/*Root has no parent*/

	/*Here I applied the method which is given in the explanations. Thus I connected nodes (creates tree)*/
	
	for(int index=2; index<=nodecount; index++){
	
		if(index%2==0){ 		/*If the index can be divided into two, This node will be index/2 node's left child*/
			numberArray[index/2].binary->left=numberArray[index].binary;
			numberArray[index].binary->parent=numberArray[index/2].binary;		/*Saves it's parent*/
		}
		else if(index%2==1){	/*If the index cannot be divided into two, This node will be (index-1)/2 node's right child*/
			numberArray[(index-1)/2].binary->right=numberArray[index].binary;
			numberArray[index].binary->parent=numberArray[(index-1)/2].binary;	/*Saves it's parent*/
		}
	}
}

void BinaryTree::ReadFile(char *filename){

	long int number1=0;		/*Number taken from the file*/
	long int index=1;		/*Holds the index of the nodes*/
	nodecount = 0;
	string line;			/*I read the file line by line, this holds the line string.*/

	Node *newnode;

	ifstream file;

	file.open( filename );		/*Opens file*/
	if(!file){					/*If the file cannot be opened*/
		
		cerr << "Cannot open grid file" << endl;
		exit(1);

	}
	
	numberArray= new Array[105];	/*Allocate a space from memory for my dynamic array*/
	numberArray[0].binary=NULL;		/*I wanted to start from index 1 for easy implementation so index 0 is unused*/

	getline(file,line);		/*Takes the first line from the file*/
	stringstream ss(line);
	string number;


	while(ss >> number){	/*Takes the numbers from line string*/

		number1=atoi(number.c_str());		/*Converts them to integer*/

		newnode= new Node;			/*Creates a node and fills the data*/
		newnode->data=number1;
		newnode->parent=NULL;
		newnode->left=NULL;
		newnode->right=NULL;

		numberArray[index].binary=newnode;	/*Put it into array in order*/
		index++;							/*Increment index of array and nodecount*/
		nodecount++;

	}

	file >> number1;	/*Takes target number from the file*/
	target=number1;

	file.close();		/*Closes file*/

}
/*When sum equals the target, my recursive function will stop and saves the last node of the path.*/
void BinaryTree::Find_Paths(Node *p, int sum){

	if (p && !found){	

		sum=sum-p->data;		/*Subtract the current number from the sum*/

		if(sum==0){				/*If sum is zero, this means that we found the path*/
			last_path_node=p;	/*The last node of the path is current node and saves it*/
			found=true;
			return;
		}
		
		Find_Paths(p->left,sum);	/*Preorder traversing*/

		if(found)
			return;

		Find_Paths(p->right,sum);
	}
}

/*Gives back all allocated spaces to the memory*/
void BinaryTree::close(){

	delete[] numberArray;
	emptytree(root);

}

/*I took this part from our lecture slides, It deletes the nodes from tree*/
void BinaryTree::emptytree(Node* p){

	if (p) {
		if (p->left != NULL){
			emptytree(p->left);
			p->left = NULL;
		}
		if (p->right != NULL){
			emptytree(p->right);
			p->right = NULL;
		}
		delete p;
	}
}

void BinaryTree::print_path(Node* currentnode){

	long int sum=target;		/*Holds the sum. Initially it equals to target*/
	found=false;				/*Initially no path found*/
	sum=sum-root->data;			/*Subtracts the root number from the sum beacuse I will send the root's left and right child to my recursive function separately*/
								/*in order to find two best paths*/


	if(sum==0){					/*If sum equals the target before calling function, this means that just root is a path.*/
		cout << "Path Found: " << root->data << endl;
	}
	else if(sum<0){				/*If sum is negative before calling function, this means that initially there is no path.*/
		cout << "No Path Found" << endl;
	}

	else{
		pathArray= new Array[105]; 	/*Create an array to keep and print the path*/
		long int indexpath=0;		/*Index of this array is 0 initially*/
		
		Find_Paths(currentnode, sum);	/*Tries to find path for left or right subtrees of the root*/

		if(found){						/*If a node was found*/
			Node* temp=last_path_node; /*For traversing nodes*/

			while(temp!=NULL){		/*Starts with the last node of the path, It saves all path's nodes into the array thanks to the nodes keeping their parents*/
				pathArray[indexpath].binary=temp; 	/*This index points the node*/
				temp=temp->parent;					/*Goes it's parent*/
				indexpath++;						/*Increment array index*/
			}
			cout << "Path Found: ";
			for(int i=indexpath-1; i>=0; i--){		/*Since array keeps the nodes in reverse order, I started to print them from end of the array*/
				cout << pathArray[i].binary->data << " ";
			}
			cout << endl;
		}
		else{		/*If a node was not found*/
			cout << "No Path Found" << endl;
		}
		delete[] pathArray;		/*Im done with this array and I can give back the allocated space to memory*/
	}
}

int main (int argc , char ** argv){
	
	BinaryTree Tree;			/*My Tree*/
	Tree.ReadFile(argv [1]);	/*Calls the function to read numbers from the file*/
	Tree.create();				/*Calls the function to create tree*/

		
	Tree.print_path(Tree.root->left);		/*First sends the left subtrees of the root*/
	Tree.print_path(Tree.root->right);		/*Sends the right subtrees of the root*/


	Tree.close();				/*Calls the function to close and to clear the tree and dynamic array*/

	return 0;
}