#include "MyForm.h"
#include "tiger/sboxes.c"
#include "tiger/tiger.c"
#include "tigertree/tigertree.c"
#include "base32/base32.c"
#include <process.h>
#include <windows.h>
#include <memory>
#include "misc/TimerDiff.cpp"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Text;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	SimplyServer::MyForm myForm;        //NameOfProject::NameOfForm instanceOfForm;
	Application::Run(%myForm);
}


void ProcessFile(String^ path)
{
	SimplyServer::TTTHFile^ tth = gcnew SimplyServer::TTTHFile(true);
	tth->File = path; // файл
	tth->Execute();
	//tth->OnWorkBegin = OnTTHWorkBegin; // функция, которая вызывается во время начала хеширования
	//tth->OnWork = OnTTHWork;           // функция, которая вызывается периодически во время хеширования
	//tth->OnWorkEnd = OnTTHWorkEnd;     // функция, которая вызывается по завершении хеширования
	//tth->OnTerminate = OnTTHTerminate; // функция, которая вызывается по завершении работы модуля
	//tth->Interval = 1000;              // период вызова OnWork
	//tth->Resume();
}

void SimplyServer::TTTHFile::ProcessDirectory(String^ targetDirectory)
{
	array<String^>^fileEntries = Directory::GetFiles(targetDirectory);
	IEnumerator^ files = fileEntries->GetEnumerator();
	while (files->MoveNext())
	{
		String^ fileName = safe_cast<String^>(files->Current);
		ProcessFile(fileName);
	}

	array<String^>^subdirectoryEntries = Directory::GetDirectories(targetDirectory);
	IEnumerator^ dirs = subdirectoryEntries->GetEnumerator();
	while (dirs->MoveNext())
	{
		String^ subdirectory = safe_cast<String^>(dirs->Current);
		ProcessDirectory(subdirectory);
	}
}

SimplyServer::TTTHFile::TTTHFile(bool CreateSuspended)
{
	//Priority = THREAD_PRIORITY_NORMAL;
	//FreeOnTerminate = true;
	iTotalReaded_ = 0;
	iFileSize_ = 0;
	Interval = 500;
}
//---------------------------------------------------------------------------

void  SimplyServer::TTTHFile::Execute()
{
	// изменить приоритет потока
	//SetThreadPriority(GetCurrentThread(), Priority);

	// открытие файла на чтение
	FileStream^ fs;
	try
	{
		fs = File::OpenRead(File);
	}
	catch (Exception^ ex)
	{
		return;
	}

	// вызов OnWorkBegin 
	iFileSize_ = fs->Length;
	//[synchronize]
	//SyncOnWorkBegin;

	// инициализация TTH
	TT_CONTEXT ctx;
	unsigned char hash[TIGERSIZE];
	tt_init(&ctx);

	// буфер
	const int BUFFER_SIZE = 65536;
	char *szBuffer = new char[BUFFER_SIZE];
	array<unsigned char>^ b = gcnew array<unsigned char>(BUFFER_SIZE);
	UTF8Encoding^ temp = gcnew UTF8Encoding(true);
	__int64 iPrevReaded = 0;
	__int64 Start = TimerInit();

	int cbReaded = 0;

	// чтение из файла и вычисление TTH
	while ((cbReaded = fs->Read(b, 0, BUFFER_SIZE)) > 0)
	{
		tt_update(&ctx, b, cbReaded);
		iTotalReaded_ += cbReaded;

		/*if (Terminated)
		{
		delete fs;
		delete szBuffer;
		return;
		}*/

		//if (TimerDiff(Start) > Interval)
		//{
		//	// вызов OnWork
		//	iProcents_ = 1.0 * iTotalReaded_ / iFileSize_ * 100;
		//	fSpeed_ = (iTotalReaded_ - iPrevReaded) / TimerDiff(Start);
		//	//[synchronize];
		//	//SyncOnWork;

		//	Start = TimerInit();
		//	iPrevReaded = iTotalReaded_;
		//}
	}

	// завершение вычисления TTH
	tt_digest(&ctx, hash);

	// конвертирование в base32
	char *dest = new char[40]; dest[39] = 0;
	to_base32(hash, 24, dest);
	//TTH = dest;

	// вызов OnWorkEnd
	//[synchronize];
	//SyncOnWorkEnd;
}
//---------------------------------------------------------------------------

void SimplyServer::TTTHFile::SyncOnWorkBegin()
{
	if (OnWorkBegin) OnWorkBegin(iFileSize_);
}

void SimplyServer::TTTHFile::SyncOnWork()
{
	if (OnWork) OnWork(iProcents_, fSpeed_, iTotalReaded_);
}

void SimplyServer::TTTHFile::SyncOnWorkEnd()
{
	if (OnWorkEnd) OnWorkEnd();
}