# Quick Reference for Blockchain using HMAC

This quick example code implements a possible reference for blockchain implementation, which is a Merkel tree with history.

The history has been saved in the form of a linked list, and each node in the list is created when certain actions - such as insert and delete - in the Merkel tree have been taken, with a timestamp of when those actions have occurred.

This example is an enhancement of the hashTree example which has been presented in other sections. See this [link](../hashTree/README.md) for more information on that example.


## Problem to solve

In contrast with the example in the previous section in [blockchain](../blockchain/README.md), here we are not only interested in the _integrity of the data_ but also in the _data origin authentication_, as well as the tree history.

From what we have seen in the past, the best candidate for the tree fingerprint is the tree hash itself. Therefore storing the variable to a concatenated list seems a good idea for memorizing the tree's history. But The other information to store - apart from a link to the successor node - is a timestamp when that vital tree structure operation did take place. We can think of only two of those operations: when inserting a new node and/or when removing an old node. 

## Data integrity vs Data Origin authentication

It is important to spell out the difference between these two concepts at this point, as the comprehension of the applied changes will be easier.

From a high-level view, the concept of _data origin authentication_ is stronger than _data integrity_. In particular, the former does include the latter. In other words, if we have data origin authentication, then we also have data integrity. More precisely, data origin authentication is data integrity with the extra property of assurance of the identity of the original source of the data.

But why introducing this add-on concept and why do we need it?

For a second, think of how we have achieved data integrity in the blockchain example: by using a hash function with a combination of fields coming from the NodeCell record, such as name and serial number.

If we were to consider transmitting the tree information from point A (Alice) to point B (Bob) and assuming we were in Bob's shoes, we would not have any assurance that the data had come exactly from Alice. We could receive reassurance on the data integrity, but there is no reassurance on who is sending the data. Even without necessarily sending the message, we might want to encapsulate a signature from where the message is originally coming from. This can be further extended to the concept of digital signature, which has an additional attribute to the one we described: non-repudiation (meaning that whoever has digitally signed the message can not repudiate that action, because from the signature it is possible to trace the author/owner; there is a _strong_ association between data authenticated and identity of the author/owner). But without transgressing too much on the digital signature, in data authentication, we are happy if we have a _weak_ reassurance that the data is coming from a valid source.

For this, we have to introduce a completely new paradigm: the concept of keys. In particular data authentication uses symmetric keys and digital signature uses an asymmetric key mechanism in a public key infrastructure (PKI) environment.

So let's focus on symmetric keys and data authentication: Alice sends (or shares) to (with) Bob a key. The key is used by Alice to create the digested message, it does not need to be encrypted. Bob receives the digest message and thanks to the key that was previously shared recognizes that the message does come from Alice. The function used for data authentication (without encryption) is the **HMAC** or **H**ash-based **M**essage **A**uthentication **C**ode. 

### Symmetric Key and ownership
A sensible owner of the symmetric key seems to be at the tree structure level. It would not make too much sense to bure the key into the node level as they are not the owners. But this decision comes with some redesign which will be addressed in the next section. Key generation can be assisted with the following Pseudo-Random Number Generation (PRNG) as follow:

	// initialise PRNG for key generation
	srand(time(0));
	// use PRNG to extract a pseudo-random value
	// for our symmetric key 
	key = rand();	

This is just to illustrate the mechanism; in real life, keys are unlikely to be stored as clearly and plainly as in this example. They normally require hardware support, such as Hardware Security Modules (HSM).

Nevertheless, special attention has been given not to share the value contained in the varaible `key` publicly outside the class. The use of `getKey()` member function is only private, so it can not be publically used. This approach has had some bearing on how the software has been organized and architected. In particular one public member fuction has been defined each time a new Node in the tree is created to commission the nodes itself without exposing the key value. 

	 NodeCell<I,S>* commissionObject (I i, pS &obj) {
	   NodeCell<I,S> *p= new NodeCell<I,S> (getKey(), i, obj);
	   p->commissionNode(getKey());
	   return p;
	 }

This means commissioning the Nodes at the tree level and passing the key value to `LibAccess` abstarct class with the dedicated visrtual member class:

	 virtual void commissionClass (size_t) = 0;

where `seed` is a protected variable; therefore, the `Student` and `Prof` derived classes inherit the variable; but it is not visible outside the classes. Each class then has their own implementation of the `commissionClass()` function which will in stall the value into `seed`.

It is important to distiguish the creation of the class from the commitioning of it; hence the separation of the two activity which in `commissionObject()` happened immediately after; but that might not be the case necessarily.



## How to solve the exercise

The tree class is composed of three major information block

1. The tree nodes: with the LibAccess abstract class pointer, the index, and the pointers - left and right - to the children on the left and right

2. The control structure for checking its integrity: today it is implemented via a likened list, containing the hash fingerprint at each key action throughout the tree's life, and a timestamp for when the action has been taken.

3. The symmetric key for the HMAC, created when the tree data structure has been created, and passed down to the rest of the NodeCell


### The tree nodes

The abstract class described in _libUsers.hpp_ has been taken from previous examples and offers the advantages of abstracting two other classes - Students and Profs - which encapsulate a specific family of Hash Funtions: `HMAC_SH3`. The `HashFunctions` abstract class represents the whole family of hash funtions, from which specific Hash function implementations may be derived. In this example, the low-level function implementing the virtual method is relatively simple in the `HMAC_SH3` derived class and takes two fields, the _key_ coming from the tree structure and a pointer to the node (in the abstract form of `LibAccess`[2]). This last parameter will allow to reach _index_ and _name_, which are used as input to the Hash256 (SHA-3) standard template library function. 

For the benefit of this exercise, we have included only one Prof, at the root and some other students down below.

During the tree construction, in the `main()` function, the same name for the students has been chosen, so that the only differentiation on the Hash input is the sequence/serial number. We will realise that, even in this constrained scenario, the Hash() function works pretty well on differentiating the data structure, and ultimately the tree fingerprint.

#### Rearchitecting the Classes
Since we have identified the Hash function as a module we want to expand on, it is appropriate to give more modularity and flexibility. Here is how we would like to rearchitect the module:

_High-Level Architecture (abstract classes):_


	Class LibAccess {                      Class HashFunctions {
	    virtual size_t getHash () = 0;         virtual size_t formHash(LibAccess*) = 0;
	};                                     };

_Mid-Level Architecture:_
                                                                          
	Class  Prof: public LibAccess {         
				  private:                                       
				    HashFunctions *pHash;          
				  };                                                                  
				      
_Low-Level Architecture:_                                                              
                                                                                    
	                                                     Class HMAC_SH3: public HashFunctions {
									                       size_t formHash(LibAccess *p) {
														   ...
														   ...
														   }
												         };                                                 

This re-architecture allows now to abstract a class function related to the Hash, but also be more specific concerning the implementation of the Hash function at the low level, with the `HMAC_SH3` class. This is achieved without necessarily duplicating the efforts in the two classes that use the Hash function, Student and Prof.


Another advantage gained is that we could define which Hash function any given class should use at the initialization time. In fact, by default, we use the HMAC_SH3, but in theory, we could use another Hash family function by adding a new module equivalent to the `hmac.hpp`. Here are the two constructors for each class:

	Prof (string s): name(s), serialNP(++profSerialNum), active(true) {
    pHash = (HashFunctions*) new HMAC_SH3();
	}

	Prof (string s, HashFunctions* & p): name(s),
				       serialNP(++profSerialNum),
				       active(true) {
    pHash = p;
	}

The first one is used in our example here. The second one could be used in case we want to define another Hash function for the future, for example, one based on Post-Quantum Cryptography (PQC) algorithms.

### The Tree fingerprints

As already discussed, the fingerprints for the hash tree have been determined by the function:

` size_t computeHash(size_t buffer, NodeCell<I,S> *r) `

within the `HTreeClass`. In there the value coming from the implemented abstract class `formHash()` - either Student or Prof - is used to combine a sequence in each of the branches, on the right and the left, and then calculated together as an input to another Hash256 (SHA-3) function to create the unique tree's fingerprint for a given data structure configuration of the tree. It is fair to assess the following:

- the fingerprint is different for each tree's node configuration
- Given the **same** tree structure in two **different times**, its fingerprint has to coincide.

In particular, we will prove both points in this exercise. The first one, while creating/building the tree from scratch, we will notice the fingerprint is always changing. The second point is demonstrated by inserting a leaf node, as last and then immediately removing it at the next action. The reader would agree that the tree node structure configuration is the same before the last leaf node is inserted and after the same node has been removed. So the expectation is that the tree's fingerprint remains the same, although two distinctive operations have been carried out which forced to recalculate the tree's fingerprint. See the output in this example:

`
TREE PRINTOUT - root: 0x55c2d58fb320->548
	History - # events:19
	=============

	 timestamp: 2527	 tree hash signature: 10017358643272133223
	 timestamp: 2580	 tree hash signature: 8645415796858111250
	 timestamp: 2600	 tree hash signature: 17290831593716222760
	 timestamp: 2626	 tree hash signature: 16134919113722894424
	 timestamp: 2651	 tree hash signature: 16134919113722894688
	 timestamp: 2677	 tree hash signature: 7489503316864783304
	 timestamp: 2743	 tree hash signature: 16134919113722895340
	 timestamp: 2897	 tree hash signature: 3166795418435727876
	 timestamp: 2944	 tree hash signature: 7489503316864783994
	 timestamp: 2996	 tree hash signature: 16134919113722896386
	 timestamp: 3028	 tree hash signature: 16134919113722896508
	 timestamp: 3062	 tree hash signature: 11812211215293840442
	 timestamp: 3094	 tree hash signature: 16134919113722896839
	 timestamp: 3127	 tree hash signature: 16134919113722897278 <--+
	 timestamp: 3212	 tree hash signature: 16134919113722897404    |-- same tree structure
	 timestamp: 3355	 tree hash signature: 16134919113722897278 <--+
	 timestamp: 3393	 tree hash signature: 11812211215293840881
	 timestamp: 3469	 tree hash signature: 7489503316864784930
	 timestamp: 3505	 tree hash signature: 3166795418435728799
`

This is all fine for checking the tree's integrity. But what about the history? For this, we have used a linked list, whose nodes are formed by the tree's fingerprint at a specific time and the timestamp of when the fingerprint has changed. Now, for what we said before there are only two operations which will force a tree's fingerprint change: the insertion and the deletion with the functions `insertNew` and `delNode` respectively. In fact, in those two functions the following code has been added:

`
	HashCell<time_t,size_t> tmp (clock(),
				     computeHash (buffer, root));
	return rHash.insertNew(tmp);
`

These lines will make sure that the tree's hash/fingerprint is recalculated - in the first line -, and it has been used to increment the _history_ linked list which is part of the tree private control structure/variable `rHash`  
  
Furthermore, a service route to check the tree integrity has been introduced to check anywhere at any time that the tree integrity is preserved. That function recalculates the hash tree in that moment and checks with the latest element in the linked list, which should be the most current event that happened. If the two values are different we can consistently say that the tree's data integrity has been compromised.

There might be two possible ways to check that this is happening:

1. changing the _name_ and/or the serial number, _serialNP/S_ for any element in the tree
2. omitting the hash recalculation (the two lines above) after one of the two operations of inserting and/or deleting.

In particular, we have tested the latter by omitting the hash recalculation from the delete function and verifying that the `integrityCheck()` function returns an error.

For reference, here is the implementation of the `integrityCheck()` function:

	bool integrityCheck() {
	  size_t buffer;
	  return ( computeHash(buffer, root) ==
	           (rHash.findLast()->hashValue) );
	}

 

### The symmetric key for the HMAC
As we discussed in the previous section, the key's owner has to be necessarily the tree structure. Therefore an additional variable has been declared which will be created at the Tree constructors and passed down to the lover level at the abstract class `LibAccess` to be used in the HMAC function for their class. 

Had we wanted to use the crypto++[^1], the HMAC could have looked like the following definition:

	Digest<HMac<SHA256>>      digest;
	HMAC< SHA256 >            hmac;

and used as follows, in creating the digest:

	hmac.hash("This is the Key", "This is the message", digest);

In this experiment we have used our own definition using `rand()` and `srand()`.

## The use of the Binary tree and the Linked list in a template format 

It makes it easy to reuse the work done in other exercises for this specific example. The format to accept any type of node structure makes the extension to the abstract class very easy.  The user can define the node completely independently at run time. The only constraint is related to the implicit need for the _index_ field to satisfy some order, typically with the overload of the '< > = ' operators. This index is typically different from the serial number of the users, as the index is a control field for the binary tree and the serial number can be used for checking the user identity, instead.

Only one attention: since an abstract class can not be instantiated, we have used _typedef_ to include a pointer to the abstract class as the information stored in the nodes tree. This is predicated on the fact that either the Professor or the Student records are created upfront and then we use the pointer to the abstract class for each of those in the tree's nodes. This is in line with a real case example where, normally the requirement to store information about Students and Professors is stored differently than from those who need to manage access to the University Library. Thanks to the Abstract class concept in C++ it is possible to share only the information needed for the two environments. It is a sort of interface that those two classes have to the Library to access the building. In fact, in C++, an interface is usually defined as a virtual class in which *all* its member functions/methods are virtual. Those virtual methods are defined as the minimum that the _user_ -in this context the University Library - would need to manage access to the building.

Thanks to Abstract class the tree is composed of Professors, with the name "Cicero", and Students, with the name "pinco pallo". Both are part of the binary tree because the tree node points to the abstract class, which is the common part of those two classes.

Typically, we have included one professor, as root at the beginning; then all others are students. 

In the deletion, we show some examples of how internal and leaf nodes/students got removed from the tree. And finally, we get rid of the root, the professor (with no offence).

Please refer to the treeTemplate example for more information about the binary tree data structure.


There is also a quick example on `main()` function on how to use the `TreeClass<>` object, a small exercise on including nodes, searching dedicated nodes and removing them. The same code is also used as a regression test in the `.yml` file for the Continuous Integration (CI). This is why the `main()` function returns 0 or 1 according to the successful, or unsuccessful, completion of the tasks.

Lastly, the implementation of the linked list as a template, it makes simple to generalise the node structure with `time_t` and `size_t`, for the timestamp and the hash respectively. 

The code has been written for C++11, but there is no clear dependency and can be compiled on other toolchains, but we have not tested it.



## Compiling
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive builds like that:

`$ cmake --build . `

The name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myBlockchainExample` generated after the build.



[^1]: see link in [here](https://www.cryptopp.com/wiki/HMAC) and [here](https://www.cryptopp.com/docs/ref/class_h_m_a_c.html)
[^2]: The tree structure relies on this abstract implementation to get the hash of the leaf nodes in the markel tree implementation and use them to combine and form that unique fingerprint which is specific to the tree structure, as we will see later in the next chapter
