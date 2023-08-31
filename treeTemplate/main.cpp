#include <iostream>
#include <string>

#define MAX_LENGTH  60

using namespace std;

/**
 * @brief Node definition in the tree 
 *
 * This structure defines the Nodes: the contents and the 
 * pointers to the left and right elements
 * 
 * Comparing two different Nodes is done over the field index.
 * Therefore implicitly the class will have those operators
 * overloaded.
 */
template <class I, class S>
class NodeCell {
public:
	I 	index;		// index class I: operators <>= needed
	S	jointName; 	// as class S
	NodeCell* dx;		// a link to the right element, 
	NodeCell* sx;		// a link to the left element.

	// default constructor
	NodeCell () {index = 0; sx = dx = NULL;}   
	// initialization constructor 
	NodeCell (I i, S s):index(i), jointName(s) {dx=sx=NULL;}
	// copy constructor
	NodeCell (const NodeCell& n):
		index(n.index),
		jointName(n.jointName)
		{ sx = n.sx; dx = n.dx; }
	// destructor: It releases all resources linked to it! 
	// set sx and dx to NULL if you do not want to remove 
        // the rest of the nodes
	~NodeCell() {
	   	if(sx)           
      			delete sx;
      		if(dx)
      			delete dx;                    
      
   	  	index.~I();       
   	  	jointName.~S();
	}
	
	NodeCell& changeIndex (I i) { index = i; return *this; }
	bool operator< (const NodeCell &n) { return (index < n.index); }
	bool operator<= (const NodeCell &n) { return (index <= n.index); }
	bool operator== (const NodeCell &n) { return (index == n.index); }
	bool operator!= (const NodeCell &n) { return (index != n.index); }
	bool operator> (const NodeCell &n) { return (index > n.index); }
	NodeCell& operator= (const NodeCell &n) { 
		index = n.index; 
		jointName = n.jointName;
		sx = n.sx; dx = n.dx;  
		return *this;			}

	// overload of the << operator for printing an element
	friend ostream& operator<< (ostream& os, NodeCell<I,S>& t) {	
		os << "\n";
		os << "\tindex: " << t.index;
		os << "\tJoint Name: " << t.jointName;
		os << "\tleft: " << t.sx;
		if (t.sx)
                   os << "->" << t.sx->index;
		os << "\tright: " << t.dx;
                if (t.dx)
                   os << "->" << t.dx->index;
                os << endl;
		return os; 	
	}
};
		 	

/**
 * @brief class for the binary tree data structure 
 *
 * This class will have a pointer to the tree, root, as 
 * private member of the class. 
 * The methods are the usual needed for insertion, deletion
 * and print of the tree on an ostream. 
 * Other service routines have been defined as private as they 
 * serve the main class method interface
 */
template <class I, class S>
class TreeClass {

private:
	NodeCell<I,S>  *root;	// point to the root element

  	// navigate inorderly and print nodes
	void traverse (ostream&, NodeCell<I,S>* );
        // service funtion to find the minimum in a branch tree
	NodeCell<I,S>* minimum(NodeCell<I,S>*);
	// service funtion to find the maximum in a branch tree
	NodeCell<I,S>* maximum(NodeCell<I,S>* );			
	// service functions for relink during deletion
	void relinkLeft(NodeCell<I,S> &, NodeCell<I,S>* );
	void relinkRight(NodeCell<I,S> &, NodeCell<I,S>* );
	void relinkRoot(NodeCell<I,S> &, NodeCell<I,S>* );
	// service function to delete a node
	void deletenode (NodeCell<I,S>&, NodeCell<I,S>* );
        // service function to search for a node	
	NodeCell<I,S>* searchNode (NodeCell<I,S>&, NodeCell<I,S>* );				

public:
	// default contructor
        TreeClass() { root = NULL; }
        // Constuctor with initialization
        TreeClass(I setup, S st) {
        	root = new NodeCell<I,S> (setup, st);
        	root->sx=root->dx=NULL;
        }
        TreeClass(const NodeCell<I,S> &t) {
        	root = new NodeCell<I,S> (t);
        	root->sx=root->dx=NULL;
        }
        // default destructor
        ~TreeClass() { delete (root); }
        
 	bool insertNew (NodeCell<I,S>&);	 
 	bool delNode (NodeCell<I,S>&);	
 	NodeCell<I,S>* searchItem (NodeCell<I,S>&);

 	// overload of the << operator
 	friend ostream& operator<< (ostream& os, TreeClass& t) {
 		NodeCell<I,S> *p=t.root;
	
		os << "\nTREE PRINTOUT - root: " << p;
		if (p)
                   os << "->" << p->index;
		os << endl;
		os << "\n=============\n";
		
		t.traverse (os, p);
		return os; 	
 	}
};

template <class I, class S>
void TreeClass<I,S>::traverse (ostream& os, NodeCell<I,S> *p) {
	if (p == NULL)
		return;

	traverse(os, p->sx);
	os << *p;
	traverse(os, p->dx);
} 

/**
 * @brief Insert one element in the tree
 *
 * This function inserts one node in the tree, in an inorder manner.
 *
 * @param the node to be included
 * @return diagnastic if the insertion was succesfull (true)
 */
template <class I, class S>
bool TreeClass<I,S>::insertNew (NodeCell<I,S> &n) {
	NodeCell<I,S> *t;
	bool retVal = false;

	cout << "Inserting Node :" << n << endl;
		
	if (root == NULL) {
		root = new NodeCell<I,S> (n);		// use the copy contructor
		return true;
	}

	t = new NodeCell<I,S> (n); 			// use the copy contructor 
	if (t == NULL)
		return retVal;  

	NodeCell<I,S> *p = root;		
	NodeCell<I,S> *q;			

	while (p) {	
		// have we found the right position?
		if (n <= *p) {
			q=p;
			p=p->sx;
		} else { 
			q=p;
			p=p->dx;
		}
	}	
	
	// add the node as the last element
	if (n <= *q) 
		q->sx = t;
	else 
		q->dx = t;	

	return true;
}

bool notFound = true;

template <class I, class S>
NodeCell<I,S> * TreeClass<I,S>::searchNode (NodeCell<I,S>& n, NodeCell<I,S>* p) {

	if ((p == NULL) || (n == *p) )
		return p;
			
	if (n < *p)
		return searchNode (n, p->sx);
	return searchNode (n, p->dx);
}


/**
 * @brief search of a given node in the tree
 *
 * This function searches one node in the tree,
 * according to the index value.
 *
 * @param the node to be searched
 * @return a node pointer the element or NULL is not found
 */
template <class I, class S>
NodeCell<I,S>* TreeClass<I,S>::searchItem (NodeCell<I,S> & n) {
	NodeCell<I,S> *p = NULL;

	cout << "Searching for node: " << n;
	
	p = searchNode (n, root);
	
	if (p)
		cout << "Found elemet " << *p << endl;
	
	return p;
}

/**
 * @brief Returns the minimum in a given tree branch
 *
 * This function searches the minimum of a given branch in the tree,
 * according to the index value. Assumption is that nodes belongs to 
 * the tree
 *
 * @param the branch tree to start the search from
 * @return a node pointer the element or NULL if not found
 */
template <class I, class S>
NodeCell<I,S>* TreeClass<I,S>::minimum(NodeCell<I,S>* p) {
        NodeCell<I,S>* current = p;
        if (current == NULL)
        	return current;
        	
        while (current->sx != NULL) {
            current = current->sx;
        }
        return current;
    }

/**
 * @brief Returns the maximum in a given tree branch
 *
 * This function searches the minimum of a given branch in the tree,
 * according to the index value. Assumption is that nodes belongs to 
 * the tree
 *
 * @param the branch tree to start the search from
 * @return a node pointer the element or NULL if not found
 */

template <class I, class S>
NodeCell<I,S>* TreeClass<I,S>::maximum(NodeCell<I,S>* p) {
        NodeCell<I,S>* current = p;
        if (current == NULL)
        	return current;

        while (current->dx != NULL) {
            current = current->dx;
        }
        return current;
    }

/**
 * @brief Support routine for the delNode() method
 * 
 * This function relinks the pointers in the tree, once the deletion 
 * node has been identified. It does also call the delete function 
 * for the node itself, after it has been isolated from the tree.
 * This method is advisable, especially when the node to be removed
 * is not a leaf.
 *
 * Here are the six steps performed in the example of a right branch
 *
 *	     500 ----+                  1.  Relink Anchestor's node 
 * morituri    \     |  1.                   to chil's node
 *   Node  --> 600   |                  2.  Remove (to NULL) Node's dx
 *           5./ \2. /                  3.  Relink child's node sx
 *	     550---650                        to Node's sx
 *	     / \ 3. / \                  4.  Relink Maximum in 
 *        548 552  648 652                    Node's sx branch to 
 *              |   |                         Minimum in Node's dx branch 
 *              +---+                    5.  Remove (to NULL) Node's sx
 *                4.                     6.  Now Node is fully isolated
 *                                            if is safe to Delete Node
 * After these six operations the tree will look like:
 *     
 *     500                 500
 *       \                   \
 *       600        -->      650
 *       / \        -->      / \
 *    550   650     -->   550   652
 *    / \    / \          / \
 *  548 552 648 652     540  552
 *                            \
 *                             648
 *
 * @param the node to be removed (the _morituri_ node)
 * @param the branch tree beneath the morituri node
 * @return N/A
 */
template <class I, class S>
void TreeClass<I,S>::relinkLeft(NodeCell<I,S> &n, NodeCell<I,S>* p) {
	NodeCell<I,S>* t;
	
        // we are working on the right branch of p
	if ( n != *(p->dx) )
		return;

        t = p->dx;	// pointing to the node to remove
        p->dx = p->dx->dx;		// 1st step
        NodeCell<I,S>* temp;
        temp = minimum(t->dx);          
        t->dx = NULL;			// 2nd step 
        if (p->dx != NULL)		
        	p->dx->sx = t->sx;	// 3th step
        NodeCell<I,S>* k = maximum(t->sx);
        if (k != NULL) 
        	k->dx = temp;		// 4th step
        t->sx = NULL;			// 5th step
        delete (t);			// 6th step 
}


template <class I, class S>
void TreeClass<I,S>::relinkRight(NodeCell<I,S> &n, NodeCell<I,S>* p) {
        NodeCell<I,S>* t;

	if ( n != *(p->sx) )
		return;

       	t = p->sx;	// pointing to the node to remove
        p->sx = p->sx->sx;		// 1st step
        // before removing the link in 2nd step,
        // let's calculate the maximum for later in step 4
        NodeCell<I,S>* k = maximum(t->sx);  
        t->sx = NULL;			// 2nd step 
        if (p->sx != NULL)		
        	p->sx->dx = t->dx;	// 3th step
	NodeCell<I,S>* temp = minimum(t->dx);          
        if (k != NULL) 
        	k->dx = temp;		// 4th step
        t->dx = NULL;			// 5th step
        delete (t);			// 6th step 
}

template <class I, class S>
void TreeClass<I,S>::relinkRoot(NodeCell<I,S> &n, NodeCell<I,S>* p) {
        NodeCell<I,S>* t;
        
        if ( n != *p )
        	return;
        
       	t = root;		// pointing to the node to remove
        root = root->sx;		// 1st step
        NodeCell<I,S>* k = maximum(t->sx);  
        t->sx = NULL;			// 2nd step 
        if (k != NULL) 
        	k->dx = t->dx;		// 3rd step
        t->dx = NULL;			// 4th step
        delete (t);			// 5th step 
}

/**
 * @brief Support routine for the delNode() method
 * 
 * This function identifies the node to be removed, first, by using a recursion 
 * method. Then it calls the relinkLeft/Right() to relink the child nodes 
 * Beneath the morituri node. This is of relevance especially if the morituri node
 * is not a leaf. The actual delete funtion is called in the relinkLeft/Right()   
 *
 * @param the node to be removed (the morituri node)
 * @param the branch tree beneath the morituri node
 * @return N/A
 */
template <class I, class S>
void TreeClass<I,S>::deletenode (NodeCell<I,S>& n, NodeCell<I,S> *p) {
      
      if (!notFound) {
        if (n == *p) {
        	relinkRoot(n,p);
        } else if (n == *(p->sx) ) {
		relinkRight(n, p);
        } else if (n == *(p->dx)) {
		relinkLeft(n, p);
        } else {
	        cout << "pit fall\n";
        }
      
      } else {        
        if (p == NULL) 
            return;
        else if (n == *p) {
        	cout << "Removing root \n";
		notFound = false;
       		deletenode(n,p);        
        }
        else if ((p->sx) && (n < *(p->sx)))
        	deletenode(n,p->sx->sx);
        else if ((p->dx) && (n > *(p->dx)))
        	deletenode(n,p->dx->dx);
        else if ( (n == *(p->sx)) || (n == *(p->dx)) ) {
        		notFound = false;
        		deletenode(n,p);
              }
        }
}

/**
 * @brief Delete one element from the tree
 *
 * This function deletes one node in the tree,
 * according to the index value.
 *
 * @param the node to be deleted
 * @return diagnastic if the deletion was succesfull (true)
 */
template <class I, class S>
bool TreeClass<I,S>::delNode (NodeCell<I,S> &n) {
	NodeCell<I,S>* p;
	
	p = searchNode (n, root);
	
	if (p == NULL) {
		cout << " Node to be deleted not found :" << n << endl;
		return false;
	}
	cout << "Deleting Node :" << *p << endl;
		
	notFound = true;	
	deletenode (n,root);
	return true;
} 

int main() {
	cout << "Let's start with the Program Exection \n\n";

/////////// Creation of the Binary Tree	
	TreeClass<int,string> t(500,"firstJoint");
	
	cout << "And this is the first class outcome :" << t <<"\n";	

/////////// Insertion of new Nodes - Test Case
	NodeCell<int,string> n(0, "Joint Name");

        // Let's populate the left branch	
	if ( !(t.insertNew(n.changeIndex(300))) )
		return (1);   // failed test 

	if ( !(t.insertNew(n.changeIndex(200))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(118))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(202))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(350))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(348))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(352))) )
		return (1);   // failed test 
	
	// ... and now the right branch
	if ( !(t.insertNew (n.changeIndex(600))) )
		return (1);   // failed test 

	if ( !(t.insertNew(n.changeIndex(550))) )
		return (1);   // failed test 

	if ( !(t.insertNew(n.changeIndex(548))) )
		return (1);   // failed test 

	if ( !(t.insertNew(n.changeIndex(552))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(650))) )
		return (1);   // failed test 
	
	if ( !(t.insertNew(n.changeIndex(652))) )
		return (1);   // failed test 

	if ( !(t.insertNew(n.changeIndex(648))) )
		return (1);   // failed test 
		
	cout << "Added new nodes:" << t << "\n";

/////////// Search and Deletion of old Nodes - Test Case
        // test removing one node at the leaf
	if ( !(t.delNode(n.changeIndex(652))) )
			return (1);   // failed test 

	// test removing one node in the middle of the 
	// right branch - using relinkLeft()
	if ( !(t.delNode(n.changeIndex(600))) )
			return (1);   // failed test 

	// test removing one node in the middle of the 
	// left branch - using relinkRight()
	if ( !(t.delNode(n.changeIndex(300))) )
			return (1);   // failed test 

        // test removing the root - using relinkRoot()
	if ( !(t.delNode(n.changeIndex(500))) )
			return (1);   // failed test 

	cout << "Deleted old nodes:" << t << "\n";

/////////// Program Closure - RegTest passed
	return (0);   // test passed
}
