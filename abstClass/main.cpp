#include <iostream>
#include <string>
#include "libUsers.hpp"


using namespace std;


// index array used to tree node insertion
int index_array [] =
 {
   500, 300, 200, 118, 202, 350, 348, 352, 600, 550, 548, 552, 650, 648, 652
   // 348, 352, 600, 550, 548, 552, 650, 652
 };

typedef LibAccess* pS;

// this is the "supposedly" data base of university staff records
// typically in a database
pS staffRec [sizeof(index_array)/sizeof(int)];

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
        S       user; 	        // as class S
	NodeCell* dx;		// a link to the right element, 
	NodeCell* sx;		// a link to the left element.

	// default constructor
	NodeCell () {index = 0; user = sx = dx = NULL;}   
	// initialisation constructor 
	NodeCell (I i, S s):index(i), user(s) {dx=sx=NULL;}
	NodeCell (I i, S *s):index(i), user(*s) {dx=sx=NULL;}
	// copy constructor
	NodeCell (const NodeCell& n):
	  index(n.index),
	  user(n.user)  { sx = n.sx; dx = n.dx; }
	// destruct: It releases all resources linked to it! 
	// set sx and dx to NULL if you do not want to remove 
        // the rest of the nodes
	~NodeCell() {
	   	if(sx)           
      			delete sx;
      		if(dx)
      			delete dx;                    
      
   	  	index.~I();
		user.~S();
	}
	
	NodeCell& changeIndex (I i) { index = i; return *this; }
	bool operator< (const NodeCell &n) { return (index < n.index); }
	bool operator<= (const NodeCell &n) { return (index <= n.index); }
	bool operator== (const NodeCell &n) { return (index == n.index); }
	bool operator!= (const NodeCell &n) { return (index != n.index); }
	bool operator> (const NodeCell &n) { return (index > n.index); }
	NodeCell& operator= (const NodeCell &n) { 
		index = n.index; 
		user = n.user; 
		return *this;
	}

	// overload of the << operator for printing an element
	friend ostream& operator<< (ostream& os, NodeCell<I,S>& t) {	
	  os << "\n";
	  os << "\tindex: " << t.index;
	  os << "  " << *(t.user);  // S is a pointer to abstract class.
	                            // we want to print the class info
	                            // either Student or Professor.
	                            // Hence *(S)
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
        // service function to find the minimum in a branch tree
	NodeCell<I,S>* minimum(NodeCell<I,S>*);
	// service function to find the maximum in a branch tree
	NodeCell<I,S>* maximum(NodeCell<I,S>* );			
        // service routine for including a new node 
        NodeCell<I,S>* insertnode(NodeCell<I,S>&,NodeCell<I,S>* &);
        // service function to delete a node
	void deletenode (NodeCell<I,S>&, NodeCell<I,S>* &);
        // service function to search for a node	
	NodeCell<I,S>* searchNode (NodeCell<I,S>&, NodeCell<I,S>* );				

public:
	// default constructor
        TreeClass() { root = NULL; }
        // Constructor with initialisation
        TreeClass(I setup, S st) {
        	root = new NodeCell<I,S> (setup, st);
        	root->sx=root->dx=NULL;
        }
        TreeClass(I setup, S *st) {
        	root = new NodeCell<I,S> (setup, st);
        	root->sx=root->dx=NULL;
        }
        TreeClass(const NodeCell<I,S> &t) {
        	root = new NodeCell<I,S> (t);
        	root->sx=root->dx=NULL;
        }
        // default destruct
        ~TreeClass() { delete (root); }
        
 	bool insertNew (NodeCell<I,S>&);	 
 	bool delNode (NodeCell<I,S>&);	
 	NodeCell<I,S>* searchItem (NodeCell<I,S>&);

 	// overload of the << operator
 	friend ostream& operator<< (ostream& os, TreeClass& t) {
	  os << "\nTREE PRINTOUT - root: " << t.root;
	  if (t.root)
	    os << "->" << t.root->index;
	  os << endl;
	  os << "\n=============\n";
	  t.traverse (os, t.root);
	  return os; 	
 	}
};

/**
 * @brief service routine function traverse
 *
 * Used in the tree print out to traverse the tree and
 * print the node associated with. It uses recursion.
 * @param ostream to print to
 * @param pointer to the root/branch to print from
 * @return N/A
 */
template <class I, class S>
void TreeClass<I,S>::traverse (ostream& os, NodeCell<I,S> *p) {
	if (p == NULL)
		return;

	traverse(os, p->sx);
	os << *p;
	traverse(os, p->dx);
} 

/**
 * @brief service routine function insertnode
 *
 * Used in the new node tree insertion to traverse the tree and
 * insert the node at the leaf level. It uses recursion.
 * @param node to insert
 * @param pointer to the root/branch to insert the node into
 * @return pointer to the node inserted
 */template <class I, class S>
NodeCell<I,S>* TreeClass<I,S>::insertnode(NodeCell<I,S>& n,NodeCell<I,S>* & p) {
  if (p == NULL) {
    p = new NodeCell(n);
    return p;
  }
  
  if (*p < n)
    p->dx = insertnode(n, p->dx);
  else if (*p > n)
    p->sx = insertnode(n, p->sx);
  
  return p;
}

/**
 * @brief Insert one element in the tree
 *
 * This function inserts one node in the tree, in an in-order manner.
 *
 * @param the node to be included
 * @return diagnostic if the insertion was successful (true)
 */
template <class I, class S>
bool TreeClass<I,S>::insertNew (NodeCell<I,S> &n) {
        cout << "Inserting Node :" << n << endl;
		
        insertnode (n, root);
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
		cout << "Found element " << *p << endl;
	
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
 * This function identifies the node to be removed, first, by using a recursion 
 * method. We have to make sure the deletion procedure is succesfull in these 
 * three scenarios:
 *
 *  1) Deletion of a node in leaf position (relarevely straight forward)
 *  2) Deletion of a node in a branch 
 *  3) Deletion of the root
 *
 * The first case is relatevely simple. The third one is a subset of the 
 * second one. So let's see in the details the case 2). The trick there is 
 * to find the minimum on the right branch in respect of the _morituri_ 
 * node. That minimum will have, necesarly the left link to NULL. Then the 
 * we copy that minimum over to the _moritury_ node, reserving the original
 * the links on the left and on the right. This function is solved by the 
 * overloaf of the operator= in the NodeCell, in which we copy only the index
 * field, but not the pointers _sx_ and _dx_. See Step 1. a). Then the relink
 * of the right branch bypassing the _old_ minimum. Setp 1. b) 
 * Here is an example: deleting node 2
 *  
 *                           Step 1. a) & b)          Step 2.
 *
 *       6                        6                     6
 *     /   \                    /   \                 /   \
 *    2     8     ===>         3     8   ===>        3     8
 *  /   \         ===>       /  \        ===>      /  \
 * 1     5        ===>      1    5       ===>     1    5
 *      /         ===>         / |       ===>           \
 *     3                      3  |                       4
 *      \                      \ |
 *       4                       4
 *
 * And the last step is to delete the node 3 in the old position.
 * See Step 2. in the grapfics 
 *
 * @param the node to be removed (the morituri node)
 * @param the branch tree beneath the morituri node
 * @return N/A
 */
template <class I, class S>
void TreeClass<I,S>::deletenode (NodeCell<I,S>& n, NodeCell<I,S> * & p) {

  if(p == NULL)
    return;  // item not found

  if (n < *p)
    deletenode(n, p->sx);
  else if (n > *p)
    deletenode(n, p->dx);
  else if (p->sx != NULL && p->dx != NULL) { // two children
    // Step 1. a)
    *p = *(minimum (p->dx));   // use operator= in NodeCell
    deletenode (*p, p->dx);
  } else {
    NodeCell<I,S> *oldnode = p;
    // Step 1. b)
    p = (p->sx != NULL) ? p->sx : p->dx;
    // Step 2.
    delete (oldnode);
  }

}

/**
 * @brief Delete one element from the tree
 *
 * This function deletes one node in the tree,
 * according to the index value.
 *
 * @param the node to be deleted
 * @return diagnostic if the deletion was successful (true)
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

/**
 * Use Case:
 * =========
 *
 * University Data Centre:                   University Library:
 * ------------------------                  ------------------- 
 *  Data Base of                               Repository of who can access
 *  staffs and Students                        the University Library
 *                            <LibAccess> 
 *      staffRev []              ==>              TreeClass<indez, LibAccess>
 ^                               ==>
 *  (in this example an                       (Records modification on binary
 *   array data structure                     tree does not imply changes to 
 *   is use, but in reality                    the Uni DB Data Centre)
 *   it could well be a DB)
 */
int main() {
	cout << "Let's start with the Program Execution \n\n";

/////////// Creation of the Binary Tree	
        staffRec[0] = (LibAccess*) new Prof ("cicero");
        NodeCell<int, pS> p (0, staffRec);
	
	TreeClass<int, pS> t(500, staffRec);

	cout << "And this is how the tree looks like :" << t <<"\n";
/////////// Insertion of new Nodes - Test Case

	for (int i=1; i < (sizeof(index_array)/sizeof(int)); i++)
	  {
	    // I am also building the staff Records
	    staffRec[i] = (LibAccess*) new Student ("pinco pallo");

	    // ... only here is pertinent for the Uni Library
	    NodeCell<int, pS> n (
				   index_array[i],
				   &staffRec[i]);
	    if ( !(t.insertNew(n)) )
	      return (1);   // failed test
	  }
	
	cout << "Added new nodes:" << t << "\n";

/////////// Search and Deletion of old Nodes - Test Case
        // test removing one node at the leaf
	Student s2 ("pinco pallo");

	NodeCell<int, pS> d (
			    652,
			    (pS) &s2);
	if ( !(t.delNode(d)) )
	  return (1);   // failed test 

	// test removing one node in the middle of the 
	// right branch - using relinkLeft()
	if ( !(t.delNode(d.changeIndex(600))) )
	  return (1);   // failed test 

	// test removing one node in the middle of the 
	// left branch - using relinkRight()
	if ( !(t.delNode(d.changeIndex(300))) )
	  return (1);   // failed test 

        // test removing the root - using relinkRoot()
	if ( !(t.delNode(d.changeIndex(500))) )
	  return (1);   // failed test 

        cout << "Deleted old nodes:" << t << "\n";
	// NOTE: as the tree belong to a separate entity
	//       it is wrong to remove the record
	//       also from the staff Record (i.e. staffRec[]
	//       array, in this example), even if we might
	//       have had access in this example.
	//       In fact, only the node in the binary tree
	//       has been removed in here  
/////////// Program Closure - RegTest passed
	return (0);   // test passed
}
