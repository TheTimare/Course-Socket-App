#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace CourseSocketAppClient;

System::Void ClientWindow::itemConnect_Click(System::Object^  sender, System::EventArgs^  e) {
	ConnectWindow^ connect = gcnew ConnectWindow(this);
	connect->ShowDialog();
	MessageBox::Show(name + " " + ip->ToString() + " " + Convert::ToString(port), "������",
		MessageBoxButtons::OK, MessageBoxIcon::Information);
}