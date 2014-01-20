/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM egeckocom.idl
 */

#ifndef __gen_egeckocom_h__
#define __gen_egeckocom_h__


#ifndef __gen_nsIDOMWindow_h__
#include "nsIDOMWindow.h"
#endif

#ifndef __gen_nsICommandLine_h__
#include "nsICommandLine.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    EIGeckoCOM */
#define EIGECKOCOM_IID_STR "07232fe0-fee4-11e0-be50-0800200c9a66"

#define EIGECKOCOM_IID \
  {0x07232fe0, 0xfee4, 0x11e0, \
    { 0xbe, 0x50, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }}

class NS_NO_VTABLE EIGeckoCOM : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(EIGECKOCOM_IID)

  /* void Create (in string ComponentId, in string Language, out string ErrorMessage); */
  NS_IMETHOD Create(const char * ComponentId, const char * Language, char * *ErrorMessage) = 0;

  /* void Retrieve (in string ComponentId, out string ErrorMessage); */
  NS_IMETHOD Retrieve(const char * ComponentId, char * *ErrorMessage) = 0;

  /* void Register (in nsIDOMWindow Window, out string ErrorMessage); */
  NS_IMETHOD Register(nsIDOMWindow *Window, char * *ErrorMessage) = 0;

  /* void Stop (in nsICommandLine CommandLine, out string ErrorMessage); */
  NS_IMETHOD Stop(nsICommandLine *CommandLine, char * *ErrorMessage) = 0;

  /* void Terminate (out string ErrorMessage); */
  NS_IMETHOD Terminate(char * *ErrorMessage) = 0;

  /* void HandlePseudoEvent (in nsIDOMElement Element, in string Parameter, out string ErrorMessage); */
  NS_IMETHOD HandlePseudoEvent(nsIDOMElement *Element, const char * Parameter, char * *ErrorMessage) = 0;

  /* void Test (out string ErrorMessage); */
  NS_IMETHOD Test(char * *ErrorMessage) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(EIGeckoCOM, EIGECKOCOM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_EIGECKOCOM \
  NS_IMETHOD Create(const char * ComponentId, const char * Language, char * *ErrorMessage); \
  NS_IMETHOD Retrieve(const char * ComponentId, char * *ErrorMessage); \
  NS_IMETHOD Register(nsIDOMWindow *Window, char * *ErrorMessage); \
  NS_IMETHOD Stop(nsICommandLine *CommandLine, char * *ErrorMessage); \
  NS_IMETHOD Terminate(char * *ErrorMessage); \
  NS_IMETHOD HandlePseudoEvent(nsIDOMElement *Element, const char * Parameter, char * *ErrorMessage); \
  NS_IMETHOD Test(char * *ErrorMessage); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_EIGECKOCOM(_to) \
  NS_IMETHOD Create(const char * ComponentId, const char * Language, char * *ErrorMessage) { return _to Create(ComponentId, Language, ErrorMessage); } \
  NS_IMETHOD Retrieve(const char * ComponentId, char * *ErrorMessage) { return _to Retrieve(ComponentId, ErrorMessage); } \
  NS_IMETHOD Register(nsIDOMWindow *Window, char * *ErrorMessage) { return _to Register(Window, ErrorMessage); } \
  NS_IMETHOD Stop(nsICommandLine *CommandLine, char * *ErrorMessage) { return _to Stop(CommandLine, ErrorMessage); } \
  NS_IMETHOD Terminate(char * *ErrorMessage) { return _to Terminate(ErrorMessage); } \
  NS_IMETHOD HandlePseudoEvent(nsIDOMElement *Element, const char * Parameter, char * *ErrorMessage) { return _to HandlePseudoEvent(Element, Parameter, ErrorMessage); } \
  NS_IMETHOD Test(char * *ErrorMessage) { return _to Test(ErrorMessage); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_EIGECKOCOM(_to) \
  NS_IMETHOD Create(const char * ComponentId, const char * Language, char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Create(ComponentId, Language, ErrorMessage); } \
  NS_IMETHOD Retrieve(const char * ComponentId, char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Retrieve(ComponentId, ErrorMessage); } \
  NS_IMETHOD Register(nsIDOMWindow *Window, char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Register(Window, ErrorMessage); } \
  NS_IMETHOD Stop(nsICommandLine *CommandLine, char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Stop(CommandLine, ErrorMessage); } \
  NS_IMETHOD Terminate(char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Terminate(ErrorMessage); } \
  NS_IMETHOD HandlePseudoEvent(nsIDOMElement *Element, const char * Parameter, char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandlePseudoEvent(Element, Parameter, ErrorMessage); } \
  NS_IMETHOD Test(char * *ErrorMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(ErrorMessage); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public EIGeckoCOM
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_EIGECKOCOM

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, EIGeckoCOM)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void Create (in string ComponentId, in string Language, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Create(const char * ComponentId, const char * Language, char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Retrieve (in string ComponentId, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Retrieve(const char * ComponentId, char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Register (in nsIDOMWindow Window, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Register(nsIDOMWindow *Window, char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Stop (in nsICommandLine CommandLine, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Stop(nsICommandLine *CommandLine, char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Terminate (out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Terminate(char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void HandlePseudoEvent (in nsIDOMElement Element, in string Parameter, out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::HandlePseudoEvent(nsIDOMElement *Element, const char * Parameter, char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void Test (out string ErrorMessage); */
NS_IMETHODIMP _MYCLASS_::Test(char * *ErrorMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_egeckocom_h__ */
