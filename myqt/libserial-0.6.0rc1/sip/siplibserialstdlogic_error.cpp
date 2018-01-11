/*
 * Interface wrapper code.
 *
 * Generated by SIP 4.9.1-snapshot-20091015 on Mon Nov 16 19:36:49 2009
 */

#include "sipAPIlibserial.h"

#line 6 "stdexcept.sip"
#include <stdexcept>
#line 12 "siplibserialstdlogic_error.cpp"

#line 4 "string.sip"
#include <string>
#include <iostream>
#line 17 "siplibserialstdlogic_error.cpp"


class sipstd_logic_error : public std::logic_error
{
public:
    sipstd_logic_error(const std::string&);
    sipstd_logic_error(const std::logic_error&);
    virtual ~sipstd_logic_error() throw();

    /*
     * There is a protected method for every virtual method visible from
     * this class.
     */
protected:
    const char * what() const throw();

public:
    sipSimpleWrapper *sipPySelf;

private:
    sipstd_logic_error(const sipstd_logic_error &);
    sipstd_logic_error &operator = (const sipstd_logic_error &);

    char sipPyMethods[1];
};

sipstd_logic_error::sipstd_logic_error(const std::string& a0): std::logic_error(a0), sipPySelf(0)
{
    memset(sipPyMethods, 0, sizeof (sipPyMethods));
}

sipstd_logic_error::sipstd_logic_error(const std::logic_error& a0): std::logic_error(a0), sipPySelf(0)
{
    memset(sipPyMethods, 0, sizeof (sipPyMethods));
}

sipstd_logic_error::~sipstd_logic_error() throw()
{
    sipCommonDtor(sipPySelf);
}

const char * sipstd_logic_error::what() const throw()
{
    sip_gilstate_t sipGILState;
    PyObject *meth;

    meth = sipIsPyMethod(&sipGILState,const_cast<char *>(&sipPyMethods[0]),sipPySelf,NULL,sipName_what);

    if (!meth)
        return std::logic_error::what();

    extern const char * sipVH_libserial_0(sip_gilstate_t,PyObject *,int,sipSimpleWrapper *);

    return sipVH_libserial_0(sipGILState,meth,8,sipPySelf);
}


extern "C" {static PyObject *meth_std_logic_error_what(PyObject *, PyObject *);}
static PyObject *meth_std_logic_error_what(PyObject *sipSelf, PyObject *sipArgs)
{
    int sipArgsParsed = 0;
    bool sipSelfWasArg = (!sipSelf || sipIsDerived((sipSimpleWrapper *)sipSelf));

    {
        std::logic_error *sipCpp;

        if (sipParseArgs(&sipArgsParsed,sipArgs,"B",&sipSelf,sipType_std_logic_error,&sipCpp))
        {
            const char *sipRes;

            sipRes = (sipSelfWasArg ? sipCpp->std::logic_error::what() : sipCpp->what());

            if (sipRes == NULL)
            {
                Py_INCREF(Py_None);
                return Py_None;
            }

            return SIPBytes_FromString(sipRes);
        }
    }

    /* Raise an exception if the arguments couldn't be parsed. */
    sipNoMethod(sipArgsParsed,sipName_logic_error,sipName_what);

    return NULL;
}


/* Cast a pointer to a type somewhere in its superclass hierarchy. */
extern "C" {static void *cast_std_logic_error(void *, const sipTypeDef *);}
static void *cast_std_logic_error(void *ptr, const sipTypeDef *targetType)
{
    void *res;

    if (targetType == sipType_std_logic_error)
        return ptr;

    if ((res = ((const sipClassTypeDef *)sipType_std_exception)->ctd_cast((std::exception *)(std::logic_error *)ptr,targetType)) != NULL)
        return res;

    return NULL;
}


/* Call the instance's destructor. */
extern "C" {static void release_std_logic_error(void *, int);}
static void release_std_logic_error(void *sipCppV,int state)
{
    if (state & SIP_DERIVED_CLASS)
        delete reinterpret_cast<sipstd_logic_error *>(sipCppV);
    else
        delete reinterpret_cast<std::logic_error *>(sipCppV);
}


extern "C" {static void dealloc_std_logic_error(sipSimpleWrapper *);}
static void dealloc_std_logic_error(sipSimpleWrapper *sipSelf)
{
    if (sipIsDerived(sipSelf))
        reinterpret_cast<sipstd_logic_error *>(sipSelf->u.cppPtr)->sipPySelf = NULL;

    if (sipIsPyOwned(sipSelf))
    {
        release_std_logic_error(sipSelf->u.cppPtr,sipSelf->flags);
    }
}


extern "C" {static void *init_std_logic_error(sipSimpleWrapper *, PyObject *, PyObject **, int *);}
static void *init_std_logic_error(sipSimpleWrapper *sipSelf, PyObject *sipArgs, PyObject **, int *sipArgsParsed)
{
    sipstd_logic_error *sipCpp = 0;

    if (!sipCpp)
    {
        const std::string * a0;
        int a0State = 0;

        if (sipParseArgs(sipArgsParsed,sipArgs,"J1",sipType_std_string,&a0,&a0State))
        {
            try
            {
            sipCpp = new sipstd_logic_error(*a0);
            }
            catch (...)
            {

            sipReleaseType(const_cast<std::string *>(a0),sipType_std_string,a0State);
                sipRaiseUnknownException();
                return NULL;
            }

            sipReleaseType(const_cast<std::string *>(a0),sipType_std_string,a0State);
        }
    }

    if (!sipCpp)
    {
        const std::logic_error * a0;

        if (sipParseArgs(sipArgsParsed,sipArgs,"J9",sipType_std_logic_error,&a0))
        {
            try
            {
            sipCpp = new sipstd_logic_error(*a0);
            }
            catch (...)
            {
                sipRaiseUnknownException();
                return NULL;
            }
        }
    }

    if (sipCpp)
        sipCpp->sipPySelf = sipSelf;

    return sipCpp;
}


/* Define this type's super-types. */
static sipEncodedTypeDef supers_std_logic_error[] = {{12, 255, 1}};


static PyMethodDef methods_std_logic_error[] = {
    {SIP_MLNAME_CAST(sipName_what), meth_std_logic_error_what, METH_VARARGS, NULL}
};


sipClassTypeDef sipTypeDef_libserial_std_logic_error = {
    {
        -1,
        0,
        0,
        SIP_TYPE_CLASS,
        sipNameNr_std__logic_error,
        {0}
    },
    {
        sipNameNr_logic_error,
        {11, 255, 0},
        1, methods_std_logic_error,
        0, 0,
        0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    -1,
    -1,
    supers_std_logic_error,
    0,
    init_std_logic_error,
    0,
    0,
#if PY_MAJOR_VERSION >= 3
    0,
    0,
#else
    0,
    0,
    0,
    0,
#endif
    dealloc_std_logic_error,
    0,
    0,
    0,
    release_std_logic_error,
    cast_std_logic_error,
    0,
    0,
    0
};
