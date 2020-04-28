#pragma once
#include <cliext/utility>  

namespace SocketChatServer {

#pragma region Namespaces
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Threading::Tasks;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Text;
	using namespace System::IO;
	using namespace System::Runtime::Serialization::Formatters::Binary;
#pragma endregion

#pragma region CONSTANT DEFINES

	int const CLIENTS_RECEIVE_TIMEOUT = 10000;

#pragma endregion

	/// <summary>
	/// —водка дл€ ServerWindow
	/// </summary>
	public ref class ServerWindow : public System::Windows::Forms::Form {

	public:
		ServerWindow(void);
	protected:
		~ServerWindow();

#pragma region Generated Fields
	protected:

	private: System::Windows::Forms::Button^  button_on_off_server;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxPort;

	private: System::Windows::Forms::DomainUpDown^  domainUpDownIPs;
	private: System::Windows::Forms::Button^  buttonSendMsg;
	private: System::Windows::Forms::TextBox^  textBoxMessage;
	private: System::Windows::Forms::RichTextBox^  richTextBoxChat;
	private: System::Windows::Forms::Button^  buttonUploadImg;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripAttach;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripAttachUpload;
	private: System::Windows::Forms::CheckedListBox^  checkedListBoxUsers;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  buttonUserDisconnect;



	private: System::ComponentModel::IContainer^  components;

	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>

#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ServerWindow::typeid));
			this->button_on_off_server = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->domainUpDownIPs = (gcnew System::Windows::Forms::DomainUpDown());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->textBoxMessage = (gcnew System::Windows::Forms::TextBox());
			this->richTextBoxChat = (gcnew System::Windows::Forms::RichTextBox());
			this->buttonUploadImg = (gcnew System::Windows::Forms::Button());
			this->contextMenuStripAttach = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripAttachUpload = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkedListBoxUsers = (gcnew System::Windows::Forms::CheckedListBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonUserDisconnect = (gcnew System::Windows::Forms::Button());
			this->contextMenuStripAttach->SuspendLayout();
			this->SuspendLayout();
			// 
			// button_on_off_server
			// 
			this->button_on_off_server->Location = System::Drawing::Point(12, 438);
			this->button_on_off_server->Name = L"button_on_off_server";
			this->button_on_off_server->Size = System::Drawing::Size(147, 35);
			this->button_on_off_server->TabIndex = 1;
			this->button_on_off_server->Text = L"Start Server";
			this->button_on_off_server->UseVisualStyleBackColor = true;
			this->button_on_off_server->Click += gcnew System::EventHandler(this, &ServerWindow::button_on_off_server_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 20);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Port:";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(56, 15);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(103, 28);
			this->textBoxPort->TabIndex = 3;
			this->textBoxPort->Text = L"8080";
			// 
			// domainUpDownIPs
			// 
			this->domainUpDownIPs->BackColor = System::Drawing::SystemColors::Window;
			this->domainUpDownIPs->Location = System::Drawing::Point(12, 49);
			this->domainUpDownIPs->Name = L"domainUpDownIPs";
			this->domainUpDownIPs->ReadOnly = true;
			this->domainUpDownIPs->Size = System::Drawing::Size(147, 28);
			this->domainUpDownIPs->TabIndex = 6;
			this->domainUpDownIPs->Text = L"No ip founded";
			this->domainUpDownIPs->Wrap = true;
			this->domainUpDownIPs->SelectedItemChanged += gcnew System::EventHandler(this, &ServerWindow::domainUpDownIPs_SelectedItemChanged);
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Enabled = false;
			this->buttonSendMsg->Location = System::Drawing::Point(601, 423);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(89, 50);
			this->buttonSendMsg->TabIndex = 8;
			this->buttonSendMsg->Text = L"Send Message";
			this->buttonSendMsg->UseVisualStyleBackColor = true;
			this->buttonSendMsg->Click += gcnew System::EventHandler(this, &ServerWindow::buttonSendMsg_Click);
			// 
			// textBoxMessage
			// 
			this->textBoxMessage->BackColor = System::Drawing::SystemColors::Control;
			this->textBoxMessage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMessage->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxMessage->Location = System::Drawing::Point(165, 423);
			this->textBoxMessage->Multiline = true;
			this->textBoxMessage->Name = L"textBoxMessage";
			this->textBoxMessage->ReadOnly = true;
			this->textBoxMessage->Size = System::Drawing::Size(383, 50);
			this->textBoxMessage->TabIndex = 7;
			// 
			// richTextBoxChat
			// 
			this->richTextBoxChat->BackColor = System::Drawing::SystemColors::HighlightText;
			this->richTextBoxChat->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->richTextBoxChat->Location = System::Drawing::Point(165, 15);
			this->richTextBoxChat->Name = L"richTextBoxChat";
			this->richTextBoxChat->ReadOnly = true;
			this->richTextBoxChat->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->richTextBoxChat->Size = System::Drawing::Size(525, 402);
			this->richTextBoxChat->TabIndex = 9;
			this->richTextBoxChat->TabStop = false;
			this->richTextBoxChat->Text = L"";
			// 
			// buttonUploadImg
			// 
			this->buttonUploadImg->ContextMenuStrip = this->contextMenuStripAttach;
			this->buttonUploadImg->Enabled = false;
			this->buttonUploadImg->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonUploadImg->Location = System::Drawing::Point(547, 423);
			this->buttonUploadImg->Name = L"buttonUploadImg";
			this->buttonUploadImg->Size = System::Drawing::Size(48, 50);
			this->buttonUploadImg->TabIndex = 10;
			this->buttonUploadImg->Text = L"Attach";
			this->buttonUploadImg->UseVisualStyleBackColor = true;
			this->buttonUploadImg->Click += gcnew System::EventHandler(this, &ServerWindow::buttonUploadImg_Click);
			// 
			// contextMenuStripAttach
			// 
			this->contextMenuStripAttach->ImageScalingSize = System::Drawing::Size(20, 20);
			this->contextMenuStripAttach->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripAttachUpload });
			this->contextMenuStripAttach->Name = L"contextMenuStripAttach";
			this->contextMenuStripAttach->Size = System::Drawing::Size(209, 28);
			// 
			// toolStripAttachUpload
			// 
			this->toolStripAttachUpload->Name = L"toolStripAttachUpload";
			this->toolStripAttachUpload->Size = System::Drawing::Size(208, 24);
			this->toolStripAttachUpload->Text = L"Upload Attachment";
			this->toolStripAttachUpload->Click += gcnew System::EventHandler(this, &ServerWindow::toolStripAttachUpload_Click);
			// 
			// checkedListBoxUsers
			// 
			this->checkedListBoxUsers->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->checkedListBoxUsers->CheckOnClick = true;
			this->checkedListBoxUsers->FormattingEnabled = true;
			this->checkedListBoxUsers->Location = System::Drawing::Point(12, 115);
			this->checkedListBoxUsers->Name = L"checkedListBoxUsers";
			this->checkedListBoxUsers->Size = System::Drawing::Size(147, 94);
			this->checkedListBoxUsers->TabIndex = 11;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(8, 92);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(125, 20);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Connected Users";
			// 
			// buttonUserDisconnect
			// 
			this->buttonUserDisconnect->Enabled = false;
			this->buttonUserDisconnect->Location = System::Drawing::Point(12, 215);
			this->buttonUserDisconnect->Name = L"buttonUserDisconnect";
			this->buttonUserDisconnect->Size = System::Drawing::Size(147, 33);
			this->buttonUserDisconnect->TabIndex = 13;
			this->buttonUserDisconnect->Text = L"Disconnect";
			this->buttonUserDisconnect->UseVisualStyleBackColor = true;
			this->buttonUserDisconnect->Click += gcnew System::EventHandler(this, &ServerWindow::buttonUserDisconnect_Click);
			// 
			// ServerWindow
			// 
			this->AcceptButton = this->buttonSendMsg;
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::HighlightText;
			this->ClientSize = System::Drawing::Size(700, 486);
			this->Controls->Add(this->buttonUserDisconnect);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->checkedListBoxUsers);
			this->Controls->Add(this->buttonUploadImg);
			this->Controls->Add(this->richTextBoxChat);
			this->Controls->Add(this->buttonSendMsg);
			this->Controls->Add(this->textBoxMessage);
			this->Controls->Add(this->domainUpDownIPs);
			this->Controls->Add(this->textBoxPort);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_on_off_server);
			this->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"ServerWindow";
			this->Text = L"SocketChat Server";
			this->contextMenuStripAttach->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: List<String^>^ chatPool;
			 List<Socket^>^ connected;
			 List<String^>^ imagePathes;
			 int selectedIP;
			 bool isStarted;
			 Hashtable^ userDB;
			 Socket^ mainSocket;
			 
	private: void button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e);
			 void serverStart();

	private: void startMessageTransfering(Object^ handler);
			 String^ getStringMessage(Socket^ handler);
			 Image^ getImageMessage(Socket^ handler);
			 String^ addUser(Socket^ handler);
			 void removeUser(String^ ip);
			 void sendChatMessage(Socket^ handler, int messageNum);
			 void sendMessagesNum(Socket^ handler, int numOfMessages);

	private: delegate void SocketDelegate(Socket^ mainSocket);
			 void setSocket(Socket^ mainSocket);

	private: void setChatWorking(bool isWorking);

	private: void domainUpDownIPs_SelectedItemChanged(System::Object^  sender, System::EventArgs^  e);

	private: void buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e);
			 delegate void MessageDelegate(String^ user, String^ msg);
			 void serverMessage(String^ user, String^ msg);

	private: void buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e);
			 void toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e);
			 void saveImage(Image^ image);
			 delegate void ImageMessageDelegate(String^ user, int imageNum);
			 void InsertChatImage(String^ user, int imageNum);

	private: void sendTextMessage(Socket^ handler, String^ message);
			 void sendImageMessage(Socket^ handler, String^ imagePath);

	private: void buttonUserDisconnect_Click(System::Object^  sender, System::EventArgs^  e);
			 delegate void CheckBoxDelegate(String^ item);
			 void addItemToCheckBox(String^ item);
			 void removeItemFromCheckBox(String^ item);
};
}
