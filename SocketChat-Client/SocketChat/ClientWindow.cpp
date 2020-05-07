#include "ClientWindow.h"
#include "ConnectWindow.h"

using namespace SocketChatClient;

ClientWindow::ClientWindow() {
	InitializeComponent();
	AutoScaleDimensions = System::Drawing::SizeF(96, 96);

	imagePathes = gcnew List<String^>;
	setChatWorking(false);
	addChatMessage("Hi there. To connect to the server, open the \"server\" tab above and click \"Connect\".");
}

ClientWindow::~ClientWindow() {
	if (connectSuccess) {
		itemDisconnect_Click(gcnew Object, gcnew EventArgs);
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
	inputSuccess = false;
	ConnectWindow^ connect = gcnew ConnectWindow(this);
	connect->ShowDialog();
	if (!inputSuccess) return;

	connectToTheServer();

	if (connectSuccess) {
		setChatWorking(true);
		addChatMessage("\r\nYou are connected to the server!");
	}
}

void ClientWindow::connectToTheServer() {
	try {
		IPEndPoint^ ipPoint = gcnew IPEndPoint(ip, port);
		messageSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		// подключаемся к удаленному хосту
		messageSocket->Connect(ipPoint);

		sendSystemMessage(name);

		//начинаем слушать хоста
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
	setChatWorking(false);
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
	if (textBoxMessage->Text->Equals("")) {
		return;
	}
	Task^ message = gcnew Task(gcnew Action(this, &ClientWindow::sendMessageFromChat));
	message->Start();
}

void ClientWindow::sendMessageFromChat() {
	String^ message = textBoxMessage->Text;
	message = message->Trim();
	message = message->Replace("&", "");
	this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::setMessage), "");

	sendTextMessage(message);
}

void ClientWindow::sendTextMessage(String^ message) {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);

		// отправляем сообщение
		BinaryWriter^ writer = gcnew BinaryWriter(netStream);
		writer->Write(message);
		writer->Flush();

		writer->Close();
		netStream->Close();
	}
	catch (...) {
		//log
	}
}

void ClientWindow::sendImageMessage(int imageNum) {
	try {
		sendTextMessage("&image");
		BinaryFormatter^ formatter = gcnew BinaryFormatter();
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		formatter->Serialize(netStream, Image::FromFile(imagePathes[imageNum]));
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
	int currentMessage = 0;
	while (true) {
		System::Threading::Thread::Sleep(250);
		sendSystemMessage("&get_num_of_messages");

		String^ toConvert = getStringMessage();
		if (toConvert->Contains("&disconnect")) {
			this->BeginInvoke(gcnew EventDelegate(this, &ClientWindow::itemDisconnect_Click), gcnew Object, gcnew EventArgs);
			break;
		}
		int numOfMessages = Convert::ToInt32(toConvert);
		if (numOfMessages == currentMessage) continue;
			
		while (currentMessage != numOfMessages) {
			sendSystemMessage("&get_message=" + currentMessage++);

			String^ messageContent = getStringMessage();
			if (messageContent->Contains("&image")) {
				Image^ image = getImageMessage();
				saveImage(image);
				String^ path = imagePathes[imagePathes->Count - 1];
				this->BeginInvoke(imageMsg, path);
				continue;
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
		writer->Write(message);

		writer->Flush();
		writer->Close();
		netStream->Close();
	}
	catch (...) { }
}

String^ ClientWindow::getStringMessage() {
	try {
		NetworkStream^ netStream = gcnew NetworkStream(messageSocket);
		BinaryReader^ reader = gcnew BinaryReader(netStream);
		String^ message = reader->ReadString();

		reader->Close();
		netStream->Close();
		return message;
	}
	catch (...) {
		if (connectSuccess) {
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

		Image^ image = (Image^)formatter->Deserialize(netStream);

		int maxWidth = richTextBoxChat->Width * 9 / 10,
			maxHeight = richTextBoxChat->Height * 9 / 10;
		image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
		saveImage(image);

		return image;
	}
	catch (...) {
		return gcnew Bitmap(1,1);
	}
}

void ClientWindow::insertChatImage(String^ imagePath) {
	DataFormats::Format^ imageFormat = DataFormats::GetFormat(DataFormats::Bitmap);
	Object^ oldClipData = Clipboard::GetDataObject();
	Clipboard::SetImage(Image::FromFile(imagePath));
	richTextBoxChat->ReadOnly = false;
	richTextBoxChat->Focus();
	richTextBoxChat->Paste(imageFormat);
	richTextBoxChat->ReadOnly = true;
	Clipboard::SetDataObject(oldClipData);
}

/*---*/

void ClientWindow::buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e) {
	buttonUploadImg->ContextMenuStrip->Show(Cursor->Position);
}

void ClientWindow::toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ dialog = gcnew OpenFileDialog();
	dialog->Filter = "Image Files | *.jpg;*.jpeg;.*.png"; // file types, that will be allowed to upload
	dialog->Multiselect = false;
	if (dialog->ShowDialog() != ::DialogResult::OK) return;

	String^ path = dialog->FileName; // get name of file
	Image^ image = Image::FromFile(path);

	int maxWidth = richTextBoxChat->Width * 9 / 10,
		maxHeight = richTextBoxChat->Height * 9 / 10;
	image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
	saveImage(image);
	sendImageMessage(imagePathes->Count - 1);
}

void ClientWindow::saveImage(Image^ image) {
	String^ imagePath = Application::StartupPath + "/downloads/img" + (imagePathes->Count + 1) + ".jpg";
	(gcnew FileInfo(imagePath))->Directory->Create();
	image->Save(imagePath, Imaging::ImageFormat::Jpeg);
	imagePathes->Add(imagePath);
}