__int64 TimerInit();
double TimerDiff(__int64 init_counter);

__int64 TimerInit()
{
	__int64 init_counter;
        QueryPerformanceCounter((LARGE_INTEGER*)&init_counter);
	return init_counter;
}

double TimerDiff(__int64 init_counter)
{
        __int64 freq, diff_counter;
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&diff_counter);
        return  (diff_counter-init_counter) * 1000.0 / freq;
}