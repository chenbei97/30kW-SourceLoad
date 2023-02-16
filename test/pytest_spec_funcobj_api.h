#ifndef PYTEST_SPEC_FUNCOBJ_API_H
#define PYTEST_SPEC_FUNCOBJ_API_H

#include <Python.h>
#include <QDebug>
#include <datetime.h>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
//#include <pytest/pytest_print.h>

// API接口：https://docs.python.org/zh-cn/3.7/c-api/concrete.html
// 这里涉及的是函数对象、实例方法对象、方法对象、迭代器对象、DateTime对象
 // 对于Cell对象、代码对象、文件对象、描述符对象、切片对象、Ellipsis对象、
// MemoryView对象、弱引用对象、胶囊、生成器对象、协程对象、上下文变量对象用不到忽略

void test_func_obj()
{
    /*
        PyFunctionObject  用于函数的 C 结构体
        PyTypeObject PyFunction_Type 表示 Python 函数类型,作为 types.FunctionType 向 Python 程序员公开
        int PyFunction_Check(PyObject *o);

         返回与 code 关联的新函数对象。 globals 必须是一个字典，该函数可以访问全局变量
         PyObject* PyFunction_New(PyObject *code, PyObject *globals);

         返回与函数对象 op 关联的代码对象
        PyObject* PyFunction_GetCode(PyObject *op);

         返回与函数对象*op*相关联的全局字典
        PyObject* PyFunction_GetGlobals(PyObject *op);

         返回函数对象 op 的 __module__ 属性，通常为一个包含了模块名称的字符串
         但可以通过 Python 代码设为返回其他任意对象
        PyObject* PyFunction_GetModule(PyObject *op);

         返回和设置函数对象 op 的参数默认值。 这可以是一个参数元组或 NULL
        PyObject* PyFunction_GetDefaults(PyObject *op);
        int PyFunction_SetDefaults(PyObject *op, PyObject *defaults);

         返回和设置关联到函数对象 op 的闭包，closure 必须为 Py_None 或 cell 对象的元组，失败时引发 SystemError 异常并返回 -1
        PyObject* PyFunction_GetClosure(PyObject *op);
        int PyFunction_SetClosure(PyObject *op, PyObject *closure);

         返回和设置函数对象 op 的标注，annotations 必须为一个字典或 Py_None,失败时引发 SystemError 异常并返回 -1
        PyObject *PyFunction_GetAnnotations(PyObject *op);
        int PyFunction_SetAnnotations(PyObject *op, PyObject *annotations);
    */
    Py_Initialize();
//    PyObject* func = PyFunction_New(Py_BuildValue("i",1), Py_BuildValue("{s:i,s:i}","a",1,"b",2));
//    qDebug()<<(PyFunction_Check(func) == 1); // 这里程序异常

    /*
        实例方法对象：
        PyTypeObject PyInstanceMethod_Type 代表 Python 实例方法类型。 它不对 Python 程序公开
        int PyInstanceMethod_Check(PyObject *o);
        PyObject* PyInstanceMethod_New(PyObject *func);返回一个新的实例方法对象，func 应为任意可调用对象，func 将在实例方法被调用时作为函数被调用

        PyObject* PyInstanceMethod_Function(PyObject *im); 返回关联到实例方法 im 的函数对象
        PyObject* PyInstanceMethod_GET_FUNCTION(PyObject *im); 宏版本的 PyInstanceMethod_Function()，略去了错误检测
*/

    /*
        方法对象:
        方法是绑定的函数对象。 方法总是会被绑定到一个用户自定义类的实例。 未绑定方法（绑定到一个类的方法）已不再可用。
        PyTypeObject PyMethod_Type 实例代表 Python 方法类型。 它作为 types.MethodType 向 Python 程序公开
        int PyMethod_Check(PyObject *o);

        PyObject* PyMethod_New(PyObject *func, PyObject *self);返回一个新的方法对象，func 应为任意可调用对象，self 为该方法应绑定的实例。
        在方法被调用时 func 将作为函数被调用。 self 必须不为 NULL

        PyObject* PyMethod_Function(PyObject *meth); 返回关联到方法 meth 的函数对象
        PyObject* PyMethod_GET_FUNCTION(PyObject *meth); 宏版本的 PyMethod_Function()，略去了错误检测
        PyObject* PyMethod_Self(PyObject *meth); 返回关联到方法 meth 的实例
        PyObject* PyMethod_GET_SELF(PyObject *meth); 宏版本的 PyMethod_Self()，省略了错误检测
        int PyMethod_ClearFreeList(); 清空释放列表。 返回所释放的条目数

*/

    /*
          迭代器对象：
            Python 提供了两个通用迭代器对象。 第一个是序列迭代器，它使用支持 __getitem__() 方法的任意序列
            第二个使用可调用对象和一个 sentinel 值，为序列中的每个项调用可调用对象，并在返回 sentinel 值时结束迭代

            PyTypeObject PySeqIter_Type  PySeqIter_New() 返回迭代器对象的类型对象和内置序列类型内置函数 iter() 的单参数形式
            int PySeqIter_Check(op)；如果 op 的类型为 PySeqIter_Type 则返回 true
            PyObject* PySeqIter_New(PyObject *seq);  返回一个与常规序列对象一起使用的迭代器 seq

            PyTypeObject PyCallIter_Type; 由函数 PyCallIter_New() 和 iter() 内置函数的双参数形式返回的迭代器对象类型对象
            int PyCallIter_Check(op); 如果 op 的类型为 PyCallIter_Type 则返回 true
            PyObject* PyCallIter_New(PyObject *callable, PyObject *sentinel);返回一个新的迭代器。 第一个参数 callable 可以是任何可以在没有参数的情况下调用的 Python 可调用对象
            每次调用都应该返回迭代中的下一个项目。 当 callable 返回等于 sentinel 的值时，迭代将终止
    */
        /*
                DateTime对象 datetime 模块提供了各种日期和时间对象。 在使用任何这些函数之前，必须在你的源码中包含头文件 datetime.h(没包含于Python.h)
                用于类型检查的宏:
                int PyDate_Check(PyObject *ob); PyDateTime_DateType 类型或 PyDateTime_DateType 的某个子类型则返回真值。 ob 不能为 NULL
                int PyDate_CheckExact(PyObject *ob);  如果 ob 为 PyDateTime_DateType 类型则返回真值。 ob 不能为 NULL
                int PyDateTime_Check(PyObject *ob); 下同理 PyDateTime_DateTimeType
                int PyDateTime_CheckExact(PyObject *ob);
                int PyTime_Check(PyObject *ob); PyDateTime_TimeType
                int PyTime_CheckExact(PyObject *ob);
                int PyDelta_Check(PyObject *ob);  PyDateTime_DeltaType
                int PyDelta_CheckExact(PyObject *ob);
                int PyTZInfo_Check(PyObject *ob);  PyDateTime_TZInfoType
                int PyTZInfo_CheckExact(PyObject *ob);

                用于创建对象的宏:
                PyObject* PyDate_FromDate(int year, int month, int day);
                PyObject* PyDateTime_FromDateAndTime(int year, int month, int day, int hour, int minute, int second, int usecond);
                PyObject* PyTime_FromTime(int hour, int minute, int second, int usecond);

                不常用的宏:
                返回具有指定 hour, minute, second, microsecond 和 fold 属性的 datetime.time 对象
                PyObject* PyTime_FromTimeAndFold(int hour, int minute, int second, int usecond, int fold);
                返回代表给定天、秒和微秒数的 datetime.timedelta 对象
                PyObject* PyDelta_FromDSU(int days, int seconds, int useconds);
                返回一个 datetime.timezone 对象，该对象带有以 offset 参数表示 的未命名固定时差
                PyObject* PyTimeZone_FromOffset(PyDateTime_DeltaType* offset);
                返回一个 datetime.timezone 对象，该对象具有以 offset 参数表示的固定时差和时区名称 name
                PyObject* PyTimeZone_FromOffsetAndName(PyDateTime_DeltaType* offset, PyUnicode* name);

                获取具体的数字:
                从 date 对象中提取字段的宏
                int PyDateTime_GET_YEAR(PyDateTime_Date *o); 参数必须是 PyDateTime_Date 包括其子类 (例如 PyDateTime_DateTime)
                int PyDateTime_GET_MONTH(PyDateTime_Date *o);
                int PyDateTime_GET_DAY(PyDateTime_Date *o);
                从 datetime 对象中提取字段的宏
                int PyDateTime_DATE_GET_HOUR(PyDateTime_DateTime *o); 参数必须是 PyDateTime_DateTime 包括其子类的实例
                int PyDateTime_DATE_GET_MINUTE(PyDateTime_DateTime *o);
                int PyDateTime_DATE_GET_SECOND(PyDateTime_DateTime *o);
                int PyDateTime_DATE_GET_MICROSECOND(PyDateTime_DateTime *o);
                从 time 对象中提取字段的宏
                int PyDateTime_TIME_GET_HOUR(PyDateTime_Time *o);参数必须是 PyDateTime_Time 包括其子类的实例
                int PyDateTime_TIME_GET_MINUTE(PyDateTime_Time *o);
                int PyDateTime_TIME_GET_SECOND(PyDateTime_Time *o);
                int PyDateTime_TIME_GET_MICROSECOND(PyDateTime_Time *o);
                从 timedelta 对象中提取字段的宏
                int PyDateTime_DELTA_GET_DAYS(PyDateTime_Delta *o);
                int PyDateTime_DELTA_GET_SECONDS(PyDateTime_Delta *o);
                int PyDateTime_DELTA_GET_MICROSECONDS(PyDateTime_Delta *o);

                其它:
                创建并返回一个给定元组参数的新 datetime.datetime 对象，适合传给 datetime.datetime.fromtimestamp()
                PyObject* PyDateTime_FromTimestamp(PyObject *args);
                创建并返回一个给定元组参数的新 datetime.date 对象，适合传给 datetime.date.fromtimestamp()
                PyObject* PyDate_FromTimestamp(PyObject *args);
        */
                PyDateTime_IMPORT; // 必须先调用这个宏导入
                int year =2022, month = 11, day = 9;
                PyObject* mydate =  PyDate_FromDate(year,month,day);
                PyObject* mydatetime = PyDateTime_FromDateAndTime(2022,11,9,9, 30, 36, 243);
                PyObject* mytime = PyTime_FromTime(9, 30, 36, 243);

                qDebug()<<"year = "<<PyDateTime_GET_YEAR(mydate); // date/datetime均可
                qDebug()<<"month = "<<PyDateTime_GET_MONTH(mydatetime);
                qDebug()<<"day = "<<PyDateTime_GET_DAY(mydatetime);

                qDebug()<<"hour = "<<PyDateTime_DATE_GET_HOUR(mydatetime); // 只能传递datetime类型不能传递time/date类型
                qDebug()<<"minute = "<<PyDateTime_DATE_GET_MINUTE(mydatetime);
                qDebug()<<"second = "<<PyDateTime_DATE_GET_SECOND(mydatetime);
                qDebug()<<"microsecond = "<<PyDateTime_DATE_GET_MICROSECOND(mydatetime);

                qDebug()<<"h = "<<PyDateTime_TIME_GET_HOUR(mytime); // 必须传递time不能传递date/datetime
                qDebug()<<"min = "<<PyDateTime_TIME_GET_MINUTE(mytime);
                qDebug()<<"s = "<<PyDateTime_TIME_GET_SECOND(mytime);
                qDebug()<<"ms = "<<PyDateTime_TIME_GET_MICROSECOND(mytime);

                Py_Finalize();

}
#endif // PYTEST_SPEC_FUNCOBJ_API_H
