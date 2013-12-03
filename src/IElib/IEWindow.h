// Interface Elements header: IEWindow
// Last modified 2 Mar 1998
// (c) 1997-98 Attila Mezei


#ifndef _IEWINDOW_H_
#define _IEWINDOW_H_

#include "InterfaceKit.h"

#include "IEResourceHandler.h"

// there must be a global resource handler object somewhere
extern IEResourceHandler *resourcehandler;



class IEWindow : public BWindow
{
public:
	IEWindow(char *windowname);
	virtual ~IEWindow(void);

	virtual	bool QuitRequested(void);
	virtual	void Quit(void);
	
	virtual void Show(void);
	
	virtual void UpdateAllViews(BView *parentview=NULL);
	virtual void UpdateAllVariables(BView *parentview=NULL);
	virtual void UpdateView(char *viewname);
	virtual void UpdateVariable(char *viewname);
	virtual void SetViewVariable(char *viewname, void *variable, const char *variabletype=NULL);
	
	BMessage *FindWindowResource(char *windowname);
	void AddDependantWindow(BWindow *window);
	
	static BMessage *RescaleArchive(BMessage *archive, float scale=0.0f);		// auto scale


protected:
	BList *dependancy_list;		// of BMessengers of dependant windows (that will be closed when this window is closed)


private:
	char *name;					// window name
};


#endif