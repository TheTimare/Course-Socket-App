#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace CourseSocketAppClient;

ClientWindow::ClientWindow() {
	InitializeComponent();
	AutoScaleDimensions = System::Drawing::SizeF(96, 96);

	setChatWorking(false);
	addChatMessage("Hi there. To connect to the server, open the \"server\" tab above and click \"Connect\".\r\n");
}

/*---*/

void ClientWindow::itemConnect_Click(System::Object^  sender, System::EventArgs^  e) {
	inputSuccess = false;
	ConnectWindow^ connect = gcnew ConnectWindow(this);
	connect->ShowDialog();
	if (!inputSuccess) return;

	connectToTheServer();

	if (connectSuccess) {
		setChatWorking(true);
		addChatMessage("You are connected to the server!\r\n");
	}
}

void ClientWindow::connectToTheServer() {
	try {
		IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port);
		messageSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		// подключаемся к удаленному хосту
		messageSocket->Connect(ipPoint);
		//начинаем слушать хоста
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
		array<unsigned char>^ sendCommand = Encoding::Unicode->GetBytes(msg);
		messageSocket->Send(sendCommand);
		addChatMessage("You have been disconnected...\r\n");

		messageSocket->Close();
	} 
	catch (SocketException^ ex) {
		MessageBox::Show(ex->ErrorCode + ": " + ex->Message, "Error",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		if (ex->ErrorCode == 10054) {
			addChatMessage("You have been disconnected...\r\n");
		}
	}
}

void ClientWindow::setChatWorking(bool toStart) {
	if (toStart){
		itemDisconnect->Available = true;
		itemConnect->Available = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::Window;
		textBoxMessage->ReadOnly = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
		buttonSendMsg->Enabled = true;
	}
	else {
		itemDisconnect->Available = false;
		itemConnect->Available = true;
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
	msg = msg->Trim();
	msg = msg->Replace("&", "");
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
			array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // буфер для ответа
			StringBuilder^ builder = gcnew StringBuilder();
			int bytes = 0; // количество полученных байт

			do { //получаем ответ
				bytes = messageSocket->Receive(receiveData);
				builder->Append(Encoding::Unicode->GetString(receiveData, 0, bytes));
			} while (messageSocket->Available > 0);

			this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::addChatMessage), builder->ToString());
		}
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			this->BeginInvoke(gcnew EventDelegate(this, &ClientWindow::itemDisconnect_Click), gcnew Object, gcnew EventArgs);
			break;
		}
		catch (ObjectDisposedException^ ex) { break; }
	}
}

void ClientWindow::addChatMessage(String^ message) {
	this->textBoxChat->Text += message;
}