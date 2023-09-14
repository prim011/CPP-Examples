#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Node definition in the list 
 *
 * This structure define the Nodes: the contents and the 
 * pointer to the next element
 */
template <class I, class S>
class HashCell {
public:
	I 	index;		// an integer index for the node
	S	hashValue; 	// a string of information
	HashCell* next;		// a link to the next element, 
				// it can be NULL for a single element
	// default constructor
	HashCell () {index = 0; next = NULL;}   
	// initialization contructor 
	HashCell (I i, S s):
	  index(i),
	  hashValue(s) { next=NULL; }
	// copy constractor
	HashCell (const HashCell& n):
	  index(n.index),
	  hashValue(n.hashValue),
	  next(n.next) {}
	// destructor: it release all resources linked to it! 
	// set "next to NULL if you do not want to remove the rest of the nodes
	~HashCell() {
	   	if(next)           
      			delete next;                    
      
   	  	hashValue.~S();
	}
	
	HashCell& changeIndex (I i) { index = i; return *this; }
	bool operator< (const HashCell &n) { return (index < n.index); }
	bool operator<= (const HashCell &n) { return (index <= n.index); }
	bool operator== (const HashCell &n) { return (index == n.index); }
	bool operator!= (const HashCell &n) { return (index != n.index); }
	bool operator> (const HashCell &n) { return (index > n.index); }

	// overload of the << operator for printing an element
	friend ostream& operator<< (ostream& os, const HashCell<I,S>& t) {	
		os << "\n";
		os << "\t timestamp: " << t.index;
		os << "\t tree hash signature: " << t.hashValue;
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
	HashCell<I,S>  *head;	// point to the first element
public:
	// default contructor
        ListClass() { nodeNum = 0; head = NULL; }
        // Constuctor with initialization
        ListClass(I setup, S st) {
        	head = new HashCell<I,S> (setup, st);
        	nodeNum = 1;
        }
        ListClass(const HashCell<I,S> &t) {
        	head = new HashCell<I,S> (t);
        	nodeNum = 1;
        }
        // default destructor
        ~ListClass() {nodeNum = 0; delete (head); }
        
 	bool insertNew (HashCell<I,S>&);		// equivalent of pop the element in 
 	bool delNode (HashCell<I,S>&);		// equivalent of push the element out
 	HashCell<I,S>* searchItem (HashCell<I,S>&);	// search an item for its index
 	
        HashCell<I,S>* findLast () {
	  HashCell<I,S> *p = head;
	  for (int i=1; i < nodeNum; i++, p=p->next) 
	      ;
	  return p;
	}
 	// overload of the << operator
 	friend ostream& operator<< (ostream& os, const ListClass& t) {
 		const HashCell<I,S> *p=t.head;
		int maxElem =t.nodeNum; 
	
		os << "\nHistory - # events:" << maxElem;
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
bool ListClass<I,S>::insertNew (HashCell<I,S> &n) {
	HashCell<I,S> *t;
	bool retVal = false;
		
	if (head == NULL) {
		head = new HashCell<I,S> (n);		// use the copy contructor
		nodeNum=1;
		return true;
	}

	t = new HashCell<I,S> (n); 			// use the copy contructor 
	if (t == NULL)
		return retVal;  

	HashCell<I,S> *p = head;		
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
HashCell<I,S>* ListClass<I,S>::searchItem (HashCell<I,S> & n) {
	HashCell<I,S> *p = head;
	
	// use overload of operator != for HashCell
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
bool ListClass<I,S>::delNode (HashCell<I,S> &n) {
	HashCell<I,S> *c;
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
		
	HashCell<I,S> *p = head;
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

