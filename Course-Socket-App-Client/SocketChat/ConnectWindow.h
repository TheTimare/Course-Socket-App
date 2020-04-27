#pragma once
#include "ClientWindow.h"

namespace SocketChatClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Net::Sockets;

	/// <summary>
	/// —водка дл€ ConnectWindow
	/// </summary>
	public ref class ConnectWindow : public System::Windows::Forms::Form
	{
	public:
		ConnectWindow(ClientWindow^ client);

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~ConnectWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::TextBox^  textBoxName;
	private: System::Windows::Forms::TextBox^  textBoxIP;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxPort;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  buttonConnect;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxIP = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->buttonConnect = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Your name";
			// 
			// textBoxName
			// 
			this->textBoxName->Location = System::Drawing::Point(24, 29);
			this->textBoxName->Name = L"textBoxName";
			this->textBoxName->Size = System::Drawing::Size(136, 22);
			this->textBoxName->TabIndex = 1;
			// 
			// textBoxIP
			// 
			this->textBoxIP->Location = System::Drawing::Point(24, 74);
			this->textBoxIP->Name = L"textBoxIP";
			this->textBoxIP->Size = System::Drawing::Size(136, 22);
			this->textBoxIP->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 54);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 17);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Server IP";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(24, 119);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(74, 22);
			this->textBoxPort->TabIndex = 5;
			this->textBoxPort->Text = L"8080";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(21, 99);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 17);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Server Port";
			// 
			// buttonConnect
			// 
			this->buttonConnect->Location = System::Drawing::Point(24, 147);
			this->buttonConnect->Name = L"buttonConnect";
			this->buttonConnect->Size = System::Drawing::Size(136, 30);
			this->buttonConnect->TabIndex = 6;
			this->buttonConnect->Text = L"Connect";
			this->buttonConnect->UseVisualStyleBackColor = true;
			this->buttonConnect->Click += gcnew System::EventHandler(this, &ConnectWindow::buttonConnect_Click);
			// 
			// ConnectWindow
			// 
			this->AcceptButton = this->buttonConnect;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(184, 191);
			this->Controls->Add(this->buttonConnect);
			this->Controls->Add(this->textBoxPort);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBoxIP);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxName);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ConnectWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Connect";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		ClientWindow^ mainWindow;

	private: System::Void buttonConnect_Click(System::Object^  sender, System::EventArgs^  e);

};
}
