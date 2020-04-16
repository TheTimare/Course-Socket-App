#include "ServerWindow.h"

using namespace CourseSocketApp;

ServerWindow::ServerWindow(void) {
	InitializeComponent();
	userDB = gcnew Hashtable();
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

void ServerWindow::button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!isStarted) {
		serverTask = gcnew Task(gcnew Action(this, &ServerWindow::serverStart));
		serverTask->Start();
		setChatWorking(true);
		serverMessage("SYSTEM", "Server is started. Waiting for users...");
	}
	else {
		setChatWorking(false);
		mainSocket->Close();
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
	listenSocket->Listen(25);

	this->BeginInvoke(gcnew SocketDelegate(this, &ServerWindow::setSocket), listenSocket);
	while (isStarted) {
		Socket^ handler;
		try {
			handler = listenSocket->Accept();
			handler->ReceiveTimeout = 0;
		}
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		startMessageGetting(handler);
	}
}

void ServerWindow::startMessageGetting(Socket^ handler) {
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	while (true) {
		try {
			StringBuilder^ builder = gcnew StringBuilder();
			int bytes = 0; // кол-во полученных байтов
			array<unsigned char>^ data = gcnew array<unsigned char>(256); // буфер для получаемых данных
			bool getUserName = !userDB->ContainsKey(handler->RemoteEndPoint->ToString()); //get, если не содержит

			do { // получаем сообщение
				bytes = handler->Receive(data); //Receive(data);
				builder->Append(Encoding::Unicode->GetString(data, 0, bytes));
			} while (handler->Available > 0);
			
			if (getUserName) {
				userDB[handler->RemoteEndPoint->ToString()] = builder->ToString();
				this->BeginInvoke(msg, "SYSTEM", userDB[handler->RemoteEndPoint->ToString()] + " joined the chat.");
			}
			else if (builder->Length != 0) {
				msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
				this->BeginInvoke(msg, userDB[handler->RemoteEndPoint->ToString()], builder->ToString());
			}

			/*String^ message = "success"; 
			data = Encoding::Unicode->GetBytes(message);
			handler->Send(data); // отправляем ответ*/
		} 
		catch (SocketException^ ex) {
			if (ex->ErrorCode != 10054) {
				MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Ошибка",
					MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			this->BeginInvoke(msg, "SYSTEM", userDB[handler->RemoteEndPoint->ToString()] + " left the chat.");
			return;
		}
	}
	try {
		handler->Shutdown(SocketShutdown::Both);
		handler->Close(); // закрываем сокет
	}
	catch (...) {}
	this->BeginInvoke(msg, "SYSTEM", userDB[handler->RemoteEndPoint->ToString()] + " left the chat.");
}

void ServerWindow::serverMessage(String^ user, String^ msg) {
	textBoxChat->AppendText("[" + DateTime::Now.ToShortTimeString() + "] ");
	textBoxChat->AppendText(user + ": " + msg + "\r\n");
}

void ServerWindow::setSocket(Socket^ mainSocket) {
	this->mainSocket = mainSocket;
}

bool ServerWindow::isSocketStillConnected(Socket^ socket) {
	bool connected = true;
	bool blockingState = socket->Blocking;
	try {
		array<unsigned char>^ tmp = gcnew array<unsigned char>(1);
		socket->Blocking = false;
		socket->Send(tmp);
	}
	catch (...) {
		connected = false;
	}
	finally {
		socket->Blocking = blockingState;
	}
	return connected;
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