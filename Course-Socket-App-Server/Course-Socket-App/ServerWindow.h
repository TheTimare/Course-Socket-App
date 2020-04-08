#pragma once

namespace CourseSocketApp {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Threading::Tasks;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Text;

	/// <summary>
	/// Сводка для ServerWindow
	/// </summary>
	public ref class ServerWindow : public System::Windows::Forms::Form
	{
	public:
		ServerWindow(void)
		{
			InitializeComponent();
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			isStarted = false;
			//get pc address
			IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
			for(int i = 0; i < host->AddressList->Length; i++){
				if (host->AddressList[i]->AddressFamily == AddressFamily::InterNetwork)
				{
					ip = host->AddressList[i]->ToString();
					labelIP->Text += ip;
					break;
				}
			}
		}

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

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxChat = (gcnew System::Windows::Forms::TextBox());
			this->button_on_off_server = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->labelIP = (gcnew System::Windows::Forms::Label());
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
			// ServerWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Menu;
			this->ClientSize = System::Drawing::Size(800, 432);
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
	private: String^ ip;
			 bool isStarted;

	private: System::Void button_on_off_server_Click(System::Object^  sender, System::EventArgs^  e) {
		if (isStarted) {
			textBoxPort->ReadOnly = false;
			button_on_off_server->Text = "Start Server";
			isStarted = false;
		}
		else {
			Task^ serverTask = gcnew Task(gcnew Action(this, &ServerWindow::serverStart));
			serverTask->Start();
			textBoxPort->ReadOnly = true;
			button_on_off_server->Text = "Shutdown Server";
			isStarted = true;
		}
	}

	private: void serverStart() {
		/*if (this->InvokeRequired){
			MethodInvoker^ d = gcnew MethodInvoker(this, &ServerWindow::foo);
			this->BeginInvoke(d);
		}
		else{
			foo();
		}*/

		int port;
		try {
			int port = Convert::ToInt32(textBoxPort->Text);
		} catch (...) {
			MessageBox::Show("Неправильно введен порт", "Ошибка",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		// получаем адреса для запуска сокета
		
		IPEndPoint^ ipPoint = gcnew IPEndPoint(IPAddress::Parse(ip), port);
		Socket^ listenSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		try {
			// связываем сокет с локальной точкой, по которой будем принимать данные
			listenSocket->Bind(ipPoint);
			// начинаем прослушивание
			listenSocket->Listen(10);

			MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
			this->BeginInvoke(msg, "SYSTEM", "Server is started. Waiting for users...");

			while (true) {
				Socket^ handler = listenSocket->Accept();
				// получаем сообщение
				StringBuilder^ builder = gcnew StringBuilder();
				int bytes = 0; // количество полученных байтов
				array<unsigned char>^ data = gcnew array<unsigned char>(256); // буфер для получаемых данных

				do {
					bytes = handler->Receive(data); //Receive(data);
					builder->Append(Encoding::Unicode->GetString(data, 0, bytes));
				} while (handler->Available > 0);

				msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
				this->BeginInvoke(msg, "USER", builder->ToString());

				// отправляем ответ
				String^ message = "success";
				data = Encoding::Unicode->GetBytes(message);
				handler->Send(data);
				// закрываем сокет
				handler->Shutdown(SocketShutdown::Both);
				handler->Close();
			}
		} catch (Exception^ ex){
			MessageBox::Show(ex->Message, "Ошибка",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		
		MessageDelegate^ msg = gcnew MessageDelegate(this, &ServerWindow::serverMessage);
		this->BeginInvoke(msg, "SYSTEM", "Server shutted down!!!");
	}
	
	private: delegate void MessageDelegate(String^ user, String^ msg);
	void serverMessage(String^ user, String^ msg) {
		textBoxChat->AppendText("[" + __TIME__ + "] ");
		textBoxChat->AppendText(user + ": " + msg + "\r\n");
	}
};
}
