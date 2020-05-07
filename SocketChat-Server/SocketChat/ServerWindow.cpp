#include "ServerWindow.h"

using namespace SocketChatServer;

/*-----*/

ServerWindow::ServerWindow(void) {
	InitializeComponent(); // windows forms initialization
	AutoScaleDimensions = System::Drawing::SizeF(96, 96); // set DPI compatibility to 96

	userDB = gcnew Hashtable(); // arrays initializing
	chatPool = gcnew List<String^>();
	connected = gcnew List<Socket^>();
	imagePathes = gcnew List<String^>();
	isStarted = false; // server is off from start

	//geting PC address
	IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
	for (int i = 0; i < host->AddressList->Length; i++) {
		if (host->AddressList[i]->AddressFamily == AddressFamily::InterNetwork) { // if ip is ipv4 then add
			domainUpDownIPs->Items->Add(host->AddressList[i]);
		}
	}

	// choose first finded IP
	domainUpDownIPs->SelectedIndex = 0;
	selectedIP = 0;

	// add text to chat, but no to chatPool
	richTextBoxChat->AppendText("Server app initialized. Choose port and ip to start the server.");
}

ServerWindow::~ServerWindow() {
	if (isStarted) { // if server is started first off it
		button_on_off_server_Click(gcnew Object, gcnew EventArgs);
	}
	if (components) {
		delete components;
	}
}

/*-----*/

// All utilities not included in Server class

// strip port from ip string
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

// image size in pixels
Image^ resizeImage(Image^ image, int width, int height) {
	Bitmap^ resizedImage = gcnew Bitmap(width, height);
	Graphics^ g = Graphics::FromImage(resizedImage);
	g->InterpolationMode = Drawing2D::InterpolationMode::High;
	g->DrawImage(image, 0, 0, width, height);
	return resizedImage;
}

// scale image to min of height or width
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

void ServerWindow::sendTextMessage(Socket^ handler, String^ message) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(handler); // make stream of socket
		BinaryWriter^ writer = gcnew BinaryWriter(netStream); // prepare write to net
		writer->Write(message); // send message to client

		writer->Flush(); // close streams
		writer->Close();
		netStream->Close();
	}
	catch (...) {
		//todo log
	}
}

void ServerWindow::sendImageMessage(Socket^ handler, String^ imagePath) {
	try {
		BinaryFormatter^ formatter = gcnew BinaryFormatter(); // prepare formatter to make binaries from image
		NetworkStream^ netStream = gcnew NetworkStream(handler);
		formatter->Serialize(netStream, Image::FromFile(imagePath)); // serializing of image
	}
	catch (...) {
		//todo log
	}
}

void ServerWindow::addItemToCheckBox(String^ item) {
	checkedListBoxUsers->Items->Add(item);
}

void ServerWindow::removeItemFromCheckBox(String^ item) {
	checkedListBoxUsers->Items->Remove(item);
}

/*-----*/

void ServerWindow::button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!isStarted) {
		selectedIP = domainUpDownIPs->SelectedIndex; // get chosen ip
		Task^ serverTask = gcnew Task(gcnew Action(this, &ServerWindow::serverStart));
		serverTask->Start(); // start connection listener on a new task
		setChatWorking(true); // set chat to enabled state
		serverMessage("SYSTEM", "Server is started. Waiting for users...");
	}
	else {
		setChatWorking(false); // set chat to disabled state
		acceptSocket->Close(); // close acception of new connections
		while (connected->Count > 0) { // close all connections
			connected[0]->Close();
			connected->RemoveAt(0);
		}
		serverMessage("SYSTEM", "Server is off.");
	}
}

void ServerWindow::serverStart() {
	int port = 8080; // standard port
	try {
		port = Convert::ToInt32(textBoxPort->Text); // parse port from textbox
	}
	catch (...) {
		MessageBox::Show("Неправильно введен порт", "Ошибка",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}
	
	IPAddress^ ip = (IPAddress^)domainUpDownIPs->Items[selectedIP]; // get chosen ip
	IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port); // get address to start the server
	Socket^ listenSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	listenSocket->Bind(ipPoint); // bind socket to chosen ip
	listenSocket->Listen(25); // allow socket to listen connections

	// set class variable to created listenSocket
	this->BeginInvoke(gcnew SocketDelegate(this, &ServerWindow::setSocket), listenSocket);
	while (isStarted) { // isStarted can be set out of function
		try {
			Socket^ handler = listenSocket->Accept(); // waiting for connection
			handler->ReceiveTimeout = CLIENTS_RECEIVE_TIMEOUT; // set option from the config
			String^ ip = stripPortIP(handler->RemoteEndPoint->ToString()); // get ip without port
			if (userDB->ContainsKey(ip)) { // if this ip already connected 
				handler->Close(); // than close this connection
			}
			else {
				connected->Add(handler); // adding new socket to list of connected
				Task^ messageTransfer = gcnew Task((Action<Object^>^)(
					gcnew Action<Object^>(this, &ServerWindow::startMessageTransfering)), handler
				);
				messageTransfer->Start(); // start message transfering for connected socket
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

void ServerWindow::setChatWorking(bool toStart) {
	if (toStart) {
		textBoxPort->ReadOnly = true;
		buttonSendMsg->Enabled = true;
		buttonUploadImg->Enabled = true;
		textBoxMessage->ReadOnly = false;
		buttonUserDisconnect->Enabled = true;
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
		buttonUserDisconnect->Enabled = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::Control;
		domainUpDownIPs->BackColor = System::Drawing::SystemColors::Window;
		button_on_off_server->Text = "Start Server";
		isStarted = false;
	}
}

void ServerWindow::setSocket(Socket^ acceptSocket) {
	this->acceptSocket = acceptSocket;
}

/*-----*/

 // this method should be called by gcnew task with object argument
void ServerWindow::startMessageTransfering(Object^ handler) {
	Socket^ localHandler = (Socket^)handler; // casting object to socket
	// creating delegates to change out processes data
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	ImageMessageDelegate^ imageMsg = gcnew ImageMessageDelegate(this, &ServerWindow::InsertChatImage);
	String^ clientIP = addUser(localHandler); // get username and ip from socket
	int startMessage = chatPool->Count; // start point of message getting

	while (true) { // while will be stopped only by catch or user disconnect
		// wait and getting message from client 
		String^ messageContent = getStringMessage(localHandler);

		if (messageContent->Contains("&disconnect")) { // if user want to disconnect
			break;
		}
		if (messageContent->Contains("&get_message=")) { // get request from client
			// get message that client want to get
			int messageNum = Convert::ToInt32(messageContent->Replace("&get_message=", ""));
			messageNum += startMessage; // client send message num from start point
			sendChatMessage(localHandler, messageNum); // send this message to client
			continue;
		}
		if (messageContent->Contains("&get_num_of_messages")) { // send num of messages in chatPool
			int deltaNum = chatPool->Count - startMessage; // count num of message from start of messaging
			sendMessagesNum(localHandler, deltaNum);
			continue;
		}
		if (messageContent->Contains("&image")) { // if user send an image
			Image^ image = getImageMessage(localHandler);
			saveImage(image); // save image to disk
			// insert image in chat
			this->BeginInvoke(imageMsg, (String^)userDB[clientIP], imagePathes->Count - 1);
			continue;
		}

		//this will only be called if none of ifs above worked
		this->BeginInvoke(msg, userDB[clientIP], messageContent);
	}
	try {
		localHandler->Shutdown(SocketShutdown::Both);
		localHandler->Close();
	}
	catch (...) {} // ignore all exceptions
	// insert user left message in chat
	this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " left the chat.");
	removeUser(clientIP); // remove user from userDB
	connected->Remove(localHandler);
}

String^ ServerWindow::getStringMessage(Socket^ handler) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(handler);
		BinaryReader^ reader = gcnew BinaryReader(netStream);
		String^ message = reader->ReadString(); // download message from socket

		reader->Close();
		netStream->Close();
		return message;
	}
	catch (...) { // if there are problem with message getting
		return "&disconnect"; // disconnect user
	}
}

Image^ ServerWindow::getImageMessage(Socket^ handler) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(handler);
		BinaryFormatter^ formatter = gcnew BinaryFormatter();

		// deserializaing image from socket data
		Image^ image = (Image^)formatter->Deserialize(netStream);

		// set image max size
		int maxWidth = richTextBoxChat->Width * 9 / 10,
			maxHeight = richTextBoxChat->Height * 9 / 10;
		image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
		saveImage(image); // save image to disk

		return image;
	}
	catch (...) {
		return gcnew Bitmap(1, 1);
	}
}

void ServerWindow::sendChatMessage(Socket^ handler, int messageNum) {
	try {
		String^ message = chatPool[messageNum]; // send message from chatPool
		if (message->StartsWith("&image=")) { // if this message define img
			// get message number
			int imageNum = Convert::ToInt32(message->Replace("&image=", ""));
			sendTextMessage(handler, "&image");
			sendImageMessage(handler, imagePathes[imageNum]);
		}
		else {
			sendTextMessage(handler, message); // send usual text message
		}
	}
	catch (SocketException^ ex) {
		// TODO: error log on a server window
	}
}

void ServerWindow::sendMessagesNum(Socket^ handler, int numOfMessages) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(handler);
		BinaryWriter^ writer = gcnew BinaryWriter(netStream);
		writer->Write(numOfMessages.ToString()); // send num of messages

		writer->Flush(); // close all streams
		writer->Close();
		netStream->Close();
	}
	catch (...) {
		// log
	}
}

/*-----*/

void ServerWindow::buttonUserDisconnect_Click(System::Object^  sender, System::EventArgs^  e) {
	// while not all chosen users disconnected
	while (checkedListBoxUsers->SelectedIndices->Count > 0) {
		String^ selectedItem = checkedListBoxUsers->SelectedItems[0]->ToString();
		int selectedIndex = checkedListBoxUsers->SelectedIndices[0];

		removeItemFromCheckBox(selectedItem);
		connected[selectedIndex]->Close();
		connected->RemoveAt(selectedIndex);
	}
}

//returns IP of new user, should be called right after connection
String^ ServerWindow::addUser(Socket^ handler) {
	String^ clientIP = stripPortIP(handler->RemoteEndPoint->ToString());
	String^ username = getStringMessage(handler); // get user name

	while (userDB->ContainsValue(username)) { // if DB already have such name
		username += "1"; // change name to another
	}
	userDB[clientIP] = username; // set hashtable key - value

	CheckBoxDelegate^ addUserToCheckBox = gcnew CheckBoxDelegate(this, &ServerWindow::addItemToCheckBox);
	this->BeginInvoke(addUserToCheckBox, userDB[clientIP]); // add to check box

	// saying that user connected to the chat
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
	this->BeginInvoke(msg, "SYSTEM", userDB[clientIP] + " joined the chat.");

	return clientIP;
}

void ServerWindow::removeUser(String^ ip) {
	CheckBoxDelegate^ removeUser = gcnew CheckBoxDelegate(this, &ServerWindow::removeItemFromCheckBox);
	this->BeginInvoke(removeUser, userDB[ip]->ToString());

	userDB->Remove(ip);
}

void ServerWindow::domainUpDownIPs_SelectedItemChanged(System::Object^  sender, System::EventArgs^  e) {
	// if server is started do not allow to change selected ip
	if (isStarted) {
		domainUpDownIPs->SelectedIndex = selectedIP;
	}
}

/*-----*/

void ServerWindow::buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e) {
	if (textBoxMessage->Text == "") { // do not send message if chat is empty
		return;
	}
	serverMessage("SYSTEM", textBoxMessage->Text);
	textBoxMessage->Text = ""; // clearing send message text box
}

void ServerWindow::serverMessage(String^ user, String^ msg) {
	// forming message view
	String^ formedMessage = "\r\n[" + DateTime::Now.ToShortTimeString() + "] " + user + ": " + msg;
	richTextBoxChat->AppendText(formedMessage);
	chatPool->Add(formedMessage); // add message to chatPool
	richTextBoxChat->ScrollToCaret(); // scroll textBox to down border
}

/*-----*/

void ServerWindow::buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonUploadImg->ContextMenuStrip->Show(Cursor->Position); // show tool strip at cursor position
}

void ServerWindow::toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Filter = "Image Files | *.jpg;*.jpeg;.*.png"; // file types, that will be allowed to upload
	dialog->Multiselect = false; // forbid multiply image uploading
	if (dialog->ShowDialog() != ::DialogResult::OK) { // handle dialog close without uploading
		return;
	}

	String^ path = dialog->FileName; // get name of file
	Image^ image = Image::FromFile(path); // form image from this files

	saveImage(image); // save image to upload directory
	InsertChatImage("SYSTEM", imagePathes->Count-1); // insert image in textbox
}

void ServerWindow::saveImage(Image^ image) {
	// resizing of image if it too big
	int maxWidth = richTextBoxChat->Width * 9 / 10,
		maxHeight = richTextBoxChat->Height * 9 / 10;
	image = resizeImageWithRatioSave(image, maxWidth, maxHeight);

	// forming image path
	String^ imagePath = Application::StartupPath + "/downloads/img" + (imagePathes->Count + 1) + ".jpg";
	(gcnew FileInfo(imagePath))->Directory->Create(); // create directories, where files is saving
	image->Save(imagePath, Imaging::ImageFormat::Jpeg); // save image to path
	imagePathes->Add(imagePath); // add image path to class array
}

void ServerWindow::InsertChatImage(String^ user, int imageNum) {
	serverMessage(user, "\r\n"); // add message to the chat

	DataFormats::Format^ imageFormat = DataFormats::GetFormat(DataFormats::Bitmap);
	String^ oldClipData = Clipboard::GetText(); // get old clipboard text
	Clipboard::SetImage(Image::FromFile(imagePathes[imageNum]));
	richTextBoxChat->ReadOnly = false;
	richTextBoxChat->Focus(); // focus on the chat text box
	richTextBoxChat->Paste(imageFormat); // paste image to text box
	richTextBoxChat->ReadOnly = true;
	Clipboard::SetText(oldClipData);

	chatPool->Add(Convert::ToString("&image=" + imageNum)); // define that next message is image
}
