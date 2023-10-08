#include "listTemp.hpp"


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

