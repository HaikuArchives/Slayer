// class IEApplication
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei

#include "IEApplication.h"


extern IEResourceHandler *resourcehandler;



IEApplication::IEApplication(const char *signature) 
	: BApplication(signature)
{
	
	errorcode=B_OK;
	
	app_info info;
	GetAppInfo(&info);
	
	// create resource handler
	resourcehandler=new IEResourceHandler(info.ref, this);

	if(resourcehandler->Error()!=B_NO_ERROR){
		errorcode=B_ERROR;
		PostMessage(IE_SHUTDOWN_APP);
		return;
	}
	
}



IEApplication::~IEApplication(void)
{	
	// this closes the windows too if not already closed
	delete resourcehandler;
}



status_t IEApplication::Error(void)
{
	return errorcode;
}



void IEApplication::DispatchMessage(BMessage *message, BHandler *target)
{
	if(message->what==IE_SHUTDOWN_APP){
		Quit();		// no questions asked…
	}else{
		BApplication::DispatchMessage(message, target);
	}
}

