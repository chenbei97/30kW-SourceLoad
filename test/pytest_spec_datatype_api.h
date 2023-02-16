#ifndef PYTEST_SPEC_DATATYPE_API_H
#define PYTEST_SPEC_DATATYPE_API_H

#include <Python.h>
#include <QDebug>
#include "pytest_print.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
// API接口：https://docs.python.org/zh-cn/3.7/c-api/concrete.html
// 这里涉及的是数值对象、序列对象、容器对象、

void test_long_obj()
{
    /*
        整数型对象：所有整数都使用以任意大小的长整数对象表示
        PyLongObject：表示 Python 整数对象的 PyObject 子类型
        PyTypeObject PyLong_Type ：这个 PyTypeObject 的实例表示 Python 的整数类型。与Python层中的 int 相同
    */
    Py_Initialize();
    PyLongObject();
    PyAPI_DATA(PyTypeObject) PyLong_Type;

    unsigned long ul = 5;
    PyObject * ulp = PyLong_FromUnsignedLong(ul); // 返回ul对象,失败返回null
    qDebug()<<"PyLong_Check(ulp) = "<<PyLong_Check(ulp) //是PyLongObject 或 PyLongObject的子类型返回true
           <<" PyLong_CheckExact(ulp) = "<<PyLong_CheckExact(ulp);//是PyLongObject 但不是 PyLongObject的子类型返回true

    long l = -3; // 类似的还有long、size_t、Py_ssize_t、long long、double
    size_t szt = 11;
    Py_ssize_t sszt = 4; // 是C Py_ssize_t 类型
    long long ll = 2;
    double d = 2.78;
    long m = LONG_MAX - 1;
    Q_UNUSED(m);

    qDebug()<<"PyLong_CheckExact(PyLong_FromLong(l)) = "<<PyLong_CheckExact(PyLong_FromLong(l));
    qDebug()<<"PyLong_CheckExact(PyLong_FromSize_t(szt)) = "<<PyLong_CheckExact(PyLong_FromSsize_t(szt));
    qDebug()<<"PyLong_CheckExact(PyLong_FromSsize_t(sszt)) = "<<PyLong_CheckExact(PyLong_FromSsize_t(sszt));
    qDebug()<<"PyLong_CheckExact(PyLong_FromLongLong(ll)) = "<<PyLong_CheckExact(PyLong_FromLongLong(ll));
    qDebug()<<"PyLong_CheckExact(PyLong_FromDouble(d)) = "<<PyLong_CheckExact(PyLong_FromDouble(d));

    // 逆变换
    qDebug()<<"PyLong_AsLong(PyLong_FromLong(l)) = "<<PyLong_AsLong(PyLong_FromLong(l));
    // long PyLong_AsLongAndOverflow(PyObject *obj, int *overflow) // 不再测试不常用
    qDebug()<<"PyLong_AsLongLong(PyLong_FromLongLong(ll)) = "<<PyLong_AsLongLong(PyLong_FromLongLong(ll));
    qDebug()<<"PyLong_AsUnsignedLongLong(PyLong_FromLongLong(ll)) = "<<PyLong_AsUnsignedLongLong(PyLong_FromLongLong(ll));
    //long long PyLong_AsLongLongAndOverflow(PyObject *obj, int *overflow) // 不再测试不常用
    qDebug()<<"PyLong_AsSsize_t(PyLong_FromSsize_t(sszt)) = "<<PyLong_AsSsize_t(PyLong_FromSsize_t(sszt));
    qDebug()<<"PyLong_AsSize_t(PyLong_FromSize_t(szt)) = "<<PyLong_AsSize_t(PyLong_FromSize_t(szt));
    qDebug()<<"PyLong_AsUnsignedLong(PyLong_FromLong(ul)) = "<<PyLong_AsUnsignedLong(PyLong_FromLong(ul));
     // unsigned long PyLong_AsUnsignedLongMask(PyObject *obj)  // 不再测试不常用
    // unsigned long long PyLong_AsUnsignedLongLongMask(PyObject *obj) // 不再测试不常用
    qDebug()<<"PyLong_AsDouble(PyLong_FromDouble(d)) = "<<PyLong_AsDouble(PyLong_FromDouble(d)); // 这里给取整了
    // PyObject* PyLong_FromUnicode(Py_UNICODE *u, Py_ssize_t length, int base) // 不再测试不常用
    // PyObject* PyLong_FromUnicodeObject(PyObject *u, int base) // 不再测试不常用
    PyObject * voidlp = PyLong_FromVoidPtr(&l);
    long * lp = (long*)PyLong_AsVoidPtr(voidlp);
    qDebug()<< "*lp = "<<*lp;

    // PyLong_FromString(const char *str, char **pend, int base) 字符串的参数有3个
    // base必须在[2,36]之间,pend是指向字符串的第几个字符,如果是null,默认就是第1个字符开始
    char  arrStrs[] = "0x12";
    char * str = &arrStrs[0];
    char ** pend = &str;
    PyObject * strp = PyLong_FromString(str,pend,10); // ValueError: invalid literal for int() with base 10: '0x12'
    Q_UNUSED(strp);
    Py_Finalize();
}

void test_bool_obj()
{
    /*
        Python 中的布尔值是作为整数的子类实现的
        只有 Py_False 和 Py_True 两个布尔值
        因此，正常的创建和删除功能不适用于布尔值，也就是没有PyLong_From*这类函数
        但是有一些宏可以使用：
        PyObject* Py_False、PyObject* Py_True：代表Python的False和True对象,且该对象没有任何方法

        Py_RETURN_FALSE、Py_RETURN_TRUE：返回 Py_False或Py_True需要增加它的引用计数
    */

    Py_Initialize();

    long v = 5;
    long c = 0;
    PyObject*  pv = PyBool_FromLong(v); // 返回一个 Py_True 或者 Py_False 的新引用
    PyObject*  pc = PyBool_FromLong(c);
    qDebug()<<PyBool_Check(pv) <<"  "<<PyBool_Check(pc);
    qDebug()<<(pv == Py_True) <<"  "<< (pc == Py_False);

    print_pyobject(Py_False);
    print_pyobject(Py_True);

    Py_Finalize();
}

void test_float_obj()
{
    /*
          PyFloatObject代表一个Python浮点数对象
          PyFloat_Type代表Python浮点类型的实例 在Python层面的类型 float 是同一个对象
    */
    PyFloatObject();

    //根据字符串 str 的值创建一个 PyFloatObject，失败时返回 NULL
    PyObject * str = PyUnicode_FromString("12.345435"); // 从字符串创建字符对象
    PyObject* pfobj =  PyFloat_FromString(str);  // 转为float对象
    qDebug()<<PyFloat_AsDouble(pfobj); // 再转为double类型,有错误检查
    // PyFloat_AS_DOUBLE 没有错误检查

    // 是一个C类型 PyFloatObject 或者是C类型 PyFloatObject 的子类型时，返回真
    // 参数是一个C类型 PyFloatObject 但不是C类型 PyFloatObject 的子类型时，返回真
    qDebug()<<PyFloat_Check(pfobj)<<"  "<<PyFloat_CheckExact(pfobj);

    double dd = 2.34335;
    PyObject * pyd =  PyFloat_FromDouble(dd);
    Q_UNUSED(pyd);
    //PyObject * info = PyFloat_GetInfo();//返回一个 structseq 实例，其中包含有关 float 的精度、最小值和最大值的信息
    qDebug()<<PyFloat_GetMax() <<"  "<<PyFloat_GetMin();

}

void test_byte_obj()
{
    /*
    当期望带一个字节串形参但却带一个非字节串形参被调用时，这些函数会引发 TypeError
    PyBytesObject：表示一个 Python 字节对象
    PyBytes_Type：代表一个 Python 字节类型，在 Python 层面它与 bytes 是相同的对象
    常用的函数，类似上边float、long等的函数
    int PyBytes_Check(PyObject *o);
    int PyBytes_CheckExact(PyObject *o);

    */

    PyObject*  pbytes = PyBytes_FromString("12dsfsdjf3"); // 成功时返回一个以字符串 v 的副本为值的新字节串对象
    Py_ssize_t len = PyBytes_Size(pbytes); // 返回字节对象*o*中字节的长度
    qDebug()<<"len = "<<len;
    char*  str = PyBytes_AsString(pbytes);
    qDebug()<<"str = "<<str;

    PyObject*  python = PyBytes_FromString("python");
    PyObject*  cpp = PyBytes_FromString("cpp");
    PyObject ** ppython = &python;
    // void PyBytes_Concat(PyObject **bytes, PyObject *newpart)
    PyBytes_Concat(ppython, cpp); // 在 *bytes 中创建新的字节串对象，其中包含添加到 bytes 的 newpart 的内容
    char*  newstr = PyBytes_AsString(*ppython);
    qDebug()<<"newstr = "<<newstr;

//    void PyBytes_ConcatAndDel(PyObject **bytes, PyObject *newpart) 不常用
//    在 *bytes 中创建新的字节串对象，其中包含添加到 bytes 的 newpart 的内容

//    int _PyBytes_Resize(PyObject **bytes, Py_ssize_t newsize) 不常用
//    改变字节串大小的一种方式，即使其为“不可变对象”。 此方式仅用于创建全新的字节串对象；

    /*下方继续测试字节数组的API
            int PyByteArray_Check(PyObject *o)
            int PyByteArray_CheckExact(PyObject *o)

            PyObject* PyByteArray_FromObject(PyObject *o) 根据任何实现了 缓冲区协议 的对象 o，返回一个新的字节数组对象

    */
    PyObject* parr =PyByteArray_FromStringAndSize("12dsfsdjf3", len);
    Py_ssize_t  l = PyByteArray_Size(parr);
    char* k = PyByteArray_AsString(parr);
    qDebug()<<"k = "<<k <<"  size = "<<l;

}

void test_unicode_obj()
{
    /*
        Unicode对象和编码解码器
        PyTypeObject PyUnicode_Type PyTypeObject的此实例表示Python Unicode类型。它以str的形式暴露给Python代码
        int PyUnicode_Check(PyObject *o);
        int PyUnicode_CheckExact(PyObject *o);

    */
    // 从字符缓冲区u创建一个Unicode对象。字节将被解释为UTF-8编码。缓冲区将复制到新对象中
    PyObject* str = PyUnicode_FromStringAndSize("chenbei", 3);
    Py_ssize_t len = PyUnicode_GET_SIZE(str);//返回不推荐使用的Py_UNICODE表示形式的大小，以代码为单位
    qDebug()<<"PyUnicode_GET_SIZE len = "<<len;//3
    len = PyUnicode_GET_LENGTH(str); // 返回Unicode字符串的长度
    qDebug()<<"PyUnicode_GET_LENGTH len = "<<len;//3
    PyObject *str1 = PyUnicode_FromString("123456789"); // 不指定长度的创建也可以
    PyObject* str2 = PyUnicode_Substring(str1, 4, 8);
    qDebug()<<PyUnicode_AsUTF8(str2); // 解析回字符串
    qDebug()<<PyUnicode_Contains(str1,str2); // 是否包含字符串

    unsigned long long val = 5;
    PyObject* ullval = PyUnicode_FromFormat("%llu",val);
    qDebug()<<"ullval = "<<PyUnicode_AsUTF8(ullval);
}

void test_tuple_obj()
{
    /*
        PyTupleObject 子类型代表一个 Python 的元组对象
        PyTuple_Type PyTypeObject 的实例代表一个 Python 元组类型，这与 Python 层面的 tuple 是相同的对象
        int PyTuple_Check(PyObject *p)
        int PyTuple_CheckExact(PyObject *p)
        int _PyTuple_Resize(PyObject **p, Py_ssize_t newsize); 可以用于调整元组的大小。 newsize 将是元组的新长度

    */
    Py_Initialize();

    PyObject* t1 = PyTuple_New(4);//成功时返回一个新的元组对象长度为 len，失败时返回NULL
    qDebug()<<(t1==Q_NULLPTR);

    PyTuple_SetItem(t1,0,PyFloat_FromDouble(2.532)); // PyTuple_SET_ITEM也可以,但是不进行错误检查
    PyTuple_SetItem(t1,1,PyLong_FromLong(3));
    PyTuple_SetItem(t1,2,PyFloat_FromString(PyUnicode_FromString("21.23423")));
    PyTuple_SetItem(t1,3,Py_BuildValue("i",28));
    qDebug()<<PyTuple_Size(t1); // PyTuple_GET_SIZE不执行错误检查
    qDebug()<<PyFloat_AsDouble(PyTuple_GetItem(t1,0)); // 得到的是2 PyTuple_GET_ITEM不执行错误检查
    qDebug()<<PyFloat_AsDouble(PyTuple_GetItem(t1,2)); // 21.2342

    // 遍历元组
    for (int i = 0; i < PyTuple_Size(t1); i++){
           PyObject* pyData = PyTuple_GetItem(t1, i);
           print_pyobject(pyData);
       }

    printf_s("get slice tuple\n");
    PyObject* pySlice = PyTuple_GetSlice(t1, 2, PyTuple_Size(t1));   // 获取切片数据(后2个元素)
    print_pyobject(pySlice);

     PyObject* t2 = PyTuple_Pack(2,PyLong_FromLong(20),PyFloat_FromDouble(3.14)); // PyTuple_Pack后边2个参数是 PyObject*
     //PyObject* t2 = Py_BuildValue("(ii)", 28,30); //OO会异常,ii不会
     qDebug()<<"PyTuple_Pack[0] = "<<PyLong_AsLong(PyTuple_GetItem(t2,0));
     qDebug()<<"PyTuple_Pack[1] = "<<PyFloat_AsDouble(PyTuple_GetItem(t2,1));

     Py_Finalize();
}

void test_list_obj()
{
    /*
        PyListObject 代表一个Python列表对象
        PyTypeObject PyList_Type 代表Python列表类型的实例。在Python层面和类型 list 是同一个对象
        int PyList_Check(PyObject *p);
        int PyList_CheckExact(PyObject *p);

*/
    Py_Initialize();
    // 成功时返回一个长度为 len 的新列表，失败时返回 NULL
    //  当 len 大于零时，被返回的列表对象项目被设成 NULL
    // 因此不能用类似C函数 PySequence_SetItem() 的抽象API或者PyList_SetItem()
    // 将所有项目设置成真实对象前对Python代码公开这个对象（这句话很重要，什么意思？）
    Py_ssize_t len = 3;
    PyObject* list = PyList_New(len); // 如果len=0意味着列表没有大小限制
    qDebug()<<(list == Q_NULLPTR); // false

    PyList_SetItem(list,0,Py_BuildValue("i",2));
    PyList_SetItem(list,1,Py_BuildValue("s","chenbei"));
    PyList_SetItem(list,2,Py_BuildValue("f",29.3f));
    PyList_Append(list,Py_BuildValue("d",3.1415926));
    PyList_Insert(list,1,Py_BuildValue("(ii)",10,100));


    qDebug()<<"len = "<<PyList_Size(list); // 5

    PyObject* tuple = PyList_GetItem(list,1);
    qDebug()<<"tuple = ("<<PyLong_AsLong(PyTuple_GetItem(tuple,0))<<","
            <<PyLong_AsLong(PyTuple_GetItem(tuple,1))<<")"; // (10,100)

    // [2,(10,100),"chenbei",29.3f,3.1415926]
    int ret = PyList_Reverse(list); // 反转列表
    qDebug()<<"reverse successful? "<<(ret==0); // 失败返回-1

    PyObject* sublist = PyList_GetSlice(list,0,2); //list[low:high)
    // int PyList_SetSlice(PyObject *list, Py_ssize_t low, Py_ssize_t high, PyObject *itemlist)
    // 等价于list[low:high) = itemlist 这个不常用
    print_pyobject(sublist);
    qDebug()<<"sublist[0] = "<<PyFloat_AsDouble(PyList_GetItem(sublist,0));
    qDebug()<<"sublist[1] = "<<PyFloat_AsDouble(PyList_GetItem(sublist,1));


    // 创建无限制的列表,让len=0即可
    PyObject* lst = PyList_New(0);
    PyList_Append(lst,Py_BuildValue("c",'c'));
    PyList_Append(lst,Py_BuildValue("c",'b'));
    PyList_Append(lst,Py_BuildValue("c",'a'));
    PyList_Sort(lst); // 升序
    print_pyobject(lst);

     PyObject* tup = PyList_AsTuple(lst);
    print_pyobject(tup); // 转换成元组

    Py_Finalize();

}

void test_dict_obj()
{
      /*
            PyDictObject 代表一个Python字典对象
            PyTypeObject PyDict_Type 表示为 PyTypeObject 的实例 这与Python层面的 dict 是相同的对象
            int PyDict_Check(PyObject *p);
            int PyDict_CheckExact(PyObject *p);
    */
        Py_Initialize();

        // 创建字典
        PyObject* dict1 = PyDict_New();
        int ret = PyDict_Check(dict1);
        if (ret) printf("is dict\n");
        else printf("is not dict\n");
        PyDict_SetItem(dict1, Py_BuildValue("s", "account_id"), Py_BuildValue("i", 1238)); // (dict,key,value)
        print_pyobject(dict1);
        qDebug()<<"dict contains s? "<<PyDict_Contains(dict1,Py_BuildValue("s", "account_id"));

        // key也可以直接传递字符串(内部使用PyUnicode_FromString(key)转换),然后指定值
        PyDict_SetItemString(dict1,"account_name",Py_BuildValue("s", "mono"));
        print_pyobject(dict1);

        // 删除键值对
        PyDict_DelItem(dict1,Py_BuildValue("s", "account_name"));
        PyDict_DelItemString(dict1,"account_id");
        qDebug()<<"PyDict_Size = "<<PyDict_Size(dict1); // 0

        // 如果键不存在则插入默认值,如果存在就会返回这个值
        PyDict_SetDefault(dict1,Py_BuildValue("s", "a"),Py_BuildValue("i",1));
        print_pyobject(dict1);
        PyObject* val = PyDict_SetDefault(dict1,Py_BuildValue("s", "a"),Py_BuildValue("i",2));
        print_pyobject(val); // 返回这个值1

        PyDict_SetDefault(dict1,Py_BuildValue("s", "b"),Py_BuildValue("i",12));
        print_pyobject(PyDict_GetItem(dict1,Py_BuildValue("s", "b"))); // 2种方式获取项
        print_pyobject(PyDict_GetItemString(dict1,"b"));

        // 获取全部的key和全部的value作为列表来返回
        print_pyobject(PyDict_Items(dict1));
        print_pyobject(PyDict_Keys(dict1));
        print_pyobject(PyDict_Values(dict1));

        PyObject* dict2 = PyDict_New();
        PyDict_SetItemString(dict2,"a",Py_BuildValue("i", 4));
        PyDict_SetItemString(dict2,"c",Py_BuildValue("i", 14));
        // PyDict_Merge(dict1,dict2,1); // 合并字典,结果是key=a的val=4被替换,如果override=0,则不会替换
        // 等价于PyDict_Merge(dict1,dict2,1),类似于python的a.update(b)
        PyDict_Update(dict1, dict2);//差别在于 PyDict_Update() 在第二个参数没有 "keys" 属性时不会回退到迭代键值对的序列
        print_pyobject(dict1);

        // 迭代字典
        // int PyDict_Next(PyObject *p, Py_ssize_t *ppos, PyObject **pkey, PyObject **pvalue)
        //  ppos 所引用的 Py_ssize_t 必须初始化为 0, 且在迭代期间不应被更改
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while (PyDict_Next(dict1, &pos, &key, &value)) {
            print_pyobject(key);
            print_pyobject(value);
            /*
             下方是官方提供的代码
            long i = PyLong_AsLong(value);
            if (i == -1 && PyErr_Occurred()) {
                return ;
            }
            PyObject *o = PyLong_FromLong(i + 1);
            if (o == NULL) return;
            if (PyDict_SetItem(dict1, key, o) < 0) {
                Py_DECREF(o);
                return ;
            }
            Py_DECREF(o); */
        }

        PyDict_Clear(dict1); // 清空字典
        print_pyobject(PyDict_Copy(dict2)); // 复制字典

        int c = PyDict_ClearFreeList(); // 清空释放列表  返回所释放的条目数
        qDebug()<<"clear free list count = "<<c;
        Py_Finalize();
}

void test_set_obj()
{
    /*
        PySetObject 用来保存 set 和 frozenset 对象的内部数据
        PyTypeObject PySet_Type 表示 Python set 类型
        PyTypeObject PyFrozenSet_Type 表示 Python frozenset 类型
        int PySet_Check(PyObject *p);
        int PyFrozenSet_Check(PyObject *p);
        int PyAnySet_Check(PyObject *p);  p 是一个 set 对象、frozenset 对象或者是其子类型的实例则返回真值
        int PyAnySet_CheckExact(PyObject *p);p 是一个 set 对象或 frozenset 对象但不是其子类型的实例则返回真值
    */
    Py_Initialize();
    PyObject* list = Py_BuildValue("[i,i,i,d,f,s]",1,2,1,3.4,2.7f,"cb");
    PyObject* pset = PySet_New(list);
    PyObject* pfset = PyFrozenSet_New(list);
    qDebug()<<"pset size = "<<PySet_Size(pset) <<"   pfset size = "<<PySet_Size(pfset); // 5 自动去重1

    qDebug()<<"set contains cb? "<<PySet_Contains(pset,Py_BuildValue("s","cb"));
    qDebug()<<"set contains 2.7f? "<<PySet_Contains(pset,Py_BuildValue("f",2.7f));

    PySet_Add(pset,Py_BuildValue("(ii)",100,1000));
    int ret = PySet_Discard(pset,Py_BuildValue("(ii)",100,1000));
    qDebug()<<"discard tuple? "<<(ret == 1);
    print_pyobject(PySet_Pop(pset));
    print_pyobject(PySet_Pop(pset));
    print_pyobject(PySet_Pop(pset));
    print_pyobject(PySet_Pop(pset));
    print_pyobject(PySet_Pop(pset));
    print_pyobject(PySet_Pop(pset)); // 这里已经删除了所有元素

    PySet_Clear(pfset); //清空现有字典的所有键值对。

    PySet_ClearFreeList();// 清空释放列表。 返回所释放的条目数
    Py_Finalize();
}

void test_structseq_obj()
{
    /*
        结构序列对象是等价于 namedtuple() 的 C 对象，即一个序列，其中的条目也可以通过属性访问
        要创建结构序列，你首先必须创建特定的结构序列类型

        关于namedtuple的定义:collections.namedtuple(typename, field_names, *, rename=False, defaults=None, module=None)
        typename: 取名
        field_names: 是个字符串列表['x','y']或纯字符串用空格或逗号隔开'x y' | 'x,y'
        rename: 为真， 无效域名会自动转换成位置名。比如 ['abc', 'def', 'ghi', 'abc'] 转换成 ['abc', '_1', 'ghi', '_3'] ， 消除关键词 def 和重复域名 abc
        defaults:可以为 None 或是一个默认值的 iterable 。如果一个默认值域必须跟其他没有默认值的域在一起出现，defaults 就应用到最右边的参数
                     比如域名 ['x', 'y', 'z'] 和默认值 (1, 2) ，那么 x 就必须指定一个参数值 ，y 默认值 1 ， z 默认值 2
        module: 如果有定义，命名元组的 __module__ 属性值就被设置
        例子:
            Point = namedtuple('Point', ['x', 'y'])
            p = Point(11, y=22)
            print(p[0] + p[1]) # 11+22=33
            33
            x, y = p
            print(x, y) #  (11, 22)
            print(p.x + p.y) # 33
            print(p)  # Point(x=11, y=22)

            from collections import namedtuple
            person = namedtuple('person',['name','sex','age','salary'])
            person._field_defaults = ('cb','man',25,13578)
            print(person.__doc__) # person(name, sex, age, salary)
            print(person._field_defaults) # ('cb', 'man', 25, 13578)
            print(person._fields) # ('name', 'sex', 'age', 'salary')
            p = person('chenbei','woman',24,20000)
            print(p) # person(name='chenbei', sex='woman', age=24, salary=20000)
            d= p._asdict() # d是OrderedDict类型
            print(d['age']) # 24
            print(person(**d)) # person(name='chenbei', sex='woman', age=24, salary=20000)
            p1 = p._replace(age=100) # person(name='chenbei', sex='woman', age=100, salary=20000)
            print(p1)

        PyTypeObject* PyStructSequence_NewType(PyStructSequence_Desc *desc); 根据 desc 中的数据创建一个新的结构序列类型
        PyObject* PyStructSequence_New(PyTypeObject *type); 创建 type 的实例，该实例必须使用 PyStructSequence_NewType() 创建

        PyObject* PyStructSequence_GetItem(PyObject *p, Py_ssize_t pos); 返回*p*所指向的结构序列中，位于*pos*处的对象。不需要进行边界检查
        PyObject* PyStructSequence_GET_ITEM(PyObject *p, Py_ssize_t pos); PyStructSequence_GetItem() 的宏版本
        void PyStructSequence_SetItem(PyObject *p, Py_ssize_t pos, PyObject *o); 将结构序列 p 的索引 pos 处的字段设置为值 o。 与 PyTuple_SET_ITEM() 一样，它应该只用于填充全新的实例
        void PyStructSequence_SET_ITEM(PyObject *p, Py_ssize_t *pos, PyObject *o); PyStructSequence_SetItem() 的宏版本

        PyStructSequence_Desc说明了定义一个新的结构类型需要的元信息：
            name   const char *   结构序列类型的名称
            doc      const char *   指向要忽略类型的文档字符串或 NULL 的指针
            fields    PyStructSequence_Field *    指向以 NULL 结尾的数组的指针，其字段名称为新类型
            n_in_sequence     int   Python侧可见的字段数（如果用作元组）

            关于 PyStructSequence_Field 描述一个结构序列的字段。 当结构序列被建模为元组时，所有字段的类型都是 PyObject*
            PyStructSequence_Desc 的 fields 数组的索引确定了描述的是结构序列的哪个字段
                    name    const char *  字段的名称或NULL ，若要结束命名字段的列表，请设置为：PyStructSequence_UnnamedField 以保留未命名字段
                    doc       const char *   要忽略的字段docstring或NULL

            void PyStructSequence_InitType(PyTypeObject *type, PyStructSequence_Desc *desc);从*desc*就地初始化结构序列类型*type*
            int PyStructSequence_InitType2(PyTypeObject *type, PyStructSequence_Desc *desc); 与 PyStructSequence_InitType 相同，但成功时返回 0 ，失败时返回 -1
            char* PyStructSequence_UnnamedField; 字段名的特殊值将保持未命名状态

    */

    Py_Initialize();
    PyStructSequence_Desc desc  ; // 创建结构体序列
    desc.name = PyUnicode_AsUTF8(PyUnicode_FromString("person")); // person has sex,age,name and salary field
    desc.doc = NULL;

    PyStructSequence_Field name; // 创建结构体字段
    name.doc = NULL; name.name=PyUnicode_AsUTF8(PyUnicode_FromString("name"));
    PyStructSequence_Field sex;
    sex.doc = NULL; sex.name = PyUnicode_AsUTF8(PyUnicode_FromString("sex"));
    PyStructSequence_Field age;
    age.doc = NULL; age.name = PyUnicode_AsUTF8(PyUnicode_FromString("age"));
    PyStructSequence_Field salary;
    salary.doc = NULL; salary.name = PyUnicode_AsUTF8(PyUnicode_FromString("salary"));
    //PyStructSequence_Field  myfields[5];
    PyStructSequence_Field * myfields[5] = {&name,&sex,&age,&salary,NULL};
    //myfields[0] = name; myfields[1] = sex; myfields[2] = age; myfields[3] = salary; myfields[4]=NULL;
    //char * unname = PyStructSequence_UnnamedField;
    desc.fields = myfields[0];
    desc.n_in_sequence = 5;

     PyTypeObject* mytype = PyStructSequence_NewType(&desc); // 构造类型,这里导致了异常不知道啥原因,不会用
//     int ok = PyStructSequence_InitType2(mytype, &desc);
//     printf("ok = %d\n",ok);
    Q_UNUSED(mytype);
     Py_Finalize();
}
#endif // PYTEST_SPEC_DATATYPE_API_H
