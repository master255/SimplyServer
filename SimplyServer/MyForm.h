#pragma once

namespace SimplyServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO;
	using namespace System::Diagnostics;
	using namespace Microsoft::Win32;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  button4;
	private: String^ folderName;
	private: String^ fileName;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: Thread ^ thread;
	private: System::Windows::Forms::Button^  button5;
	private: ToolTip^ tt = gcnew ToolTip();
	private: bool stop;
	private: StreamWriter^ sw;
	private: int skip = 3;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  label4;
	private: RegistryKey^ CU;
	private: RegistryKey^ ML;
	private: System::Windows::Forms::LinkLabel^  linkLabel1;
	private: System::Windows::Forms::LinkLabel^  linkLabel2;
	private: System::Windows::Forms::LinkLabel^  linkLabel3;

	private: char*TTH = new char[24];
	public:
		delegate void UpdateTextCallback(String^ text);
		delegate void StopScanCallback();
		void ScanDir();
		void ProcessDirectory(String^ targetDirectory);
		void ProcessFile(char *file, String^ file1);
		void UpdateText(String^ text)
		{
			if (label3->InvokeRequired)
			{
				UpdateTextCallback^ tempDelegate =
					gcnew UpdateTextCallback(this, &SimplyServer::MyForm::UpdateText);
				//cli::array<System::Object^>^ args = gcnew cli::array<System::Object^>(1);
				//args[0] = text; на будущее
				label3->Invoke(tempDelegate, text);
			}
			else {
				label3->Text = text;
				tt->SetToolTip(label3, text);
			}
		}
		void StopScan()
		{
			if ((button3->InvokeRequired)||(button5->InvokeRequired))
			{
				StopScanCallback^ tempDelegate1 =
					gcnew StopScanCallback(this, &SimplyServer::MyForm::StopScan);
				//cli::array<System::Object^>^ args = gcnew cli::array<System::Object^>(1);
				//args[0] = text; на будущее
				button3->Invoke(tempDelegate1);
				button5->Invoke(tempDelegate1);
			}
			else {
				button3->Visible = true;
				button5->Visible = false;
			}
		}
	protected:
	private:
		
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->linkLabel2 = (gcnew System::Windows::Forms::LinkLabel());
			this->linkLabel3 = (gcnew System::Windows::Forms::LinkLabel());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 348);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(107, 38);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Add dir";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Enabled = false;
			this->checkBox1->Location = System::Drawing::Point(22, 24);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(95, 17);
			this->checkBox1->TabIndex = 1;
			this->checkBox1->Text = L"Server running";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Enabled = false;
			this->label1->Location = System::Drawing::Point(229, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Web Pin:";
			// 
			// textBox1
			// 
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(286, 26);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 3;
			this->textBox1->Text = L"no id";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->listBox1);
			this->groupBox1->Location = System::Drawing::Point(12, 52);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(470, 264);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Folders";
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(6, 19);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(457, 238);
			this->listBox1->TabIndex = 0;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(125, 348);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(89, 37);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Remove";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(220, 349);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 37);
			this->button3->TabIndex = 6;
			this->button3->Text = L"Scan TTH";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(18, 322);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(358, 20);
			this->textBox2->TabIndex = 7;
			this->textBox2->Text = L"Choose directory to save file";
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox2_TextChanged);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(382, 322);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(100, 31);
			this->button4->TabIndex = 8;
			this->button4->Text = L"Browse";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// folderBrowserDialog1
			// 
			this->folderBrowserDialog1->HelpRequest += gcnew System::EventHandler(this, &MyForm::folderBrowserDialog1_HelpRequest);
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->DefaultExt = L"txt";
			this->saveFileDialog1->FileName = L"tth";
			this->saveFileDialog1->Filter = L"CSV (*.csv)|*.csv|All files (*.*)|*.*";
			this->saveFileDialog1->InitialDirectory = L"%USERPROFILE%\\Desktop";
			this->saveFileDialog1->RestoreDirectory = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(19, 399);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(60, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Current file:";
			this->label2->Visible = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(86, 399);
			this->label3->MaximumSize = System::Drawing::Size(400, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 10;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(220, 348);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(100, 37);
			this->button5->TabIndex = 11;
			this->button5->Text = L"Stop Scan";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Visible = false;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(421, 370);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(34, 20);
			this->textBox3->TabIndex = 12;
			this->textBox3->Text = L"3";
			this->textBox3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox3_TextChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(326, 373);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(89, 13);
			this->label4->TabIndex = 13;
			this->label4->Text = L"Don\'t write folder:";
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(460, 9);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(25, 13);
			this->linkLabel1->TabIndex = 14;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"Site";
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel1_LinkClicked);
			// 
			// linkLabel2
			// 
			this->linkLabel2->AutoSize = true;
			this->linkLabel2->Location = System::Drawing::Point(430, 22);
			this->linkLabel2->Name = L"linkLabel2";
			this->linkLabel2->Size = System::Drawing::Size(55, 13);
			this->linkLabel2->TabIndex = 15;
			this->linkLabel2->TabStop = true;
			this->linkLabel2->Text = L"Feedback";
			this->linkLabel2->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel2_LinkClicked);
			// 
			// linkLabel3
			// 
			this->linkLabel3->AutoSize = true;
			this->linkLabel3->Location = System::Drawing::Point(444, 37);
			this->linkLabel3->Name = L"linkLabel3";
			this->linkLabel3->Size = System::Drawing::Size(41, 13);
			this->linkLabel3->TabIndex = 16;
			this->linkLabel3->TabStop = true;
			this->linkLabel3->Text = L"Source";
			this->linkLabel3->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &MyForm::linkLabel3_LinkClicked);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(494, 435);
			this->Controls->Add(this->linkLabel3);
			this->Controls->Add(this->linkLabel2);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->Text = L"Simply Server";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::DialogResult result = folderBrowserDialog1->ShowDialog();
			if (result == System::Windows::Forms::DialogResult::OK)
			{
				folderName = folderBrowserDialog1->SelectedPath;
				listBox1->Items->Add(folderName);
			}
		}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		listBox1->Items->Remove(listBox1->SelectedItem);
	}
	private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void folderBrowserDialog1_HelpRequest(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		System::Windows::Forms::DialogResult result = saveFileDialog1->ShowDialog();
		if (result == System::Windows::Forms::DialogResult::OK)
		{
			fileName = saveFileDialog1->FileName;
			textBox2->Text = fileName;
		}
	}
	 

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		
		if (thread == nullptr)
		{
			thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::ScanDir));
		}
		else {
			delete thread;
			System::GC::Collect();
			thread = gcnew Thread(gcnew ThreadStart(this, &MyForm::ScanDir));
		}
		if ((listBox1->Items->Count>0) && (!thread->IsAlive) && ((fileName!=nullptr)&&(fileName->Length>3))) {
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
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	stop = true;
	button3->Visible = true;
	button5->Visible = false;
}
private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	LinkLabel::Link^ link = gcnew LinkLabel::Link();
	link->LinkData = "http://master255.org/";
	linkLabel1->Links->Add(link);
	LinkLabel::Link^ link1 = gcnew LinkLabel::Link();
	link1->LinkData = "http://4pda.ru/forum/index.php?showtopic=580001";
	linkLabel2->Links->Add(link1);
	LinkLabel::Link^ link2 = gcnew LinkLabel::Link();
	link2->LinkData = "https://github.com/master255/SimplyServer";
	linkLabel3->Links->Add(link2);
	
	try{
	CU = Registry::CurrentUser;
	ML = CU->OpenSubKey("SOFTWARE\\Masters\\Media Library", true);
	if (ML != nullptr) {
		fileName = static_cast<String^>(ML->GetValue("SaveFile"));
		skip = static_cast<int>(ML->GetValue("Dontfldr"));
		textBox3->Text = skip.ToString();
		String^ fold = static_cast<String^>(ML->GetValue("Folders"));
		int y;
		for (int i = 0; i < fold->Length; i++)
			{
				y = fold->IndexOf("|", i);
				if (y>0) {
					listBox1->Items->Add(fold->Substring(i, y-i));
					i = y;
				}
				else { break; }
			}
	}
	else {
		ML=CU->CreateSubKey("SOFTWARE\\Masters\\Media Library");
		ML->SetValue("SaveFile", "");
		ML->SetValue("Dontfldr", 3);
		ML->SetValue("Folders", "");
	}
	}
	catch (Exception^ e)
	{
		
	}
	try{
		if (fileName == nullptr)
	fileName = Environment::GetFolderPath(Environment::SpecialFolder::DesktopDirectory) + "\\tth.csv";
	textBox2->Text = fileName;
	}
	catch (Exception^ e)
	{}
}
private: System::Void linkLabel1_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
	Process::Start(e->Link->LinkData->ToString());
}
private: System::Void linkLabel2_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
	Process::Start(e->Link->LinkData->ToString());
}
private: System::Void linkLabel3_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
	Process::Start(e->Link->LinkData->ToString());
}
	private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		try{
		skip = int::Parse(textBox3->Text);
	}
			 catch (Exception^ e)
			 {
				 textBox3->Text = "3";
			 }
}
private: System::Void MyForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
	try
	{
		ML->SetValue("SaveFile", fileName);
		ML->SetValue("Dontfldr", skip);
		String^ fold;
			for (int i = 0; i < listBox1->Items->Count; i++)
			{
				fold = fold + listBox1->GetItemText(listBox1->Items[i])+"|";
			}
		ML->SetValue("Folders", fold);
	}
	catch (Exception^ e)
	{
		MessageBox::Show(e->Message);
	}
	finally
	{
		if (CU) CU->Close();
		if (ML) ML->Close();
	}
}

private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	fileName = textBox2->Text;
}
};
}
