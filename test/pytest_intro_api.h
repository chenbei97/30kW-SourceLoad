#ifndef PYTEST_INTRO_API_H
#define PYTEST_INTRO_API_H

#include <Python.h>
#include <QString>
#include "pytest_print_test.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
// 引入测试：引入python、引入python且引入第三方库
/*
 * 要在object.h中添加2行代码,因为python定义的slots和qt定义的slots重合了
typedef struct{
    const char* name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    #undef slots 添加
    PyType_Slot *slots;
    #define slots Q_SLOTS 添加
} PyType_Spec;
**/

// 引入python的测试
int test_python()
{
    const char * workdir = "C:/Users/Lenovo/Desktop/30kW-SourceLoad-abridged";
    size_t sz = sizeof (workdir);

    Py_Initialize();
    if(!Py_IsInitialized())
    {
        printf("Python init fail!\n"); // python初始化失败
        return -1;
    }

    printf("cur program path = %s\n",QString::fromWCharArray(Py_GetProgramName(),-1).toStdString().c_str()); // python,默认
    printf("cur home path = %s\n",QString::fromWCharArray(Py_GetPythonHome(),-1).toStdString().c_str()); // 没有返回值
    printf("build info: %s\n",Py_GetBuildInfo()); // python解释器的信息 tags/v3.7.8:4b47a5b6ba, Jun 28 2020, 07:55:33
    printf("compiler info: %s\n",Py_GetCompiler()); // [MSC v.1916 32 bit (Intel)]
    printf("copyright info: %s\n",Py_GetCopyright()); // copyright info: Copyright (c) 2001-2020 Python Software Foundation.All Rights Reserved.
    printf("platform info: %s\n",Py_GetPlatform());// platform info: win32
    printf("versinon info: %s\n",Py_GetVersion()); // 3.7.8 (tags/v3.7.8:4b47a5b6ba, Jun 28 2020, 07:55:33) [MSC v.1916 32 bit (Intel)]
    wchar_t * workpath = Py_DecodeLocale(workdir,&sz);
    Py_SetPath(workpath);
    PyMem_RawFree(workpath);

    wchar_t * curpath = Py_GetPath(); // 如果不设置会返回下方4个路径,设置后就变成workdir
    printf("cur path = %s\n",QString::fromWCharArray(curpath).toStdString().c_str());
    // C:\Users\Lenovo\AppData\Local\Programs\Python\Python37-32\python37.zip;
    // C:\Users\Lenovo\AppData\Local\Programs\Python\Python37-32\Lib\;
    // C:\Users\Lenovo\AppData\Local\Programs\Python\Python37-32\DLLs\;
    // C:\Users\Lenovo\Desktop\30kW-SourceLoad-abridged\build-30kW-SourceLoad-Debug\debug

//    PyRun_SimpleString("import os,sys,numpy");
    PyRun_SimpleString("import os,sys");
//    PyRun_SimpleString("print(os.getcwd())");
//    PyRun_SimpleString("import numpy as np");
//    PyRun_SimpleString("print(np.linspace(0,10,11))");
    PyRun_SimpleString("sys.argv = ['python.py']");
    PyRun_SimpleString("sys.path.append('../pytest')"); // 如果用./,python脚本要放在debug目录下,如果用../，那么脚本可以放在main.cpp同级，这里放在同级的文件夹下
    PyRun_SimpleString("print(os.getcwd())"); // 测试当前路径还是 build-30kW-SourceLoad-Debug，似乎sys.path不被Py_SetPath设置的影响

    PyObject* pModule = PyImport_ImportModule("test_python");//自定义的py文件名称
    if (!pModule)
     {
         printf("Cant open python file!\n"); // 找不到自定义的python脚本
         return -1;
     }

    PyObject* pFunshow= PyObject_GetAttrString(pModule,"show"); // 调用脚本的show函数
    if(!pFunshow){
        printf("Get show function hello failed\n");
        return -1;
    }
    PyObject_CallFunction(pFunshow,NULL);
    printf("pFunshow is called\n");
    PyObject* pFunsum= PyObject_GetAttrString(pModule,"sum"); // 调用脚本的show函数
    if(!pFunsum){
        printf("Get sum function hello failed\n");
        return -1;
    }
    PyObject_CallFunction(pFunsum,"ii",1,9); // 传参的规则可见 https://blog.csdn.net/Windgs_YF/article/details/91431282
    printf("pFunsum is called\n");

    //结束，释放python
    Py_Finalize();

    return 0;
}

// 引入第三方库的测试
int test_plot()
{
    //Py_SetPythonHome((wchar_t *)(L"C:\\Users\\Lenovo\\AppData\\Local\\Programs\\Python\\Python37-32")); // 注释掉好像也没事
    Py_Initialize();
    wchar_t * str = Py_GetPythonHome();
    printf("%s\n",QString::fromWCharArray(str).toStdString().c_str());
    if ( !Py_IsInitialized() )
    {
        printf("Cant Py_IsInitialized !\n");return -1 ;
    }
    PyRun_SimpleString("import os,sys");
    PyRun_SimpleString("sys.path.append('../pytest')");
    PyRun_SimpleString("print(os.getcwd())");
    PyRun_SimpleString("print(sys.path)");

    PyObject* pModule = PyImport_ImportModule("test_plot");
    qDebug()<<"try to call test_plot.py...";
    if (!pModule)
     {
         printf("Cant open python file!\n");
         return -1;
     }
    PyObject* pFunshow= PyObject_GetAttrString(pModule,"main"); // 调用函数
    if(!pFunshow){
        printf("Get main function hello failed\n");
        return -1;
    }
    PyObject_CallFunction(pFunshow,NULL);
    Py_Finalize();
    return 0;
}

#endif // PYTEST_INTRO_API_H
