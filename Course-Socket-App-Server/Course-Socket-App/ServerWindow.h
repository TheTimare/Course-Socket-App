#pragma once
#include <cliext/utility>  

namespace CourseSocketApp {

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

	/// <summary>
	/// ������ ��� ServerWindow
	/// </summary>
	public ref class ServerWindow : public System::Windows::Forms::Form
	{
	public:
		ServerWindow(void);

	protected:
		~ServerWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:

	private: System::Windows::Forms::Button^  button_on_off_server;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxPort;



	private: System::Windows::Forms::DomainUpDown^  domainUpDownIPs;
	private: System::Windows::Forms::Button^  buttonSendMsg;
	private: System::Windows::Forms::TextBox^  textBoxMessage;
	private: System::Windows::Forms::RichTextBox^  richTextBoxChat;




	protected:

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ServerWindow::typeid));
			this->button_on_off_server = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->domainUpDownIPs = (gcnew System::Windows::Forms::DomainUpDown());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->textBoxMessage = (gcnew System::Windows::Forms::TextBox());
			this->richTextBoxChat = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// button_on_off_server
			// 
			this->button_on_off_server->Location = System::Drawing::Point(16, 438);
			this->button_on_off_server->Name = L"button_on_off_server";
			this->button_on_off_server->Size = System::Drawing::Size(143, 35);
			this->button_on_off_server->TabIndex = 1;
			this->button_on_off_server->Text = L"Start Server";
			this->button_on_off_server->UseVisualStyleBackColor = true;
			this->button_on_off_server->Click += gcnew System::EventHandler(this, &ServerWindow::button_on_off_server_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 20);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Port:";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(60, 15);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(99, 28);
			this->textBoxPort->TabIndex = 3;
			this->textBoxPort->Text = L"8080";
			// 
			// domainUpDownIPs
			// 
			this->domainUpDownIPs->BackColor = System::Drawing::SystemColors::Window;
			this->domainUpDownIPs->Location = System::Drawing::Point(16, 49);
			this->domainUpDownIPs->Name = L"domainUpDownIPs";
			this->domainUpDownIPs->ReadOnly = true;
			this->domainUpDownIPs->Size = System::Drawing::Size(143, 28);
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
			this->textBoxMessage->Size = System::Drawing::Size(430, 50);
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
			// ServerWindow
			// 
			this->AcceptButton = this->buttonSendMsg;
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::HighlightText;
			this->ClientSize = System::Drawing::Size(700, 486);
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
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: List<String^>^ chatPool;
			 List<Socket^>^ connected;
			 int selectedIP;
			 bool isStarted;
			 Hashtable^ userDB;
			 Socket^ mainSocket;
			 
	private: void button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e);
			 void serverStart();

	private: void startMessageGetting(Object^ handler);
			 void ServerWindow::startSynchronizeMessages(Object^ handler);

	private: delegate void SocketDelegate(Socket^ mainSocket);
			 void setSocket(Socket^ mainSocket);
	
	private: delegate void MessageDelegate(String^ user, String^ msg);
			 void serverMessage(String^ user, String^ msg);
	
	private: void sendSystemCommand(Socket^ connection, String^ command);

	private: void setChatWorking(bool isWorking);

	private: void domainUpDownIPs_SelectedItemChanged(System::Object^  sender, System::EventArgs^  e);

	private: void buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e);
};
}
