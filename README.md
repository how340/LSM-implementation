This repo houses the version of the code that should contain and allow all optimizations. 
Currently, this version is missing the following functionalities. Some of them will be first implemented on the unoptimized version, and the code will be migrated over here. 

List of TODO elements: 
1. implement data persistence - create API to handle saving normal files, reading in saved files, and handling exiting applications. - DONE. Already in this repo
2. Implement the two other query operators (range, delete). I need to figure out what to save delete as. (add an additional bit?) - In progress with the un-optimized version. 
2. Implement a more advanced merge policy to improve performance. - unique to the optimized version. 
3. Improve the two optimization proposed in the paper. - unique to the optimized version
5. multi-threaded support
6. better file I/O approach. - probably delta encoding?
