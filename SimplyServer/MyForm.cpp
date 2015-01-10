#include "MyForm.h"
#include "TTH/tiger_sbox.c"
#include "TTH/tiger.c"
#include "TTH/tth.c"
#include <process.h>
#include <windows.h>
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace std;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	SimplyServer::MyForm myForm;
	Application::Run(%myForm);
}

void byte_to_base32(char* dest, const unsigned char* src, unsigned len, int upper_case)
{
	const char a = (upper_case ? 'A' : 'a');
	unsigned shift = 0;
	unsigned char word;
	const unsigned char* e = src + len;
	while (src < e) {
		if (shift > 3) {
			word = (*src & (0xFF >> shift));
			shift = (shift + 5) % 8;
			word <<= shift;
			if (src + 1 < e)
				word |= *(src + 1) >> (8 - shift);
			++src;
		}
		else {
			shift = (shift + 5) % 8;
			word = (*src >> ((8 - shift) & 7)) & 0x1F;
			if (shift == 0) src++;
		}
		*dest++ = (word < 26 ? word + a : word + '2' - 26);
	}
	*dest = '\0';
}

void  SimplyServer::MyForm::ProcessFile(char *file, String^ file1)
{

	// инициализаци€ TTH
	tth_ctx ctx;
	unsigned char hash[tiger_hash_length];
	tth_init(&ctx);

	// буфер
	const int BUFFER_SIZE = 65536;
	unsigned char buf[BUFFER_SIZE];
	INT64 iFileSize_ = 0, iTotalReaded_=0;
	HANDLE hFile;
	DWORD dwBytesRead;
	hFile = CreateFile(file, // open testfile.txt
		GENERIC_READ, // open for reading
		0, // do not share
		NULL, // default security
		OPEN_EXISTING, // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	iFileSize_ = GetFileSize(hFile, NULL);
	do
	{
		ReadFile(hFile, buf, 65536, &dwBytesRead, NULL);
		if (dwBytesRead < 1) { break; }
		tth_update(&ctx, buf, dwBytesRead);
		iTotalReaded_ += dwBytesRead;
		if (stop == true) break;
	} while (true);
	CloseHandle(hFile);
	// завершение вычислени€ TTH
	tth_final(&ctx, hash);
	if (stop == true) return;
	byte_to_base32(TTH, hash, 24, 1);
	String^ TTH1 = gcnew String(TTH, 0, 39);
	try {
	for (int i = 0; i < skip; i++)
	{
		file1 = file1->Substring((file1->IndexOf("\\") + 1), (file1->Length - (file1->IndexOf("\\") + 1)));
	}
	}
	catch (Exception^ e)
	{}
	sw->WriteLine("\""+((skip>0)?"\\":"") + file1 + "\";" + iFileSize_ + ";" + TTH1);
	//delete TTH1;
}
//---------------------------------------------------------------------------

void SimplyServer::MyForm::ProcessDirectory(String^ targetDirectory)
{
	String^ fileName1;
	try {
		array<String^>^fileEntries = Directory::GetFiles(targetDirectory);
		IEnumerator^ files = fileEntries->GetEnumerator();
		while (files->MoveNext())
		{
			fileName1 = safe_cast<String^>(files->Current);
			char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(fileName1);
			UpdateText(fileName1);
			ProcessFile(str2, fileName1);
			Marshal::FreeHGlobal((IntPtr)str2);
			if (stop == true) break;
		}
	}
	catch (Exception^ e)
	{
		UpdateText("Error read file: " + fileName1);
	}
	String^ subdirectory;
	try {
		array<String^>^subdirectoryEntries = Directory::GetDirectories(targetDirectory);
		IEnumerator^ dirs = subdirectoryEntries->GetEnumerator();
		while (dirs->MoveNext())
		{
			subdirectory = safe_cast<String^>(dirs->Current);
			ProcessDirectory(subdirectory);
			if (stop == true) break;
		}
	}
	catch (Exception^ e)
	{
		UpdateText("Error read directory: " + subdirectory);
	}
}

void SimplyServer::MyForm::ScanDir()
{
	try {
	sw = gcnew StreamWriter(fileName);
	for each (String ^ item in listBox1->Items)
	{
		ProcessDirectory(item);
		if (stop == true) break;
	}
	sw->Close();
	delete sw;
	UpdateText("Ready");
	}
	catch (Exception^ e)
	{
		UpdateText("Error write to file");
	}
	StopScan();
	System::GC::Collect();
}