#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace CourseSocketAppClient;

ClientWindow::ClientWindow() {
	InitializeComponent();
	setChatWorking(false);
}

void ClientWindow::itemConnect_Click(System::Object^  sender, System::EventArgs^  e) {
	inputSuccess = false;
	ConnectWindow^ connect = gcnew ConnectWindow(this);
	connect->ShowDialog();
	if (!inputSuccess) return;

	clientConnect();

	if (connectSuccess) {
		setChatWorking(true);
	}
}

void ClientWindow::clientConnect() {
	try {
		IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port);
		messageSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		// подключаемся к удаленному хосту
		messageSocket->Connect(ipPoint);

		array<unsigned char>^ sendData = Encoding::Unicode->GetBytes(name);
		messageSocket->Send(sendData);

		connectSuccess = true;
	}
	catch (Exception^ e) {
		MessageBox::Show("Connection error!\n" + e->Message, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	};
}

void ClientWindow::itemDisconnect_Click(System::Object^  sender, System::EventArgs^  e){
	setChatWorking(false);
	try {
		//messageSocket->Shutdown(SocketShutdown::Both);
		messageSocket->Close();
	} 
	catch (Exception^ ex) {
		MessageBox::Show("Close connection error." + ex->Message, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}

void ClientWindow::setChatWorking(bool toStart) {
	if (toStart){
		itemDisconnect->Available = true;
		textBoxMessage->BackColor = System::Drawing::SystemColors::Window;
		textBoxMessage->ReadOnly = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
		buttonSendMsg->Enabled = true;
	}
	else {
		itemDisconnect->Available = false;
		connectSuccess = false;
		textBoxMessage->ReadOnly = true;
		textBoxMessage->BackColor = System::Drawing::SystemColors::MenuBar;
		buttonSendMsg->Enabled = false;
	}
}

void ClientWindow::buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e) {
	if (textBoxMessage->Equals("")) {
		return;
	}
	Task^ message = gcnew Task(gcnew Action(this, &ClientWindow::sendMessage));
	message->Start();
}

void ClientWindow::sendMessage() {
	String^ msg = textBoxMessage->Text;
	this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::setMessage), "");

	array<unsigned char>^ sendData = Encoding::Unicode->GetBytes(msg);
	try {
		messageSocket->Send(sendData);
	}
	catch (SocketException^ ex) {
		MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	/*// получаем ответ
	data = new byte[256]; // буфер для ответа
	StringBuilder builder = new StringBuilder();
	int bytes = 0; // количество полученных байт

	do
	{
		bytes = socket.Receive(data, data.Length, 0);
		builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
	} while (socket.Available > 0);
	Console.WriteLine("ответ сервера: " + builder.ToString());*/
}

void ClientWindow::setMessage(String^ message) {
	textBoxMessage->Text = message;
}
