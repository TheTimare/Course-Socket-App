#include "ServerWindow.h"

using namespace CourseSocketApp;

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
		button_on_off_server->Text = "Shutdown Server";
		isStarted = true;
	}
	else {
		textBoxPort->ReadOnly = false;
		button_on_off_server->Text = "Start Server";
		isStarted = false;
	}
}

/*-----*/

ServerWindow::ServerWindow(void) {
	InitializeComponent();
	userDB = gcnew Hashtable();
	chatPool = gcnew List<String^>();
	connected = gcnew List<Socket^>();
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
	isStarted = false;
	//get pc address
	IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
	for (int i = 0; i < host->AddressList->Length; i++) {
		if (host->AddressList[i]->AddressFamily == AddressFamily::InterNetwork)
		{
			ip = host->AddressList[i];
			textBoxIP->Text = ip->ToString();
			break;
		}
	}
}

/*-----*/

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
		for (int i = 0; i < connected->Count; i++)
			connected[i]->Close();
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

	// получаем адреса для запуска сокета
	IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port);
	Socket^ listenSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	// связываем сокет с локальной точкой, по которой будем принимать данные
	listenSocket->Bind(ipPoint);
	// начинаем прослушивание
	listenSocket->Listen(10);

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
				Task^ messageTask = gcnew Task((Action<Object^>^)(gcnew Action<Object^>(this, &ServerWindow::startMessageGetting)), handler);
				messageTask->Start();
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
	int lastMsg = 0;
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
				userDB[clientIP] = builder->ToString();
				this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " joined the chat.");
				getUserName = false;
			}
			else {
				this->BeginInvoke(msg, userDB[clientIP], builder->ToString());
			}

			lastMsg = synchronizeMessages(localHandler, lastMsg);
		} 
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054) {
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

int ServerWindow::synchronizeMessages(Socket^ handler, int msgNum) {
	array<unsigned char>^ data;

	for (int i = msgNum; i < chatPool->Count; i++) {
		data = Encoding::Unicode->GetBytes(chatPool[i]);
		handler->Send(data);
	}
	int lastMsg = chatPool->Count;
	return lastMsg;
}

/*-----*/
