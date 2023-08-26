# Quick Reference using CMake 

This quick example code creates a liked list with a well-defined node structure. The two classes involved are defined for the node structure and the object associated with the linked list.
Two considerations are worth mentioning:
-	The Node destructor
-	And the Insertion on the list of new elements

### The Node destructor 
The Node’s destruction includes also all the resources associated with it; that includes pointers to other nodes in the field `next`. If a single node needs to be removed/deallocated, it is necessary to set `next = NULL;`,  *before* calling the destructor, otherwise the subsequent linked nodes will be deallocated as well, potentially compromising the list itself, if not careful.
We thought this was a much better design when it comes to removing the entire list (i.e. calling the list destructor), instead of calling the node destructor for each node within the list destructor. In this way, we have taken advantage of the recursion inherited from the linked list as a structured object.

### Node’s Insertion and unordered linked list
The function design is aiming for simplicity and therefore the insertion always takes place at the end of the linked list. It is an equivalent of `push_back()` in the standard list object. We preferred this approach to the more complete one where the insertion happens *in order*. In that context, a little bit more complexity should have gone into the `insertNew()` function; we saved that work for later. Based on this consideration the linked list has to be considered *unordered*. In the example code, the `main()` function already includes the nodes in an orderly sequencing pattern, so the issue is not transpiring. 


There is also a quick example on `main()` function on how to use the `ListClass` object, a small exercise on including nodes, searching dedicated nodes and removing them.


The code has been written for C++11, but there is no clear dependency and can be compiled on other toolchains, but we have not tested it.



## Compiling
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive build like that:

`$ cmake --build . `

the name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myexample` generated after the build.

