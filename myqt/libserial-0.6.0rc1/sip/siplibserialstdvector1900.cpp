/*
 * Interface wrapper code.
 *
 * Generated by SIP 4.9.1-snapshot-20091015 on Mon Nov 16 19:36:49 2009
 */

#include "sipAPIlibserial.h"

#line 274 "vector.sip"
#include <vector>
#line 12 "siplibserialstdvector1900.cpp"



extern "C" {static void assign_std_vector_1900(void *, SIP_SSIZE_T, const void *);}
static void assign_std_vector_1900(void *sipDst, SIP_SSIZE_T sipDstIdx, const void *sipSrc)
{
    reinterpret_cast<std::vector<unsigned> *>(sipDst)[sipDstIdx] = *reinterpret_cast<const std::vector<unsigned> *>(sipSrc);
}


extern "C" {static void *array_std_vector_1900(SIP_SSIZE_T);}
static void *array_std_vector_1900(SIP_SSIZE_T sipNrElem)
{
    return new std::vector<unsigned>[sipNrElem];
}


extern "C" {static void *copy_std_vector_1900(const void *, SIP_SSIZE_T);}
static void *copy_std_vector_1900(const void *sipSrc, SIP_SSIZE_T sipSrcIdx)
{
    return new std::vector<unsigned>(reinterpret_cast<const std::vector<unsigned> *>(sipSrc)[sipSrcIdx]);
}


/* Call the mapped type's destructor. */
extern "C" {static void release_std_vector_1900(void *, int);}
static void release_std_vector_1900(void *ptr, int)
{
    delete reinterpret_cast<std::vector<unsigned> *>(ptr);
}



extern "C" {static int convertTo_std_vector_1900(PyObject *, void **, int *, PyObject *);}
static int convertTo_std_vector_1900(PyObject *sipPy,void **sipCppPtrV,int *sipIsErr,PyObject *sipTransferObj)
{
    std::vector<unsigned> **sipCppPtr = reinterpret_cast<std::vector<unsigned> **>(sipCppPtrV);

#line 297 "vector.sip"
    // Check if type is compatible
    if (sipIsErr == NULL)
    {
        // Must be any iterable
        PyObject *i = PyObject_GetIter(sipPy);
        bool iterable = (i != NULL);
        Py_XDECREF(i);
        return iterable;
    }

    // Iterate over the object
    PyObject *iterator = PyObject_GetIter(sipPy);
    PyObject *item;

    // Maximum number of elements
    unsigned int len = PyObject_Size(sipPy);
    std::vector<unsigned int> *V = new std::vector<unsigned int>();
    V->reserve(len);

    if (len)
    {
        while ((item = PyIter_Next(iterator)))
        {
            if (!PyInt_Check(item))
            {
                PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to float");
                *sipIsErr = 1;
                break;
            }

            unsigned int val = PyInt_AsLong(item);
            V->push_back(val);

            Py_DECREF(item);
        }

        Py_DECREF(iterator);

        if (*sipIsErr)
        {
            delete V;
            return 0;
        }
    }

    *sipCppPtr = V;
    return sipGetState(sipTransferObj);
#line 99 "siplibserialstdvector1900.cpp"
}


extern "C" {static PyObject *convertFrom_std_vector_1900(void *, PyObject *);}
static PyObject *convertFrom_std_vector_1900(void *sipCppV,PyObject *)
{
   std::vector<unsigned> *sipCpp = reinterpret_cast<std::vector<unsigned> *>(sipCppV);

#line 278 "vector.sip"
    PyObject *l;

    // Create the Python list of the correct length.
    if ((l = PyList_New(sipCpp -> size())) == NULL)
        return NULL;

    // Go through each element in the C++ instance and convert it to a
    // wrapped object.
    for (int i = 0; i < (int)sipCpp -> size(); ++i)
    {
        // Add the wrapper to the list.
        PyList_SET_ITEM(l, i, PyInt_FromLong(sipCpp -> at(i)));
    }

    // Return the Python list.
    return l;
#line 125 "siplibserialstdvector1900.cpp"
}


sipMappedTypeDef sipTypeDef_libserial_std_vector_1900 = {
    {
        -1,
        0,
        0,
        SIP_TYPE_MAPPED,
        sipNameNr_59,
        {0}
    },
    {
        -1,
        {0, 0, 1},
        0, 0,
        0, 0,
        0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    },
    assign_std_vector_1900,
    array_std_vector_1900,
    copy_std_vector_1900,
    release_std_vector_1900,
    convertTo_std_vector_1900,
    convertFrom_std_vector_1900
};
