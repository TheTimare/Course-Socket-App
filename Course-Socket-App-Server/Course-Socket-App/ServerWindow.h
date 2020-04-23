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
	/// —водка дл€ ServerWindow
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
	private: System::Windows::Forms::TextBox^  textBoxChat;
	protected:

	private: System::Windows::Forms::Button^  button_on_off_server;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxPort;

	private: System::Windows::Forms::Label^  labelIP;

	private: System::Windows::Forms::DomainUpDown^  domainUpDownIPs;
	private: System::Windows::Forms::Button^  buttonSetIP;

	private: System::Windows::Forms::TextBox^  textBoxIP;

	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxChat = (gcnew System::Windows::Forms::TextBox());
			this->button_on_off_server = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->labelIP = (gcnew System::Windows::Forms::Label());
			this->domainUpDownIPs = (gcnew System::Windows::Forms::DomainUpDown());
			this->buttonSetIP = (gcnew System::Windows::Forms::Button());
			this->textBoxIP = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// textBoxChat
			// 
			this->textBoxChat->BackColor = System::Drawing::SystemColors::Window;
			this->textBoxChat->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxChat->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->textBoxChat->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBoxChat->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->textBoxChat->Location = System::Drawing::Point(172, 14);
			this->textBoxChat->Multiline = true;
			this->textBoxChat->Name = L"textBoxChat";
			this->textBoxChat->ReadOnly = true;
			this->textBoxChat->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxChat->Size = System::Drawing::Size(518, 459);
			this->textBoxChat->TabIndex = 0;
			this->textBoxChat->TabStop = false;
			// 
			// button_on_off_server
			// 
			this->button_on_off_server->Location = System::Drawing::Point(16, 438);
			this->button_on_off_server->Name = L"button_on_off_server";
			this->button_on_off_server->Size = System::Drawing::Size(148, 35);
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
			// labelIP
			// 
			this->labelIP->AutoSize = true;
			this->labelIP->Location = System::Drawing::Point(12, 64);
			this->labelIP->Name = L"labelIP";
			this->labelIP->Size = System::Drawing::Size(30, 20);
			this->labelIP->TabIndex = 4;
			this->labelIP->Text = L"IP: ";
			// 
			// domainUpDownIPs
			// 
			this->domainUpDownIPs->BackColor = System::Drawing::SystemColors::Window;
			this->domainUpDownIPs->Location = System::Drawing::Point(16, 95);
			this->domainUpDownIPs->Name = L"domainUpDownIPs";
			this->domainUpDownIPs->ReadOnly = true;
			this->domainUpDownIPs->Size = System::Drawing::Size(143, 28);
			this->domainUpDownIPs->TabIndex = 6;
			this->domainUpDownIPs->Text = L"No ip founded";
			this->domainUpDownIPs->Wrap = true;
			// 
			// buttonSetIP
			// 
			this->buttonSetIP->Location = System::Drawing::Point(16, 129);
			this->buttonSetIP->Name = L"buttonSetIP";
			this->buttonSetIP->Size = System::Drawing::Size(52, 27);
			this->buttonSetIP->TabIndex = 7;
			this->buttonSetIP->Text = L"Set IP";
			this->buttonSetIP->UseVisualStyleBackColor = true;
			this->buttonSetIP->Click += gcnew System::EventHandler(this, &ServerWindow::buttonSetIP_Click);
			// 
			// textBoxIP
			// 
			this->textBoxIP->Location = System::Drawing::Point(48, 61);
			this->textBoxIP->Name = L"textBoxIP";
			this->textBoxIP->ReadOnly = true;
			this->textBoxIP->Size = System::Drawing::Size(111, 28);
			this->textBoxIP->TabIndex = 8;
			// 
			// ServerWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->BackColor = System::Drawing::SystemColors::HighlightText;
			this->ClientSize = System::Drawing::Size(700, 486);
			this->Controls->Add(this->textBoxIP);
			this->Controls->Add(this->buttonSetIP);
			this->Controls->Add(this->domainUpDownIPs);
			this->Controls->Add(this->labelIP);
			this->Controls->Add(this->textBoxPort);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_on_off_server);
			this->Controls->Add(this->textBoxChat);
			this->Font = (gcnew System::Drawing::Font(L"Open Sans", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ServerWindow";
			this->Text = L"SocketChat";
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
			 
	private: void buttonSetIP_Click(System::Object^  sender, System::EventArgs^  e);
			 void button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e);
			 void serverStart();

	private: void startMessageGetting(Object^ handler);
			 void ServerWindow::startSynchronizeMessages(Object^ handler);

	private: delegate void SocketDelegate(Socket^ mainSocket);
			 void setSocket(Socket^ mainSocket);
	
	private: delegate void MessageDelegate(String^ user, String^ msg);
			 void serverMessage(String^ user, String^ msg);
	
	private: void sendSystemCommand(Socket^ connection, String^ command);

	private: void setChatWorking(bool isWorking);

};
}
