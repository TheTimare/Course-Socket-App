#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace CourseSocketAppClient;

ConnectWindow::ConnectWindow(ClientWindow^ client) {
	InitializeComponent();
	mainWindow = client;
	AutoScaleDimensions = System::Drawing::SizeF(96, 96);
}

System::Void ConnectWindow::buttonConnect_Click(System::Object^  sender, System::EventArgs^  e) {
	try {
		mainWindow->setPort(Convert::ToInt32(textBoxPort->Text));
		if (mainWindow->getPort() < 1024 || mainWindow->getPort() > 49151)
			throw gcnew Exception();
		mainWindow->setIP(IPAddress::Parse(textBoxIP->Text));
		mainWindow->setName(textBoxName->Text);
		mainWindow->setInputSuccess(true);
	}
	catch (...) {
		MessageBox::Show("Incorrect input. Try again...", "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	this->~ConnectWindow();
}