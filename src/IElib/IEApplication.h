// Interface Elements header: IEApplication
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei


#ifndef _IEAPPLICATION_H_
#define _IEAPPLICATION_H_

#include "AppKit.h"

#include "IEResourceHandler.h"



#define IE_SHUTDOWN_APP 'IEsd'


class IEApplication : public BApplication
{
public:
	IEApplication(const char *signature);
	virtual ~IEApplication(void);
	
	virtual void DispatchMessage(BMessage *message, BHandler *target);

	status_t Error(void);
	
private:
	status_t errorcode;
};



#endif