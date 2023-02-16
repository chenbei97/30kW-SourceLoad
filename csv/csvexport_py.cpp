#include "csvexport.h"
#include <Python.h>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void CSVExport::writeTablePy(const char *  header,const char *  content)
{
        /* 这些步骤其实python程序也进行了优化和检查做了相同工作,不加也行
       if (QString(content).right(1) !="\n") return; // 内容必须有换行符
       QString head(header);
       if ( head.right(1) !="\n") head = head + QStringLiteral("\n"); // 表头可以没有换行结束符
       const char * h = head.toStdString().c_str(); */

       Py_Initialize();
       PyRun_SimpleString("import os,sys");
       PyRun_SimpleString("os.chdir('../csv')"); // 改变当前工作路径
       PyRun_SimpleString("sys.path.append('../csv')");
       //PyRun_SimpleString("print(os.getcwd())"); // 30kW-SourceLoad-abridged\csv
       PyObject* module = PyImport_ImportModule("csvexport");
       PyObject* func= PyObject_GetAttrString(module,"export_csv");

       // 在pytest_tool_api.h已经测试过c++可以传入str给python,python返回的str也可以被c++解析
       // 传递字符串是唯一的选择,c++不支持传递元组,列表和字典给python,如果用基本类型int/float/double又很繁琐不灵活
       // 所以选择传递字符串的方式最合理,规定好传递的格式在python程序内再解析即可
       // 核心的4个步骤如下
       //  c++ => python:  const char* mstr="123"; PyObject* pret = PyObject_CallFunction(func,"s",mstr);
       // python => c++ :         char *ret;PyArg_Parse(pret,"s",&ret);
       // 考虑到python返回的元组可以被c++解析,所以python返回的(int,str,int,int)
       // 第1个含义是导出是否成功,第2个导出失败会反馈的错误信息,第3和第4表示行列数,如果导出失败就是0,0
       // 解析元组的话需要使用PyArg_ParseTuple不能使用PyArg_Parse

        PyObject* pret = PyObject_CallFunction(func,"ss",header,content); // 二选一都对
        //PyObject* pret = PyObject_CallObject(func, Py_BuildValue("ss",header,content));

        int ok, ret, row, col;
        ok = PyArg_ParseTuple(pret,"iii",&ret,&row,&col); // 不能用(i,i,i)和(iii),只能用iii才正确
        if (ret == 0) {
            printf("export csv is failed\n");
            this->clear(); // 导出失败清理掉
            QMessageBox::information(Q_NULLPTR,tr("消息"),tr("导出CSV文件失败!"));
        }
        if (ok == 0) printf("failed to parse parameters\n");
        else printf("export csv is successful, table's shape is (%d,%d)\n",row,col);

        mrows = row; mcols = col; // 成功的话才给内部变量赋值
        mTableHeader = QString(header);
        mTableContent = QString(content);
        QMessageBox::information(Q_NULLPTR,tr("消息"),tr("成功导出CSV文件!"));
        Py_Finalize();
        //Py_Exit(0);
}

void CSVExport::writeTablePy(const QString& header, const QString& content)
{
        /* 这些步骤其实python程序也进行了优化和检查做了相同工作,不加也行
        if (content.right(1) !="\n") return; // 内容必须有换行符
        QString head = header;
        if ( header.right(1) !="\n") head = header + QStringLiteral("\n"); // 表头可以没有换行结束符,这里会自己加上
        */
         writeTablePy(header.toStdString().c_str(),content.toStdString().c_str());
}

void CSVExport::writeTablePy(const QString &header, TableContent contentFunc)
{
        writeTablePy(header.toStdString().c_str(),contentFunc().toStdString().c_str());
}

QString CSVExport::importCsvPy()
{
        Py_Initialize();
        PyRun_SimpleString("import sys,os");
        PyRun_SimpleString("os.chdir('../csv')");
        PyRun_SimpleString("sys.path.append('../csv')");

        PyObject* module = PyImport_ImportModule("csvexport");
        PyObject* func= PyObject_GetAttrString(module,"import_csv");
        PyObject* pret = PyObject_CallFunction(func,"s","python_test.csv");
        const char* ret =  PyByteArray_AsString(pret); // ret = bytearray((header+content),'gb2312') python返回字节对象这边解析不会乱码
        //PyArg_Parse(pret,"s",&ret); // 用PyArg_Parse或者PyUnicode_AsUTF8都不行
        //printf("%s\n",ret);
        QString res = QString::fromLocal8Bit(ret); // 必须用fromLocal8Bit,不能用fromUtf8/fromLatin1,也不能直接返回QString(ret)
        Py_Finalize();
        return res;
}
