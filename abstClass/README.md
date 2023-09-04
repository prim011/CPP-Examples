# Quick Reference for Abstract Classes using Template Binary Tree Class

This quick example code implements a possible reference of an abstract class used in conjunction with a Binary Tree Template, where the main node's information is the abstract class. We will learn how this property allows us to include in the tree other classes, derived by that abstract common factor.

## The Abstract class
Imagine you would like to store only the information needed for accessing the University Library without necessarily disclosing if the user is a professor or a student. For the benefit of this exercise, we have assumed that minimal information is constituted by
- _serial number_: incrementally increasing number shared between professors and students (that might not be what happens in reality)
- The _name_ is an identification, but it could have been anything else for identifying the users, for example, a digital identity.
- Whether the user has _active_ permission. In this case, the student or the professor are allowed in the University Library. It could be a proof from a digital signature or a zero-knowledge proof (zkp) in a more sophisticated digital identity scenario.

The _LibAccess_ class is not a pure virtual class as we have introduced a dedicated printout function of only the information needed for accessing the library (i.e. those three fields we have presented before).

The other three methods are defined as virtual, ` = 0;`, as they need to be developed/implemented by the derived classes. In our context _Student_ and _Professor_.


## The use of the Binary tree in a template format 

It makes it easy to reuse the work done in other exercises for this specific example. The format to accept any type of node structure makes the extension to the abstract class very easy.  The user can define the node completely independently at run time. The only constraint is related to the implicit need for the _index_ field to satisfy some order, typically with the overload of the '< > = ' operators. This index is typically different from the serial number of the users, as the index is a control field for the binary tree and the serial number can be used for checking the user identity, instead.

Only one attention: since an abstract class can not be instantiated, we have used _typedef_ to include a pointer to the abstract class as the information stored in the nodes tree. This is predicated on the fact that either the Professor or the Student records are created upfront and then we use the pointer to the abstract class for each of those in the tree's nodes. This is in line with a real case example where, normally the requirement to store information about Students and Professors is stored differently than from those who need to manage access to the University Library. Thanks to the Abstract class concept in C++ it is possible to share only the information needed for the two environments. It is a sort of interface that those two classes have to the Library to access the building. In fact, in C++, an interface is usually defined as a virtual class in which *all* its member functions/methods are virtual. Those virtual methods are defined as the minimum that the _user_ -in this context the University Library - would need to manage access to the building.

Thanks to Abstract class the tree is composed of Professors, with the name "Cicero", and Students, with the name "pinco pallo". Both are part of the binary tree because the tree node points to the abstract class, which is the common part of those two classes.

Typically, we have included one professor, as root at the beginning; then all others are students. 

In the deletion, we show some examples of how internal and leaf nodes/students got removed from the tree. And finally, we get rid of the root, the professor (with no offence).

Please refer to the treeTemplate example for more information about the binary tree data structure.


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

