# Quick Reference for Blockchain on Abstract Classes

This quick example code implements a possible reference for blockchain implementation, which is a Merkel tree with history.

The history has been saved in the form of a linked list, and each node in the list is created when certain actions - such as insert and delete - in the Merkel tree have been taken, with a timestamp of when those actions have occurred.

This example is an enhancement of the hashTree example which has been presented in other sections. See this [link](../hashTree/README.md) for more information on that example.


## Problem to solve

We want to save the integrity of the data structure built with the hashTree **and** we want to save a history of what has happened in the tree with some sort of fingerprint.

From what we have seen in the past, the best candidate for the tree fingerprint is the tree hash itself. Therefore storing the variable to a concatenated list seems a good idea for memorizing the tree's history. But The other information to store - apart from a link to the successor node - is a timestamp when that vital tree structure operation did take place. We can think of only two of those operations: when inserting a new node and/or when removing an old node. 

## How to solve the exercise

The tree class is composed of two major information block

1. The tree nodes: with the LibAccess abstract class pointer, the index, and the pointers - left and right - to the children on the left and right

2. The control structure for checking its integrity: today it is implemented via a likened list, containing the hash fingerprint at each key action throughout the tree's life, and a timestamp for when the action has been taken.


### The tree nodes

The abstract class described in _libUsers.hpp_ has been taken from previous examples and offers the advantages of abstracting two other classes - Students and Profs - with the only variation to add a dedicated Hash function, `Student::formHash()` and `Prof::formHash()` respectively. Both implementations, in this example, are the same, but in theory, could have been different for each class. The function is relatively simple and takes two fields, _index_ and _name_, as input to the Hash256 (SHA-3) standard template library function. Those two functions are abstracted by the LibAccess Class, which is ultimately used in the hash tree.

_NOTE:_ The tree relies on this abstract implementation to get the hash of the leaf nodes in the markel tree implementation and use them to combine and form that unique fingerprint which is specific to the tree structure, as we will see later in the next chapter.

For the benefit of this exercise, we have included only one Prof, at the root and some other students down below.

During the tree construction, in the `main()` function, the same name for the students has been chosen, so that the only differentiation on the Hash input is the sequence/serial number. We will realise that, even in this constrained scenario, the Hash() function works pretty well on differentiating the data structure, and ultimately the tree fingerprint.


### The Tree fingerprints

As already discussed, the fingerprints for the hash tree have been determined by the function:

` size_t computeHash(size_t buffer, NodeCell<I,S> *r) `

within the `HTreeClass`. In there the value coming from the implemented abstract class `formHash()` - either Student or Prof - is used to combine a sequence in each of the branches, on the right and the left, and then calculated together as an input to another Hash256 (SHA-3) function to create the unique tree's fingerprint for a given data structure configuration of the tree. In fact, it is fair to assess the following:

- the fingerprint is different for each tree's node configuration
- Given the **same** tree structure in two **different times**, its fingerprint has to coincide.

In particular, we will prove both points in this exercise. The first one, while cresting/building the tree from scratch, we will notice the fingerprint is always changing. The second point is demonstrated by inserting a leaf node, as last and then immediately removing it at the next action. The reader would agree that the tree node structure configuration is the same before the last leaf node is inserted and after the same node has been removed. So the expectation is that the tree's fingerprint remains the same, although two distinctive operations have been carried out which forced to recalculate the tree's fingerprint


This is all fine for checking the tree's integrity. But what about the history? For this, we have used a linked list, whose nodes are formed by the tree's fingerprint at a specific time and the timestamp of when the fingerprint has changed. Now, for what we said before there are only two operations which will force a tree's fingerprint change: the insertion and the deletion with the functions `insertNew` and `delNode` respectively. In fact, in those two functions the following code has been added:

`
	HashCell<time_t,size_t> tmp (clock(),
				     computeHash (buffer, root));
	return rHash.insertNew(tmp);
`

These lines will make sure that the tree's hash/fingerprint is recalculated - in the first line -, and it has been used to increment the _history_ linked list which is part of the tree private control structure/variable `rHash`  
  
Furthermore, a service route to check the tree integrity has been introduced to check anywhere at any time that the tree integrity is preserved. That function recalculates the hash tree at the moment and checks with the latest element in the linked list, which should be the most current event that happened. If the the two values are different we can consistently say that the tree's data integrity has been compromised.

There might be two possible ways to check that this is happening:

1. changing the _name_ and/or the serial number, _serialNP/S_ for any element in the tree
2. omitting the hash recalculation (the two lines above) after one of the two operations of inserting and/or deleting.

In particular, we have tested the latter by omitting the hash recalculation from the delete function and verifying that the `integrityCheck()` function returns an error.

For reference, here is the implementation of the `integrityCheck()` function:

`
        bool integrityCheck() {
	  size_t buffer;
	  return ( computeHash(buffer, root) ==
		   (rHash.findLast()->hashValue) );
	}
`

Finally here is a snapshot of the program outcome 


## The use of the Binary tree and the Linked list in a template format 

It makes it easy to reuse the work done in other exercises for this specific example. The format to accept any type of node structure makes the extension to the abstract class very easy.  The user can define the node completely independently at run time. The only constraint is related to the implicit need for the _index_ field to satisfy some order, typically with the overload of the '< > = ' operators. This index is typically different from the serial number of the users, as the index is a control field for the binary tree and the serial number can be used for checking the user identity, instead.

Only one attention: since an abstract class can not be instantiated, we have used _typedef_ to include a pointer to the abstract class as the information stored in the nodes tree. This is predicated on the fact that either the Professor or the Student records are created upfront and then we use the pointer to the abstract class for each of those in the tree's nodes. This is in line with a real case example where, normally the requirement to store information about Students and Professors is stored differently than from those who need to manage access to the University Library. Thanks to the Abstract class concept in C++ it is possible to share only the information needed for the two environments. It is a sort of interface that those two classes have to the Library to access the building. In fact, in C++, an interface is usually defined as a virtual class in which *all* its member functions/methods are virtual. Those virtual methods are defined as the minimum that the _user_ -in this context the University Library - would need to manage access to the building.

Thanks to Abstract class the tree is composed of Professors, with the name "Cicero", and Students, with the name "pinco pallo". Both are part of the binary tree because the tree node points to the abstract class, which is the common part of those two classes.

Typically, we have included one professor, as root at the beginning; then all others are students. 

In the deletion, we show some examples of how internal and leaf nodes/students got removed from the tree. And finally, we get rid of the root, the professor (with no offence).

Please refer to the treeTemplate example for more information about the binary tree data structure.


There is also a quick example on `main()` function on how to use the `TreeClass<>` object, a small exercise on including nodes, searching dedicated nodes and removing them. The same code is also used as a regression test in the `.yml` file for the Continuous Integration (CI). This is why the `main()` function returns 0 or 1 according to the successful, or unsuccessful, completion of the tasks.

Lastly, the implementation of the linked list as a template, it makes simple to generalise the node structure with `time_t` and `size_t`, for the timestamp and the hash respectively. 



	 timestamp: 2355	 tree hash signature: 11813688941146856373
	 timestamp: 2379	 tree hash signature: 5478620386116794821
	 timestamp: 2403	 tree hash signature: 8941924468534910821
	 timestamp: 2427	 tree hash signature: 8941924468534910845
	 timestamp: 2451	 tree hash signature: 11813688941146856325
	 timestamp: 2476	 tree hash signature: 8941924468534910757
	 timestamp: 2501	 tree hash signature: 8941924468534910829 <--+
	 timestamp: 2528	 tree hash signature: 8941924468534910797    |--same tree structure
	 timestamp: 2643	 tree hash signature: 8941924468534910829 <--+ 
	 timestamp: 2670	 tree hash signature: 3186359960298042465
	 timestamp: 2697	 tree hash signature: 6612012521036440939
	 timestamp: 2727	 tree hash signature: 12449989354134733581



The code has been written for C++11, but there is no clear dependency and can be compiled on other toolchains, but we have not tested it.



## Compiling
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive builds like that:

`$ cmake --build . `

The name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myBlockchainExample` generated after the build.

