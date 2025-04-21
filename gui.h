/*
Program: ESPFileXfer
File: gui.h
Author: Listerine-debug
Description: This file contains the declarations for the GUI components of ESPFileXfer,
a file transfer application for ESP32 and ESP8266 microcontrollers.
License: Unlicense
Date of Last Implementation: 2025-04-21 , YYYY-MM-DD
*/


#ifndef _GUI_H_
#define _GUI_H_

#include "wx/wx.h"
#include "asio.hpp"
#include "memory"
#include "Windows.h"
#include "string"
#include "iostream"
#include "thread"
#include "fstream"
#include "wx/progdlg.h"
#include "wx/filedlg.h"

using asio::ip::tcp;

class ESPFileXfer : public wxApp
{
public:
	virtual bool OnInit();
};

class mainFrame : public wxFrame
{
public:
	mainFrame(const wxString& title);
private:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnScan(wxCommandEvent& event);
	void OnConnect(wxCommandEvent& event);
	void OnConnectWiFi(wxCommandEvent& event);
	void OnDetail(wxCommandEvent& event);
	void OnCode(wxCommandEvent& event);
	wxPanel* mainPanel;
	wxBoxSizer* mainSizer;
	std::vector<std::pair<wxCheckBox*, std::string>> deviceList;
};

class aboutESPfileXfer : public wxDialog
{
public:
	aboutESPfileXfer(const wxString& title);
};

class helpESPfileXfer : public wxDialog
{
public:
	helpESPfileXfer(const wxString& title);
};

class detailsESPfileXfer : public wxDialog
{
public:
	detailsESPfileXfer(const std::string& portName);
};

class arduinoCode : public wxDialog
{
public:
	arduinoCode(const wxString& title);
};

class serialFrame : public wxFrame
{
public:
	serialFrame(const std::string& portName);
private:
	void OnClear(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);
	void OnExtract(wxCommandEvent& event);
	void asioListening();

	std::string namePort;
	std::unique_ptr<asio::io_context> ioContext;
	std::unique_ptr<asio::serial_port> serialPort;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* inputBox;
	wxTextCtrl* chatLog;
};

class wifiSerialFrame : public wxFrame
{
public:
	wifiSerialFrame(const std::string& ipAddress, const std::string& port);

private:
	void OnClear(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);
	void OnExtract(wxCommandEvent& event);
	void OnExtractTimer(wxTimerEvent& event);
	void asioListening();
	//void cancelListening();

	std::atomic<bool> asioListeningActive = false;

	std::string serverIp;
	std::string serverPort;

	std::unique_ptr<asio::io_context> ioContext;
	std::unique_ptr<asio::ip::tcp::socket> socket;
	std::string extractFilePath;
	std::unique_ptr<std::ofstream> extractFileStream;
	std::unique_ptr<wxProgressDialog> extractProgressDialog;
	std::ofstream outFile;
	/*wxDialog* processingDialog = nullptr;
	wxTimer* extractionTimer = nullptr;
	bool extractionInProgress = false;*/

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* inputBox;
	wxTextCtrl* chatLog;

	const uint8_t EXTRACT = 0x00;
	const uint8_t HANDSHAKE = 0x01;
	const uint8_t FAILURE = 0x02;
	const uint8_t SUCCESS = 0x03;
};


enum externalID
{
	ID_SCAN_DEVICES = wxID_HIGHEST + 1,
	ID_DEVICE_DETAILS,
	ID_DEVICE_LIST,
	ID_DEVICE_CONNECT,
	ID_DEVICE_WIFI,
	ID_SEND,
	ID_EXTRACT,
	ID_CODE
};

#endif// _GUI_H_