## c extension
Code to pass numpy vectors from Python to C, generate a grid of values from the and return grid as Numpy matrix
Heavily based on https://scipy-cookbook.readthedocs.io/items/C_Extensions_NumPy_arrays.html
Explanation here was super helpful http://folk.uio.no/inf3330/scripting/doc/python/NumPy/Numeric/numpy-13.html

This code can be extended to do something more meaningful as C extension.

It was tested on Windows 10 64 bit, Visual Studio Community 2017, Python 3.7 64 bit, though extension is compiled for 32 bit.
Machine has pure Python, Anaconda and VS Python  installed at the same time.

To build a C extension, following has to be done:
1. Visual Studio installed (it is possible to compile extensions with distutils too, but here Visual Suudio is used )
2. Extension written
3. Extension wrapped in functions an structure to make it available for Python imports
4. Extension built
5. Extension installed to Python

A detailed intro to Python extensions in VS is available here: https://docs.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2017
Though it explainsw to wrap C code and mport it to Python, it does not cover how to pass and return arrays.
Also, it does not cover all the path pecularities, which may occur on a real machine.

### 1. Install Visual Studio
- install Visual Studio Build Tools 2017 
      - Visual C++ build tools
- install Visual Studio Community edition 2017 fom Microsoft website
      - Desktop development with C++
      - Python development

### 2. Check that installation works
Follow the steps in https://docs.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2017 to make sure, that installation worked.
This article gives a good overview on how to present C code to python.
I did not use PyBind11.
For paths, use paths to 32 bit Python installation, provided by Visual Studio, such as C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86\include,  NOT your other Python installation folders. You may experiment with other paths too of course.
When you start dealing with arrays, you will have to add Numpy to this Python installation to include its headers, and stop using Py_LIMITED_API; more on this later
To install your extension to Python, navigate to the folder, where you .pyd file resides and run:
Folder example: C:\Users\username\source\repos\Project4\Project4
pip install .
This will install your extension to the python, which is found on your PATH.
If you need to install the extension to Anaconda python to use in Juputer, or any other Python you have, use its own pip, such as provide full path to the pip you intend to use.

### 3. Make Numpy available to Visual Studio
To start dealing with arrays, you need Numpy installed to you VS Python, and its files availale to VS.
- Install Numpy with pip, related to your VS Python installation, e'g. C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86\Scripts\pip

- Add Numpy includes to VS paths in C\C++ additional include directories, e.g. C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86\Lib\site-packages\numpy\core\include\numpy

### 4. Use code from this repo
Create an empty project, as described at 
https://docs.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2017
Double check, that:
 - Numpy inclde folder is specified in project properties in C\C++ additional include directories
- Py_LIMITED_API;  is NOT specified in Preprocessor Definitions, as Numpy headers check for this limitation and do nor provide some definitions, when the setting is on.

To test it in Python, run the Python, where you installed the extension to and type:
import numpy as np
ll = np.array([1,2,3,1,2,2,1,3], dtype = float)
l1 = np.array([1,2,3,1,2,2,1,318.7], dtype = float)
from pass_array import take_and_return
aa = np.linspace(1., 4., 6)
tt = take_and_return((ll,l1,aa,8,8,6))

You should receive a lot of printed output, not zeroes.
When passing Numpy arrays, mind dtypes. Wrong dtype and C returns all zeroes.





