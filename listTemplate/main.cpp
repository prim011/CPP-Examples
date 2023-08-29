#include <iostream>
#include <string>

#define MAX_LENGTH  1060

using namespace std;

/**
 * @brief Node definition in the list 
 *
 * This structure define the Nodes: the contents and the 
 * pointer to the next element
 */
template <class I, class S>
class NodeCell {
public:
	I 	index;		// an integer index for the node
	S	jointName; 	// a string of information
	NodeCell* next;		// a link to the next element, 
				// it can be NULL for a single element
	// default constructor
	NodeCell () {index = 0; next = NULL;}   
	// initialization contructor 
	NodeCell (I i, S s):index(i), jointName(s) {next=NULL;}
	// copy constractor
	NodeCell (const NodeCell& n):index(n.index),jointName(n.jointName),next(n.next) {}
	// destructor: it release all resources linked to it! 
	// set "next to NULL if you do not want to remove the rest of the nodes
	~NodeCell() {
	   	if(next)           
      			delete next;                    
      
   	  	jointName.~S();
	}
	
	NodeCell& changeIndex (I i) { index = i; return *this; }
	bool operator< (const NodeCell &n) { return (index < n.index); }
	bool operator<= (const NodeCell &n) { return (index <= n.index); }
	bool operator== (const NodeCell &n) { return (index == n.index); }
	bool operator!= (const NodeCell &n) { return (index != n.index); }
	bool operator> (const NodeCell &n) { return (index > n.index); }

	// overload of the << operator for printing an element
	friend ostream& operator<< (ostream& os, const NodeCell<I,S>& t) {	
		os << "\n";
		os << "\tindex: " << t.index;
		os << "\tJoint Name: " << t.jointName;
		os << "\tnext: " << t.next << endl;
		return os; 	
	}
};

/**
 * @brief class for the List 
 */
template <class I, class S>
class ListClass {

private:
	int nodeNum;		// Number of nodes in the list
	NodeCell<I,S>  *head;	// point to the first element
public:
	// default contructor
        ListClass() { nodeNum = 0; head = NULL; }
        // Constuctor with initialization
        ListClass(I setup, S st) {
        	head = new NodeCell<I,S> (setup, st);
        	nodeNum = 1;
        }
        ListClass(const NodeCell<I,S> &t) {
        	head = new NodeCell<I,S> (t);
        	nodeNum = 1;
        }
        // default destructor
        ~ListClass() {nodeNum = 0; delete (head); }
        
 	bool insertNew (NodeCell<I,S>&);		// equivalent of pop the element in 
 	bool delNode (NodeCell<I,S>&);		// equivalent of push the element out
 	NodeCell<I,S>* searchItem (NodeCell<I,S>&);	// search an item for its index
 	
 	// overload of the << operator
 	friend ostream& operator<< (ostream& os, const ListClass& t) {
 		const NodeCell<I,S> *p=t.head;
		int maxElem =t.nodeNum; 
	
		os << "\nLIST PRINTOUT - elements:" << maxElem;
		os << "\n=============\n";

		for (int i=1; i<=maxElem; i++, p=p->next) {
			os << *p;
		}	
		return os; 	
 	}
};


/**
 * @brief Insert one element in the list
 * This function inserts one node in the list, at the last position.
 *
 * @param the node to be included
 * @return diagnastic if the insertion was succesfull (true)
 */
template <class I, class S>
bool ListClass<I,S>::insertNew (NodeCell<I,S> &n) {
	NodeCell<I,S> *t;
	bool retVal = false;
		
	if (head == NULL) {
		head = new NodeCell<I,S> (n);		// use the copy contructor
		nodeNum=1;
		return true;
	}

	t = new NodeCell<I,S> (n); 			// use the copy contructor 
	if (t == NULL)
		return retVal;  

	NodeCell<I,S> *p = head;		
	while (p->next) {
		p = p->next;
	}	
	
	// add the node as the last element
	p->next = t;
	t->next = NULL; // just making sure!!
	nodeNum++;
	retVal = true;			

	return retVal;
}

/**
 * @brief search of a gine node in the list
 * This function searches one node in the list,
 * according to the index value.
 *
 * @param the node to be searched
 * @return a node pointer the the element or NULL is not founded
 */
template <class I, class S>
NodeCell<I,S>* ListClass<I,S>::searchItem (NodeCell<I,S> & n) {
	NodeCell<I,S> *p = head;
	
	// use overload of operator != for NodeCell
	while (p->next && n != *p) {
		p = p->next;
	}
	
	if (n == *p)
		return p;
	return NULL;
}

/**
 * @brief Delete one element from the list
 * This function deletes one node in the list,
 * according to the index value.
 *
 * @param the node to be deleted
 * @return diagnastic if the deletion was succesfull (true)
 */
template <class I, class S>
bool ListClass<I,S>::delNode (NodeCell<I,S> &n) {
	NodeCell<I,S> *c;
	bool retVal = false;
	
	if (head == NULL)
		return retVal;
	
	if (n == *head) {
		c = head; 
		head = head->next;
		c->next=NULL;	// without this instruction the 
				// following delete will remove the 
				// rest of the list
		delete (c);
		nodeNum--;
		return true;
	}
		
	NodeCell<I,S> *p = head;
	while ( p->next->next && (n != *(p->next)) ) {
		p = p->next;
	}
	
	if  (n == *(p->next))  {
		c = p->next; 
		p->next = p->next->next;
		c->next=NULL;
		delete (c);
		nodeNum--;
		retVal = true;	
	}	
	return retVal;
} 

int main() {
	cout << "Let's start with the Program Exection \n\n";

/////////// Creation of the List	
	ListClass<double,string> t(1,"firstJoint");
	
	cout << "And this is the first class outcome :" << t <<"\n";	

/////////// Insertion of new Nodes - Test Case
	NodeCell<double,string> n(0, "Joint Name ");

	for (int i = 2; i <= MAX_LENGTH; i++)
		if (!(t.insertNew (n.changeIndex(i))) )
			return (1);   // fail the test 
	
	cout << "Added new nodes:" << t << "\n";
	
/////////// Search and Deletion of new Nodes - Test Case	
	for (int i = 3; i <= MAX_LENGTH; i++)
		if ((t.searchItem (n.changeIndex(i)) != NULL)) 
			if (!(t.delNode(n.changeIndex(i))))
				return (1);   // fail the test 
					
	cout << "Deleted old nodes:" << t << "\n";		

/////////// Program closure - RegTest passed
	return (0);   // test passed
}
