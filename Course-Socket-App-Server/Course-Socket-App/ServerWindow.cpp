#include "ServerWindow.h"

using namespace CourseSocketApp;


ServerWindow::ServerWindow(void) {
	InitializeComponent();
	AutoScaleDimensions = System::Drawing::SizeF(96, 96);

	userDB = gcnew Hashtable();
	chatPool = gcnew List<String^>();
	connected = gcnew List<Socket^>();
	isStarted = false;
	//get pc address
	IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
	for (int i = 0; i < host->AddressList->Length; i++) {
		if (host->AddressList[i]->AddressFamily == AddressFamily::InterNetwork) {
			domainUpDownIPs->Items->Add(host->AddressList[i]);
		}
	}
	if (domainUpDownIPs->Items->Count == 0) {
		button_on_off_server->Enabled = false;
		buttonSetIP->Enabled = false;
		MessageBox::Show("No IPs available to start the server!\n"
			"Check your internet connection.The server cannot be started."
			, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	domainUpDownIPs->SelectedIndex = 0;
	textBoxIP->Text = domainUpDownIPs->SelectedItem->ToString();
	selectedIP = 0;
}

/*-----*/

String^ stripPortIP(String^ endPoint) {
	array<String^>^ splitList = endPoint->Split(':');
	if (splitList->Length > 2) {
		endPoint = IPAddress::Parse(endPoint)->ToString();
	}
	else if (splitList->Length == 2) {
		endPoint = splitList[0];
	}
	return endPoint;
}

void ServerWindow::serverMessage(String^ user, String^ msg) {
	String^ formedMessage = "[" + DateTime::Now.ToShortTimeString() + "] " + user + ": " + msg + "\r\n";
	textBoxChat->AppendText(formedMessage);
	chatPool->Add(formedMessage);
}

void ServerWindow::setSocket(Socket^ mainSocket) {
	this->mainSocket = mainSocket;
}

void ServerWindow::setChatWorking(bool toStart) {
	if (toStart) {
		textBoxPort->ReadOnly = true;
		buttonSetIP->Enabled = false;
		button_on_off_server->Text = "Shutdown Server";
		isStarted = true;
	}
	else {
		textBoxPort->ReadOnly = false;
		buttonSetIP->Enabled = true;
		button_on_off_server->Text = "Start Server";
		isStarted = false;
	}
}

//commands must begin with the symbol '&'
void ServerWindow::sendSystemCommand(Socket^ connection, String^ command) {
	if (command->Length == 0 || command[0] != '&')
		return;
	array<unsigned char>^ data;
	data = Encoding::Unicode->GetBytes(command);
	try {
		connection->Send(data);
	}
	catch (...) {}
}

/*-----*/

void ServerWindow::buttonSetIP_Click(System::Object^  sender, System::EventArgs^  e) {
	selectedIP = domainUpDownIPs->SelectedIndex;
	textBoxIP->Text = domainUpDownIPs->SelectedItem->ToString();
}

void ServerWindow::button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!isStarted) {
		Task^ serverTask = gcnew Task(gcnew Action(this, &ServerWindow::serverStart));
		serverTask->Start();
		setChatWorking(true);
		serverMessage("SYSTEM", "Server is started. Waiting for users...");
	}
	else {
		setChatWorking(false);
		mainSocket->Close();
		for (int i = 0; i < connected->Count; i++) {
			//sendSystemCommand(connected[i], "&server_off");
			connected[i]->Close();
		}
		serverMessage("SYSTEM", "Server is off.");
	}
}

void ServerWindow::serverStart() {
	int port = 8080;
	try {
		port = Convert::ToInt32(textBoxPort->Text);
	}
	catch (...) {
		MessageBox::Show("Неправильно введен порт", "Ошибка",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	
	IPAddress^ ip = (IPAddress^)domainUpDownIPs->Items[selectedIP];
	IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port); // получаем адреса для запуска сокета
	Socket^ listenSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	listenSocket->Bind(ipPoint); // связываем сокет с локальной точкой, по которой будем принимать данные
	listenSocket->Listen(25); // начинаем прослушивание

	this->BeginInvoke(gcnew SocketDelegate(this, &ServerWindow::setSocket), listenSocket);
	while (isStarted) {
		try {
			Socket^ handler = listenSocket->Accept();
			handler->ReceiveTimeout = 0;
			String^ ip = stripPortIP(handler->RemoteEndPoint->ToString());
			if (userDB->ContainsKey(ip)) {
				handler->Close();
			}
			else {
				connected->Add(handler);
				Task^ messageGet = gcnew Task((Action<Object^>^)(gcnew Action<Object^>(this, &ServerWindow::startMessageGetting)), handler);
				Task^ messageSynchronize = gcnew Task((Action<Object^>^)(gcnew Action<Object^>(this, &ServerWindow::startSynchronizeMessages)), handler);
				messageGet->Start();
				messageSynchronize->Start();
			}
		}
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054 && ex->ErrorCode != 10004) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
	}
}

/*-----*/

void ServerWindow::startMessageGetting(Object^ handler) {
	Socket^ localHandler = (Socket^)handler;
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	bool getUserName = true;
	String^ clientIP;
	while (true) {
		try {
			if (localHandler == nullptr) break;

			int bytes = 0; // кол-во полученных байтов
			StringBuilder^ builder = gcnew StringBuilder();
			array<unsigned char>^ data = gcnew array<unsigned char>(256); // буфер для получаемых данных

			do { // получаем сообщение
				bytes = localHandler->Receive(data); //Receive(data);
				builder->Append(Encoding::Unicode->GetString(data, 0, bytes));
			} while (localHandler->Available > 0);
			
			if (builder->ToString() == "&disconnect") break;

			if (getUserName) {
				clientIP = stripPortIP(localHandler->RemoteEndPoint->ToString());
				if (userDB->ContainsValue(builder->ToString())) builder->Append("1");
				userDB[clientIP] = builder->ToString();
				this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " joined the chat.");
				getUserName = false;
			}
			else {
				this->BeginInvoke(msg, userDB[clientIP], builder->ToString());
			}
		} 
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054 && ex->ErrorCode != 10053) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			break;
		}
		//while end//
	}
	try {
		localHandler->Shutdown(SocketShutdown::Both);
		localHandler->Close(); // закрываем сокет
	}
	catch (...) {}
	this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " left the chat.");
	userDB->Remove(clientIP);
}

void ServerWindow::startSynchronizeMessages(Object^ handler) {
	Socket^ localHandler = (Socket^)handler;
	array<unsigned char>^ data;
	int lastMsg = chatPool->Count;
	while (true) {
		System::Threading::Thread::Sleep(250);
		if (lastMsg == chatPool->Count) continue;

		try {
			for (int i = lastMsg; i < chatPool->Count; i++) {
				data = Encoding::Unicode->GetBytes(chatPool[i]);
				localHandler->Send(data);
			}
		}
		catch (SocketException^ ex) {
			return;
		}
		catch (ObjectDisposedException^ ex) {
			return;
		}

		lastMsg = chatPool->Count;
	}
}

/*-----*/
