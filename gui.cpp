/*
Program: ESPFileXfer
File: gui.cpp
Author: Listerine-debug
Description: This file contains the implementation of the GUI for ESPFileXfer, 
a file transfer application for ESP32 and ESP8266 microcontrollers.
License: Unlicense
Date of Last Implementation: 2025-04-21 , YYYY-MM-DD
*/

#include "gui.h"


mainFrame::mainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 800), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX)
{
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_EXIT, "&Quit\tCtrl-Q", "Quit the application");
	fileMenu->Append(wxID_ABOUT, "&About\tCtrl-A", "Show about dialog");
	fileMenu->Append(wxID_HELP, "&Help\tCtrl-H", "Show help dialog");
	fileMenu->Append(ID_CODE, "&Arduino Code\tCtrl-K", "Show Arduino code for ESPFileXfer");

	wxMenu* toolMenu = new wxMenu;
	toolMenu->Append(ID_SCAN_DEVICES, "&Scan Devices\tCtrl-S", "Scan for active COM ports");
	toolMenu->Append(ID_DEVICE_DETAILS, "&Device Details\tCtrl-D", "Show selected device details");
	toolMenu->Append(ID_DEVICE_CONNECT, "&Connect via Serial\tCtrl-C", "Connect to a microcontrollers COM port");
	toolMenu->Append(ID_DEVICE_WIFI, "&Connect via WiFi\tCtrl-W", "Connect to a microcontrollers network");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, "&App");
	menuBar->Append(toolMenu, "&Tools");
	SetMenuBar(menuBar);

	mainPanel = new wxPanel(this, ID_DEVICE_LIST);
	mainSizer = new wxBoxSizer(wxVERTICAL);
	mainPanel->SetSizer(mainSizer);

	Bind(wxEVT_MENU, &mainFrame::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &mainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &mainFrame::OnHelp, this, wxID_HELP);
	Bind(wxEVT_MENU, &mainFrame::OnScan, this, ID_SCAN_DEVICES);
	Bind(wxEVT_MENU, &mainFrame::OnDetail, this, ID_DEVICE_DETAILS);
	Bind(wxEVT_MENU, &mainFrame::OnConnect, this, ID_DEVICE_CONNECT);
	Bind(wxEVT_MENU, &mainFrame::OnConnectWiFi, this, ID_DEVICE_WIFI);
	Bind(wxEVT_MENU, &mainFrame::OnCode, this, ID_CODE);

	CreateStatusBar();
	SetStatusText("Welcome to ESPFileXfer!");
}

aboutESPfileXfer::aboutESPfileXfer(const wxString& title)
	: wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxString aboutText = wxT(
		"=== About ESPFileXfer ===\n\n"
		"ESPFileXfer is a robust and efficient file transfer application designed for developers and hobbyists working with ESP32 and ESP8266 microcontroller systems. "
		"This application allows you to wirelessly extract files from these microcontrollers over a Wi-Fi connection. By creating a TCP connection to an ESP32 or ESP8266 device running in Access Point (AP) mode, ESPFileXfer ensures smooth and secure communication between your PC and the embedded system.\n\n"

		"Key Features:\n"
		"• Wireless file transfer over TCP/IP, supporting both ESP32 and ESP8266 microcontrollers.\n"
		"• Simple, intuitive interface that enables effortless file extraction.\n"
		"• Reliable handshake mechanism and error handling for secure data transmission.\n"
		"• Supports binary file streaming, ideal for log files, configuration files, and sensor data extraction.\n\n"

		"Whether you're working on a hobby project, debugging sensor data, or managing configurations on your embedded systems, ESPFileXfer makes the process simple and reliable, without requiring complex software setups. This tool is optimized for quick, seamless file extraction, making it an essential companion for any embedded system developer.\n\n"

		"Compatibility:\n"
		"• ESP32 and ESP8266 devices (running in Access Point mode).\n"
		"• Windows OS for the host PC connection.\n\n"

		"For more information, contact me(Listerine-debug) on Github on my Github account."
	);

	wxStaticText* aboutLabel = new wxStaticText(panel, wxID_ANY, aboutText, wxDefaultPosition, wxSize(560, -1), wxALIGN_LEFT);
	aboutLabel->Wrap(560);

	panelSizer->Add(aboutLabel, 1, wxALL | wxEXPAND, 10);
	panel->SetSizer(panelSizer);

	mainSizer->Add(panel, 1, wxALL | wxEXPAND, 10);
	SetSizerAndFit(mainSizer);
}

arduinoCode::arduinoCode(const wxString& title)
	: wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 500))
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxString codeText = wxT(
		"// Simplified ESP32 File Transfer Example\n"
		"#include <WiFi.h>\n"
		"#include <SD.h>\n\n"
		"const char* ssid = \"ESP32_AP\";\n"
		"const char* password = \"12345678\";\n\n"
		"WiFiServer server(8080);\n"
		"WiFiClient client;\n\n"
		"const uint8_t CMD_HANDSHAKE = 0x01;\n"
		"const uint8_t CMD_EXTRACT   = 0x00;\n"
		"const uint8_t CMD_FAIL      = 0x02;\n"
		"const uint8_t CMD_SUCCESS   = 0x03;\n\n"
		"const char* filePath = \"/data.txt\";\n\n"
		"void setup() {\n"
		"  Serial.begin(115200);\n"
		"  SD.begin();\n"
		"  WiFi.softAP(ssid, password);\n"
		"  server.begin();\n"
		"}\n\n"
		"void loop() {\n"
		"  if (!client || !client.connected()) {\n"
		"    client = server.available();\n"
		"    return;\n"
		"  }\n\n"
		"  if (client.available()) {\n"
		"    uint8_t cmd = client.read();\n\n"
		"    if (cmd == CMD_HANDSHAKE) {\n"
		"      client.write(CMD_HANDSHAKE);\n\n"
		"      if (waitForCommand(CMD_EXTRACT)) {\n"
		"        sendFile();\n"
		"      } else {\n"
		"        client.write(CMD_FAIL);\n"
		"      }\n"
		"    }\n"
		"  }\n"
		"}\n\n"
		"bool waitForCommand(uint8_t expected) {\n"
		"  unsigned long start = millis();\n"
		"  while (millis() - start < 3000) {\n"
		"    if (client.available() && client.read() == expected) return true;\n"
		"    delay(10);\n"
		"  }\n"
		"  return false;\n"
		"}\n\n"
		"void sendFile() {\n"
		"  File file = SD.open(filePath);\n"
		"  if (!file) {\n"
		"    client.write(CMD_FAIL);\n"
		"    return;\n"
		"  }\n\n"
		"  while (file.available()) {\n"
		"    uint8_t buffer[128];\n"
		"    size_t len = file.read(buffer, sizeof(buffer));\n"
		"    client.write(buffer, len);\n"
		"    delay(5);\n"
		"  }\n\n"
		"  file.close();\n"
		"  client.write(CMD_SUCCESS);\n"
		"}\n"
	);
	wxStaticText* codeLabel = new wxStaticText(panel, wxID_ANY, codeText, wxDefaultPosition, wxSize(560, -1), wxALIGN_LEFT);
	codeLabel->Wrap(560);

	panelSizer->Add(codeLabel, 1, wxALL | wxEXPAND, 10);
	panel->SetSizer(panelSizer);

	mainSizer->Add(panel, 1, wxALL | wxEXPAND, 10);
	SetSizerAndFit(mainSizer);
}

helpESPfileXfer::helpESPfileXfer(const wxString& title)
	: wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 600))
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxString helpText = wxT(
		"=== ESPFileXfer Help ===\n\n"
		"ESPFileXfer is designed to be simple and intuitive, but here are some helpful tips and shortcuts for maximizing its use.\n\n"

		"=== Program Shortcuts ===\n"
		"• Ctrl + S   : Scan for available COM ports to connect to ESP32 or ESP8266 devices.\n"
		"• Ctrl + D   : Show details of the selected device from the list.\n"
		"• Ctrl + C   : Connect to the selected device via serial communication.\n"
		"• Ctrl + W   : Connect to an ESP32 or ESP8266 device over Wi-Fi by entering the IP address and port.\n"
		"* Ctrl + Q   : Quit the application.\n"
		"• Ctrl + H   : Open this Help dialog.\n"
		"• Ctrl + K   : Open the Arduino code dialog to view the code used for ESPFileXfer.\n"
		"• Ctrl + A   : Open the About dialog to learn more about the application.\n\n"

		"=== How to Connect to an ESP32 or ESP8266 Access Point on Windows ===\n\n"

		"1. Power on your ESP32 or ESP8266 device:\n"
		"   Ensure that the microcontroller is running in Access Point (AP) mode. The device will create a Wi-Fi network.\n\n"

		"2. Connect your PC to the device's Wi-Fi network:\n"
		"   - Click on the Wi-Fi icon in the system tray.\n"
		"   - Look for the Wi-Fi network named 'ESP32_AP' (or any custom SSID).\n"
		"   - Select the network and enter the password.\n\n"

		"3. Launch ESPFileXfer:\n"
		"   Open ESPFileXfer on your PC. Once connected to Wi-Fi, you can initiate file extraction.\n\n"

		"4. Open Connection:\n"
		"   Click 'Open Connection'. The software will detect the AP device.\n\n"

		"5. Extract Files:\n"
		"   Click 'Extract'. Files will be transferred from the ESP to your PC.\n\n"

		"=== Troubleshooting ===\n"
		"• Ensure AP mode and correct credentials.\n"
		"• Check the Wi-Fi signal and power.\n"
		"• Reconnect or restart if needed.\n\n"

		"For more help, do not consult the documentation or support. (because there is none, sorry : (, I did this project in a rush.)"
	);

	wxStaticText* helpLabel = new wxStaticText(panel, wxID_ANY, helpText, wxDefaultPosition, wxSize(560, -1), wxALIGN_LEFT);
	helpLabel->Wrap(560);

	panelSizer->Add(helpLabel, 1, wxALL | wxEXPAND, 10);
	panel->SetSizer(panelSizer);

	mainSizer->Add(panel, 1, wxALL | wxEXPAND, 10);
	SetSizerAndFit(mainSizer);
}


detailsESPfileXfer::detailsESPfileXfer(const std::string& portName)
	: wxDialog(NULL, wxID_ANY, wxString::Format("Details - %s", portName), wxDefaultPosition, wxSize(400, 300))
{
	wxStaticText* text = new wxStaticText(this, wxID_ANY, "Basic ESPFileXfer Details dialog", wxPoint(20, 20));
	wxStaticText* text1 = new wxStaticText(this, wxID_ANY, "This feature has yet to be implemented", wxPoint(20, 40));
};

void mainFrame::OnQuit(wxCommandEvent& event)
{
	Close(true);
}

void mainFrame::OnHelp(wxCommandEvent& event)
{
	helpESPfileXfer* helpDialog = new helpESPfileXfer("Help ESPFileXfer");
	helpDialog->Show(true);
}

void mainFrame::OnAbout(wxCommandEvent& event)
{
	aboutESPfileXfer* aboutDialog = new aboutESPfileXfer("About ESPFileXfer");
	aboutDialog->Show(true);
}

void mainFrame::OnCode(wxCommandEvent& event)
{
	arduinoCode* codeDialog = new arduinoCode("Arduino Code for ESPFileXfer");
	codeDialog->Show(true);
}

void mainFrame::OnDetail(wxCommandEvent& event)
{
	for (const auto& port : deviceList)
	{
		if (port.first->IsChecked())
		{
			detailsESPfileXfer* detailsDialog = new detailsESPfileXfer(port.second);
			detailsDialog->Show(true);
		}
	}
}

void mainFrame::OnConnect(wxCommandEvent& event)
{
	//wxMessageBox("Device connect not implemented yet!", "Device Connect", wxOK | wxICON_INFORMATION);
	for (const auto& port : deviceList)
	{
		if (port.first->IsChecked())
		{
			serialFrame* serial = new serialFrame(port.second);
			serial->Show(true);
		}
	}
}

void mainFrame::OnConnectWiFi(wxCommandEvent& event)
{
	wxTextEntryDialog ipDialog(this, "Enter Microcontroller IP Address:", "WiFi Connection");
	if (ipDialog.ShowModal() == wxID_OK)
	{
		std::string ip = ipDialog.GetValue().ToStdString();

		wxTextEntryDialog portDialog(this, "Enter Port Number:", "WiFi Connection");
		if (portDialog.ShowModal() == wxID_OK)
		{
			std::string port = portDialog.GetValue().ToStdString();

			wifiSerialFrame* wifiFrame = new wifiSerialFrame(ip, port);
			wifiFrame->Show(true);
		}
	}
}


void mainFrame::OnScan(wxCommandEvent& event)
{
	wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Clear(true);
	deviceList.clear();
	for (int i = 1; i <= 256; i++)
	{
		std::wstring portName = L"\\\\.\\COM" + std::to_wstring(i);
		HANDLE devicePort = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

		if (devicePort != INVALID_HANDLE_VALUE)
		{
			CloseHandle(devicePort);
			std::string portStr = "COM" + std::to_string(i);
			wxCheckBox* comCheckBox = new wxCheckBox(mainPanel, wxID_ANY, wxString::Format("COM %d", i));
			deviceList.emplace_back(comCheckBox, portStr);
			listSizer->Add(comCheckBox, 0, wxALL, 5); 
		}
	}
	mainSizer->Add(listSizer, 1, wxEXPAND | wxALL, 5);
	mainPanel->Layout();
}


/* ------------------------------------------------------------------------------------------------------------------------------ */ 

serialFrame::serialFrame(const std::string& portName)
	: wxFrame(NULL, wxID_ANY, wxString::Format("Serial Communication - %s", portName), wxDefaultPosition, wxSize(800, 650)), namePort(portName)
{
	//wxStaticText* text = new wxStaticText(this, wxID_ANY, "Basic ESPFileXfer Serial Communication", wxPoint(20, 20));

	chatLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(800, 500), wxTE_MULTILINE | wxTE_READONLY);
	inputBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(800, 30));
	wxButton* sendButton = new wxButton(this, ID_SEND, "Send");
	wxButton* extractButton = new wxButton(this, ID_EXTRACT, "Extract");
	wxButton* exitButton = new wxButton(this, wxID_EXIT, "Exit");
	wxButton* clearButton = new wxButton(this, wxID_CLEAR, "Clear");

	mainSizer->Add(chatLog, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(inputBox, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(sendButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(extractButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(clearButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(exitButton, 0, wxALIGN_CENTER | wxALL, 5);
	SetSizer(mainSizer);

	sendButton->Bind(wxEVT_BUTTON, &serialFrame::OnSend, this);
	exitButton->Bind(wxEVT_BUTTON, &serialFrame::OnQuit, this);
	clearButton->Bind(wxEVT_BUTTON, &serialFrame::OnClear, this);
	extractButton->Bind(wxEVT_BUTTON, &serialFrame::OnExtract, this);

	try
	{
		ioContext = std::make_unique<asio::io_context>();
		serialPort = std::make_unique<asio::serial_port>(*ioContext, namePort);
		serialPort->set_option(asio::serial_port_base::baud_rate(115200)); // intended for espressif esp32 esp8266

		asioListening();
	}
	catch (const std::exception& e)
	{
		wxMessageBox(wxString("Error opening port: ") + e.what(), "Serial Error", wxOK | wxICON_ERROR);
		this->Close(true);
		return;
	}
	std::thread([this]() { ioContext->run(); }).detach();

}

void serialFrame::OnSend(wxCommandEvent& event)
{
	std::string message = inputBox->GetValue().ToStdString() + "\n";
	try
	{
		asio::write(*serialPort, asio::buffer(message));
		chatLog->AppendText("You: " + inputBox->GetValue() + "\n");
		inputBox->Clear();
	}
	catch (const std::exception& e)
	{
		wxMessageBox(wxString("Write Error: ") + e.what(), "Serial Error", wxOK | wxICON_ERROR);
	}
}

void serialFrame::OnExtract(wxCommandEvent& event)
{
	wxMessageBox("Extract not implemented yet!", "Extract", wxOK | wxICON_INFORMATION);
}

void serialFrame::OnClear(wxCommandEvent& event)
{
	try
	{
		chatLog->Clear();
	}
	catch(std::exception& e)
	{
		wxMessageBox(wxString("Command Error: ") + e.what(), "Serial Error", wxOK | wxICON_ERROR);
	}
}

void serialFrame::OnQuit(wxCommandEvent& event)
{
	if (serialPort && serialPort->is_open())
	{
		serialPort->cancel();
		serialPort->close();
	}
	if (ioContext)
	{
		ioContext->stop();
	}
	this->Close(true);
}

void serialFrame::asioListening()
{
	auto buf = std::make_shared<std::array<char, 128>>();

	serialPort->async_read_some(asio::buffer(*buf),
		[this, buf](const asio::error_code& error, std::size_t len)
		{
			if (!error)
			{
				std::string response(buf->data(), len);
				wxTheApp->CallAfter([=]()
					{
						chatLog->AppendText(response);
					});

				// Continue listening
				asioListening();
			}
			else
			{
				wxTheApp->CallAfter([=]()
					{
						wxMessageBox(wxString("Read Error: ") + error.message(), "Serial Error", wxOK | wxICON_ERROR);
					});
			}
		});
}

/* ------------------------------------------------------------------------------------------------------------------------------ */

wifiSerialFrame::wifiSerialFrame(const std::string& ipAddress, const std::string& port)
	: wxFrame(NULL, wxID_ANY, wxString::Format("WiFi Serial Communication - %s:%s", ipAddress, port),
		wxDefaultPosition, wxSize(800, 650)), serverIp(ipAddress), serverPort(port)
{
	// Create UI components
	chatLog = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(800, 500), wxTE_MULTILINE | wxTE_READONLY);
	inputBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(800, 30));
	wxButton* sendButton = new wxButton(this, ID_SEND, "Send");
	wxButton* extractButton = new wxButton(this, ID_EXTRACT, "Extract");
	wxButton* exitButton = new wxButton(this, wxID_EXIT, "Exit");
	wxButton* clearButton = new wxButton(this, wxID_CLEAR, "Clear");

	mainSizer->Add(chatLog, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(inputBox, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(sendButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(extractButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(clearButton, 0, wxALIGN_CENTER | wxALL, 5);
	mainSizer->Add(exitButton, 0, wxALIGN_CENTER | wxALL, 5);
	SetSizer(mainSizer);

	// Bind events
	sendButton->Bind(wxEVT_BUTTON, &wifiSerialFrame::OnSend, this);
	exitButton->Bind(wxEVT_BUTTON, &wifiSerialFrame::OnQuit, this);
	clearButton->Bind(wxEVT_BUTTON, &wifiSerialFrame::OnClear, this);
	extractButton->Bind(wxEVT_BUTTON, &wifiSerialFrame::OnExtract, this);

	// Attempt to connect to the server
	try
	{
		// Initialize io_context and socket
		ioContext = std::make_unique<asio::io_context>();
		tcp::endpoint endpoint(asio::ip::make_address(serverIp), std::stoi(serverPort));

		socket = std::make_unique<tcp::socket>(*ioContext);
		socket->connect(endpoint);

		// Start asynchronous listening for incoming data
		asioListening();
	}
	catch (const std::exception& e)
	{
		wxMessageBox(wxString("Error opening port: ") + e.what(), "WiFi Serial Error", wxOK | wxICON_ERROR);
		this->Close(true);
		return;
	}
}

void wifiSerialFrame::OnSend(wxCommandEvent& event)
{
	std::string message = inputBox->GetValue().ToStdString() + "\n";
	try
	{
		// Send the message to the server
		asio::write(*socket, asio::buffer(message));

		// Display the sent message in the chat log
		chatLog->AppendText("You: " + inputBox->GetValue() + "\n");
		inputBox->Clear();
	}
	catch (const std::exception& e)
	{
		wxMessageBox(wxString("Write Error: ") + e.what(), "WiFi Serial Error", wxOK | wxICON_ERROR);
	}
}

void wifiSerialFrame::OnExtract(wxCommandEvent& event)
{
	try
	{
		asioListeningActive = false;
		asioListening();
		ioContext->restart();

		wxFileDialog saveFileDialog(
			this, "Save Extracted File", "", "extracted.txt",
			"Text files (*.txt)|*.txt|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;

		std::ofstream outFile(saveFileDialog.GetPath().ToStdString(), std::ios::binary);
		if (!outFile)
		{
			wxMessageBox("Failed to open file for writing.", "Error", wxOK | wxICON_ERROR);
			return;
		}

		// Handshake
		char handshakeCmd = static_cast<char>(HANDSHAKE);
		asio::write(*socket, asio::buffer(&handshakeCmd, 1));

		char response = 0;
		asio::read(*socket, asio::buffer(&response, 1));
		if (response != static_cast<char>(HANDSHAKE))
		{
			wxMessageBox("Handshake failed. Extraction aborted.", "Failure", wxOK | wxICON_ERROR);
			outFile.close();
			return;
		}

		char extractCmd = static_cast<char>(EXTRACT);
		asio::write(*socket, asio::buffer(&extractCmd, 1));

		// Show processing dialog
		wxDialog processingDialog(this, wxID_ANY, "Processing", wxDefaultPosition, wxSize(300, 100));
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		wxStaticText* label = new wxStaticText(&processingDialog, wxID_ANY, "Extracting, please wait...");
		sizer->Add(label, 1, wxALIGN_CENTER | wxALL, 10);
		processingDialog.SetSizerAndFit(sizer);
		processingDialog.Show();
		processingDialog.Update();

		// Blocking read loop until SUCCESS is received
		char buffer[128];
		bool foundSuccess = false;

		while (!foundSuccess)
		{
			std::size_t len = socket->read_some(asio::buffer(buffer, sizeof(buffer)));

			for (std::size_t i = 0; i < len; ++i)
			{
				if (static_cast<unsigned char>(buffer[i]) == SUCCESS)
				{
					// Write everything before SUCCESS
					if (i > 0)
						outFile.write(buffer, i);
					foundSuccess = true;
					break;
				}
			}

			if (!foundSuccess)
			{
				outFile.write(buffer, len);
			}
		}
		outFile.flush();
		outFile.close();
		processingDialog.Destroy();
		wxMessageBox("Extraction complete!", "Success", wxOK | wxICON_INFORMATION);

		// Resume listening
		asioListeningActive = true;
		asioListening();
	}
	catch (const std::exception& e)
	{
		if (outFile.is_open()) outFile.close();
		wxMessageBox(wxString("Exception: ") + e.what(), "Error", wxOK | wxICON_ERROR);
	}
}

void wifiSerialFrame::OnClear(wxCommandEvent& event)
{
	try
	{
		chatLog->Clear();
	}
	catch (std::exception& e)
	{
		wxMessageBox(wxString("Command Error: ") + e.what(), "WiFi Serial Error", wxOK | wxICON_ERROR);
	}
}

void wifiSerialFrame::OnQuit(wxCommandEvent& event)
{
	if (socket && socket->is_open())
	{
		socket->close();
	}
	if (ioContext)
	{
		ioContext->stop();
	}
	this->Close(true);
}

void wifiSerialFrame::asioListening()
{
	if (!asioListeningActive) return;

	auto buf = std::make_shared<std::array<char, 128>>();

	socket->async_read_some(asio::buffer(*buf),
		[this, buf](const asio::error_code& error, std::size_t len)
		{
			if (!error)
			{
				std::string response(buf->data(), len);

				wxTheApp->CallAfter([=]() {
					chatLog->AppendText(response);
					});

				asioListening(); // Continue listening
			}
			else
			{
				wxTheApp->CallAfter([=]() {
					wxMessageBox(wxString("Read Error: ") + error.message(), "WiFi Serial Error", wxOK | wxICON_ERROR);
					});
			}
		});
}