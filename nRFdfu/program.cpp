// dfu.cpp : main project file.

#include "stdafx.h"

using namespace System;

//#include "dfu_W_Form.h"
#include "dfu_Form.h"
using namespace dfu;

//Added because OpenFileDialog had a fit
[System::STAThread] //https://smellegantcode.wordpress.com/2008/06/07/ccliwpf/

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hello World");
    
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false); 
    Application::Run(gcnew dfu_Form());
    //Application::Run(gcnew dfu_W_Form());
    

    Console::WriteLine(L"Good Bye World");
    return 0;
}
