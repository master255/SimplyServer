//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TTHfile.h"

#include "tiger/sboxes.c"
#pragma warn -8060
#pragma warn -8070
#include "tiger/tiger.c"
#include "tigertree/tigertree.c"
#pragma warn +8060
#pragma warn +8070
#include "base32/base32.c"
#include <process.h>
#include <memory>

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TTTHFile::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TTTHFile::TTTHFile(bool CreateSuspended)
        : TThread(CreateSuspended)
{
        Priority = THREAD_PRIORITY_NORMAL;
        FreeOnTerminate = true;
        iTotalReaded_ = 0;
        iFileSize_ = 0;
        Interval = 500;
}
//---------------------------------------------------------------------------
#pragma warn -8004
void __fastcall TTTHFile::Execute()
{
        // изменить приоритет потока
        SetThreadPriority(GetCurrentThread(), Priority);

        // открытие файла на чтение
        TFileStream *fs;
        try
        {
                fs = new TFileStream(File.c_str(), fmOpenRead | fmShareDenyWrite);
        }
        catch (Exception &ex)
        {
                MessageBox(NULL, ex.Message.c_str(), "Ошибка", MB_ICONERROR);
                return;
        }

        // вызов OnWorkBegin 
        iFileSize_ = fs->Size;
        Synchronize(SyncOnWorkBegin);

        // инициализация TTH
        TT_CONTEXT ctx;
        unsigned char hash[TIGERSIZE];
        tt_init(&ctx);

        // буфер
        const int BUFFER_SIZE = 65536;
        char *szBuffer = new char[BUFFER_SIZE];

        __int64 iPrevReaded = 0;
        __int64 Start = TimerInit();

        int cbReaded = 0;

        // чтение из файла и вычисление TTH
        while ( (cbReaded = fs->Read(szBuffer, BUFFER_SIZE)) != 0 )
        {
                tt_update(&ctx, szBuffer, cbReaded);
                iTotalReaded_ += cbReaded;

                if (Terminated)
                {
                        delete fs;
                        delete szBuffer;
                        return;
                }

                if ( TimerDiff(Start) > Interval )
                {
                        // вызов OnWork
                        iProcents_ = 1.0 * iTotalReaded_ / iFileSize_ * 100;
                        fSpeed_ = (iTotalReaded_ - iPrevReaded) / TimerDiff(Start);
                        Synchronize(SyncOnWork);

                        Start = TimerInit();
                        iPrevReaded = iTotalReaded_;
                }
        }

        // завершение вычисления TTH
        tt_digest(&ctx, hash);

        // конвертирование в base32
        char *dest = new char[40]; dest[39] = 0;
        to_base32(hash, 24, dest);
        TTH = dest;

        // вызов OnWorkEnd
        Synchronize(SyncOnWorkEnd);
}
#pragma warn +8004
//---------------------------------------------------------------------------

void __fastcall TTTHFile::SyncOnWorkBegin()
{
        if (OnWorkBegin) OnWorkBegin(iFileSize_);
}

void __fastcall TTTHFile::SyncOnWork()
{
        if (OnWork) OnWork(iProcents_, fSpeed_, iTotalReaded_);
}

void __fastcall TTTHFile::SyncOnWorkEnd()
{
        if (OnWorkEnd) OnWorkEnd();
}