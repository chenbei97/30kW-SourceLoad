#ifndef PYTEST_PRINT_TEST_H
#define PYTEST_PRINT_TEST_H

#include <QDebug>
#include <Python.h>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void print_pyobject(PyObject *py_obj);

//把const char *c转 wchar_t * ，作为Py_SetPythonHome()参数匹配
wchar_t *GetWC(const char *c)
{
   const size_t cSize = strlen(c) + 1;
   wchar_t* wc = new wchar_t[cSize];
   mbstowcs(wc, c, cSize);

   return wc;
}

void print_line()
{
    printf("--------------------------------\n");
}

// 测试上边的输出函数
void test_print_pyobject()
{
    Py_Initialize();
    print_pyobject(Q_NULLPTR);
    print_pyobject(Py_BuildValue("i",123));
    print_pyobject(Py_BuildValue("iii",1,2,3));
    print_pyobject(Py_BuildValue("((ii)(ii)) (ii)",1,2,3,4,5,6));
    print_pyobject(Py_BuildValue("idf",1,2.54799878858,3.2565785786f));
    print_pyobject(Py_BuildValue("css",'2',"c++","python"));
    print_pyobject(Py_BuildValue("CzUU#y#u#",110,"java","html","abcdef",4,
                                 "0xffacd",6,GetWC("handsome"),5)); // 110对应字符n,GetWC是转为宽字符类型的函数

    /*
            Py_BuildValue 对应关系 左边是Python-Type,右边是C-Type,将C-Type转为Python-Type
            (1) 整型
            i (int) [int]     I (int) [unsigned int]
            b (int) [char]   B (int) [unsigned char]
            h (int) [short]    H (int) [unsigned short]
            l (int) [long]  L (int) [long long]
            k (int) [unsigned long]  K (int) [unsigned long long]
            n (int) [Py_ssize_t]

            (2)浮点类型
            d (float) [double]
            f (float) [float]
            D (complex) [Py_complex *]

            (3)字符、字符串类型
            c (bytes of length 1) [char]  转换为长度为1的Python字节对象
            C (str of length 1) [int] 转换为长度为1的Python str对象

            s (str or None) [const char *]、
            z (str or None) [const char *]、
            U (str or None) [const char *]  使用“utf-8”编码将以空结尾的C字符串转换为Python str对象
            s# (str or None) [const char *, int or Py_ssize_t]、
            z# (str or None) [const char *, int or Py_ssize_t]、
            U# (str or None) [const char *, int or Py_ssize_t]  使用“utf-8”编码将C字符串及其长度转换为Python str对象 例如("s#","hello",4)=>"hell"

            y (bytes) [const char *] C字符串(或者指定长度)转换为Python字节对象
            y# (bytes) [const char *, int or Py_ssize_t]

            u (str) [const wchar_t *] 将Unicode(UTF-16或UCS-4)数据的以空结尾的wchar_t缓冲区(或者指定长度)转换为Python Unicode对象
            u# (str) [const wchar_t *, int or Py_ssize_t]

            (4)元组、列表和字典需要分别使用()、[]和{}括起来表示
            (items) (tuple) [matching-items] 例如("(ii)",1,2)或("(i,i)"1,2)或("((ii)(ii)) (ii)",1,2,3,4,5,6);
            [items] (list) [matching-items] 例如("[ii]",1,2)或("[i,i]"1,2)
            {items} (dict) [matching-items] 例如("{s:i,s:i}","a",1,"b",2);

            (5) 其他类型
            O (object) [PyObject *]、
            S (object) [PyObject *] 传递一个Python对象，该对象保持不变但引用计数递增1
            N (object) [PyObject *] 和O相同,但是不会改变引用计数

            O& (object) [converter, anything] 通过转换器函数将任何内容转换为Python对象

    */
    int a = 11; unsigned int b = 34; long c = 3; unsigned long d = 6;
    long long e = 5;  unsigned long long f = 101; size_t g = 53;Py_ssize_t h = 26;
    print_pyobject(Py_BuildValue("[i,I,l,k,L,K,I,n]",a,b,c,d,e,f,g,h));
    print_pyobject(Py_BuildValue("{s:i,s:i}","a",1,"b",2));

    struct people{
        int id;
        int age;
        const char * name;
    };
    people p ={110,25,"chenbei"};
    // print_pyobject(Py_BuildValue("O",p)); 会异常
    Q_UNUSED(p);
    Py_Finalize();
}

#endif // PYTEST_PRINT_TEST_H
