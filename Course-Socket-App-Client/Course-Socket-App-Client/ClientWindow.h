#pragma once

namespace CourseSocketAppClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Threading;
	using namespace System::Threading::Tasks;
	using namespace System::Text;

	/// <summary>
	/// —водка дл€ ClientWindow
	/// </summary>
	public ref class ClientWindow : public System::Windows::Forms::Form
	{
	public:
		ClientWindow(void);

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

	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  itemConnect;

	private: System::Windows::Forms::ToolStripMenuItem^  itemDisconnect;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ClientWindow::typeid));
			this->textBoxChat = (gcnew System::Windows::Forms::TextBox());
			this->textBoxMessage = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripDropDownButton1 = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->itemConnect = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemDisconnect = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBoxChat
			// 
			this->textBoxChat->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->textBoxChat->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxChat->Location = System::Drawing::Point(12, 40);
			this->textBoxChat->Multiline = true;
			this->textBoxChat->Name = L"textBoxChat";
			this->textBoxChat->ReadOnly = true;
			this->textBoxChat->Size = System::Drawing::Size(682, 355);
			this->textBoxChat->TabIndex = 1;
			this->textBoxChat->TabStop = false;
			// 
			// textBoxMessage
			// 
			this->textBoxMessage->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->textBoxMessage->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMessage->Location = System::Drawing::Point(12, 401);
			this->textBoxMessage->Multiline = true;
			this->textBoxMessage->Name = L"textBoxMessage";
			this->textBoxMessage->Size = System::Drawing::Size(584, 45);
			this->textBoxMessage->TabIndex = 0;
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Location = System::Drawing::Point(603, 401);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(91, 45);
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
			this->toolStrip1->Size = System::Drawing::Size(706, 27);
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
			// ClientWindow
			// 
			this->AcceptButton = this->buttonSendMsg;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->ClientSize = System::Drawing::Size(706, 458);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->buttonSendMsg);
			this->Controls->Add(this->textBoxMessage);
			this->Controls->Add(this->textBoxChat);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ClientWindow";
			this->Text = L"Chat Client";
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
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
		
	private: void itemConnect_Click(System::Object^  sender, System::EventArgs^  e);

	private: void clientConnect();

	private: void itemDisconnect_Click(System::Object^  sender, System::EventArgs^  e);

	private: void setChatWorking(bool toStart);

	private: void buttonSendMsg_Click(System::Object^  sender, System::EventArgs^  e);

	private: delegate void MessageDelegate(String^ message);

	private: void sendMessage(); 
			 void setMessage(String^ message); 

	private: void startMessageReceiving();
			 void addChatMessage(String^ message);
};
}
