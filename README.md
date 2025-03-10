# Project Structure
-  main.cpp: Contains the exercise development.
- CMakeLists.txt: Instructions on how to compile with CMake under C++11.
- README.md: Short description of the exercise and how to compile.

# Exercises
The folders beneath this level are the following:
- 	listExample	(single linked list structure, unordered). See README file in [here](/listExample/README.md)
-	listTemplate	(same as before, but using template in C++). See README file in [here](/listTemplate/README.md)
- treeClass: an example of Binary Search Tree (BST) using template format in C++. See README file in [here](/treeTemplate/README.md)
- abstClass: an example of an abstract class using the template binary tree previously defined. See README file in [here](/abstClass/README.md)
- hashTree: an example of the hash/Merkel tree using the _abstClass_ and _treeClass_ as starting point. See README file in [here](/hashTree/README.md)
- Blockchain: an example of how the Merkel tree can be used for data integrity, traceability using  a hashbased fingerprint for data integrity and the history of the tree saved into a linked list. In the following example also the concept of message authentication will be integrated on the same data structure. See this [link](/blockchain/README.md) for more details on this example.
- Blockchain with confidentiality built-in (that is Authenticated Encryption, or simply AE): the message authentication is coming from HMAC, an extention of the above example, introducing symmetric keys with **HMAC** or **H**ash-based **M**essage **A**uthentication **C**ode[^1] for generating the fingerprint. See more details in [here](/hmac/README.md). The encryption security service functionality is implemented via AES, with another dedicated symmetric key. The AES and HMAC algorithms have been imported from standard external libraries.
See more information in [here](/Confidentiality/README.md)

## Compiling locally
Assuming GCC v.11 and cmake already installed

Only for the first time run the following:

`$ cmake . `

Then successive builds like that:

`$ cmake --build . `

The name of the file to compile and the project are identified in the `CMakeLists.txt` file. 
Output is the executable `myexample` generated after the build.

## Using *Actions* in gitHub to compile and test
A `.yml` file has been created under the usual repository place for actions, under `./github/workflows/..`. See file here [cmake-single-platform.yml](/.github/workflows/cmake-single-platform.yml)

Use the Action to compile and test in CI (i.e. triggered at each commit/push)

## Contributing
Feel free to contribute in respect to the [Individual CLA](<./Individual CLA.md>)

## Getting in touch
We would love to hear from you. If you fancy the code and would like to improve it feel free to raise a bug or get in contact with me, Francesco Fiaschi at *prim011@yahoo.com*

[^1]: Comprared to previous versions published in this repository [see REL 2.0](./releases/tag/v2.0) a rearchitecture of the C++ code was necessary in order to provide modulaity and flexibility to the `HashFunctions()` abstract class. Now it has been possible to choose which HMAC to select right from the compilation stage with a simple define.
