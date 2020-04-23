#include "ServerWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[System::Runtime::InteropServices::DllImport("user32.dll")]
extern bool SetProcessDPIAware();

[STAThreadAttribute]
void Main(array<String^>^ args) {
	if (Environment::OSVersion->Version->Major >= 6)
		SetProcessDPIAware();
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CourseSocketApp::ServerWindow());
}