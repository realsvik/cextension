#include <Windows.h>
#include <cmath>
#include <Python.h>
#include <tupleobject.h>
#include "C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86\Lib\site-packages\numpy\core\include\numpy\ndarraytypes.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86\Lib\site-packages\numpy\core\include\numpy\arrayobject.h"



double *pyvector_to_Carrayptrs(PyArrayObject *arrayin) {
	//int n;

	//n = arrayin->dimensions[0];
	return (double *)arrayin->data;  /* pointer to arrayin data as double */
};



void free_Cint2Darrayptrs(int **v) {
	free((char*)v);
}
double **ptrvector(long n) {
	double **v;
	v = (double **)malloc((size_t)(n * sizeof(double)));
	if (!v) {
		printf("In **ptrvector. Allocation of memory for double array failed.");
		exit(0);
	}
	return v;
}

double **pymatrix_to_Carrayptrs(PyArrayObject *arrayin) {
	double **c, *a;
	int i, n, m;

	n = arrayin->dimensions[0];
	m = arrayin->dimensions[1];
	c = ptrvector(n);
	a = (double *)arrayin->data; // pointer to arrayin data as double 
	for (i = 0; i < n; i++) {
		c[i] = a + i * m;
	}
	return c;
}


void print_array(double *arr, int len) {
	for (int i = 0; i < len; i++) {
		printf("%f\n", arr[i]);
	};
}
PyObject *take_and_return(PyObject *self, PyObject *args)
{
	// pointers to Python objects to use in the function
	PyArrayObject *array1;
	PyArrayObject *array2;
	PyArrayObject *array3;
	// pointers to data in received Python objects
	double *c1in;
	double *c2in;
	double *c3in;
	double **cout;
	// object to store calculation result and pass back to Numpy
	PyArrayObject *matout;
	// variables to store array dimension, technical checks
	int n1,n2,n3;
	int l1;
	int l2;
	int l3;
	// variable to keep multiplication result
	int value;
	double **res_array;
	printf("entered function\n");
	// Parse received Python arguments into 3 Python objects and 3 integer lengths
	if (!PyArg_ParseTuple(args, "O!O!O!iii", 
		&PyArray_Type, &array1, 
		&PyArray_Type, &array2,
		&PyArray_Type, &array3,
		&l1, &l2, &l3))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be an array.");
	}
	else
		printf("got argument\n");
	
	//setup ppointer to vector data and print techincal checks for array 1
	n1 = array1->nd;
	printf("array1 num dim %d\n", n1);
	printf("array1 len %d\n", l1);
	c1in = pyvector_to_Carrayptrs(array1);
	print_array(c1in, l1);
	
	//setup ppointer to vector data and print techincal checks for array 2. Yes, this should be a function
	n2 = array2->nd;
	printf("array2 num dim %d\n", n2);
	printf("array2 len %d\n", l2);
	c2in = pyvector_to_Carrayptrs(array2);
	print_array(c2in, l2);
	//setup ppointer to vector data and print techincal checks for array 3. Yes, this should be a function
	n3 = array3->nd;
	printf("array3 num dim %d\n", n3);
	printf("array3 len %d\n", l3);
	c3in = pyvector_to_Carrayptrs(array3);
	print_array(c3in, l3);
	// number of rows in result array
	value = l1 * l2 * l3;
	printf("value %d\n", value);
	int nd = 2;
	int dims[] = {value,3 };
	printf("dim 1 %d\n", dims[0]);
	printf("dim 2 %d\n", dims[1]);
	// create object to return to Numpy
	matout = (PyArrayObject *)PyArray_FromDims(2, dims, NPY_DOUBLE); //memory allocated for new object
	// create pointer to data in resulting Python object
	cout = pymatrix_to_Carrayptrs(matout);
	// here do something with the new array. A the moment it is filled with the values of passed vectors
	int row_counter = 0;

	for (int i = 0; i < l1; i++) {
		for (int j = 0; j < l2; j++) {
			for (int k = 0; k < l3; k++) {
				cout[row_counter][0] = c1in[i];
				cout[row_counter][1] = c2in[j];
				cout[row_counter][2] = c3in[k];
				row_counter++;
			};

		};
	};
	//printf("rows iterated %d\n", row_counter);
	return PyArray_Return(matout);
};


static PyMethodDef pass_array_methods[] = {
	// The first property is the name exposed to Python, fast_tanh, the second is the C++
	// function name that contains the implementation.
	{ "take_and_return", (PyCFunction)take_and_return, METH_O, nullptr },
	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};
static PyModuleDef pass_array_module = {
	PyModuleDef_HEAD_INIT,
	"pass_array",                        // Module name to use with Python import statements
	"only accepts and returns a vector for fun",  // Module description
	0,
	pass_array_methods                   // Structure that defines the methods of the module
};
PyMODINIT_FUNC PyInit_pass_array() {
	import_array()
	return PyModule_Create(&pass_array_module);
}
