#pragma once

namespace CourseSocketAppClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ ClientWindow
	/// </summary>
	public ref class ClientWindow : public System::Windows::Forms::Form
	{
	public:
		ClientWindow(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~ClientWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBoxChat;
	protected:
	private: System::Windows::Forms::TextBox^  textBoxMessage;
	private: System::Windows::Forms::Button^  buttonSendMsg;
	private: System::Windows::Forms::Label^  label1;
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
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxChat = (gcnew System::Windows::Forms::TextBox());
			this->textBoxMessage = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxIP = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->buttonConnect = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBoxChat
			// 
			this->textBoxChat->BackColor = System::Drawing::SystemColors::Window;
			this->textBoxChat->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxChat->Location = System::Drawing::Point(177, 12);
			this->textBoxChat->Multiline = true;
			this->textBoxChat->Name = L"textBoxChat";
			this->textBoxChat->ReadOnly = true;
			this->textBoxChat->Size = System::Drawing::Size(507, 355);
			this->textBoxChat->TabIndex = 0;
			// 
			// textBoxMessage
			// 
			this->textBoxMessage->BackColor = System::Drawing::SystemColors::Window;
			this->textBoxMessage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMessage->Location = System::Drawing::Point(177, 374);
			this->textBoxMessage->Multiline = true;
			this->textBoxMessage->Name = L"textBoxMessage";
			this->textBoxMessage->ReadOnly = true;
			this->textBoxMessage->Size = System::Drawing::Size(409, 52);
			this->textBoxMessage->TabIndex = 1;
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Location = System::Drawing::Point(593, 374);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(91, 52);
			this->buttonSendMsg->TabIndex = 2;
			this->buttonSendMsg->Text = L"Send Message";
			this->buttonSendMsg->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 17);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Your Name";
			// 
			// textBoxName
			// 
			this->textBoxName->Location = System::Drawing::Point(16, 32);
			this->textBoxName->Name = L"textBoxName";
			this->textBoxName->Size = System::Drawing::Size(143, 22);
			this->textBoxName->TabIndex = 4;
			// 
			// textBoxIP
			// 
			this->textBoxIP->Location = System::Drawing::Point(16, 77);
			this->textBoxIP->Name = L"textBoxIP";
			this->textBoxIP->Size = System::Drawing::Size(143, 22);
			this->textBoxIP->TabIndex = 6;
			this->textBoxIP->Text = L"127.0.0.1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(13, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 17);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Server IP";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(16, 122);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(63, 22);
			this->textBoxPort->TabIndex = 8;
			this->textBoxPort->Text = L"8080";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 102);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 17);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Server Port";
			// 
			// buttonConnect
			// 
			this->buttonConnect->Location = System::Drawing::Point(16, 150);
			this->buttonConnect->Name = L"buttonConnect";
			this->buttonConnect->Size = System::Drawing::Size(143, 28);
			this->buttonConnect->TabIndex = 9;
			this->buttonConnect->Text = L"button1";
			this->buttonConnect->UseVisualStyleBackColor = true;
			// 
			// ClientWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Menu;
			this->ClientSize = System::Drawing::Size(696, 438);
			this->Controls->Add(this->buttonConnect);
			this->Controls->Add(this->textBoxPort);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBoxIP);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxName);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonSendMsg);
			this->Controls->Add(this->textBoxMessage);
			this->Controls->Add(this->textBoxChat);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ClientWindow";
			this->Text = L"Client Chat";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
