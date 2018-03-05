// SettingsWindow.cpp
// Generated by Interface Elements (Window v2.2) on Jan 3 1999
// This is a user written class and will not be overwritten.

#include <Catalog.h>
#include <LayoutBuilder.h>

#include "SettingsWindow.h"
#include "SlayerApp.h"
#include "MiniSlayer.h"

#include <stdlib.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

SettingsWindow::SettingsWindow(void)
	: BWindow(BRect(200,200,300,300), B_TRANSLATE("Settings"), B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE |B_AUTO_UPDATE_SIZE_LIMITS )
{
	current_workspace = new BRadioButton("SettingsCurrentWorkspace", B_TRANSLATE("Open window in current workspace"), new BMessage(IE_SETTINGSWINDOW_SETTINGSCURRENTWORKSPACE));
	all_workspaces = new BRadioButton("SettingsAllWorkspaces", B_TRANSLATE("Open window in all workspaces"), new BMessage(IE_SETTINGSWINDOW_SETTINGSALLWORKSPACES));
	saved_workspace = new BRadioButton("SettingsSavedWorkspace", B_TRANSLATE("Open window in saved workspace"), new BMessage(IE_SETTINGSWINDOW_SETTINGSSAVEDWORKSPACE));

	BGroupLayout* workspaceBox = BLayoutBuilder::Group<>(B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(current_workspace)
		.Add(all_workspaces)
		.Add(saved_workspace);

	BBox* top = new BBox("top");
	top->AddChild(workspaceBox->View());
	top->SetLabel( B_TRANSLATE("Worspace"));
	refresh = new BTextControl("SettingsRefresh", B_TRANSLATE("Refresh display (seconds)"), "0.5", NULL);

	save_window = new BCheckBox("SettingsSaveWindow", B_TRANSLATE("Save window position on exit"), new BMessage(IE_SETTINGSWINDOW_SETTINGSSAVEWINDOW));
	save_workspace = new BCheckBox("SettingsSaveWorkspace", B_TRANSLATE("Save workspace"), new BMessage(IE_SETTINGSWINDOW_SETTINGSSAVEWORKSPACE));


	BButton* deskbarButton;
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_SMALL_INSETS)
		.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(top, 100)
		.Add(refresh)
		.Add(save_window)
		.Add(save_workspace)
		.AddGroup(B_HORIZONTAL)
			.Add(new BButton("", B_TRANSLATE("Revert"), new BMessage(IE_SETTINGSWINDOW_SETTINGSREVERT)))
			.AddGlue()
			.Add(deskbarButton = new BButton("SettingsDeskbar", B_TRANSLATE("Dock to deskbar"), new BMessage(IE_SETTINGSWINDOW_SETTINGSDESKBAR)));
	// read options
	if (Lock()) {
		OptionsToDialog();

		// save old options:
		saved_options = slayer->options;

		// if necessary, disable docking button
		if (slayer->docked) {
			deskbarButton->SetEnabled(false);
		}
		Unlock();
	}

	Show();
}


SettingsWindow::~SettingsWindow(void)
{
}


// Handling of user interface and other events
void SettingsWindow::MessageReceived(BMessage *message)
{

	switch(message->what){
		case IE_SETTINGSWINDOW_SETTINGSCURRENTWORKSPACE:
			slayer->options.workspace_activation = Options::current_workspace; break;
		case IE_SETTINGSWINDOW_SETTINGSSAVEDWORKSPACE:
			slayer->options.workspace_activation = Options::saved_workspace; break;
		case IE_SETTINGSWINDOW_SETTINGSALLWORKSPACES:
			slayer->options.workspace_activation = Options::all_workspaces; break;
		case IE_SETTINGSWINDOW_SETTINGSSAVEWINDOW:	// 'SettingsSaveWindow' is pressed...
			slayer->options.save_wind_pos = 
				save_window->Value() ? true : false;
			break;
		case IE_SETTINGSWINDOW_SETTINGSSAVEWORKSPACE:	// 'SettingsSaveWorkspace' is pressed...
			slayer->options.save_workspace = 
				save_workspace->Value() ? true : false;
			break;
		case IE_SETTINGSWINDOW_SETTINGSREFRESH:	// Enter is pressed in 'SettingsRefresh'...
			SetRefreshDelay();
			break;
		case IE_SETTINGSWINDOW_SETTINGSREVERT:
			slayer->options = saved_options;
			OptionsToDialog();
			// well this fucker..
			PostMessage(IE_SETTINGSWINDOW_SETTINGSREFRESH);
			break;
		case IE_SETTINGSWINDOW_SETTINGSDESKBAR:
		{
			// Dock to deskbar
			MiniSlayer *replicant =
				new MiniSlayer();
			BMessage archiveMsg(B_ARCHIVED_OBJECT);
			replicant->Archive(&archiveMsg);
			BMessenger messenger("application/x-vnd.Be-TSKB", -1, NULL);
			messenger.SendMessage(&archiveMsg);
			be_app->PostMessage(B_QUIT_REQUESTED);						
		}
			break;
		default:
			BWindow::MessageReceived(message);

			break;
	}

}

void SettingsWindow::Quit()
{
	SetRefreshDelay();
	
	BWindow::Quit();
}

void SettingsWindow::OptionsToDialog()
{
	char str[21];
	sprintf(str, "%g", ((float)slayer->options.refresh)/1000);
	refresh->SetText(str);
	
	if (slayer->options.save_wind_pos)
		save_window->SetValue(1);
	else
		save_window->SetValue(0);
		
	save_workspace->SetValue(slayer->options.save_workspace ? 1 : 0);
	
	switch (slayer->options.workspace_activation) {
	case Options::current_workspace:
		current_workspace->SetValue(1); break;
	case Options::all_workspaces:
		all_workspaces->SetValue(1); break;
	default:
		saved_workspace->SetValue(1); break;
	}
}

void SettingsWindow::SetRefreshDelay()
{
	int32 ref = (int32)(atof(refresh->Text()) * 1000);
	if (ref < 0) ref = 0;
	// lock the window so that it is certain that the threads
	// aren't updating when we do kill
	if (slayer->mainWindow->Lock()) {
		// kill updater
		slayer->mainWindow->refreshThread->Kill();
		slayer->options.refresh = ref;
		// run the updater again
		slayer->mainWindow->refreshThread->Go();
		slayer->mainWindow->Unlock();
	}
}
