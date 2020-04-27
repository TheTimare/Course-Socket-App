#include "ServerWindow.h"

using namespace CourseSocketApp;

ServerWindow::ServerWindow(void) {
	InitializeComponent();
	AutoScaleDimensions = System::Drawing::SizeF(96, 96);

	userDB = gcnew Hashtable();
	chatPool = gcnew List<String^>();
	connected = gcnew List<Socket^>();
	imagePathes = gcnew List<String^>();
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
		MessageBox::Show("No IPs available to start the server!\n"
			"Check your internet connection.The server cannot be started."
			, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	domainUpDownIPs->SelectedIndex = 0;
	selectedIP = 0;

	richTextBoxChat->AppendText("Server app initialized. Choose port and ip to start the server.");
}

ServerWindow::~ServerWindow() {
	if (isStarted) {
		button_on_off_server_Click(gcnew Object, gcnew EventArgs);
	}
	if (components) {
		delete components;
	}
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

void ServerWindow::setSocket(Socket^ mainSocket) {
	this->mainSocket = mainSocket;
}

void ServerWindow::setChatWorking(bool toStart) {
	if (toStart) {
		textBoxPort->ReadOnly = true;
		buttonSendMsg->Enabled = true;
		buttonUploadImg->Enabled = true;
		textBoxMessage->ReadOnly = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
		domainUpDownIPs->BackColor = System::Drawing::SystemColors::Control;
		button_on_off_server->Text = "Shutdown Server";
		isStarted = true;
	}
	else {
		textBoxPort->ReadOnly = false;
		buttonSendMsg->Enabled = false;
		buttonUploadImg->Enabled = false;
		textBoxMessage->ReadOnly = true;
		textBoxMessage->BackColor = System::Drawing::SystemColors::Control;
		domainUpDownIPs->BackColor = System::Drawing::SystemColors::Window;
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

Image^ resizeImage(Image^ image, int width, int height) {
	Bitmap^ resizedImage = gcnew Bitmap(width, height);
	Graphics^ g = Graphics::FromImage(resizedImage);
	g->InterpolationMode = Drawing2D::InterpolationMode::High;
	g->DrawImage(image, 0, 0, width, height);
	return resizedImage;
}

Image^ resizeImageWithRatioSave(Image^ image, int maxWidth, int maxHeight) {
	int width = image->Width,
		height = image->Height;
	if (width > maxWidth) {
		width = maxWidth;
		height = maxWidth * image->Height / image->Width;
	}
	if (height > maxHeight) {
		height = maxHeight;
		width = image->Width  * maxHeight / image->Height, maxHeight;
	}
	if (image->Width != width || image->Height != height) {
		image = resizeImage(image, width, height);
	}
	return image;
}

/*-----*/

void ServerWindow::button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!isStarted) {
		selectedIP = domainUpDownIPs->SelectedIndex;
		Task^ serverTask = gcnew Task(gcnew Action(this, &ServerWindow::serverStart));
		serverTask->Start();
		setChatWorking(true);
		serverMessage("SYSTEM", "Server is started. Waiting for users...");
	}
	else {
		setChatWorking(false);
		mainSocket->Close();
		while (connected->Count > 0) {
			//sendSystemCommand(connected[i], "&server_off");
			connected[0]->Close();
			connected->RemoveAt(0);
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
			handler->ReceiveTimeout = CLIENTS_RECEIVE_TIMEOUT; 
			String^ ip = stripPortIP(handler->RemoteEndPoint->ToString());
			if (userDB->ContainsKey(ip)) {
				handler->Close();
			}
			else {
				connected->Add(handler);
				Task^ messageTransfer = gcnew Task((Action<Object^>^)(gcnew Action<Object^>(this, &ServerWindow::startMessageTransfering)), handler);
				messageTransfer->Start();
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

void ServerWindow::startMessageTransfering(Object^ handler) {
	Socket^ localHandler = (Socket^)handler;
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	ImageMessageDelegate^ imageMsg = gcnew ImageMessageDelegate(this, &ServerWindow::InsertChatImage);
	String^ clientIP = stripPortIP(localHandler->RemoteEndPoint->ToString());
	int startMessage = chatPool->Count;
	addUserName(localHandler);
	while (true) {
		try {
			String^ messageContent = getStringMessage(localHandler);

			if (messageContent->Contains("&disconnect")) break;
			if (messageContent->Contains("&get_message=")) {
				int messageNum = Convert::ToInt32(messageContent->Replace("&get_message=", ""));
				messageNum += startMessage;
				sendChatMessage(localHandler, messageNum);
				continue;
			}
			if (messageContent->Contains("&get_num_of_messages")) {
				sendMessagesNum(localHandler, chatPool->Count - startMessage);
				continue;
			}
			if (messageContent->Contains("&image")) {
				Image^ image = getImageMessage(localHandler);
				saveImage(image);
				this->BeginInvoke(imageMsg, (String^)userDB[clientIP], imagePathes->Count - 1);
				continue;
			}

			//this will only be called if none of ifs above worked
			this->BeginInvoke(msg, userDB[clientIP], messageContent);
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

String^ ServerWindow::getStringMessage(Socket^ handler) {
	array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // буфер для ответа
	StringBuilder^ builder = gcnew StringBuilder();
	int bytes = 0; // количество полученных байт

	do { //получаем ответ
		bytes = handler->Receive(receiveData);
		builder->Append(Encoding::Unicode->GetString(receiveData, 0, bytes));
	} while (handler->Available > 0);

	return builder->ToString();
}

Image^ ServerWindow::getImageMessage(Socket^ handler) {
	array<unsigned char>^ imageData = gcnew array<unsigned char>(0);
	array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // буфер для ответа
	int bytes = 0; // количество полученных байт
	try {
		do { //получаем ответ
			bytes = handler->Receive(receiveData);
			Array::Resize(imageData, imageData->Length + bytes);
			Array::Copy(receiveData, 0, imageData, imageData->Length - bytes, bytes);
		} while (handler->Available > 0);

		ImageConverter^ convertData = gcnew ImageConverter();
		Image^ image = (Image^)convertData->ConvertFrom(imageData);
		int maxWidth = richTextBoxChat->Width * 9 / 10,
			maxHeight = richTextBoxChat->Height * 9 / 10;
		image = resizeImageWithRatioSave(image, maxWidth, maxHeight);

		return image;
	}
	catch (...) {
		this->BeginInvoke(gcnew MessageDelegate(this, &ServerWindow::serverMessage), "\r\nImage getting error");
		return gcnew Bitmap(1, 1);
	}
}

void ServerWindow::addUserName(Socket^ handler) {
	String^ clientIP;
	String^ messageContent;
	try {
		clientIP = stripPortIP(handler->RemoteEndPoint->ToString());
		messageContent = getStringMessage(handler);
	}
	catch (...) {
		messageContent = "unknown";
	}

	if (userDB->ContainsValue(messageContent)) {
		messageContent += "1";
	}
	userDB[clientIP] = messageContent;

	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " joined the chat.");
}

void ServerWindow::sendChatMessage(Socket^ handler, int messageNum) {
	try {
		String^ message = chatPool[messageNum];
		if (message->StartsWith("&image=")) {
			int imageNum = Convert::ToInt32(message->Replace("&image=", ""));
			array<unsigned char>^ imageDeclaring = Encoding::Unicode->GetBytes("&image");
			handler->Send(imageDeclaring);
			handler->SendFile(imagePathes[imageNum]);
		}
		else {
			array<unsigned char>^ data = Encoding::Unicode->GetBytes(chatPool[messageNum]);
			handler->Send(data);
		}
	}
	catch (...) {
		// TODO: error log on a server window
	}
}

void ServerWindow::sendMessagesNum(Socket^ handler, int numOfMessages) {
	try {
		array<unsigned char>^ data = Encoding::Unicode->GetBytes(numOfMessages.ToString());
		handler->Send(data);
	}
	catch (...) {
		// log
	}
}

/*-----*/

void ServerWindow::domainUpDownIPs_SelectedItemChanged(System::Object^  sender, System::EventArgs^  e) {
	if (isStarted)
		domainUpDownIPs->SelectedIndex = selectedIP;
}

/*-----*/

void ServerWindow::buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e) {
	if (textBoxMessage->Text == "") return;
	serverMessage("SYSTEM", textBoxMessage->Text);
	textBoxMessage->Text = "";
}

void ServerWindow::serverMessage(String^ user, String^ msg) {
	String^ formedMessage = "\r\n[" + DateTime::Now.ToShortTimeString() + "] " + user + ": " + msg;
	richTextBoxChat->AppendText(formedMessage);
	chatPool->Add(formedMessage);
	richTextBoxChat->ScrollToCaret();
}

/*-----*/

void ServerWindow::buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonUploadImg->ContextMenuStrip->Show(Cursor->Position);
}

void ServerWindow::toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Filter = "Image Files | *.jpg;*.jpeg;.*.png"; // file types, that will be allowed to upload
	dialog->Multiselect = false;
	if (dialog->ShowDialog() != ::DialogResult::OK) return;

	String^ path = dialog->FileName; // get name of file
	Image^ image = Image::FromFile(path);

	saveImage(image);
	InsertChatImage("SYSTEM", imagePathes->Count-1);
}

void ServerWindow::saveImage(Image^ image) {
	int maxWidth = richTextBoxChat->Width * 9 / 10,
		maxHeight = richTextBoxChat->Height * 9 / 10;
	image = resizeImageWithRatioSave(image, maxWidth, maxHeight);

	String^ imagePath = Application::StartupPath + "/downloads/img" + (imagePathes->Count + 1) + ".jpg";
	(gcnew FileInfo(imagePath))->Directory->Create();
	image->Save(imagePath, Imaging::ImageFormat::Jpeg);
	imagePathes->Add(imagePath);
}

void ServerWindow::InsertChatImage(String^ user, int imageNum) {
	serverMessage(user, "\r\n");

	DataFormats::Format^ imageFormat = DataFormats::GetFormat(DataFormats::Bitmap);
	Object^ oldClipData = Clipboard::GetDataObject();
	Clipboard::SetImage(Image::FromFile(imagePathes[imageNum]));
	richTextBoxChat->ReadOnly = false;
	richTextBoxChat->Focus();
	richTextBoxChat->Paste(imageFormat);
	richTextBoxChat->ReadOnly = true;
	Clipboard::SetDataObject(oldClipData);

	chatPool->Add(Convert::ToString("&image=" + imageNum));
}
