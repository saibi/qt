/*
 * Interface wrapper code.
 *
 * Generated by SIP 4.9.1-snapshot-20091015 on Mon Nov 16 19:36:49 2009
 */

#include "sipAPIlibserial.h"

#line 38 "stdexcept.sip"
#include <stdexcept>
#line 12 "siplibserialstdinvalid_argument.cpp"

#line 4 "string.sip"
#include <string>
#include <iostream>
#line 17 "siplibserialstdinvalid_argument.cpp"


class sipstd_invalid_argument : public std::invalid_argument
{
public:
    sipstd_invalid_argument(const std::string&);
    sipstd_invalid_argument(const std::invalid_argument&);
    virtual ~sipstd_invalid_argument() throw();

    /*
     * There is a protected method for every virtual method visible from
     * this class.
     */
protected:
    const char * what() const throw();

public:
    sipSimpleWrapper *sipPySelf;

private:
    sipstd_invalid_argument(const sipstd_invalid_argument &);
    sipstd_invalid_argument &operator = (const sipstd_invalid_argument &);

    char sipPyMethods[1];
};

sipstd_invalid_argument::sipstd_invalid_argument(const std::string& a0): std::invalid_argument(a0), sipPySelf(0)
{
    memset(sipPyMethods, 0, sizeof (sipPyMethods));
}

sipstd_invalid_argument::sipstd_invalid_argument(const std::invalid_argument& a0): std::invalid_argument(a0), sipPySelf(0)
{
    memset(sipPyMethods, 0, sizeof (sipPyMethods));
}

sipstd_invalid_argument::~sipstd_invalid_argument() throw()
{
    sipCommonDtor(sipPySelf);
}

const char * sipstd_invalid_argument::what() const throw()
{
    sip_gilstate_t sipGILState;
    PyObject *meth;

    meth = sipIsPyMethod(&sipGILState,const_cast<char *>(&sipPyMethods[0]),sipPySelf,NULL,sipName_what);

    if (!meth)
        return std::logic_error::what();

    extern const char * sipVH_libserial_0(sip_gilstate_t,PyObject *,int,sipSimpleWrapper *);

    return sipVH_libserial_0(sipGILState,meth,6,sipPySelf);
}


/* Cast a pointer to a type somewhere in its superclass hierarchy. */
extern "C" {static void *cast_std_invalid_argument(void *, const sipTypeDef *);}
static void *cast_std_invalid_argument(void *ptr, const sipTypeDef *targetType)
{
    void *res;

    if (targetType == sipType_std_invalid_argument)
        return ptr;

    if ((res = ((const sipClassTypeDef *)sipType_std_logic_error)->ctd_cast((std::logic_error *)(std::invalid_argument *)ptr,targetType)) != NULL)
        return res;

    return NULL;
}


/* Call the instance's destructor. */
extern "C" {static void release_std_invalid_argument(void *, int);}
static void release_std_invalid_argument(void *sipCppV,int state)
{
#line 44 "stdexcept.sip"
	        return ;
#line 97 "siplibserialstdinvalid_argument.cpp"

    if (state & SIP_DERIVED_CLASS)
        delete reinterpret_cast<sipstd_invalid_argument *>(sipCppV);
    else
        delete reinterpret_cast<std::invalid_argument *>(sipCppV);
}


extern "C" {static void dealloc_std_invalid_argument(sipSimpleWrapper *);}
static void dealloc_std_invalid_argument(sipSimpleWrapper *sipSelf)
{
    if (sipIsDerived(sipSelf))
        reinterpret_cast<sipstd_invalid_argument *>(sipSelf->u.cppPtr)->sipPySelf = NULL;

    if (sipIsPyOwned(sipSelf))
    {
        release_std_invalid_argument(sipSelf->u.cppPtr,sipSelf->flags);
    }
}


extern "C" {static void *init_std_invalid_argument(sipSimpleWrapper *, PyObject *, PyObject **, int *);}
static void *init_std_invalid_argument(sipSimpleWrapper *sipSelf, PyObject *sipArgs, PyObject **, int *sipArgsParsed)
{
    sipstd_invalid_argument *sipCpp = 0;

    if (!sipCpp)
    {
        const std::string * a0;
        int a0State = 0;

        if (sipParseArgs(sipArgsParsed,sipArgs,"J1",sipType_std_string,&a0,&a0State))
        {
            try
            {
            sipCpp = new sipstd_invalid_argument(*a0);
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
        const std::invalid_argument * a0;

        if (sipParseArgs(sipArgsParsed,sipArgs,"J9",sipType_std_invalid_argument,&a0))
        {
            try
            {
            sipCpp = new sipstd_invalid_argument(*a0);
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
static sipEncodedTypeDef supers_std_invalid_argument[] = {{14, 255, 1}};


sipClassTypeDef sipTypeDef_libserial_std_invalid_argument = {
    {
        -1,
        0,
        0,
        SIP_TYPE_CLASS,
        sipNameNr_std__invalid_argument,
        {0}
    },
    {
        sipNameNr_invalid_argument,
        {11, 255, 0},
        0, 0,
        0, 0,
        0, 0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    -1,
    -1,
    supers_std_invalid_argument,
    0,
    init_std_invalid_argument,
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
    dealloc_std_invalid_argument,
    0,
    0,
    0,
    release_std_invalid_argument,
    cast_std_invalid_argument,
    0,
    0,
    0
};
