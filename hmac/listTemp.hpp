#ifndef __LISTTEMP_HPP__
#define __LISTTEMP_HPP__

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

template <class I, class S>
class ListClass;

#include "listTemp.tpp"

#endif // __LISTTEM_HPP__
