#include <iostream>
#include <string>
#include "libUsers.hpp"


using namespace std;


// index array used to tree node insertion
int index_array [] =
 {
   500, 300, 200, 118, 202, 350, 348, 352, 600, 550, 548, 552, 650, 648, 652
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
	// destructor: It releases all resources linked to it! 
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
		sx = n.sx; dx = n.dx;  
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
	// service functions for re-link during deletion
	void relinkLeft(NodeCell<I,S> &, NodeCell<I,S>* );
	void relinkRight(NodeCell<I,S> &, NodeCell<I,S>* );
	void relinkRoot(NodeCell<I,S> &, NodeCell<I,S>* );
        // service routine for including a new node 
        NodeCell<I,S>* insertnode(NodeCell<I,S>&,NodeCell<I,S>*);
        // service function to delete a node
	void deletenode (NodeCell<I,S>&, NodeCell<I,S>* );
        // service function to search for a node	
	NodeCell<I,S>* searchNode (NodeCell<I,S>&, NodeCell<I,S>* );				

public:
	// default contructor
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
        // default destructor
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
NodeCell<I,S>* TreeClass<I,S>::insertnode(NodeCell<I,S>& n,NodeCell<I,S>* p) {
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
 * This function inserts one node in the tree, in an inorder manner.
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
 * This function re-links the pointers in the tree, once the deletion 
 * node has been identified. It does also call the delete function 
 * for the node itself, after it has been isolated from the tree.
 * This method is advisable, especially when the node to be removed
 * is not a leaf.
 *
 * Here are the six steps performed in the example of a right branch
 *
 *	     500 ----+                  1.  Re-link Ancestor's node 
 * morituri    \     |  1.                   to child's node
 *   Node  --> 600   |                  2.  Remove (to NULL) Node's dx
 *           5./ \2. /                  3.  Re-link child's node sx
 *	     550---650                        to Node's sx
 *	     / \ 3. / \                  4.  Re-link Maximum in 
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
        	p->dx->sx = t->sx;	// 3rd step
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
        	p->sx->dx = t->dx;	// 3rd step
	NodeCell<I,S>* temp = minimum(t->dx);          
        if (k != NULL) 
        	k->dx = temp;		// 4th step
        t->dx = NULL;			// 5th step
        delete (t);			// 6th step 
}

/**
 * @brief Support routine for the delNode() method
 * 
 * This function re-links the pointers in the tree, once the deletion 
 * node has been identified in the root. It does also call the delete 
 * function for the root node itself, after it has been isolated from 
 * the tree and a new root has been identified
 *
 * Here are the five steps performed in the example of a right branch
 *      root ---------+ 
 *	 \            50                1. Re-link root node to 
 *        \      3./     \4.                child's node
 *    1.   \_    30  2.__80             2. Relink maximum on sx
 *           \  /  \/    / \                branch to (ex-)root->dx
 *	      20  40   60  90           3. Remove ex-root sx   
 *	                \                    sx = NULL
 *                       70             4. Remove ex-root dx 
 *                                           dx = NULL 
 *                                      5. delete ex-root
 * After these six operations the tree will look like:
 *     
 *        50                      30
 *       /   \                   /   \
 *     30     80    -->         20    40
 *    / \    / \    -->                \
 *   20 40  60 90   -->                80
 *            \                       /  \
 *            70                     60  90
 *                                    \
 *                                     70
 *
 * @param the node to be removed (the _morituri_ node)
 * @param the branch tree beneath the morituri node
 * @return N/A
 */
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
 * method. Then it calls the relinkLeft/Right() to re-link the child nodes 
 * Beneath the morituri node. This is of relevance especially if the morituri node
 * is not a leaf. The actual delete function is called in the relinkLeft/Right()   
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
	    // I am also building the staff Recrds
	    staffRec[i] = (LibAccess*) new Student ("pinco pallo");

	    // ... only here is pertinet for the Uni Lirary
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
