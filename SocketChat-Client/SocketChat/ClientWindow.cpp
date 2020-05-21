#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace SocketChatClient;

ClientWindow::ClientWindow() {
	InitializeComponent(); // initialize windows forms components

	imagePathes = gcnew List<String^>;
	setChatCondition(false); // set chat to off state
	addChatMessage("Hi there. To connect to the server, open the \"server\" tab above and click \"Connect\".");
}

ClientWindow::~ClientWindow() {
	if (connectSuccess) { // if client still connected
		itemDisconnect_Click(gcnew Object, gcnew EventArgs); // disconnect user
	}
	if (components) {
		delete components;
	}
}

/*---*/

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

/*---*/

void ClientWindow::itemConnect_Click(System::Object^  sender, System::EventArgs^  e) {
	inputSuccess = false; // initialize indicator
	ConnectWindow^ connect = gcnew ConnectWindow(this);
	connect->ShowDialog(); // show connection data form
	if (!inputSuccess) { // if user leave without input
		return;
	}

	connectToTheServer();

	if (connectSuccess) {
		setChatCondition(true);
		addChatMessage("\r\nYou are connected to the server!");
	}
}

void ClientWindow::connectToTheServer() {
	try {
		IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port); // create end point from ip and port
		messageSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		// try connect to the server
		messageSocket->Connect(ipPoint);

		sendSystemMessage(name); // send user name to server

		Task^ messageTransfering = gcnew Task(gcnew Action(this, &ClientWindow::startMessageTransfering));
		messageTransfering->Start();

		connectSuccess = true;
	}
	catch (Exception^ e) {
		MessageBox::Show("Connection error!\n" + e->Message, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	};
}

void ClientWindow::itemDisconnect_Click(System::Object^  sender, System::EventArgs^  e){
	if (!connectSuccess) {
		return;
	}
	connectSuccess = false;
	setChatCondition(false);
	sendTextMessage("&disconnect");
	messageSocket->Close();
	addChatMessage("\r\nYou have been disconnected...");
}

void ClientWindow::setChatCondition(bool toStart) {
	if (toStart){
		itemDisconnect->Available = true;
		itemConnect->Available = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::Window;
		textBoxMessage->ReadOnly = false;
		textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
		buttonSendMsg->Enabled = true;
		buttonUploadImg->Enabled = true;
	}
	else {
		itemDisconnect->Available = false;
		itemConnect->Available = true;
		connectSuccess = false;
		textBoxMessage->ReadOnly = true;
		textBoxMessage->BackColor = System::Drawing::SystemColors::MenuBar;
		buttonSendMsg->Enabled = false;
		buttonUploadImg->Enabled = false;
	}
}

/*---*/

void ClientWindow::buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e) {
	if (textBoxMessage->Text->Equals("")) { // if message is empty
		return;
	}
	Task^ message = gcnew Task(gcnew Action(this, &ClientWindow::sendMessageFromChat));
	message->Start();
}

void ClientWindow::sendMessageFromChat() {
	String^ message = textBoxMessage->Text;
	message = message->Trim();
	message = message->Replace("&", ""); // remove system symbols
	this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::setMessage), "");

	sendTextMessage(message);
}

void ClientWindow::sendTextMessage(String^ message) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		BinaryWriter^ writer = gcnew BinaryWriter(netStream);
		writer->Write(message); // send message

		writer->Flush(); // close all streams
		writer->Close();
		netStream->Close();
	}
	catch (...) {
		//log
	}
}

void ClientWindow::sendImageMessage(int imageNum) {
	try {
		sendTextMessage("&image"); // send message that next message is image
		BinaryFormatter^ formatter = gcnew BinaryFormatter();
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		formatter->Serialize(netStream, Image::FromFile(imagePathes[imageNum]));

		netStream->Close();
	}
	catch (...) {
		//log
	}
}

void ClientWindow::setMessage(String^ message) {
	textBoxMessage->Text = message;
}

/*---*/

void ClientWindow::startMessageTransfering() {
	MessageDelegate^ msg = gcnew MessageDelegate(this, &ClientWindow::addChatMessage);
	ImagePathDelegate^ imageMsg = gcnew ImagePathDelegate(this, &ClientWindow::insertChatImage);
	int currentMessage = 0; // indicate how many message already downloaded
	while (true) { 
		System::Threading::Thread::Sleep(250); // wait for message asking
		sendSystemMessage("&get_num_of_messages"); // request for messages num

		String^ toConvert = getStringMessage(); // get asked messages num
		if (toConvert->Contains("&disconnect")) { // if server don't response
			this->BeginInvoke(gcnew EventDelegate(this, &ClientWindow::itemDisconnect_Click), gcnew Object, gcnew EventArgs);
			break;
		}
		int numOfMessages = Convert::ToInt32(toConvert);
		if (numOfMessages == currentMessage) {
			continue; // wait for new messages
		}
			
		while (currentMessage != numOfMessages) { // update messages pool
			// send request for message
			sendSystemMessage("&get_message=" + currentMessage++);

			String^ messageContent = getStringMessage();
			if (messageContent->Contains("&image")) { // if next message is img
				Image^ image = getImageMessage();
				saveImage(image); // save image to disk
				String^ path = imagePathes[imagePathes->Count - 1];
				this->BeginInvoke(imageMsg, path);
				continue; // do not add "&image" to the chat
			}
			if (messageContent->Contains("&disconnect")) {
				break;
			}

			this->BeginInvoke(msg, messageContent);
		}
	}
}

void ClientWindow::sendSystemMessage(String^ message) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		BinaryWriter^ writer = gcnew BinaryWriter(netStream);
		writer->Write(message); // send messages

		writer->Flush(); // close all streams
		writer->Close();
		netStream->Close();
	}
	catch (...) { }
}

String^ ClientWindow::getStringMessage() {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		BinaryReader^ reader = gcnew BinaryReader(netStream);
		String^ message = reader->ReadString(); // download messages

		reader->Close(); // close all streams
		netStream->Close();
		return message;
	}
	catch (...) {
		if (connectSuccess) { // if user still connected
			MessageBox::Show("An error occurred - Connection lost", "Error",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		return "&disconnect";
	}
}

void ClientWindow::addChatMessage(String^ message) {
	richTextBoxChat->AppendText(message);
	richTextBoxChat->ScrollToCaret();
}

Image^ ClientWindow::getImageMessage() {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		BinaryFormatter^ formatter = gcnew BinaryFormatter();

		// deserialize image from stream
		Image^ image = (Image^)formatter->Deserialize(netStream);

		int maxWidth = richTextBoxChat->Width * 9 / 10,
			maxHeight = richTextBoxChat->Height * 9 / 10;
		image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
		saveImage(image); // save image to disk

		return image;
	}
	catch (...) {
		return gcnew Bitmap(1,1);
	}
}

void ClientWindow::insertChatImage(String^ imagePath) {
	DataFormats::Format^ imageFormat = DataFormats::GetFormat(DataFormats::Bitmap);
	String^ oldClipData = Clipboard::GetText();
	Clipboard::SetImage(Image::FromFile(imagePath));
	richTextBoxChat->ReadOnly = false;
	richTextBoxChat->Focus();
	if (richTextBoxChat->CanPaste(imageFormat)) {
		richTextBoxChat->Paste(imageFormat);
	}
	richTextBoxChat->ReadOnly = true;
	Clipboard::SetText(oldClipData);
}

/*---*/

void ClientWindow::buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e) {
	// open tool strip menu
	buttonUploadImg->ContextMenuStrip->Show(Cursor->Position);
}

void ClientWindow::toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Filter = "Image Files | *.jpg;*.jpeg;.*.png"; // file types, that will be allowed to upload
	dialog->Multiselect = false;
	if (dialog->ShowDialog() != ::DialogResult::OK) return;

	String^ path = dialog->FileName; // get name of file
	Image^ image = Image::FromFile(path);

	// resize image if it too big
	int maxWidth = richTextBoxChat->Width * 9 / 10,
		maxHeight = richTextBoxChat->Height * 9 / 10;
	image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
	saveImage(image); // save image to upload directory
	sendImageMessage(imagePathes->Count - 1); // send new image
}

void ClientWindow::saveImage(Image^ image) {
	// forming image path to download directory
	String^ imagePath = Application::StartupPath + "/downloads/img" + (imagePathes->Count + 1) + ".jpg";
	(gcnew FileInfo(imagePath))->Directory->Create();
	image->Save(imagePath, Imaging::ImageFormat::Jpeg);
	imagePathes->Add(imagePath);
}