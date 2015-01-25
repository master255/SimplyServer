bool autostart;
#include "MyForm.h"
#include "TTH/tiger_sbox.c"
#include "TTH/tiger.c"
#include "TTH/tth.c"
#include <process.h>
#include <windows.h>
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::IO::Compression;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace std;
using namespace System::Xml;
using namespace System::Text;

[STAThread]
void Main(array<String^>^ args)
{
	//bool autostart;

	for each(String^ arg in args)
	{
		if (arg == "/scan") { autostart = true; }
	}
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
void  SimplyServer::MyForm::Bclick()
{
	if (thread == nullptr)
	{
		thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::ScanDir));
	}
	else {
		delete thread;
		System::GC::Collect();
		thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::ScanDir));
	}
	if ((listBox1->Items->Count > 0) && (!thread->IsAlive) && ((fileName != nullptr) && (fileName->Length > 3))) {
		stop = false;
		thread->Start();
		label2->Visible = true;
		button3->Visible = false;
		button5->Visible = true;
	}
	else {
		label3->Text = "Choose folders and file to save";
	}
}
void  SimplyServer::MyForm::ProcessFile(String^ file1)
{
	HANDLE hFile;
	LPCWSTR file2 = (LPCWSTR)Marshal::StringToHGlobalUni(file1).ToPointer();
	hFile = CreateFileW(file2,
		GENERIC_READ, // open for reading
		0, // do not share
		NULL, // default security
		OPEN_EXISTING, // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	LARGE_INTEGER size;
	GetFileSizeEx(hFile, &size);
	if (size.QuadPart > (skipFile * 1048576))
	{
		recalc = false;
		if (first == false)
		{
			query = "/";
			filepaths = file1->Substring(file1->IndexOf(item->Substring(item->LastIndexOf("\\") + 1)))->Split(sl->ToCharArray());
			for (int i = 0; i < filepaths->Length; i++)
			{
				if (i == (filepaths->Length - 1)) {
					query = query + "/File [@Name=\"" + filepaths[i] + "\"]";
				}
				else {
					query = query + "/Directory [@Name=\"" + filepaths[i] + "\"]";
				}
			}
			node = Xml1->SelectSingleNode(query);
			if ((node != nullptr) && (node->Name == "File")) {
				if ((int::Parse(node->Attributes->GetNamedItem("Size")->Value)) == size.QuadPart)
				{
					CloseHandle(hFile);
						return; }
				else {
					recalc = true;
				}
			};
		}

		// init TTH
		tth_ctx ctx;
		unsigned char hash[tiger_hash_length];
		tth_init(&ctx);

		// buffer
		const int BUFFER_SIZE = 65536;
		unsigned char buf[BUFFER_SIZE];
		DWORD dwBytesRead;
		do
		{
			ReadFile(hFile, buf, 65536, &dwBytesRead, NULL);
			if (dwBytesRead < 1) { break; }
			tth_update(&ctx, buf, dwBytesRead);
			if (stop == true) break;
		} while (true);
		CloseHandle(hFile);
		// end calculating TTH
		tth_final(&ctx, hash);
		if (stop == true) return;
		byte_to_base32(TTH, hash, 24, 1);
		String^ TTH1 = gcnew String(TTH, 0, 39);
		if (first == true) {
			xw->WriteStartElement("File");
			xw->WriteAttributeString("Name", file1->Substring(file1->LastIndexOf("\\") + 1));
			xw->WriteAttributeString("Size", size.QuadPart.ToString());
			xw->WriteAttributeString("TTH", TTH1);
			xw->WriteEndElement();
		}
		else {
			query = "/";
			for (int i1 = 0; i1 < filepaths->Length; i1++)
			{
				if (i1 == (filepaths->Length - 1)) {
					query = query + "/File [@Name=\"" + filepaths[i1] + "\"]";
				}
				else {
					query = query + "/Directory [@Name=\"" + filepaths[i1] + "\"]";
				}
				node = Xml1->SelectSingleNode(query);
				if (node != nullptr) {
					tmpNode = node;
					if ((recalc == false) || (i1 < filepaths->Length - 1)) {
						continue;
					}
				};
				if (i1 == filepaths->Length - 1) {
					NewElem = Xml1->CreateElement("File");
					NewElem->SetAttribute("Name", filepaths[i1]);
					NewElem->SetAttribute("Size", size.QuadPart.ToString());
					NewElem->SetAttribute("TTH", TTH1);
				}
				else {
					NewElem = Xml1->CreateElement("Directory");
					NewElem->SetAttribute("Name", filepaths[i1]);
				}

				if (tmpNode != nullptr) {
					if ((recalc == false) || (i1 < filepaths->Length - 1))
					{
						tmpNode = tmpNode->AppendChild(NewElem);
					}
					else {
						tmpNode = tmpNode->ParentNode->ReplaceChild(NewElem, tmpNode);
					}
				}
				else {
						tmpNode = Xml1->DocumentElement->AppendChild(NewElem);
				}
			}
		}
	}
	else {
		CloseHandle(hFile);
	}
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
			UpdateText(fileName1);
			ProcessFile(fileName1);
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
			if (first == true) {
				xw->WriteStartElement("Directory");
				xw->WriteAttributeString("Name", subdirectory->Substring(subdirectory->LastIndexOf("\\") + 1));
			}
			ProcessDirectory(subdirectory);
			if (first == true) {
				xw->WriteEndElement();
			}
			if (stop == true) break;
		}
	}
	catch (Exception^ e)
	{
		UpdateText("Error read directory: " + subdirectory);
	}
}

void Compress(FileInfo^ fileToCompress)
{
	FileStream^ originalFileStream = fileToCompress->OpenRead();
	{
		if ((File::GetAttributes(fileToCompress->FullName) & FileAttributes::Hidden) != FileAttributes::Hidden & fileToCompress->Extension != ".gz")
		{
			FileStream^ compressedFileStream = File::Create(fileToCompress->FullName + ".gz");
			{
				GZipStream^ compressionStream = gcnew GZipStream(compressedFileStream, CompressionMode::Compress);
				{
					originalFileStream->CopyTo(compressionStream);
					originalFileStream->Close();
					compressionStream->Close();
				}
			}
		}
	}
}

void Decompress(FileInfo^ fileToDecompress)
{
	FileStream^ originalFileStream = fileToDecompress->OpenRead();
	{
		String^ currentFileName = fileToDecompress->FullName;
		String^ newFileName = currentFileName->Remove(currentFileName->Length - fileToDecompress->Extension->Length);

		FileStream^ decompressedFileStream = File::Create(newFileName);
		{
			GZipStream^ decompressionStream = gcnew GZipStream(originalFileStream, CompressionMode::Decompress);
			{
				decompressionStream->CopyTo(decompressedFileStream);
				decompressionStream->Close();
				decompressedFileStream->Close();
			}
		}
	}
}

void SimplyServer::MyForm::ScanDir()
{
	try {
		FileInfo^ fi1 = gcnew FileInfo(fileName);
		if (fi1->Exists == true) {
			Decompress(fi1);
			first = false;
		}
		else {
			first = true;
		}

		String^ path = fileName->Remove(fileName->LastIndexOf("."));
		if (first == false){
			Xml1 = gcnew XmlDocument;
			fs = gcnew FileStream(path, FileMode::Open, FileAccess::ReadWrite, FileShare::None);
			Xml1->Load(fs);
		}
		else {
			UTF8Encoding^ BOM = gcnew UTF8Encoding(false);
			fs = gcnew FileStream(path, FileMode::OpenOrCreate, FileAccess::Write, FileShare::None);
			xw = gcnew XmlTextWriter(fs, BOM);
			xw->Formatting = System::Xml::Formatting::Indented;
			xw->WriteStartDocument(true);
			xw->WriteStartElement("FileListing");
			xw->WriteAttributeString("Version", "1.0");
		}
		for each (item in listBox1->Items)
		{
			if (first == true) {
				xw->WriteStartElement("Directory");
				xw->WriteAttributeString("Name", item->Substring(item->LastIndexOf("\\") + 1));
			}
			ProcessDirectory(item);
			if (first == true) {
				xw->WriteEndElement();
			}
			if (stop == true) break;
		}
		if (first == true){
			xw->WriteEndElement();
			xw->WriteEndDocument();
			xw->Close();
			fs->Close();
		}
		else {
			fs->Seek(0, SeekOrigin::Begin);
			fs->SetLength(0);
			Xml1->Save(fs);
			fs->Close();
		}
		FileInfo^ fi2 = gcnew FileInfo(path);
		Compress(fi2);
		fi2->Delete();
		UpdateText("Ready");
	}
	catch (Exception^ e)
	{
		UpdateText("Error write to file");
	}
	StopScan();
	System::GC::Collect();
	if (autostart == true)
	{
		Application::Exit();
	}
}