#ifndef PYTEST_PRINT_H
#define PYTEST_PRINT_H

#include "pytest_print_test.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
// 一个便利函数打印各种类型的PyObject
void print_pyobject(PyObject *py_obj)
{
        if (py_obj == Q_NULLPTR){
                printf("i am nullptr\n");
                print_line();
                return;
        }
        if (py_obj == Py_None)
        {
            printf("i am python-None\n");
            print_line();
        } else if (PyLong_Check(py_obj)){
            // Python代表的是int类型
            // 不打印下方这么多东西,C类型的各种整数类型在Python都只有int类型
            // 兼容性最强的long long来打印值,不管原本的值是什么类型用long long来打印值都是可以的
            long long val = PyLong_AsLongLong(py_obj);
            printf("i am python-int, val = %lld\n", val);
            print_line();
            /*
            long long llval = PyLong_AsLongLong(py_obj);
            if (llval != -1) {
                printf("i am long long, val = %lld\n", llval);
            }

            unsigned long long ullval = PyLong_AsUnsignedLongLong(py_obj);
            if (ullval != ULONG_LONG_MAX-1) {
                printf("i am unsigned long long, val = %llu\n", ullval);
            }

            long lval = PyLong_AsLong(py_obj);
            if (lval != -1) {
                printf("i am long, val = %ld\n", lval);
            }

            unsigned long ulval = PyLong_AsUnsignedLong(py_obj);
            if (ulval != ULONG_MAX-1) {
                printf("i am unsigned long, val = %lu\n", ulval);
            }
            size_t szval = PyLong_AsSize_t(py_obj);
            if (szval != SIZE_MAX-1) {
                printf("i am size_t, val = %zu\n", szval); // unsigned int
            }

            Py_ssize_t sszval = PyLong_AsSsize_t(py_obj);
            if (sszval != -1){
                printf("i am ssize_t, val = %zd\n", sszval);  // ssize_t在32位是int 4字节,64位是long int 8字节
            }
            print_line();
            */
        }
        else if (PyFloat_Check(py_obj)){
            // python的float类型
            double fval = PyFloat_AS_DOUBLE(py_obj);
            printf("i am python-float, val = %lf\n", fval);
            print_line();
        } else if (PyBool_Check(py_obj)){
            // python的bool类型
            size_t bval = PyLong_AsSize_t(py_obj);
            if (bval == 1){
                printf("i am python-bool, val = true\n");
            }else {
                printf("i am python-bool, val = false\n");
            }
            print_line();
        } else if (PyBytes_Check(py_obj))
        {
            // python的bytes类型
            char* str = PyBytes_AsString(py_obj);
            printf("i am python-bytes, val = %s\n", str);
            print_line();
        }else if (PyByteArray_Check(py_obj)){
             // python的bytearray类型
            char *str = PyByteArray_AsString(py_obj);
            printf("i am python-bytearray, val = %s\n", str);// 不能含有中文
            print_line();
        } else if (PyUnicode_Check(py_obj)){
            // python的str类型
            PyObject *py_utf8 = PyUnicode_AsUTF8String(py_obj);
            Q_UNUSED(py_utf8);
            const char *ustr = PyUnicode_AsUTF8(py_obj); // PyUnicode_AS_DATA(只能得到首字母)  PyUnicode_AsUTF8
            printf("i am python-str, val = %s\n", ustr); // 可以含有中文
            print_line();
        } else if (PyList_Check(py_obj)){
            // Python list
            printf("i am python-list======>\n");
            for (int i = 0;i < PyList_Size(py_obj); i++){
                PyObject *py_data = PyList_GetItem(py_obj, i);
                print_pyobject(py_data);
            }
            print_line();
        } else if (PyTuple_Check(py_obj)){
            // Python tuple
            printf("i am python-tuple======>\n");
            for (int i = 0;i < PyTuple_Size(py_obj); i++){
                PyObject *py_data = PyTuple_GetItem(py_obj, i);
                print_pyobject(py_data);
            }
            print_line();
        } else if (PyDict_Check(py_obj)){
            // Python dict
            PyObject *key, *value;
            Py_ssize_t pos = 0;

            printf("i am python-dict======>\n");
            while (PyDict_Next(py_obj, &pos, &key, &value)) {
                print_pyobject(key);
                print_pyobject(value);
            }
            print_line();
        }

}
#endif // PYTEST_PRINT_H
