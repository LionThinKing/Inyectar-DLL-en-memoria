#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
 HWND hwProceso;
 DWORD idProceso;
 HANDLE hProceso; 
 HANDLE hThread;
 LPVOID memoria;
 char *DLL="C:\\dll.dll";
 
 //Buscamos el proceso a inyectar.
 if(!(hwProceso=FindWindowEx(0,0,0,"Calculadora")))
 {
  MessageBox(0,"Ventana no encontrada","ERROR X",0);
  return 1;
 }
 
 if(!(GetWindowThreadProcessId(hwProceso,&idProceso)))
 {
  MessageBox(0,"No se pudo Obtener la id del proceso","ERROR X",0);
  return 1;
 }
 
 //Abrimos el proceso
 if(!(hProceso=OpenProcess(PROCESS_ALL_ACCESS,false,idProceso)))
 {
  MessageBox(0,"No se pudo abrir el proceso","ERROR X",0);
  return 1;
 }
 
 //Reservamos memoria en el proceso para la DLL 
 memoria=VirtualAllocEx(hProceso,0,strlen(DLL),MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
 SIZE_T write=0;
 
 //Escribimos en el proceso
 if(WriteProcessMemory(hProceso,memoria,(LPVOID)DLL,strlen(DLL),&write)==0)
 {
  MessageBox(0,"No se pudo escribir en el proceso","ERROR X",0);
 }
 
 //Creamos un nuevo hilo de ejecucion en el proceso
 if( (hThread=CreateRemoteThread(hProceso,0,0,(LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("Kernel32.dll"),"LoadLibraryA"),memoria,0,0))==NULL)
 {
  MessageBox(0,"No se pudo EJECUTAR en el proceso","ERROR X",0);
 }
 
}
