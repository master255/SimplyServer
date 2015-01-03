#include "MyForm.h"
//#include "tiger/sboxes.c"
//#include "tiger/tiger.c"
//#include "tigertree/tigertree.c"
#include "base32/base32.c"
#include <process.h>
#include <windows.h>
#include <memory>
#include "misc/TimerDiff.cpp"
#include "tth/MerkleTree.h"
#include <iostream>
#include <fstream>
//#include "File.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Text;
using namespace System::Runtime::InteropServices;
using namespace std;
[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	SimplyServer::MyForm myForm;        //NameOfProject::NameOfForm instanceOfForm;
	Application::Run(%myForm);
}


void ProcessFile(char *path)
{
	SimplyServer::TTTHFile^ tth = gcnew SimplyServer::TTTHFile(true);
	tth->File1 = path; // файл
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
		char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(fileName);
		ProcessFile(str2);
		Marshal::FreeHGlobal((IntPtr)str2);
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
	unique_ptr<TigerTree>* p_tth;
	/*dcassert(p_tth != nullptr);
	if (p_tth == nullptr)
		return false;
	AutoArray<uint8_t> buf(p_buffSize);*/
	/*try
	{*/
	//	File f (File1, File::READ, File::OPEN, p_isAbsPath);
	
	const int BUFFER_SIZE = 65536;
	unsigned char buf[BUFFER_SIZE];
	int cbReaded = 0;
	HANDLE hFile;
	DWORD dwBytesRead;
	hFile = CreateFile(File1, // open testfile.txt
		GENERIC_READ, // open for reading
		0, // do not share
		NULL, // default security
		OPEN_EXISTING, // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	iFileSize_= GetFileSize(hFile, NULL);
	*p_tth = unique_ptr<TigerTree>(new TigerTree(TigerTree::calcBlockSize(iFileSize_, 1)));
	do
	{
		ReadFile(hFile, buf, BUFFER_SIZE, &dwBytesRead, NULL);
		if (dwBytesRead < 0) { break; }
		p_tth->get()->update(buf, dwBytesRead);
		//n = p_buffSize;
		//tt_update(&ctx, buf, dwBytesRead);
		iTotalReaded_ += dwBytesRead;
	} while (dwBytesRead>-1);







		
		
			// *****************************************************
			/*if (f.getSize() > 0)
			{
				size_t n = p_buffSize;
				while ((n = f.read(buf.data(), n)) > 0)
				{
					
				}
			}
			else
			{
				p_tth->get()->update("", 0);
			}*/
			// *****************************************************
		
		/*f.close();*/
		p_tth->get()->finalize();
		//return true;
	//}
	//catch (ifstream::failure e)
	//{
	//	//-V565
	//	// No File
	//}
	//return false;






	// изменить приоритет потока
	//SetThreadPriority(GetCurrentThread(), Priority);

	// открытие файла на чтение


	// вызов OnWorkBegin 
	//iFileSize_ = fs->Length;
	//[synchronize]
	//SyncOnWorkBegin;

	// инициализация TTH
	//TT_CONTEXT ctx;
	//unsigned char hash[TIGERSIZE];
	//tt_init(&ctx);

	//// буфер
	//const int BUFFER_SIZE = 65536;
	//char *szBuffer = new char[BUFFER_SIZE];
	//unsigned char buf[BUFFER_SIZE];
	//__int64 iPrevReaded = 0;
	//__int64 Start = TimerInit();
	//int cbReaded = 0;
	//HANDLE hFile;
	//DWORD dwBytesRead;
	//hFile = CreateFile(File, // open testfile.txt
	//	GENERIC_READ, // open for reading
	//	0, // do not share
	//	NULL, // default security
	//	OPEN_EXISTING, // existing file only
	//	FILE_ATTRIBUTE_NORMAL, // normal file
	//	NULL);
	//iFileSize_= GetFileSize(hFile, NULL);
	//do
	//{
	//	ReadFile(hFile, buf, 65536, &dwBytesRead, NULL);
	//	if (dwBytesRead < 0) { break; }
	//	tt_update(&ctx, buf, dwBytesRead);
	//	iTotalReaded_ += dwBytesRead;
	//} while (dwBytesRead>-1);

	//	//if (TimerDiff(Start) > Interval)
	//	//{
	//	//	// вызов OnWork
	//	//	iProcents_ = 1.0 * iTotalReaded_ / iFileSize_ * 100;
	//	//	fSpeed_ = (iTotalReaded_ - iPrevReaded) / TimerDiff(Start);
	//	//	//[synchronize];
	//	//	//SyncOnWork;

	//	//	Start = TimerInit();
	//	//	iPrevReaded = iTotalReaded_;
	//	//}
	////}

	//// завершение вычисления TTH
	//tt_digest(&ctx, hash);

	//// конвертирование в base32
	//char *dest = new char[40]; dest[39] = 0;
	//to_base32(hash, 24, dest);
	////TTH = dest;

	//// вызов OnWorkEnd
	////[synchronize];
	////SyncOnWorkEnd;
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