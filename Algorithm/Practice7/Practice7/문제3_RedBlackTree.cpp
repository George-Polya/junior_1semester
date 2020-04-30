// 2016112158 김희수
#include <iostream>
#include <string>
#include <iomanip>
#include <queue>
using namespace std;

enum Color {RED,BLACK};



struct Node
{
	int data;
	bool color;
	Node* left, * right, * parent;

	// Constructor 
	Node(int data)
	{
		this->data = data;
		left = right = parent = NULL;
		this->color = RED;
	}
};

// Class to represent Red-Black Tree 
class RBTree
{
private:
	Node* root;
protected:
	void rotateLeft(Node*&, Node*&);
	void rotateRight(Node*&, Node*&);
	void fixViolation(Node*&, Node*&);
public:
	// Constructor 
	RBTree() { root = NULL; }
	void insert(const int& n);
	void inorder();
	void levelOrder();
	void print2DUtil(Node*, int space);
	void print2D(int space);
};

// A recursive function to do level order traversal 
void inorderHelper(Node* root)
{
	if (root == NULL)
		return;

	inorderHelper(root->left);
	cout << root->data << "  ";
	inorderHelper(root->right);
}

/* A utility function to insert a new node with given key
   in BST */
Node* BSTInsert(Node* root, Node* pt)
{
	/* If the tree is empty, return a new node */
	if (root == NULL)
		return pt;

	/* Otherwise, recur down the tree */
	if (pt->data < root->data)
	{
		root->left = BSTInsert(root->left, pt);
		root->left->parent = root;
	}
	else if (pt->data > root->data)
	{
		root->right = BSTInsert(root->right, pt);
		root->right->parent = root;
	}

	/* return the (unchanged) node pointer */
	return root;
}

// Utility function to do level order traversal 
void levelOrderHelper(Node* root)
{
	if (root == NULL)
		return;

	std::queue<Node*> q;
	q.push(root);

	while (!q.empty())
	{
		Node* temp = q.front();
		cout << temp->data << "  ";
		q.pop();

		if (temp->left != NULL)
			q.push(temp->left);

		if (temp->right != NULL)
			q.push(temp->right);
	}
}

void RBTree::print2DUtil(Node* root, int space)
{
	// base case of recursion
	if (root == NULL)
		return;

	// 계층간의 거리 증가
	space += 5;

	// 오른쪽 자식노드에 대해서 먼저 수행
	print2DUtil(root->right, space);

	cout << endl;
	for (int i = 5; i < space; i++)
		cout << "  ";
	cout << root->data << "\n";

	// 왼쪽 자식 수행
	print2DUtil(root->left, space);


}

void RBTree::print2D(int space)
{
	print2DUtil(this->root, space);
}

void RBTree::rotateLeft(Node*& root, Node*& pt)
{
	Node* pt_right = pt->right;

	pt->right = pt_right->left;

	if (pt->right != NULL)
		pt->right->parent = pt;

	pt_right->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_right;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_right;

	else
		pt->parent->right = pt_right;

	pt_right->left = pt;
	pt->parent = pt_right;
}

void RBTree::rotateRight(Node*& root, Node*& pt)
{
	Node* pt_left = pt->left;

	pt->left = pt_left->right;

	if (pt->left != NULL)
		pt->left->parent = pt;

	pt_left->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_left;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_left;

	else
		pt->parent->right = pt_left;

	pt_left->right = pt;
	pt->parent = pt_left;
}

// This function fixes violations caused by BST insertion 
void RBTree::fixViolation(Node*& root, Node*& pt)
{
	Node* parent_pt = NULL;
	Node* grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != BLACK) &&
		(pt->parent->color == RED))
	{

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		/*  Case : A
			Parent of pt is left child of Grand-parent of pt */
		if (parent_pt == grand_parent_pt->left)
		{

			Node* uncle_pt = grand_parent_pt->right;

			/* Case : 1
			   The uncle of pt is also red
			   Only Recoloring required */
			if (uncle_pt != NULL && uncle_pt->color == RED)
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}

			else
			{
				/* Case : 2
				   pt is right child of its parent
				   Left-rotation required */
				if (pt == parent_pt->right)
				{
					rotateLeft(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				   pt is left child of its parent
				   Right-rotation required */
				rotateRight(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}

		/* Case : B
		   Parent of pt is right child of Grand-parent of pt */
		else
		{
			Node* uncle_pt = grand_parent_pt->left;

			/*  Case : 1
				The uncle of pt is also red
				Only Recoloring required */
			if ((uncle_pt != NULL) && (uncle_pt->color == RED))
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}
			else
			{
				/* Case : 2
				   pt is left child of its parent
				   Right-rotation required */
				if (pt == parent_pt->left)
				{
					rotateRight(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				   pt is right child of its parent
				   Left-rotation required */
				rotateLeft(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}
	}

	root->color = BLACK;
}

// Function to insert a new node with given data 
void RBTree::insert(const int& data)
{
	Node* pt = new Node(data);

	// Do a normal BST insert 
	root = BSTInsert(root, pt);

	// fix Red Black Tree violations 
	fixViolation(root, pt);
}

// Function to do inorder and level order traversals 
void RBTree::inorder() { inorderHelper(root); }
void RBTree::levelOrder() { levelOrderHelper(root); }






int main()
{
	cout << "Commands: " << endl;
	cout << "+ key : Insert(or update) element" << endl;
	cout << "? key : Retrieve element" << endl;
	cout << "- key : Remove element" << endl;
	cout << "Q     : Quit the test program" << endl;
	bool flag = true;

	RBTree tree;
	while (flag)
	{
		string input;
		cout << "Command: ";
		cin >> input;
		char command = input.c_str()[0];	// +, -, ?, Q 선택
		int key;							// key값
		if (input.size() > 1)
			key = stoi(input.substr(1, input.length()));


		switch (command)
		{
		case '+':
			cout << "insert : key = " << key << endl;
			tree.insert(key);
			tree.print2D(0);

			break;
		
	
		case 'Q':
			cout << "Quit" << endl;
			flag = false;

		}
	}


}