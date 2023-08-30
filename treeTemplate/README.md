# Quick Reference for Template Binary Tree Class

This quick example code implements the Binary Tree in a Template format to accept any type of node structure. The user can define the node completely independently at run time. The only constraint is related to the implicit need for the _index_ field to satisfy some order, typically with the overload of the '< > = ' operators. This has been made possible thanks to the template  definitions in C++ 

The two Template classes, one for the node and one for the tree itself have now been defined based on the binary tree data structure. The migration of those has been pretty straightforward and the flexibility gained is apparent in the `main()` function when the tree is utilized with a versatile node structure. Three considerations are worth mentioning:
-	The Node destructor
- The Node insertion happens inorder
-	The deletion of one element from the tree

### The Node destructor 
The Node’s destruction includes also all the resources associated with it; including pointers to other nodes in the field `sx` and `dx`. If a single node needs to be removed/deallocated, it is necessary to set `sx = dx = NULL;`,  *before* calling the destructor, otherwise the subsequent linked nodes in the branch will be deallocated as well, potentially compromising the tree itself, if not careful.
We thought this was a much better design when it comes to removing the entire tree (i.e. calling the tree destructor), instead of calling the node destructor for each node within the tree destructor. In this way, we have taken advantage of the recursion inherited from the binary tree as a structured object.

### New Node's insertion is always in order
The `insertNew()` member function is straightforward, at least compared with the others. The insertion happens inorder (i.e. navigating the tree from the left-most side branch, first; then from the right-most side). That has also the nice effect of keeping the node ordered according to the _index_ value in the node.


### Node’s deletion
The function design aims for simplicity and uses recursion to first find the match with the node we want to delete. Later, when a correct match was found, great care was been given to preserve the branch beneath and the inherited inorder structure of the tree. The problem typically arises when the node to be removed is *not* in a leaf position.



There is also a quick example on `main()` function on how to use the `TreeClass<>` object, a small exercise on including nodes, searching dedicated nodes and removing them. The same code is also used as a regression test in the `.yml` file for the Continuous Integration (CI). This is why the `main()` function returns 0 or 1 according to the successful, or unsuccessful, completion of the tasks.


The code has been written for C++11, but there is no clear dependency and can be compiled on other toolchains, but we have not tested it.



## Compiling
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive builds like that:

`$ cmake --build . `

The name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myexample` generated after the build.

