## Building and Running the Code 
To build and run your code you have two options. Either using the CMake command line interface or using an IDE, which in this case will be VSCode. I suggest reading through command line setup so you understand whats going under the hood as your IDE will be running the same (or very similar) commands, just in the background. 

### Command Line

- The first step is to open a terminal and navigate inside the project folder you would like to use. 
- Create a build directory with:
    - `mkdir build`    
- Configure your CMake:
    - `cmake -S . -B build`
- To create a debug build just add the below flag to the end of the previous command:
  -  `-DCMAKE_BUILD_TYPE=Debug`  
- Now to build the project:
  - `cmake --build build`    
- If everything has gone to plan you can now run the binary with:
  -  `.\build\ProjectName.exe`
 
You now have a basic C/C++ with CMake environment setup.
