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
			this->textBoxIP = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// textBoxChat
			// 
			this->textBoxChat->BackColor = System::Drawing::SystemColors::Window;
			this->textBoxChat->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxChat->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->textBoxChat->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->textBoxChat->Location = System::Drawing::Point(196, 12);
			this->textBoxChat->Multiline = true;
			this->textBoxChat->Name = L"textBoxChat";
			this->textBoxChat->ReadOnly = true;
			this->textBoxChat->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBoxChat->Size = System::Drawing::Size(592, 408);
			this->textBoxChat->TabIndex = 0;
			this->textBoxChat->TabStop = false;
			// 
			// button_on_off_server
			// 
			this->button_on_off_server->Location = System::Drawing::Point(17, 389);
			this->button_on_off_server->Name = L"button_on_off_server";
			this->button_on_off_server->Size = System::Drawing::Size(169, 31);
			this->button_on_off_server->TabIndex = 1;
			this->button_on_off_server->Text = L"Start Server";
			this->button_on_off_server->UseVisualStyleBackColor = true;
			this->button_on_off_server->Click += gcnew System::EventHandler(this, &ServerWindow::button_on_off_server_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(14, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(38, 17);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Port:";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(58, 13);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(123, 22);
			this->textBoxPort->TabIndex = 3;
			this->textBoxPort->Text = L"8080";
			// 
			// labelIP
			// 
			this->labelIP->AutoSize = true;
			this->labelIP->Location = System::Drawing::Point(14, 47);
			this->labelIP->Name = L"labelIP";
			this->labelIP->Size = System::Drawing::Size(74, 17);
			this->labelIP->TabIndex = 4;
			this->labelIP->Text = L"Server IP: ";
			// 
			// textBoxIP
			// 
			this->textBoxIP->Location = System::Drawing::Point(81, 44);
			this->textBoxIP->Name = L"textBoxIP";
			this->textBoxIP->ReadOnly = true;
			this->textBoxIP->Size = System::Drawing::Size(100, 22);
			this->textBoxIP->TabIndex = 5;
			// 
			// ServerWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::HighlightText;
			this->ClientSize = System::Drawing::Size(800, 432);
			this->Controls->Add(this->textBoxIP);
			this->Controls->Add(this->labelIP);
			this->Controls->Add(this->textBoxPort);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_on_off_server);
			this->Controls->Add(this->textBoxChat);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ServerWindow";
			this->Text = L"Chat Server";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//main code
	private: IPAddress^ ip;
			 bool isStarted;
			 Task^ serverTask;
			 Hashtable^ userDB;
			 
	private: Socket^ mainSocket;
			 void button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e);
			 void serverStart();
			 void startMessageGetting(Socket^ handler);

	private: delegate void SocketDelegate(Socket^ mainSocket);
			 void setSocket(Socket^ mainSocket);
	
	private: delegate void MessageDelegate(String^ user, String^ msg);
			 void serverMessage(String^ user, String^ msg);

	private: static bool isSocketStillConnected(Socket^ socket);

	private: void setChatWorking(bool isWorking);

};
}
