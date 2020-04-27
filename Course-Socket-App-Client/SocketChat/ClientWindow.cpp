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

		//начинаем слушать хоста
		Task^ messageTransfering = gcnew Task(gcnew Action(this, &ClientWindow::startMessageTransfering));
		messageTransfering->Start();

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
		messageSocket->Close();
	} 
	catch (SocketException^ ex) {
		if (ex->ErrorCode != 10054 && ex->ErrorCode != 10053) {
			MessageBox::Show(ex->ErrorCode + ": " + ex->Message, "Error",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
	addChatMessage("\r\nYou have been disconnected...");
}

void ClientWindow::setChatWorking(bool toStart) {
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

void ClientWindow::sendImageMessage(int imageNum) {
	try {
		array<unsigned char>^ imageDeclaring = Encoding::Unicode->GetBytes("&image");
		messageSocket->Send(imageDeclaring);
		messageSocket->SendFile(imagePathes[imageNum]);
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
	int currentMessage = 0;
	while (true) {
		try {
			System::Threading::Thread::Sleep(250);
			sendSystemMessage("&get_num_of_messages");
			int numOfMessages = Convert::ToInt32(getStringMessage());
			if (numOfMessages == currentMessage) continue;
			
			while (currentMessage != numOfMessages) {
				sendSystemMessage("&get_message=" + currentMessage);
				String^ messageContent = getStringMessage();
				if (messageContent->Contains("&image")) {
					Image^ image = getImageMessage();
					this->BeginInvoke(gcnew imageDelegate(this, &ClientWindow::insertChatImage), image);
				}
				else {
					this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::addChatMessage), messageContent);
				}
				currentMessage++;
			}
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

void ClientWindow::sendSystemMessage(String^ message) {
	array<unsigned char>^ sendData = Encoding::Unicode->GetBytes(message);
	try {
		messageSocket->Send(sendData);
	}
	catch (SocketException^ ex) {
		MessageBox::Show("Error code: " + ex->ErrorCode + ". " + ex->Message, "Error",
			MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}

String^ ClientWindow::getStringMessage() {
	array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // буфер для ответа
	StringBuilder^ builder = gcnew StringBuilder();
	int bytes = 0; // количество полученных байт

	do { //получаем ответ
		bytes = messageSocket->Receive(receiveData);
		builder->Append(Encoding::Unicode->GetString(receiveData, 0, bytes));
	} while (messageSocket->Available > 0);

	return builder->ToString();
}

void ClientWindow::addChatMessage(String^ message) {
	richTextBoxChat->AppendText(message);
	richTextBoxChat->ScrollToCaret();
}

Image^ ClientWindow::getImageMessage() {
		array<unsigned char>^ imageData = gcnew array<unsigned char>(0);
		array<unsigned char>^ receiveData = gcnew array<unsigned char>(256); // буфер для ответа
		int bytes = 0; // количество полученных байт
		try {
			do { //получаем ответ
				bytes = messageSocket->Receive(receiveData);
				Array::Resize(imageData, imageData->Length + bytes);
				Array::Copy(receiveData, 0, imageData, imageData->Length - bytes, bytes);
			} while (messageSocket->Available > 0);

			ImageConverter^ convertData = gcnew ImageConverter();
			Image^ image = (Image^)convertData->ConvertFrom(imageData);
			int maxWidth = richTextBoxChat->Width * 9 / 10,
				maxHeight = richTextBoxChat->Height * 9 / 10;
			image = resizeImageWithRatioSave(image, maxWidth, maxHeight);
			saveImage(image);

			return image;
		}
		catch (...) {
			this->BeginInvoke(gcnew MessageDelegate(this, &ClientWindow::addChatMessage), "\r\nImage getting error");
			return gcnew Bitmap(1,1);
		}
}

void ClientWindow::insertChatImage(Image^ image) {
	DataFormats::Format^ imageFormat = DataFormats::GetFormat(DataFormats::Bitmap);
	Object^ oldClipData = Clipboard::GetDataObject();
	Clipboard::SetImage(image);
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