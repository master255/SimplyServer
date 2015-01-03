#pragma once

namespace SimplyServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	typedef void(*ONTTHWORKBEGINFUNC)(__int64 FileSize);
	typedef void(*ONTTHWORKFUNC)(int iProcents, double fSpeed, __int64 Readed);
	typedef void(*ONTTHWORKENDFUNC)();
	public ref class TTTHFile
	{
	private:

	protected:
		static int iProcents_;
		static double fSpeed_;
		static __int64 iFileSize_;
		static __int64 iTotalReaded_;
		void  SyncOnWorkBegin();
		void  SyncOnWork();
		void  SyncOnWorkEnd();
	public:
		TTTHFile(bool CreateSuspended);
		static void ProcessDirectory(String^ targetDirectory);
		ONTTHWORKBEGINFUNC OnWorkBegin;
		ONTTHWORKFUNC OnWork;
		ONTTHWORKENDFUNC OnWorkEnd;
		static void Execute();
		static char *File1;
		int Priority; // приоритет потока (см. SetThreadPriority)
		String^ TTH;
		static int Interval; // период вызовы OnWork
	};

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
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(22, 348);
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
			this->button2->Location = System::Drawing::Point(136, 349);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(89, 37);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Remove";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(232, 349);
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
			this->textBox2->Text = L"Choose save file directory";
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
			this->saveFileDialog1->Filter = L"Text files (*.txt)|*.txt|All files (*.*)|*.*";
			this->saveFileDialog1->InitialDirectory = L"%USERPROFILE%\\Desktop";
			this->saveFileDialog1->RestoreDirectory = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(494, 411);
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
			this->Name = L"MyForm";
			this->Text = L"Simply Server";
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
		for each (String ^ item in listBox1->Items)
		{
			SimplyServer::TTTHFile::ProcessDirectory(item);
		}
	}
};
	
}
