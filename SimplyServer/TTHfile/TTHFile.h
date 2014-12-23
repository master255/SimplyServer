//---------------------------------------------------------------------------

#ifndef TTHfileH
#define TTHfileH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
#include "misc/TimerDiff.cpp"

//---------------------------------------------------------------------------
typedef void  ( *ONTTHWORKBEGINFUNC)(__int64 FileSize);
typedef void  ( *ONTTHWORKFUNC)(int iProcents, double fSpeed, __int64 Readed);
typedef void  ( *ONTTHWORKENDFUNC)();

class TTTHFile : public TThread
{
private:
protected:
        int iProcents_;
        double fSpeed_;
        __int64 iFileSize_;        
        __int64 iTotalReaded_;

        void __fastcall Execute();
        void __fastcall SyncOnWorkBegin();
        void __fastcall SyncOnWork();
        void __fastcall SyncOnWorkEnd();        
public:
        TTTHFile(bool CreateSuspended);
        ONTTHWORKBEGINFUNC OnWorkBegin;
        ONTTHWORKFUNC OnWork;
        ONTTHWORKENDFUNC OnWorkEnd;                
        //AnsiString File;
        int Priority; // приоритет потока (см. SetThreadPriority)
       // AnsiString TTH; 
        int Interval; // период вызовы OnWork
};
//---------------------------------------------------------------------------
#endif
