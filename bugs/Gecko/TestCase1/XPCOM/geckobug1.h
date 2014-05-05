/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM GeckoBug1.idl
 */

#ifndef __gen_GeckoBug1_h__
#define __gen_GeckoBug1_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    IGeckoBug1 */
#define IGECKOBUG1_IID_STR "18dce255-172e-4661-8899-7b4a85a521b1"

#define IGECKOBUG1_IID \
  {0x18dce255, 0x172e, 0x4661, \
    { 0x88, 0x99, 0x7b, 0x4a, 0x85, 0xa5, 0x21, 0xb1 }}

class NS_NO_VTABLE IGeckoBug1 : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IGECKOBUG1_IID)

  /* void Test (); */
  NS_IMETHOD Test(void) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(IGeckoBug1, IGECKOBUG1_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IGECKOBUG1 \
  NS_IMETHOD Test(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IGECKOBUG1(_to) \
  NS_IMETHOD Test(void) { return _to Test(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IGECKOBUG1(_to) \
  NS_IMETHOD Test(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IGeckoBug1
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IGECKOBUG1

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, IGeckoBug1)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void Test (); */
NS_IMETHODIMP _MYCLASS_::Test()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_GeckoBug1_h__ */
