// Interface Elements header: IEResourceHandler
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei


#ifndef _IERESOURCEHANDLER_H_
#define _IERESOURCEHANDLER_H_

#include "Application.h"


class IEResourceHandler
{

friend class IEApplication;

public:
	IEResourceHandler(entry_ref rsrcfile, BApplication *application=NULL);
	virtual ~IEResourceHandler(void);
	status_t Error(void);
	
	void AddResourceFile(entry_ref rsrcfile);

	void *FindResource(char *rsrcname, uint32 type);
	
	void AddToWindowList(BWindow *window);
	void RemoveFromWindowList(BWindow *window);
	
private:
	BApplication *application;		// or NULL if created in an add-on
	
	BList *resource_list;			// of BResources
	BList *window_list;				// of BMessengers of subwindows

	status_t errorcode;
	
};



#endif
