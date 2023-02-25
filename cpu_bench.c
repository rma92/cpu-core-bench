#include <stdio.h>
#include <windows.h>
//CPU benchmark to measure the speed on different CPUs.

double measureFunctionRuntime(void (*function)())
{
    LARGE_INTEGER frequency, start, end;
    double elapsedTime;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // Call the function whose runtime we want to measure
    function();

    QueryPerformanceCounter(&end);

    elapsedTime = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    printf("Elapsed time: %f milliseconds\n", elapsedTime);

    return elapsedTime;
}

// Example function to measure runtime
void exampleFunction()
{
    // code to be timed goes here
    int sum = 0;
    for (int i = 0; i < 100000000; i++)
    {
        sum += i;
    }
}

int main()
{
    SYSTEM_INFO systemInfo;
    int i;
    int iNumCPUs = 0;
    double runtime;

    GetSystemInfo(&systemInfo);
    iNumCPUs = systemInfo.dwNumberOfProcessors;
    printf("Number of processors: %d\n", iNumCPUs);

    for( i = 0; i < iNumCPUs; ++i )
    {
      //Set the affinity
      HANDLE hThread = GetCurrentThread();
      DWORD_PTR affinityMask = 1 << i;
      DWORD_PTR previousAffinityMask = SetThreadAffinityMask(hThread, affinityMask);
      if (previousAffinityMask == 0)
      {
          printf("Error setting thread affinity: %d\n", GetLastError());
          return 1;
      }
      else
      {
        printf("current cpu: %d\n", i );
      }
      double runtime = measureFunctionRuntime(&exampleFunction);
    }
    return 0;
}
