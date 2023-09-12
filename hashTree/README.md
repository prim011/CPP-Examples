# Quick Reference for hash/Merkel Tree on Abstract Classes

This quick example code implements a possible reference of the hash tree based on the example reported in the Abstract Class section. Reusing most of the implementation in there, we will have to extend some of the fields to address the solution to this exercise

## Problem to solve

Instead of focusing on the interface between a database of Students/Professors and a University Library, we are more interested in the *integrity of the data* stored in the database centre. 

## How to solve the exercise

We will be using a data structure called a hash (or Merkel) tree to preserve and check on the data integrity information. The Merkel Tree use the hash at each node of the tree to preserve data integrity. More information is in the following subsections. Let's explain some of the implementation details we used for this exercise.

- used abstract classes. This time to provide information related to the record (either Student or Professor), but for the internal use of the Merkel Tree. Therefore the abstract class will still give the ability to incorporate entities of different kinds - such as Stundet and Professor classes -, but the class will need to be slightly adjusted for the benefit of this exercise. In particular, we will need to include a new field which is named _hashRec_ which is the result of a hash function for the record in question. That hash value will be used in the Markel tree at the leaf nodes  

- The Hash function in the LibClass uses the name and the record number as entries to the hash function

- During the creation of the tree we have used deliberately the same name for students populating the tree so that we can also check on the hash function consistency to provide a different result, due to the registration number, which is necessarily different.

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

