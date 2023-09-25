#include <iostream>
#include <ctime>
#include <sstream>

#include "libUsers.hpp"
#include "studClass.hpp"
#include "profClass.hpp"
#include "listTemp.hpp"

using namespace std;


// index array used to tree node insertion.
int index_array [] =
 {
   500, 300, 200, 118, 202, 350, 348,
   600, 550, 548, 552, 650, 648, 352, 652
 };

typedef LibAccess* pS;

// this is the "supposedly" data base of university staff records
// typically in a database
pS staffRec [sizeof(index_array)/sizeof(int)];

// Here is how the tree looks like in this example
//              ___ 500___
//             /          \
//          300            600
//         /   \          /    \
//        200   350      550    650
//       /  \   /  \     /  \   /  \
//     118 202 348 352 548 552 648 652

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
private:
        size_t seed;            // Symmetric Key passed for HMAC
public:
	I 	index;		// index class I: operators <>= needed
        S       user; 	        // as class S
	NodeCell* dx;		// a link to the right element, 
	NodeCell* sx;		// a link to the left element.

	// default constructor
        NodeCell () {
	  seed = 0; index = 0; user = sx = dx = NULL;
	}   
	// initialisation constructor 
        NodeCell (size_t sd, I i, S s):seed(sd),
				       index(i),
				       user(s) {
	  dx = sx = NULL;
	}
        NodeCell (size_t sd, I i, S *s): seed(sd),
					 index(i),
					 user(*s) {
	  dx = sx = NULL;
	}
	// copy constructor
        NodeCell (size_t sd, const NodeCell& n):seed(sd),
						index(n.index),
						user(n.user)  {
	  sx = n.sx; dx = n.dx;
	}
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
	        seed = n.seed;
	        index = n.index; 
		user = n.user; 
		return *this;
	}
        void commissionNode(size_t k) {
	  user->commissionClass(k);
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
class HTreeClass {

private:
	NodeCell<I,S>  *root;	// point to the root element
        // to save the history of the hashes we use the list

        ListClass<clock_t, size_t> rHash;

        // Symmetric Key for the HMAC
        size_t key; 

  	// NAVIGATE INORDERLY AND PRINT NODES
	void traverse (ostream&, NodeCell<I,S>* );
        // SERVICE FUNCTION TO FIND THE MINIMUM IN A BRANCH TREE
	NodeCell<I,S>* minimum(NodeCell<I,S>*);
	// SERVICE FUNCTION TO FIND THE MAXIMUM IN A BRANCH TREE
	NodeCell<I,S>* maximum(NodeCell<I,S>* );			
        // SERVICE ROUTINE FOR INCLUDING A NEW NODE 
        NodeCell<I,S>* insertnode(NodeCell<I,S>&,NodeCell<I,S>*&);
        // SERVICE FUNCTION TO DELETE A NODE
        void deletenode (NodeCell<I,S>&, NodeCell<I,S>* &);
        // SERVICE FUNCTION TO SEARCH FOR A NODE	
	NodeCell<I,S>*& searchNode (NodeCell<I,S>&, NodeCell<I,S>*&);

        // return seed to initialise nodes and HMAC
        size_t getKey() { return key; }

public:
	// default constructor
        HTreeClass(): root(NULL), rHash() {
	  // initialise PRNG for key generation
	  srand(time(0));
	  // use PRNG to extract a pseudo-random value
	  // for our symmetric key 
	  key = rand();
	}
        // Constructor with initialisation
        HTreeClass(I setup, S st):
          root(new NodeCell<I,S>(setup, st)),
	  rHash(clock(), root->user->getHash()) {
      	  root->sx=root->dx=NULL;
	  // initialise PRNG for key generation
	  srand(time(0));
	  // use PRNG to extract a pseudo-random value
	  // for our symmetric key 
	  key = rand();
        }
        HTreeClass(I setup, S *st):
	  root(new NodeCell<I,S>(setup, st)),
	  rHash(clock(), root->user->getHash()) {
	  root->sx=root->dx=NULL;
	  // initialise PRNG for key generation
	  srand(time(0));
	  // use PRNG to extract a pseudo-random value
	  key = rand();
        }
        HTreeClass(const NodeCell<I,S> &t):
	  root(new NodeCell<I,S>(t)),
	  rHash(clock(), root->user->getHash()) {
	  root = new NodeCell<I,S> (t);
	  root->sx=root->dx=NULL;
	  // initialise PRNG for key generation
	  srand(time(0));
	  // use PRNG to extract a pseudo-random value
	  // for our symmetric key 
	  key = rand();
        }
        // default destructor
        ~HTreeClass() {
	  delete (root);
	}
        
 	bool insertNew (NodeCell<I,S>&);	 
 	bool delNode (NodeCell<I,S>&);	
 	NodeCell<I,S>* searchItem (NodeCell<I,S>&);
       
       // Computes the hash based on the children. If no right child
       // exists, reuse the left child's value
       size_t computeHash(size_t buffer, NodeCell<I,S> *r) {
	 if (r == NULL)
	   return 0;

	 const size_t leftHash = computeHash(buffer, r->sx);
         const size_t rightHash = ( computeHash(buffer, r->dx) ? \
	 			    computeHash(buffer, r->dx) : \
	 			    leftHash );

 	 buffer = leftHash + rightHash;
	 
	 if (r != NULL && buffer == 0)
	   // case of tree only with root
	   buffer = r->user->getHash();

	 return hash<size_t>{} (buffer);
       }

       // calculate the tree fingerprint as the Hash value
       // at the root
       size_t fingerPrint() {
	 size_t buffer;
	 return computeHash(buffer, root);
       }

       // compere the tree's finger print at any point in
       // time with the last fingerprint stored in the list
       bool integrityCheck() {
	 return ( fingerPrint() ==
		   (rHash.findLast()->hashValue) );
	}
  
       NodeCell<I,S>* commissionObject (I i, pS &obj) {
	 NodeCell<I,S> *p= new NodeCell<I,S> (getKey(), i, obj);
	 p->commissionNode(getKey());
	 return p;
       }
  
 	// overload of the << operator
 	friend ostream& operator<< (ostream& os, HTreeClass& t) {
	  os << "\nTREE PRINTOUT - root: " << t.root;
	  if (t.root)
	    os << "->" << t.root->index;
	  os << "\nHMAC's key: " << t.key;
	  os <<  t.rHash << endl;
	  // not interested in the nodes printout at this point
	  //t.traverse (os, t.root);  
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
void HTreeClass<I,S>::traverse (ostream& os, NodeCell<I,S> *p) {
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
NodeCell<I,S>* HTreeClass<I,S>::insertnode(NodeCell<I,S>& n,NodeCell<I,S>* & p) {
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
bool HTreeClass<I,S>::insertNew (NodeCell<I,S> &n) {
        cout << "Inserting Node :" << n << endl;
		
        insertnode (n, root);
	
	size_t buffer;
	HashCell<time_t,size_t> tmp (clock(),
				     computeHash (buffer, root));
	return rHash.insertNew(tmp);
}

template <class I, class S>
NodeCell<I,S>*& HTreeClass<I,S>::searchNode (NodeCell<I,S>& n, NodeCell<I,S>* &p) {
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
NodeCell<I,S>* HTreeClass<I,S>::searchItem (NodeCell<I,S> & n) {
	cout << "Searching for node: " << n;
	return searchNode (n, root);
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
NodeCell<I,S>* HTreeClass<I,S>::minimum(NodeCell<I,S>* p) {
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
NodeCell<I,S>* HTreeClass<I,S>::maximum(NodeCell<I,S>* p) {
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
 * method. We have to make sure the deletion procedure is successful in these 
 * three scenarios:
 *
 *  1) Deletion of a node in leaf position (relatively straight forward)
 *  2) Deletion of a node in a branch 
 *  3) Deletion of the root
 *
 * The first case is relatively simple. The third one is a subset of the 
 * second one. So let's see in the details the case 2). The trick there is 
 * to find the minimum on the right branch in respect of the _morituri_ 
 * node. That minimum will have, necessarily the left link to NULL. Then the 
 * we copy that minimum over to the _moritury_ node, reserving the original
 * the links on the left and on the right. This function is solved by the 
 * overload of the operator= in the NodeCell, in which we copy only the index
 * field, but not the pointers _sx_ and _dx_. See Step 1. a). Then the relink
 * of the right branch bypassing the _old_ minimum. Step 1. b) 
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
 * See Step 2. in the graphics 
 *
 * @param the node to be removed (the morituri node)
 * @param the branch tree beneath the morituri node
 * @return N/A
 */
template <class I, class S>
void HTreeClass<I,S>::deletenode (NodeCell<I,S>& n, NodeCell<I,S> * & p) {

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
bool HTreeClass<I,S>::delNode (NodeCell<I,S> &n) {
	NodeCell<I,S>* p;
	
	p = searchNode (n, root);
	
	if (p == NULL) {
		cout << " Node to be deleted not found :" << n << endl;
		return false;
	}
	cout << "Deleting Node :" << *p << endl;
		
	deletenode (n,root);
	
	size_t buffer;
	HashCell<time_t,size_t> tmp (clock(),
			     computeHash (buffer, root));
	return rHash.insertNew(tmp);
} 


int main() {
	cout << "Let's start with the Program Execution \n\n";
	
/////////// Creation of the Binary Tree	
        staffRec[0] = (pS) new Prof ("cicero");
	HTreeClass<int, pS> t;

        NodeCell<int, pS> *p = t.commissionObject(500,staffRec[0]);

	if ( !(t.insertNew(*p)) )
	  return (1);   // failed test

	
	cout << "And this is how the tree looks like :" << t <<"\n";
/////////// Insertion of new Nodes - Test Case
	for (int i=1; i < (sizeof(index_array)/sizeof(int)); i++)
	  {
	    // I am also building the staff Records
	    staffRec[i] = (LibAccess*) new Student("pinco pallo");

	    // ... only here is pertinent for the Uni Library
	    if ( !(t.insertNew(*(t.commissionObject
			       (index_array[i],
				staffRec[i])))) )
	      return (1);   // failed test

	    // check for integrity
	    if (t.integrityCheck())
	      cout << "Integrity preserved\n";
	    else
	      cout << " INTEGRITY COMPROMISED !! \n";
	    
	  }

	cout << "Added new nodes:" << t << "\n";
/////////// Search and Deletion of old Nodes - Test Case
        // test removing one node at the leaf
	Student s2 ("pinco pallo");

	NodeCell<int, pS> d (0,
			    652,
			    (pS) &s2);
	if ( !(t.delNode(d)) )
	  return (1);   // failed test
        // check for integrity
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";
        // NOTE: After this deletion the tree has the same
	//       structure than two steps before. This is
	//       visible in the tree fingerprints history
	if ( !(t.delNode(d.changeIndex(352))) )
	  return (1);   // failed test 
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";
        // NOTE: After this deletion the tree has the same
	//       structure than two steps before. This is
	//       visible in the tree fingerprints history
	if ( !(t.delNode(d.changeIndex(648))) )
	  return (1);   // failed test 
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";
        // NOTE: After this deletion the tree has the same
	//       structure than two steps before. This is
	//       visible in the tree fingerprints history
	
	// test removing one node in the middle of the 
	// right branch - using relinkLeft()
	if ( !(t.delNode(d.changeIndex(600))) )
	  return (1);   // failed test 
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";
	    	
	// test removing one node in the middle of the 
	// left branch - using relinkRight()
	if ( !(t.delNode(d.changeIndex(300))) )
	  return (1);   // failed test 
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";

        // test removing the root - using relinkRoot()
	if ( !(t.delNode(d.changeIndex(500))) )
	  return (1);   // failed test 
        if (t.integrityCheck())
	  cout << "Integrity preserved\n";
	else
	  cout << " !! INTEGRITY COMPROMISED !! \n";

        cout << "Deleted old nodes:" << t << "\n";

/////////// Simulating data corruption
	NodeCell<int, pS> *ts = t.searchItem(d.changeIndex(202));
	
	if ( ts ) {

	  cout << "Tree's fingerprint before:\t"
	       << t.fingerPrint() << endl;

	  cout << "\nData before manipulation: \n"
	       << *(ts->user) << endl;

	  Student *ps4 = (Student*) ts->user;
	  Student s3 ("test1");
	  
	  // Tampering data deliberately
	  // using the overload of operator=
	  *ps4 = s3;

	  cout << "\nData after manipulation: \n"
	       << *((pS)ps4) << endl;
	  
	  cout << "Tree's fingerprint after:\t"
	     << t.fingerPrint() << endl;
	  
	  if ( (t.integrityCheck()) ) {
	    cout << "Integrity Test failed\n";
	    return (1); // test failed
	  }
	  else
	    cout << " !! INTEGRITY COMPROMISED !! -- Test passed \n";	
	}
	
/////////// Program Closure - RegTest passed
	return (0);   // test passed
}
