// IEVariableHandler class
// (c) 1998 by Attila Mezei
// Base class for views with variable handling. 
// Inherit from this class with multiple inheritence.


#ifndef _IEVARIABLEHANDLER_H_
#define _IEVARIABLEHANDLER_H_

#if !defined(_IMPEXP_OHLIB)
#define _IMPEXP_OHLIB __declspec(dllimport)
#endif


class _IMPEXP_OHLIB IEVariableHandler
{
public:
	IEVariableHandler(BMessage *data=NULL);
	virtual ~IEVariableHandler(void);

	void Archive(BMessage *data) const;		// archives the variable settings if they exist and valid

	// methods for assigning/requesting the target variable
	virtual void SetVariable(void *pointer, const char *type=NULL);	// leave type unchanged
	virtual void *Variable(void);					// returns the variable pointer

	// methods for converting between the view and variable; the actual view must implement these hooks
	virtual void UpdateView(void);					// updates the view according to the variable
	virtual void UpdateVariable(void);				// updates the variable according to the view

	// utility methods
	BMessage *GetAvailableTypes(void);				// B_STRING_TYPE "type"
	BMessage *VariableDefinition(void);

protected:	
	BMessage data_types;							// which the view can handle; B_STRING_TYPE "type"
	BMessage variable_definition;		
													// B_STRING_TYPE "type"			e.g. "float"
													// B_STRING_TYPE "language"		e.g. "C++" (currently not used)
													// B_STRING_TYPE "identifier"		e.g. "some_object->point.x" (this will be needed by source generation in the future)
	void *variable;

};

#endif