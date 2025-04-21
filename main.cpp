/*
Program: ESPFileXfer
File: main.cpp
Author: Listerine-debug
Description: This file contains the main entry point for the ESPFileXfer application, 
initializing the GUI and setting up the main frame.
License: Unlicense
Date of Last Implementation: 2025-04-21 , YYYY-MM-DD
*/

#include "gui.h"

wxIMPLEMENT_APP(ESPFileXfer);
wxDECLARE_APP(ESPFileXfer);

bool ESPFileXfer::OnInit()
{
	mainFrame* frame = new mainFrame("ESPFilexfer");
	frame->SetSizeHints(400, -1, 400, -1);
	frame -> Show(true);
	return true;
}