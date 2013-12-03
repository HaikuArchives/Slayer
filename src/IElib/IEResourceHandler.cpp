// class IEResourceHandler
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei

#include "IEResourceHandler.h"
#include "Alert.h"
#include "Resources.h"




IEResourceHandler::IEResourceHandler(entry_ref rsrcfile, BApplication *bapplication)
{
	application=bapplication;
	
	resource_list=new BList();
	window_list=new BList();
	
	errorcode=B_OK;

	AddResourceFile(rsrcfile);
}


void IEResourceHandler::AddResourceFile(entry_ref rscfileref)
{
	
	BFile rscfile;
	BResources *rsc=new BResources;
	
	if(rscfile.SetTo(&rscfileref, B_READ_ONLY)!=B_OK || rsc->SetTo(&rscfile)!=B_OK){
		// Alert to the user: could not open resource file
		BAlert *alert=new BAlert("", "Resource file open error!\n", "Cancel", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
		alert->Go();
		errorcode=B_ERROR;
		delete rsc;
		return;
	}
		
	resource_list->AddItem(rsc);
}


IEResourceHandler::~IEResourceHandler(void)
{
	// close all the windows
	BMessenger *messenger;
	BLooper *looper;
	
	for(int32 i=0;i<window_list->CountItems();i++){
		messenger=(BMessenger *)window_list->ItemAt(i);
		if(messenger->IsValid()){
			messenger->Target(&looper);
			if(looper->Lock()) looper->Quit();
		}
	}

	delete window_list;
	delete resource_list;
}


status_t IEResourceHandler::Error(void)
{
	return errorcode;
}


void *IEResourceHandler::FindResource(char *rsrcname, uint32 rsrctype)
{
	BResources *rsc;
	size_t datasize;
	
	// in all resource files
	for(int32 i=0;i<resource_list->CountItems();i++){
		rsc=(BResources*)resource_list->ItemAt(i);
		if(rsc->HasResource(rsrctype, rsrcname )){
			return rsc->FindResource(rsrctype, rsrcname, &datasize);
		}
	}
	
	return NULL;
}



void IEResourceHandler::AddToWindowList(BWindow *window)
{

	if(window->Type()==B_MODAL_WINDOW) return;
	
	BMessenger *messenger;
	BWindow *a_window;

	// lets search it in the window list
	for(int32 i=0;i<window_list->CountItems();i++){
		messenger=(BMessenger*)window_list->ItemAt(i);
		if(messenger->IsValid()){
			messenger->Target((BLooper**)&a_window);
			if(a_window==window)	return;		// already added
		}else{
			window_list->RemoveItem(messenger);
			i--;
		}
	}

	window_list->AddItem(new BMessenger(NULL, window));
}


void IEResourceHandler::RemoveFromWindowList(BWindow *window)
{	

	if(window->Type()==B_MODAL_WINDOW) return;
	
	BMessenger *messenger;
	BWindow *a_window;

	// lets search it in the window list
	for(int32 i=0;i<window_list->CountItems();i++){
		messenger=(BMessenger*)window_list->ItemAt(i);
		if(messenger->IsValid()){
			messenger->Target((BLooper**)&a_window);
			if(a_window==window){
				window_list->RemoveItem(messenger);
				return;
			}
		}else{
			window_list->RemoveItem(messenger);
			i--;
		}
	}
	
	if(application){
		if(window_list->CountItems()==0 || application->CountWindows()==0){
			application->PostMessage(B_QUIT_REQUESTED);
		}
	}
}


