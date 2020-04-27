#pragma once

namespace CourseSocketAppClient {

#pragma region Namespaces
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Threading;
	using namespace System::Threading::Tasks;
	using namespace System::Text;
	using namespace System::IO;
#pragma endregion

#pragma region CONSTANT DEFINES

#pragma endregion

	/// <summary>
	/// —водка дл€ ClientWindow
	/// </summary>
	public ref class ClientWindow : public System::Windows::Forms::Form {

	public:
		ClientWindow(void);
	protected:
		~ClientWindow()
		{
			if (components)
			{
				delete components;
			}
		}

#pragma region Generated Fields
	protected:
	private: System::Windows::Forms::TextBox^  textBoxMessage;
	private: System::Windows::Forms::Button^  buttonSendMsg;

	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  itemConnect;

	private: System::Windows::Forms::ToolStripMenuItem^  itemDisconnect;
	private: System::Windows::Forms::RichTextBox^  richTextBoxChat;
	private: System::Windows::Forms::Button^  buttonUploadImg;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripAttach;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripAttachUpload;
	private: System::ComponentModel::IContainer^  components;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ClientWindow::typeid));
			this->textBoxMessage = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->itemConnect = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemDisconnect = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->richTextBoxChat = (gcnew System::Windows::Forms::RichTextBox());
			this->buttonUploadImg = (gcnew System::Windows::Forms::Button());
			this->contextMenuStripAttach = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripAttachUpload = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1->SuspendLayout();
			this->contextMenuStripAttach->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBoxMessage
			// 
			this->textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->textBoxMessage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMessage->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxMessage->Location = System::Drawing::Point(12, 438);
			this->textBoxMessage->Multiline = true;
			this->textBoxMessage->Name = L"textBoxMessage";
			this->textBoxMessage->Size = System::Drawing::Size(462, 50);
			this->textBoxMessage->TabIndex = 0;
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Location = System::Drawing::Point(527, 438);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(79, 50);
			this->buttonSendMsg->TabIndex = 2;
			this->buttonSendMsg->Text = L"Send Message";
			this->buttonSendMsg->UseVisualStyleBackColor = true;
			this->buttonSendMsg->Click += gcnew System::EventHandler(this, &ClientWindow::buttonSendMsg_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->BackColor = System::Drawing::SystemColors::Window;
			this->toolStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripDropDownButton1 });
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->toolStrip1->Size = System::Drawing::Size(618, 27);
			this->toolStrip1->TabIndex = 11;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripDropDownButton1
			// 
			this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripDropDownButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->itemConnect,
					this->itemDisconnect
			});
			this->toolStripDropDownButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripDropDownButton1.Image")));
			this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->Size = System::Drawing::Size(64, 24);
			this->toolStripDropDownButton1->Text = L"Server";
			// 
			// itemConnect
			// 
			this->itemConnect->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->itemConnect->Name = L"itemConnect";
			this->itemConnect->Size = System::Drawing::Size(157, 26);
			this->itemConnect->Text = L"Connect";
			this->itemConnect->Click += gcnew System::EventHandler(this, &ClientWindow::itemConnect_Click);
			// 
			// itemDisconnect
			// 
			this->itemDisconnect->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->itemDisconnect->Name = L"itemDisconnect";
			this->itemDisconnect->Size = System::Drawing::Size(157, 26);
			this->itemDisconnect->Text = L"Disconnect";
			this->itemDisconnect->Click += gcnew System::EventHandler(this, &ClientWindow::itemDisconnect_Click);
			// 
			// richTextBoxChat
			// 
			this->richTextBoxChat->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->richTextBoxChat->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->richTextBoxChat->Location = System::Drawing::Point(12, 30);
			this->richTextBoxChat->Name = L"richTextBoxChat";
			this->richTextBoxChat->ReadOnly = true;
			this->richTextBoxChat->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->richTextBoxChat->Size = System::Drawing::Size(594, 402);
			this->richTextBoxChat->TabIndex = 12;
			this->richTextBoxChat->TabStop = false;
			this->richTextBoxChat->Text = L"";
			// 
			// buttonUploadImg
			// 
			this->buttonUploadImg->ContextMenuStrip = this->contextMenuStripAttach;
			this->buttonUploadImg->Enabled = false;
			this->buttonUploadImg->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonUploadImg->Location = System::Drawing::Point(473, 438);
			this->buttonUploadImg->Name = L"buttonUploadImg";
			this->buttonUploadImg->Size = System::Drawing::Size(48, 50);
			this->buttonUploadImg->TabIndex = 13;
			this->buttonUploadImg->Text = L"Attach";
			this->buttonUploadImg->UseVisualStyleBackColor = true;
			this->buttonUploadImg->Click += gcnew System::EventHandler(this, &ClientWindow::buttonUploadImg_Click);
			// 
			// contextMenuStripAttach
			// 
			this->contextMenuStripAttach->ImageScalingSize = System::Drawing::Size(20, 20);
			this->contextMenuStripAttach->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripAttachUpload });
			this->contextMenuStripAttach->Name = L"contextMenuStripAttach";
			this->contextMenuStripAttach->Size = System::Drawing::Size(211, 56);
			// 
			// toolStripAttachUpload
			// 
			this->toolStripAttachUpload->Name = L"toolStripAttachUpload";
			this->toolStripAttachUpload->Size = System::Drawing::Size(210, 24);
			this->toolStripAttachUpload->Text = L"Upload Attachment";
			this->toolStripAttachUpload->Click += gcnew System::EventHandler(this, &ClientWindow::toolStripAttachUpload_Click);
			// 
			// ClientWindow
			// 
			this->AcceptButton = this->buttonSendMsg;
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(618, 499);
			this->Controls->Add(this->buttonUploadImg);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->richTextBoxChat);
			this->Controls->Add(this->buttonSendMsg);
			this->Controls->Add(this->textBoxMessage);
			this->Font = (gcnew System::Drawing::Font(L"Open Sans", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"ClientWindow";
			this->Text = L"SocketChat";
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->contextMenuStripAttach->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		int port;
		IPAddress^ ip;
		String^ name;
		bool inputSuccess;
	public:
#pragma region setter-getters
		int getPort() {
			return port;
		}
		IPAddress^ getIP() {
			return ip;
		}
		String^ getName() {
			return name;
		}
		bool getInputSuccess() {
			return inputSuccess;
		}
	
		void setPort(int port) {
			this->port = port;
		}
		void setIP(IPAddress^ ip) {
			this->ip = ip;
		}
		void setName(String^ name) {
			this->name = name;
		}
		void setInputSuccess(bool success) {
			this->inputSuccess = success;
		}
#pragma endregion

	private:
		Socket^ messageSocket;
		bool connectSuccess;
		List<String^>^ imagePathes;
		
	private: void itemConnect_Click(System::Object^  sender, System::EventArgs^  e);

	private: void ClientWindow::connectToTheServer();

	private: void itemDisconnect_Click(System::Object^  sender, System::EventArgs^  e);

	private: void setChatWorking(bool toStart);

	private: void buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e);

	private: delegate void MessageDelegate(String^ message);
			 delegate void EventDelegate(Object^ sender, EventArgs^ e);
			 delegate void imageDelegate(Image^ image);

	private: void sendMessage(); 
			 void sendImageMessage(int imageNum);
			 void setMessage(String^ message); 

	private: void startMessageTransfering();
			 void sendSystemMessage(String^ message);
			 String^ getStringMessage();
			 void addChatMessage(String^ message);
			 Image^ getImageMessage();
			 void insertChatImage(Image^ image);

	private: void buttonUploadImg_Click(System::Object^  sender, System::EventArgs^  e);
			 void toolStripAttachUpload_Click(System::Object^  sender, System::EventArgs^  e);
			 void saveImage(Image^ image);
};
}
