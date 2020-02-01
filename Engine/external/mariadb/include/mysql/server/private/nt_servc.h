#ifndef NT_SERVC_INCLUDED
#define NT_SERVC_INCLUDED




typedef void (*THREAD_FC)(void *);

class NTService
{
  public:
    NTService();
   ~NTService();

    BOOL    bOsNT;	      
    
    DWORD   dwDesiredAccess;
    DWORD   dwServiceType;
    DWORD   dwStartType;
    DWORD   dwErrorControl;

    LPSTR   szLoadOrderGroup;
    LPDWORD lpdwTagID;
    LPSTR   szDependencies;
    OSVERSIONINFO osVer;

    
    int     nStartTimeOut;
    int     nStopTimeOut;
    int     nPauseTimeOut;
    int     nResumeTimeOut;

    
    DWORD   my_argc;
    LPTSTR *my_argv;
    HANDLE  hShutdownEvent;
    int     nError;
    DWORD   dwState;

    
    long Init(LPCSTR szInternName,THREAD_FC ServiceThread);

    
    void SetShutdownEvent(HANDLE hEvent){ hShutdownEvent=hEvent; }


    
    BOOL Install(int startType,LPCSTR szInternName,LPCSTR szDisplayName,
                 LPCSTR szFullPath, LPCSTR szAccountName=NULL,
                 LPCSTR szPassword=NULL);
    BOOL SeekStatus(LPCSTR szInternName, int OperationType);
    BOOL Remove(LPCSTR szInternName);
    BOOL IsService(LPCSTR ServiceName);
    BOOL got_service_option(char **argv, const char *service_option);
    BOOL is_super_user();

    
    void SetRunning(void);

    
    void SetSlowStarting(unsigned long timeout);

    
    void Stop(void); 

  protected:
    LPSTR		   ServiceName;
    HANDLE		   hExitEvent;
    SERVICE_STATUS_HANDLE  hServiceStatusHandle;
    BOOL		   bPause;
    BOOL		   bRunning;
    HANDLE		   hThreadHandle;
    THREAD_FC		   fpServiceThread;

    void PauseService();
    void ResumeService();
    void StopService();
    BOOL StartService();

    static void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
    static void WINAPI ServiceCtrlHandler (DWORD ctrlCode);

    void Exit(DWORD error);
    BOOL SetStatus (DWORD dwCurrentState,DWORD dwWin32ExitCode,
		    DWORD dwServiceSpecificExitCode,
		    DWORD dwCheckPoint,DWORD dwWaitHint);

};


#endif 
