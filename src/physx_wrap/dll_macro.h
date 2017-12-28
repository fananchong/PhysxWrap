#ifndef __DLL_MACRO_H__

#ifndef MY_DLL_EXPORT_CLASS
#define MY_DLL_EXPORT_CLASS __declspec(dllexport)
#endif

#ifndef MY_DLL_EXPORT_FUNC
#define MY_DLL_EXPORT_FUNC extern "C" __declspec(dllexport)
#endif

#endif
