#pragma once

#include <cinttypes>
#include "CLRBridge.h"

namespace Emulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FrmMain
	/// </summary>
	public ref class FrmMain : public System::Windows::Forms::Form
	{
	public:
		FrmMain(void)
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
		~FrmMain()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  lbl_r0;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  lbl_r1;

    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  lbl_r2;

    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  lbl_r3;

    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  lbl_r4;

    private: System::Windows::Forms::Label^  label10;
    private: System::Windows::Forms::Label^  lbl_r5;

    private: System::Windows::Forms::Label^  label12;
    private: System::Windows::Forms::Label^  lbl_r6;

    private: System::Windows::Forms::Label^  label14;
    private: System::Windows::Forms::Label^  lbl_r7;

    private: System::Windows::Forms::Label^  label16;
    private: System::Windows::Forms::Label^  lbl_r8;

    private: System::Windows::Forms::Label^  label18;
    private: System::Windows::Forms::Label^  lbl_r9;

    private: System::Windows::Forms::Label^  label20;
    private: System::Windows::Forms::Label^  lbl_r10;

    private: System::Windows::Forms::Label^  label22;
    private: System::Windows::Forms::Label^  lbl_r11;

    private: System::Windows::Forms::Label^  label24;
    private: System::Windows::Forms::Label^  lbl_r12;

    private: System::Windows::Forms::Label^  label26;
    private: System::Windows::Forms::Label^  lbl_sp;

    private: System::Windows::Forms::Label^  label28;
    private: System::Windows::Forms::Label^  lbl_lr;

    private: System::Windows::Forms::Label^  label30;
    private: System::Windows::Forms::Label^  lbl_pc;

    private: System::Windows::Forms::Button^  btnPowerToggle;
    private: System::Windows::Forms::Button^  btnReset;
    private: System::Windows::Forms::Timer^  timer1;
    private: System::ComponentModel::IContainer^  components;



    protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->lbl_r0 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->lbl_r1 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->lbl_r2 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->lbl_r3 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->lbl_r4 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->lbl_r5 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->lbl_r6 = (gcnew System::Windows::Forms::Label());
            this->label14 = (gcnew System::Windows::Forms::Label());
            this->lbl_r7 = (gcnew System::Windows::Forms::Label());
            this->label16 = (gcnew System::Windows::Forms::Label());
            this->lbl_r8 = (gcnew System::Windows::Forms::Label());
            this->label18 = (gcnew System::Windows::Forms::Label());
            this->lbl_r9 = (gcnew System::Windows::Forms::Label());
            this->label20 = (gcnew System::Windows::Forms::Label());
            this->lbl_r10 = (gcnew System::Windows::Forms::Label());
            this->label22 = (gcnew System::Windows::Forms::Label());
            this->lbl_r11 = (gcnew System::Windows::Forms::Label());
            this->label24 = (gcnew System::Windows::Forms::Label());
            this->lbl_r12 = (gcnew System::Windows::Forms::Label());
            this->label26 = (gcnew System::Windows::Forms::Label());
            this->lbl_sp = (gcnew System::Windows::Forms::Label());
            this->label28 = (gcnew System::Windows::Forms::Label());
            this->lbl_lr = (gcnew System::Windows::Forms::Label());
            this->label30 = (gcnew System::Windows::Forms::Label());
            this->lbl_pc = (gcnew System::Windows::Forms::Label());
            this->btnPowerToggle = (gcnew System::Windows::Forms::Button());
            this->btnReset = (gcnew System::Windows::Forms::Button());
            this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
            this->SuspendLayout();
            // 
            // pictureBox1
            // 
            this->pictureBox1->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->pictureBox1->BackColor = System::Drawing::Color::Black;
            this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBox1->Location = System::Drawing::Point(164, 12);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(512, 480);
            this->pictureBox1->TabIndex = 0;
            this->pictureBox1->TabStop = false;
            // 
            // label1
            // 
            this->label1->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label1->Location = System::Drawing::Point(684, 13);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(23, 13);
            this->label1->TabIndex = 1;
            this->label1->Text = L"r0";
            // 
            // lbl_r0
            // 
            this->lbl_r0->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r0->AutoSize = true;
            this->lbl_r0->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r0->Location = System::Drawing::Point(706, 13);
            this->lbl_r0->Name = L"lbl_r0";
            this->lbl_r0->Size = System::Drawing::Size(77, 14);
            this->lbl_r0->TabIndex = 2;
            this->lbl_r0->Text = L"0x00000000";
            // 
            // label2
            // 
            this->label2->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label2->Location = System::Drawing::Point(684, 26);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(23, 13);
            this->label2->TabIndex = 1;
            this->label2->Text = L"r1";
            // 
            // lbl_r1
            // 
            this->lbl_r1->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r1->AutoSize = true;
            this->lbl_r1->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r1->Location = System::Drawing::Point(706, 26);
            this->lbl_r1->Name = L"lbl_r1";
            this->lbl_r1->Size = System::Drawing::Size(77, 14);
            this->lbl_r1->TabIndex = 2;
            this->lbl_r1->Text = L"0x00000000";
            // 
            // label4
            // 
            this->label4->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label4->Location = System::Drawing::Point(684, 39);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(23, 13);
            this->label4->TabIndex = 1;
            this->label4->Text = L"r2";
            // 
            // lbl_r2
            // 
            this->lbl_r2->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r2->AutoSize = true;
            this->lbl_r2->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r2->Location = System::Drawing::Point(706, 39);
            this->lbl_r2->Name = L"lbl_r2";
            this->lbl_r2->Size = System::Drawing::Size(77, 14);
            this->lbl_r2->TabIndex = 2;
            this->lbl_r2->Text = L"0x00000000";
            // 
            // label6
            // 
            this->label6->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label6->Location = System::Drawing::Point(684, 52);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(23, 13);
            this->label6->TabIndex = 1;
            this->label6->Text = L"r3";
            // 
            // lbl_r3
            // 
            this->lbl_r3->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r3->AutoSize = true;
            this->lbl_r3->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r3->Location = System::Drawing::Point(706, 52);
            this->lbl_r3->Name = L"lbl_r3";
            this->lbl_r3->Size = System::Drawing::Size(77, 14);
            this->lbl_r3->TabIndex = 2;
            this->lbl_r3->Text = L"0x00000000";
            // 
            // label8
            // 
            this->label8->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label8->Location = System::Drawing::Point(684, 73);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(23, 13);
            this->label8->TabIndex = 1;
            this->label8->Text = L"r4";
            // 
            // lbl_r4
            // 
            this->lbl_r4->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r4->AutoSize = true;
            this->lbl_r4->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r4->Location = System::Drawing::Point(706, 73);
            this->lbl_r4->Name = L"lbl_r4";
            this->lbl_r4->Size = System::Drawing::Size(77, 14);
            this->lbl_r4->TabIndex = 2;
            this->lbl_r4->Text = L"0x00000000";
            // 
            // label10
            // 
            this->label10->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label10->Location = System::Drawing::Point(684, 86);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(23, 13);
            this->label10->TabIndex = 1;
            this->label10->Text = L"r5";
            // 
            // lbl_r5
            // 
            this->lbl_r5->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r5->AutoSize = true;
            this->lbl_r5->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r5->Location = System::Drawing::Point(706, 86);
            this->lbl_r5->Name = L"lbl_r5";
            this->lbl_r5->Size = System::Drawing::Size(77, 14);
            this->lbl_r5->TabIndex = 2;
            this->lbl_r5->Text = L"0x00000000";
            // 
            // label12
            // 
            this->label12->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label12->Location = System::Drawing::Point(684, 99);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(23, 13);
            this->label12->TabIndex = 1;
            this->label12->Text = L"r6";
            // 
            // lbl_r6
            // 
            this->lbl_r6->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r6->AutoSize = true;
            this->lbl_r6->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r6->Location = System::Drawing::Point(706, 99);
            this->lbl_r6->Name = L"lbl_r6";
            this->lbl_r6->Size = System::Drawing::Size(77, 14);
            this->lbl_r6->TabIndex = 2;
            this->lbl_r6->Text = L"0x00000000";
            // 
            // label14
            // 
            this->label14->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label14->Location = System::Drawing::Point(684, 112);
            this->label14->Name = L"label14";
            this->label14->Size = System::Drawing::Size(23, 13);
            this->label14->TabIndex = 1;
            this->label14->Text = L"r7";
            // 
            // lbl_r7
            // 
            this->lbl_r7->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r7->AutoSize = true;
            this->lbl_r7->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r7->Location = System::Drawing::Point(706, 112);
            this->lbl_r7->Name = L"lbl_r7";
            this->lbl_r7->Size = System::Drawing::Size(77, 14);
            this->lbl_r7->TabIndex = 2;
            this->lbl_r7->Text = L"0x00000000";
            // 
            // label16
            // 
            this->label16->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label16->Location = System::Drawing::Point(684, 125);
            this->label16->Name = L"label16";
            this->label16->Size = System::Drawing::Size(23, 13);
            this->label16->TabIndex = 1;
            this->label16->Text = L"r8";
            // 
            // lbl_r8
            // 
            this->lbl_r8->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r8->AutoSize = true;
            this->lbl_r8->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r8->Location = System::Drawing::Point(706, 125);
            this->lbl_r8->Name = L"lbl_r8";
            this->lbl_r8->Size = System::Drawing::Size(77, 14);
            this->lbl_r8->TabIndex = 2;
            this->lbl_r8->Text = L"0x00000000";
            // 
            // label18
            // 
            this->label18->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label18->Location = System::Drawing::Point(684, 138);
            this->label18->Name = L"label18";
            this->label18->Size = System::Drawing::Size(23, 13);
            this->label18->TabIndex = 1;
            this->label18->Text = L"r9";
            // 
            // lbl_r9
            // 
            this->lbl_r9->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r9->AutoSize = true;
            this->lbl_r9->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r9->Location = System::Drawing::Point(706, 138);
            this->lbl_r9->Name = L"lbl_r9";
            this->lbl_r9->Size = System::Drawing::Size(77, 14);
            this->lbl_r9->TabIndex = 2;
            this->lbl_r9->Text = L"0x00000000";
            // 
            // label20
            // 
            this->label20->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label20->Location = System::Drawing::Point(684, 151);
            this->label20->Name = L"label20";
            this->label20->Size = System::Drawing::Size(23, 13);
            this->label20->TabIndex = 1;
            this->label20->Text = L"r10";
            // 
            // lbl_r10
            // 
            this->lbl_r10->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r10->AutoSize = true;
            this->lbl_r10->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r10->Location = System::Drawing::Point(706, 151);
            this->lbl_r10->Name = L"lbl_r10";
            this->lbl_r10->Size = System::Drawing::Size(77, 14);
            this->lbl_r10->TabIndex = 2;
            this->lbl_r10->Text = L"0x00000000";
            // 
            // label22
            // 
            this->label22->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label22->Location = System::Drawing::Point(684, 164);
            this->label22->Name = L"label22";
            this->label22->Size = System::Drawing::Size(23, 13);
            this->label22->TabIndex = 1;
            this->label22->Text = L"r11";
            // 
            // lbl_r11
            // 
            this->lbl_r11->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r11->AutoSize = true;
            this->lbl_r11->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r11->Location = System::Drawing::Point(706, 164);
            this->lbl_r11->Name = L"lbl_r11";
            this->lbl_r11->Size = System::Drawing::Size(77, 14);
            this->lbl_r11->TabIndex = 2;
            this->lbl_r11->Text = L"0x00000000";
            // 
            // label24
            // 
            this->label24->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label24->Location = System::Drawing::Point(684, 177);
            this->label24->Name = L"label24";
            this->label24->Size = System::Drawing::Size(23, 13);
            this->label24->TabIndex = 1;
            this->label24->Text = L"r12";
            // 
            // lbl_r12
            // 
            this->lbl_r12->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_r12->AutoSize = true;
            this->lbl_r12->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_r12->Location = System::Drawing::Point(706, 177);
            this->lbl_r12->Name = L"lbl_r12";
            this->lbl_r12->Size = System::Drawing::Size(77, 14);
            this->lbl_r12->TabIndex = 2;
            this->lbl_r12->Text = L"0x00000000";
            // 
            // label26
            // 
            this->label26->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label26->Location = System::Drawing::Point(684, 198);
            this->label26->Name = L"label26";
            this->label26->Size = System::Drawing::Size(23, 13);
            this->label26->TabIndex = 1;
            this->label26->Text = L"sp";
            // 
            // lbl_sp
            // 
            this->lbl_sp->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_sp->AutoSize = true;
            this->lbl_sp->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_sp->Location = System::Drawing::Point(706, 198);
            this->lbl_sp->Name = L"lbl_sp";
            this->lbl_sp->Size = System::Drawing::Size(77, 14);
            this->lbl_sp->TabIndex = 2;
            this->lbl_sp->Text = L"0x00000000";
            // 
            // label28
            // 
            this->label28->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label28->Location = System::Drawing::Point(684, 211);
            this->label28->Name = L"label28";
            this->label28->Size = System::Drawing::Size(23, 13);
            this->label28->TabIndex = 1;
            this->label28->Text = L"lr";
            // 
            // lbl_lr
            // 
            this->lbl_lr->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_lr->AutoSize = true;
            this->lbl_lr->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_lr->Location = System::Drawing::Point(706, 211);
            this->lbl_lr->Name = L"lbl_lr";
            this->lbl_lr->Size = System::Drawing::Size(77, 14);
            this->lbl_lr->TabIndex = 2;
            this->lbl_lr->Text = L"0x00000000";
            // 
            // label30
            // 
            this->label30->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->label30->Location = System::Drawing::Point(684, 224);
            this->label30->Name = L"label30";
            this->label30->Size = System::Drawing::Size(23, 13);
            this->label30->TabIndex = 1;
            this->label30->Text = L"pc";
            // 
            // lbl_pc
            // 
            this->lbl_pc->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->lbl_pc->AutoSize = true;
            this->lbl_pc->Font = (gcnew System::Drawing::Font(L"Courier New", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lbl_pc->Location = System::Drawing::Point(706, 224);
            this->lbl_pc->Name = L"lbl_pc";
            this->lbl_pc->Size = System::Drawing::Size(77, 14);
            this->lbl_pc->TabIndex = 2;
            this->lbl_pc->Text = L"0x00000000";
            // 
            // btnPowerToggle
            // 
            this->btnPowerToggle->Location = System::Drawing::Point(12, 12);
            this->btnPowerToggle->Name = L"btnPowerToggle";
            this->btnPowerToggle->Size = System::Drawing::Size(75, 23);
            this->btnPowerToggle->TabIndex = 3;
            this->btnPowerToggle->Text = L"Power ON";
            this->btnPowerToggle->UseVisualStyleBackColor = true;
            this->btnPowerToggle->Click += gcnew System::EventHandler(this, &FrmMain::btnPowerToggle_Click);
            // 
            // btnReset
            // 
            this->btnReset->Location = System::Drawing::Point(12, 41);
            this->btnReset->Name = L"btnReset";
            this->btnReset->Size = System::Drawing::Size(75, 23);
            this->btnReset->TabIndex = 3;
            this->btnReset->Text = L"Reset";
            this->btnReset->UseVisualStyleBackColor = true;
            this->btnReset->Click += gcnew System::EventHandler(this, &FrmMain::btnReset_Click);
            // 
            // timer1
            // 
            this->timer1->Tick += gcnew System::EventHandler(this, &FrmMain::timer1_Tick);
            // 
            // FrmMain
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(839, 506);
            this->Controls->Add(this->btnReset);
            this->Controls->Add(this->btnPowerToggle);
            this->Controls->Add(this->lbl_pc);
            this->Controls->Add(this->label30);
            this->Controls->Add(this->lbl_lr);
            this->Controls->Add(this->label28);
            this->Controls->Add(this->lbl_sp);
            this->Controls->Add(this->label26);
            this->Controls->Add(this->lbl_r12);
            this->Controls->Add(this->label24);
            this->Controls->Add(this->lbl_r11);
            this->Controls->Add(this->label22);
            this->Controls->Add(this->lbl_r10);
            this->Controls->Add(this->label20);
            this->Controls->Add(this->lbl_r9);
            this->Controls->Add(this->label18);
            this->Controls->Add(this->lbl_r8);
            this->Controls->Add(this->label16);
            this->Controls->Add(this->lbl_r7);
            this->Controls->Add(this->label14);
            this->Controls->Add(this->lbl_r6);
            this->Controls->Add(this->label12);
            this->Controls->Add(this->lbl_r5);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->lbl_r4);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->lbl_r3);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->lbl_r2);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->lbl_r1);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->lbl_r0);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->pictureBox1);
            this->Name = L"FrmMain";
            this->Text = L"DV1032 Emulator";
            this->Load += gcnew System::EventHandler(this, &FrmMain::FrmMain_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void FrmMain_Load(System::Object^  sender, System::EventArgs^  e)
    {
        FrmMain_load();
    }
private: System::Void btnPowerToggle_Click(System::Object^  sender, System::EventArgs^  e)
{
    static bool bGoOn = true;
    if (bGoOn)
    {
        btnPowerToggle->Text = "Power OFF";
        FrmMain_powerOn();
        timer1->Start();
    }
    else
    {
        btnPowerToggle->Text = "Power ON";
        FrmMain_powerOff();
        timer1->Stop();
    }
    bGoOn = !bGoOn;
}
private: System::Void btnReset_Click(System::Object^  sender, System::EventArgs^  e)
{  
    FrmMain_reset();
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
    uint32_t registers[16];
    getRegisters(registers);
    lbl_r0->Text = "0x" + System::Convert::ToString((int)registers[0], 16);
    lbl_r1->Text = "0x" + System::Convert::ToString((int)registers[1], 16);
    lbl_r2->Text = "0x" + System::Convert::ToString((int)registers[2], 16);
    lbl_r3->Text = "0x" + System::Convert::ToString((int)registers[3], 16);
    lbl_r4->Text = "0x" + System::Convert::ToString((int)registers[4], 16);
    lbl_r5->Text = "0x" + System::Convert::ToString((int)registers[5], 16);
    lbl_r6->Text = "0x" + System::Convert::ToString((int)registers[6], 16);
    lbl_r7->Text = "0x" + System::Convert::ToString((int)registers[7], 16);
    lbl_r8->Text = "0x" + System::Convert::ToString((int)registers[8], 16);
    lbl_r9->Text = "0x" + System::Convert::ToString((int)registers[9], 16);
    lbl_r10->Text = "0x" + System::Convert::ToString((int)registers[10], 16);
    lbl_r11->Text = "0x" + System::Convert::ToString((int)registers[11], 16);
    lbl_r12->Text = "0x" + System::Convert::ToString((int)registers[12], 16);
    lbl_sp->Text = "0x" + System::Convert::ToString((int)registers[13], 16);
    lbl_lr->Text = "0x" + System::Convert::ToString((int)registers[14], 16);
    lbl_pc->Text = "0x" + System::Convert::ToString((int)registers[15], 16);
}
};
}
