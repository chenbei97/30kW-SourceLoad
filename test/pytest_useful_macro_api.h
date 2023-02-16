#ifndef PYTEST_USEFUL_MACRO_API_H
#define PYTEST_USEFUL_MACRO_API_H

#include <Python.h>
#include <QtCore>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

 // https://docs.python.org/zh-cn/3.7/c-api/intro.html#useful-macros
// 测试<Python.h>提供的有用的宏
enum class UNREACHABLE_Test {UNR1,UNR2,UNR3,UNR0};

void test_useful_macro(UNREACHABLE_Test unr)
{
    // 一个不打算被触及的代码路径时使用
    // 例如switch语句所有case的情况都已经涉及,default什么也不会执行,就可以使用
    // 或者非常想在某个位置放一个 assert(0) 或 abort() 调用时也可以用这个

    switch (unr) {
            case UNREACHABLE_Test::UNR0: printf("unr0\n");break;
            case UNREACHABLE_Test::UNR1: printf("unr1\n");break;
            case UNREACHABLE_Test::UNR2: printf("unr2\n");break;
            case UNREACHABLE_Test::UNR3: printf("unr3\n");break;
            default:Py_UNREACHABLE(); break;
    }

    // 返回绝对值、最小值和最大值
    printf("abs(-1) = %d\n",Py_ABS(-1));
    printf("min(-1,5) = %d\n",Py_MIN(-1,5));
    printf("max(-1,5) = %d\n",Py_MAX(-1,5));

    // 转换为 C 字符串例如 Py_STRINGIFY(123) 返回 "123"Py_STRINGIFY(x)
    printf("string(-115) = %s\n",Py_STRINGIFY(-115.345));

    // Py_MEMBER_SIZE(type, member)返回结构 (type) member 的大小，以字节表示
    struct people {
        int age;
        char name[4];
        double salary;
    } p;
    p.age = 5;
    p.name[0] = 'c';p.name[1] = 'h';p.name[2] = 'e';p.name[3] = 'n';
    p.salary = 13000.;
    printf("people.salary's size = %d bytes\n",(int)Py_MEMBER_SIZE(people,salary));
    Q_UNUSED(p);
    // 参数必须为 [-128, 127] 或 [0, 255] 范围内的字符或整数类型
    // 这个宏将 c 强制转换为 unsigned char 返回
    unsigned char c1 =Py_CHARMASK(-128);unsigned char c2 =Py_CHARMASK(-1);
    printf("c1 = %c c2 = %d\n",c1,c2);

    // Py_GETENV(s) 与 getenv(s) 类似,但如果命令行上传递了 -E ，则返回 NULL
    // 即如果设置了 Py_IgnoreEnvironmentFlag ，不太常用

    // Py_UNUSED(arg) 相当于Q_UNUSED(arg)
    // 不过这里用于函数定义中未使用的参数以隐藏编译器警告
    // 不知道咋用
    int k = 0;
    PyObject* pk = PyLong_FromLong(k); // 这里pk依然显示未使用警告
    Q_UNUSED(pk);
    //PyObject * Py_UNUSED(pk);
    //PyObject* func(PyObject * Py_UNUSED(ignored));
    //Q_UNUSED(func);
    // PyDoc_STRVAR(name, str) 创建一个可以在文档字符串中使用的名字为 name 的变量
    // 如果不和文档字符串一起构建 Python，该值将为空
    //PyDoc_STR(str)为给定的字符串输入创建一个文档字符串
    // 或者当文档字符串被禁用时，创建一个空字符串
    /*
        官方提供的例子（不常用）
        PyDoc_STRVAR(pop_doc, "Remove and return the rightmost element.");
        static PyMethodDef deque_methods[] = {
            // ...
            {"pop", (PyCFunction)deque_pop, METH_NOARGS, pop_doc},
            // ...
        };

        static PyMethodDef pysqlite_row_methods[] = {
            {"keys", (PyCFunction)pysqlite_row_keys, METH_NOARGS,
                PyDoc_STR("Returns the keys of the row.")},
            {NULL, NULL}
        };
    */
}

// https://docs.python.org/zh-cn/3.7/c-api/refcounting.html
// 测试关于引用计数的宏
void test_ref_count()
{
    /*
       增加对象 o 的引用计数, 对象必须不为 NULL,如果你不确定它不为 NULL,可使用 Py_XINCREF()
        void Py_INCREF(PyObject *o);
        增加对象 o 的引用计数, 对象可以为 NULL,在此情况下该宏不产生任何效果
        void Py_XINCREF(PyObject *o);
        // 同理,减少引用计数
        void Py_DECREF(PyObject *o);
        void Py_XDECREF(PyObject *o);
        void Py_CLEAR(PyObject *o);
    */
}
#endif // PYTEST_USEFUL_MACRO_API_H
