// class IEWindow
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei


#include "IEWindow.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.h"
#include "Resources.h"

// if a view is able to handle (convert its state from/to) a variable then it is an IEVariableHandler
#define _IMPEXP_OHLIB
#include "IEVariableHandler.h"


// IEflags in the window archive; currently not put in the window archive
#define IEF_CENTRE 1





IEWindow::IEWindow(char *windowname)
	: BWindow(FindWindowResource(windowname) )
{
	
	dependancy_list=new BList;
	name=strdup(windowname);
	
	if(resourcehandler->Error()!=B_NO_ERROR) return;	// if something is seriously wrong...

	Run();
		
	// centre screen?
	BMessage *window_data=FindWindowResource(windowname);
	if(window_data->FindInt32("IEflags")&IEF_CENTRE){
		BRect screenRect = BScreen(this).Frame();
		MoveTo((screenRect.Width()-Frame().Width())/2,  (screenRect.Height()-Frame().Height())/2);
	}
	if(window_data->HasFloat("minWidth")){
		SetSizeLimits(window_data->FindFloat("minWidth"), window_data->FindFloat("maxWidth"), window_data->FindFloat("minHeight"), window_data->FindFloat("maxHeight") );
	}
	delete window_data;
	
	
	// register the window in the main application
	resourcehandler->AddToWindowList(this);
		
}


IEWindow::~IEWindow(void)
{
	if(dependancy_list) delete dependancy_list;
	free(name);
}


// close all dependant windows
void IEWindow::Quit(void)
{
	int32 i;
	BMessenger *mess;
	BWindow *a_window;
		
	// close all dependant windows
	for(i=0;i<dependancy_list->CountItems();i++){
		mess=(BMessenger*)(dependancy_list->ItemAt(i));
		if(mess->IsValid()){
			mess->Target((BLooper**)&a_window);
			if(a_window){
				if(a_window->Lock()){
					a_window->Quit();
				}
			}
		}
	}

	// delete list items
	for(i=0;i<dependancy_list->CountItems();i++){
		mess=(BMessenger*)(dependancy_list->ItemAt(i));
		delete mess;
	}
	
	dependancy_list->MakeEmpty();
	
	// remove the window from the main application
	resourcehandler->RemoveFromWindowList(this);

	BWindow::Quit();
}


// the default QuitRequested updates all variables and exits
// if you override QuitRequested you need to call UpdateAllVariables() if needed
bool IEWindow::QuitRequested(void)
{

	UpdateAllVariables();

	return true;
}


// last minute update of GUI elements before displaying the window
void IEWindow::Show(void)
{
	// last minute update of GUI elements
	UpdateAllViews();
	
	BWindow::Show();
}


// assigns a variable address and type to a view (an IEVariableHandler derived class)
void IEWindow::SetViewVariable(char *viewname, void *variable, const char *variabletype)
{	
	if(Lock()){
	
		BView *view=FindView(viewname);
		if(view){
			IEVariableHandler *variablehandlerview=cast_as(view, IEVariableHandler);
		
			if(variablehandlerview){
				variablehandlerview->SetVariable(variable, variabletype);
			}
		}
	
		Unlock();

	}
}


// tells the view to update itself from its variable
void IEWindow::UpdateView(char *viewname)
{
	if(Lock()){
	
		BView *view=FindView(viewname);
		if(view){
			IEVariableHandler *variablehandlerview=cast_as(view, IEVariableHandler);
		
			if(variablehandlerview){
				variablehandlerview->UpdateView();
			}
		}
	
		Unlock();
	}
}


// tells the view to update its variable (convert its state to the variable of the given type)
void IEWindow::UpdateVariable(char *viewname)
{
	if(Lock()){
	
		BView *view=FindView(viewname);
		if(view){
			IEVariableHandler *variablehandlerview=cast_as(view, IEVariableHandler);
		
			if(variablehandlerview){
				variablehandlerview->UpdateVariable();
			}
		}
	
		Unlock();
	}
}


// calls UpdateView for all views in the window
void IEWindow::UpdateAllViews(BView *parentview)
{	
	int32 i;
	BView *child;
	IEVariableHandler *variablehandlerview;
	
	// go through all the views...
	
	if(Lock()){
	
		if(parentview==NULL){
			for(i=0;i<CountChildren();i++){
				child=ChildAt(i);
	
				variablehandlerview=cast_as(child, IEVariableHandler);
				if(variablehandlerview){
					variablehandlerview->UpdateView();
				}
				
				if(child->CountChildren()>0) UpdateAllViews(child);
				
			}
		
		}else{
	
			for(i=0;i<parentview->CountChildren();i++){
				child=parentview->ChildAt(i);
	
				variablehandlerview=cast_as(child, IEVariableHandler);
				if(variablehandlerview){
					variablehandlerview->UpdateView();
				}
				
				if(child->CountChildren()>0) UpdateAllViews(child);
				
			}
		}
		
		Unlock();
	}
	
}


// calls UpdateVariable for all views
void IEWindow::UpdateAllVariables(BView *parentview)
{
	int32 i;
	BView *child;
	IEVariableHandler *variablehandlerview;
	
	// go through all the views...
	
	if(Lock()){
	
		if(parentview==NULL){
			for(i=0;i<CountChildren();i++){
				child=ChildAt(i);
	
				variablehandlerview=cast_as(child, IEVariableHandler);
				if(variablehandlerview){
					variablehandlerview->UpdateVariable();
				}
				
				if(child->CountChildren()>0) UpdateAllVariables(child);
				
			}
		
		}else{
	
			for(i=0;i<parentview->CountChildren();i++){
				child=parentview->ChildAt(i);
	
				variablehandlerview=cast_as(child, IEVariableHandler);
				if(variablehandlerview){
					variablehandlerview->UpdateVariable();
				}
				
				if(child->CountChildren()>0) UpdateAllVariables(child);
				
			}
		}
		
		Unlock();
	}
}


// not a Be type code but I use this for archived objects
#define ARCHIVED_OBJECT_TYPE (B_ARCHIVED_OBJECT)

// finds a resource of type B_ARCHIVED_OBJECT
BMessage *IEWindow::FindWindowResource(char *windowname)
{
	BMessage *window_message=new BMessage();
	
	char *window_data=(char*)resourcehandler->FindResource(windowname, ARCHIVED_OBJECT_TYPE);
	
	if(window_data==NULL || window_message->Unflatten(window_data)!=B_OK){
		// we should not return NULL here since this is passed to the BWindow constructor
		// construct a window archive with "Window not found in resources" title
		// this error may happen only in the development phase; the end-user will never see this error window!
		char errortext[128];
		sprintf(errortext, "Window \"%s\" not found in resources!", windowname);
		BWindow *errorwindow=new BWindow(BRect(32,32,400,64), errortext, B_TITLED_WINDOW, 0L);
		
		errorwindow->Archive(window_message, true);
		
		delete errorwindow;
		return window_message;
	}
	
//	return RescaleArchive(window_message);
	// fuck rescaling
	return window_message;
}


// IEWindow maintains a window dependancy list which it will close when Quit()ted
// you can add a window to this list
void IEWindow::AddDependantWindow(BWindow *window)
{
	if(Lock()){
		dependancy_list->AddItem(new BMessenger(NULL, window));
		Unlock();
	}
}


// Scale the archive in-place according to the current plain font size and the design font size
// RescaleArchive returns the input message so that you can do a 
//	new BWindow( IEWindow::RescaleArchive(archive) );  or an
//	instantiate_object( IEWindow::RescaleArchive(view_archive) );
// Original RescaleArchive is a contribution of Mathias Agopian (mathias.agopian@capway.com). Thanks a lot!
// Note: BColorControl has a bug when resized: the slider will be longer than expected
BMessage *IEWindow::RescaleArchive(BMessage *archive, float scale)
{

	if(scale<0.001f){	// if scale parameter is not provided, it is 0.0f
		// go find out the real scale
		font_height fh;
		be_plain_font->GetHeight(&fh);
		
		if(archive->HasFloat("designfsize")){		// "designfsize" is archived in each window or view (root view only). It is the ascent+descent, not the point size
			scale = (fh.ascent+fh.descent) / archive->FindFloat("designfsize");
		}else{
			scale = (fh.ascent+fh.descent) / (9.628906f+2.358398f);	// the ascent+descent for the default font (Swis721 BT Roman) 
		}
		if (scale > 0.99f && scale < 1.01f) return archive;		// do nothing
		scale*=1.03f;	// increase the scale with 3% to avoid rounding problems
	}
	
	// Update the size of this view
	BRect frame;
	if(archive->FindRect("_frame", &frame)==B_OK){
		frame.left		= frame.left	* scale + 0.5f;
		frame.top		= frame.top		* scale + 0.5f;
		frame.right		= frame.right	* scale + 0.5f;
		frame.bottom	= frame.bottom	* scale + 0.5f;
		archive->ReplaceRect("_frame", frame);
	}

	// Update size for each child view
	int32 i=0;
	BMessage nextArchive;
	while (archive->FindMessage("_views", i, &nextArchive) == B_OK)
	{
		RescaleArchive(&nextArchive, scale);
		archive->ReplaceMessage("_views", i, &nextArchive);
		i++;
	}
	
	return archive;
}

