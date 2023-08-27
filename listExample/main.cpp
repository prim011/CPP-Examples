#include <iostream>
#include <string>

#define MAX_LENGTH  160

using namespace std;

/**
 * @brief Node definition in the list 
 *
 * This structure defines the Nodes: the contents of the information and the
 * pointer to the next element
 */
class NodeCell {
public:
	int 	index;		// an integer index for the node
	string	jointName; 	// a string of information
	NodeCell* next;		// a link to the next element, 
				// it can be NULL for a single element
	// default constructor
	NodeCell () {index = 0; next = NULL;}   
	// initialization contructor 
	NodeCell (int i, string s):index(i), jointName(s) {next=NULL;}
	// copy constractor
	NodeCell (const NodeCell& n):index(n.index),jointName(n.jointName),next(n.next) {}
	// destructor: it release all resources linked to it! 
	// set "next to NULL if you do not want to remove the rest of the nodes
	~NodeCell() {
	   	if(next)           
      			delete next;                    
      
   	  	jointName.~string();
	}
	
	bool operator< (const NodeCell &n) { return (index < n.index); }
	bool operator<= (const NodeCell &n) { return (index <= n.index); }
	bool operator== (const NodeCell &n) { return (index == n.index); }
	bool operator!= (const NodeCell &n) { return (index != n.index); }
	bool operator> (const NodeCell &n) { return (index > n.index); }

	// overload of the << operator for printing an element
	friend ostream& operator<< (ostream& os, NodeCell& t) {	
		os << "\n";
		os << "\tindex: " << t.index;
		os << "\tJoint Name: " << t.jointName;
		os << "\tnext: " << t.next << endl;
		return os; 	
	}
};

/**
 * @brief class for the List 
 * 
 * 
 */

class ListClass {

private:
	int nodeNum;		// Number of nodes in the list
	NodeCell *head;		// point to the first element
public:
	// default contructor
        ListClass() { nodeNum = 0; head = NULL; }
        // Constuctor with initialization
        ListClass(int setup, string st) {
        	head = new NodeCell(setup, st);
        	nodeNum = 1;
        }
        ListClass(const NodeCell &t) {
        	head = new NodeCell(t);
        	nodeNum = 1;
        }
        // default destructor
        ~ListClass() {nodeNum = 0; delete (head); }
        
 	bool insertNew (NodeCell&);		// equivalent of pop the element in 
 	bool delNode (NodeCell&);		// equivalent of push the element out
 	NodeCell* searchItem (NodeCell&);	// search an item for its index
 	
 	// overload of the << operator
 	friend ostream& operator<< (ostream&, const ListClass&);
};


/**
 * @brief Insert one element in the list
 * This function inserts one node in the list, at the last position.
 *
 * @param the node to be included
 * @return diagnastic if the insertion was succesfull (true)
 */
bool ListClass::insertNew (NodeCell &n) {
	NodeCell *t;
	bool retVal = false;
		
	if (head == NULL) {
		head = new NodeCell(n);		// use the copy contructor
		nodeNum=1;
		return true;
	}

	t = new NodeCell(n); 			// use the copy contructor 
	if (t == NULL)
		return retVal;  

	NodeCell *p = head;		
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
 * @param the node to be included
 * @return a node pointer the the element or NULL is not founded
 */
NodeCell* ListClass::searchItem (NodeCell& n) {
	NodeCell *p = head;
	
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
 * @param the node to be included
 * @return diagnastic if the deletion was succesfull (true)
 */
bool ListClass::delNode (NodeCell &n) {
	NodeCell *c;
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
		
	NodeCell *p = head;
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
/**
 * @brief overloading of the << operator
 * This function travers the list and print all the nodes
 * @param ostream to print to
 * @param the class 
 * @return the same out stream
 */
ostream& operator<< (ostream &os, const ListClass &t) {
	NodeCell *p=t.head;
	
	os << "\nLIST PRINTOUT - elements:" << t.nodeNum << " head: " << t.head;
	os << "\n=============\n";

	for (int i=1; i<=t.nodeNum; i++, p=p->next) {
		os << *p;
	}	
		
	return os; 	
}

int main() {
	cout << "Let's start with the Program Exection \n\n";
	
	ListClass t(1,"firstJoint");
	string st;
	
	cout << "And this is the first class outcome :" << t <<"\n";	

	for (int i = 2; i <= MAX_LENGTH; i++)
	{
		st = "Joint Name # " + to_string(i);
		NodeCell n(i, st);
		if (!(t.insertNew (n)) )
			cout << "\n Error on inserting node :" << i << endl;
	}	
	
	cout << "Added new nodes:" << t << "\n";
	
	for (int i = 3; i <= MAX_LENGTH; i++)
	{
		st = "##### " + to_string(i);
		NodeCell n(i, st);
		if ((t.searchItem (n) != NULL)) 
			if (!(t.delNode(n)))
				return (1);  // fail the regression test 
	}	
	cout << "Deleted old nodes:" << t << "\n";		
	return (0); // all good!
}
