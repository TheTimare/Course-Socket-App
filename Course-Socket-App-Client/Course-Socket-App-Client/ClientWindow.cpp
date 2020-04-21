#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace CourseSocketAppClient;

ClientWindow::ClientWindow() {
	InitializeComponent();
	setChatWorking(false);
}

/*---*/

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
		// ������������ � ���������� �����
		messageSocket->Connect(ipPoint);
		//�������� ������� �����
		Task^ messageListener = gcnew Task(gcnew Action(this, &ClientWindow::startMessageReceiving));
		messageListener->Start();

		array<unsigned char>^ nameData = Encoding::Unicode->GetBytes(name);
		messageSocket->Send(nameData);

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
		String^ msg = "&disconnect";
		array<unsigned char>^ sendDisconnect = Encoding::Unicode->GetBytes(msg);
		messageSocket->Send(sendDisconnect);

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

/*---*/

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
}

void ClientWindow::setMessage(String^ message) {
	textBoxMessage->Text = message;
}

/*---*/

void ClientWindow::startMessageReceiving() {
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ClientWindow::addChatMessage);
	while (true) {
		try {
			array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // ����� ��� ������
			StringBuilder^ builder = gcnew StringBuilder();
			int bytes = 0; // ���������� ���������� ����

			do { //�������� �����
				bytes = messageSocket->Receive(receiveData);
				builder->Append(Encoding::Unicode->GetString(receiveData, 0, bytes));
			} while (messageSocket->Available > 0);

			this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::addChatMessage), builder->ToString());
		}
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054 && ex->ErrorCode != 10053) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "������",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			break;
		}
		catch (ObjectDisposedException^ ex) { break; }
	}
}

void ClientWindow::addChatMessage(String^ message) {
	this->textBoxChat->Text += message;
}