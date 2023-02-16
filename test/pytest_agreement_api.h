#ifndef PYTEST_AGREEMENT_API_H
#define PYTEST_AGREEMENT_API_H

#include <Python.h>
#include <QList>
#include <QDebug>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// 对象协议、数字协议、序列协议、映射协议、迭代器协议
void test_obj_agreement()
{
    /*
        主要是调用函数、调用可调用对象和调用成员函数3种函数,可调用来自python文件定义的函数
        PyObject_GetAttrString可以获取来自PyImport_ImportModule导入py文件内部定义的某个函数

        PyObject* PyObject_Call(PyObject *callable, PyObject *args, PyObject *kwargs);
        调用一个可调用的Python对象 callable，附带由元组 args 所给出的参数，以及由字典 kwargs 所给出的关键字参数
        等价于 Python 表达式 callable(*args, **kwargs)
        args不能为NULL，如果不需要参数，请使用空元组，kwargs可以为NULL

        PyObject* PyObject_CallObject(PyObject *callable, PyObject *args);
        调用可调用的Python对象，参数由元组参数给定。如果不需要参数，则args可以为NULL
        成功时返回调用结果，或引发异常，失败时返回NULL
        等价于 Python 表达式 callable(*args)

        PyObject* PyObject_CallFunction(PyObject *callable, const char *format, ...);
        使用可变数量的C参数调用可调用的Python对象
        C参数使用Py_BuildValue()样式的格式字符串描述。format可以为NULL，表示未提供任何参数
        成功时返回调用结果，或引发异常，失败时返回NULL
        等价于 Python 表达式 callable(*args)
        请注意，如果只传递PyObject*参数，则PyObject_CallFunctionObjArgs()是一种更快的替代方法

        PyObject* PyObject_CallFunctionObjArgs(PyObject *callable, ..., NULL);
        使用可变数量的PyObject*参数调用可调用的Python对象。参数是以可变数量的参数形式提供的，后跟NULL
        成功时返回调用结果，或引发异常，失败时返回NULL
        和Python表达式callable(arg1, arg2, ...)是一样的

        PyObject* PyObject_CallMethod(PyObject *obj, const char *name, const char *format, ...);
        使用可变数量的C参数调用对象obj的名为name的方法。C参数由应生成元组的Py_BuildValue()格式字符串描述
        args可以为NULL，表示未提供任何参数。成功时返回调用结果，或引发异常，失败时返回NULL
        和Python表达式obj.name(arg1, arg2, ...)是一样的
        请注意，如果只传递PyObject*参数，则PyObject_CallMethodObjArgs()是一种更快的替代方法

        PyObject* PyObject_CallMethodObjArgs(PyObject *obj, PyObject *name, ..., NULL);
        调用Python对象obj的方法，其中该方法的名称以名称中的Python字符串对象的形式给出
        使用可变数量的PyObject*参数调用它。参数是以可变数量的参数形式提供的，后跟NULL
        成功时返回调用结果，或引发异常，失败时返回NULL

        int PyObject_HasAttr(PyObject *o, PyObject *attr_name);
        int PyObject_HasAttrString(PyObject *o, const char *attr_name);
        是否存在指定属性 相当于 Python 表达式 hasattr(o, attr_name)

        PyObject* PyObject_GetAttr(PyObject *o, PyObject *attr_name); 成功时返回属性值,失败则返回 NULL
        PyObject* PyObject_GetAttrString(PyObject *o, const char *attr_name);
        int PyObject_SetAttr(PyObject *o, PyObject *attr_name, PyObject *v); 失败返回-1,成功返回0
        int PyObject_SetAttrString(PyObject *o, const char *attr_name, PyObject *v);
        int PyObject_DelAttr(PyObject *o, PyObject *attr_name); 失败时返回 -1
        int PyObject_DelAttrString(PyObject *o, const char *attr_name);
        获取、设置和删除指定的属性。这相当于Python表达式o.attr_name,o.attr_name = v,del o.attr_name

        PyObject* PyObject_Str(PyObject *o);
        计算对象 o 的字符串形式。 成功时返回字符串，失败时返回 NULL。 这相当于 Python 表达式 str(o)
        PyObject* PyObject_Bytes(PyObject *o);
        计算对象 o 的字节形式。失败时返回 NULL，成功时返回一个字节串对象。这相当于 o 不是整数时的 Python 表达式 bytes(o)

        int PyCallable_Check(PyObject *o);确定对象 o 是可调对象。如果对象是可调对象则返回 1 ，其他情况返回 0
        int PyObject_IsTrue(PyObject *o);对象 o 被认为是 true，则返回 1，否则返回 0。这相当于 Python 表达式 not not o。 失败则返回 -1
        int PyObject_Not(PyObject *o); 对象 o 被认为是 true，则返回 1，否则返回 0。这相当于 Python 表达式 not not o。 失败则返回 -1

        Py_ssize_t PyObject_Size(PyObject *o);如果对象 o 支持序列和映射协议，则返回序列长度,出错时返回-1。这等同于 Python 表达式 len(o)
        Py_ssize_t PyObject_Length(PyObject *o);

        PyObject* PyObject_GetItem(PyObject *o, PyObject *key);返回对象 key 对应的 o 元素，或在失败时返回 NULL。这等同于 Python 表达式 o[key]
        int PyObject_SetItem(PyObject *o, PyObject *key, PyObject *v);将对象键映射到值v。引发异常并在失败时返回-1；成功时返回0。这相当于Python语句o[key]=v
        int PyObject_DelItem(PyObject *o, PyObject *key);从对象 o 中移除对象 key 的映射。失败时返回 -1。 这相当于 Python 语句 del o[key]

        PyObject* PyObject_Dir(PyObject *o);相当于 Python 表达式 dir(o)，返回一个（可能为空）适合对象参数的字符串列表，如果出错则返回 NULL
        PyObject* PyObject_GetIter(PyObject *o);等同于 Python 表达式 iter(o)。为对象参数返回一个新的迭代器，如果该对象已经是一个迭代器，则返回对象本身。如果对象不能被迭代，会引发 TypeError ，并返回 NULL

    */

    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../pytest')");
    PyObject* pModule = PyImport_ImportModule("test_function");
    printf("test_function.py is exist? %d\n",PyObject_IsTrue(pModule));
    printf("test_function.py has func 'nvoid_nvoid_it'? %d\n",PyObject_HasAttrString(pModule,"nvoid_nvoid_it"));

    PyObject* strlist = PyObject_Dir(pModule);
    printf("strlist is seq? %d\n",PySequence_Check(strlist));
    printf("strlist's len = ? %I64d\n",PyObject_Length(strlist)); // 长度20
    PyObject* list_iter =  PyObject_GetIter(strlist);
    QList <const char *> strlist_ret;
    while (1) {
      PyObject *next = PyIter_Next(list_iter); // 这个函数是迭代器协议中的
      if (!next)break;
      //printf("is str? %d\n",PyUnicode_Check(next));
      strlist_ret.append(PyUnicode_AsUTF8(next)); // 每次迭代的项本身是double类型要转换
    }
    // strlist =  (List, Tuple, __builtins__, __cached__, __doc__, __file__, __loader__, __name__, __package__, __spec__,
    // np, nvoid_nvoid_id, nvoid_nvoid_if, nvoid_nvoid_il, nvoid_nvoid_it, nvoid_nvoid_ti, nvoid_nvoid_tii, nvoid_void, void_nvoid, void_void)
    qDebug()<<"strlist = "<<strlist_ret; // dir返回具有的属性,这些属性就是函数名称和一些魔法方法

    PyObject* nvoid_nvoid_it= PyObject_GetAttrString(pModule,"nvoid_nvoid_it");
    PyObject* nvoid_nvoid_it_ret = PyObject_CallFunction(nvoid_nvoid_it,"i",5);
    //double nvoid_nvoid_it_ret1 ; int nvoid_nvoid_it_ret2;
    //PyArg_Parse(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret1,&nvoid_nvoid_it_ret2); // PyArg_Parse可能解析不了元组
    //printf("nvoid_nvoid(int)->tuple = (%.15lf,%d)\n",nvoid_nvoid_it_ret1,nvoid_nvoid_it_ret2); // 第一个值是准确的,但是第2个值不正确
    double  nvoid_nvoid_it_ret3 = 1.2;int nvoid_nvoid_it_ret4 = 11, ok;
    ok = PyArg_ParseTuple(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret3,&nvoid_nvoid_it_ret4);
    printf("ok = %d nvoid_nvoid(int)->tuple = (%.15f,%d)\n",ok,nvoid_nvoid_it_ret3,nvoid_nvoid_it_ret4);//用PyArg_ParseTuple可以解析正确
    // 使用PyObject_CallObject依然可以
    nvoid_nvoid_it_ret = PyObject_CallObject(nvoid_nvoid_it, Py_BuildValue("(i)",5));
    ok = PyArg_ParseTuple(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret3,&nvoid_nvoid_it_ret4);
    printf("ok = %d nvoid_nvoid(int)->tuple = (%.15f,%d)\n",ok,nvoid_nvoid_it_ret3,nvoid_nvoid_it_ret4);


    // 下边测试成员函数的调用
    PyObject* Module = PyImport_ImportModule("test_class");
    printf("test_class.py has MyClass1 MyClass2? %d %d\n",PyObject_HasAttrString(Module,"MyClass1"),PyObject_HasAttrString(Module,"MyClass2"));
    PyObject* myclass1= PyObject_GetAttrString(Module,"MyClass1");
    PyObject_CallMethod(myclass1, "print_info","sssi",NULL,"chenbei","w",25); // self参数不传,用s代替,这个成员函数是有参数的
    //PyObject_CallMethodObjArgs(myclass1, PyUnicode_FromString("print_info"),"ssi","chenbei","w",25,NULL); // 这个不行出现异常

    // MyClass2的成员函数虽然没有参数,但是构造函数需要参数
    PyObject* myclass2= PyObject_GetAttrString(Module,"MyClass2");
    //PyObject_CallMethod(myclass2, "print_info","sssi",NULL,"wangxiawei","m",40);  // 不异常,但是不起作用
    PyObject_CallMethod(myclass2, "__init__","sssi",NULL,"wangxiawei","m",40); // 试图调用初始化函数来给构造函数赋值,没有结果说明不行
    PyObject_CallMethod(myclass2, "print_info","s",NULL);

    // 所以C++应该不支持在这里实例化1个类对象,直接在Python实例化可以,然后可以调用py文件定义的类实例方法
    printf("test_class.py has global_m? %d\n",PyObject_HasAttrString(Module,"global_m"));
    PyObject* global_m= PyObject_GetAttrString(Module,"global_m");
    PyObject_CallMethodObjArgs(global_m,PyUnicode_FromString("print_info"),NULL); // 必须跟着NULL否则异常

    Py_Finalize();
}

void test_digit_agreement()
{
    /*
        int PyNumber_Check(PyObject *o); 是否为数字,真返回1
        PyObject* PyNumber_Add(PyObject *o1, PyObject *o2) ; 加减乘

        PyObject* PyNumber_Subtract(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_Multiply(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_MatrixMultiply(PyObject *o1, PyObject *o2); 矩阵乘 o1 @ o2
        PyObject* PyNumber_FloorDivide(PyObject *o1, PyObject *o2); o1除o2 的向下取整
        PyObject* PyNumber_TrueDivide(PyObject *o1, PyObject *o2); 返回o1除o2 的一个合理的近似值
        PyObject* PyNumber_Remainder(PyObject *o1, PyObject *o2);返回 o1 除以 o2 得到的余数 o1 % o2
        PyObject* PyNumber_Divmod(PyObject *o1, PyObject *o2);等价divmod(o1, o2)
        PyObject* PyNumber_Power(PyObject *o1, PyObject *o2, PyObject *o3);pow(o1, o2, o3),要忽略 o3则需传入 Py_None
        PyObject* PyNumber_Negative(PyObject *o);返回-o
        PyObject* PyNumber_Positive(PyObject *o);返回 +o
        PyObject* PyNumber_Absolute(PyObject *o);返回绝对值 等价 abs(o)
        PyObject* PyNumber_Invert(PyObject *o) ;返回 o 的按位取反后的结果，等价 ~o
        PyObject* PyNumber_Lshift(PyObject *o1, PyObject *o2)返回 o1 左移 o2 个比特后的结果 等价 o1 << o2
        PyObject* PyNumber_Rshift(PyObject *o1, PyObject *o2); 等价 o1 >> o2
        PyObject* PyNumber_And(PyObject *o1, PyObject *o2);等价 o1 & o2
        PyObject* PyNumber_Xor(PyObject *o1, PyObject *o2);按位异或 等价 o1 ^o2
        PyObject* PyNumber_Or(PyObject *o1, PyObject *o2);按位或 等价 o1 | o2

        PyObject* PyNumber_InPlaceAdd(PyObject *o1, PyObject *o2); 加法,区别直接加给o1 其他同理
        PyObject* PyNumber_InPlaceSubtract(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceMultiply(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceMatrixMultiply(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceFloorDivide(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceTrueDivide(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceRemainder(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlacePower(PyObject *o1, PyObject *o2, PyObject *o3);
        PyObject* PyNumber_InPlaceLshift(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceRshift(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceAnd(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceXor(PyObject *o1, PyObject *o2);
        PyObject* PyNumber_InPlaceOr(PyObject *o1, PyObject *o2);

        PyObject* PyNumber_Long(PyObject *o); 等价于 Python 表达式 int(o)
        PyObject* PyNumber_Float(PyObject *o);等价于 Python 表达式 float(o)
        PyObject* PyNumber_Index(PyObject *o)成功时返回 o 转换为 Python int 类型后的结果，失败时返回 NULL 并引发 TypeError 异常
        PyObject* PyNumber_ToBase(PyObject *n, int base)返回整数 n 转换成以 base 为基数的字符串结果。base = 2，8，10 或者 16 。对于基数 2，8，或 16
        如果 n 不是 Python 中的整数 int 类型，就先通过 PyNumber_Index() 将它转换成整数类型
        Py_ssize_t PyNumber_AsSsize_t(PyObject *o, PyObject *exc);如果 o 是一个整数类型的解释型，返回 o 转换成一个 Py_ssize_t 值项后的结果
        int PyIndex_Check(PyObject *o);如果 o 是一个索引整数（存有 nb_index 位置并有 tp_as_number 填入其中）则返回 1，否则返回 0

    */

    Py_Initialize();
    PyObject * v1 = Py_BuildValue("i",1);PyObject * v2 = Py_BuildValue("i",2);
    qDebug()<<"is digit? "<<PyNumber_Check(v1);
    qDebug()<<"v1+v2="<<PyLong_AsLong(PyNumber_Add(v1,v2));
    qDebug()<<"v3="<<PyLong_AsLong(PyNumber_InPlaceAdd(v1, v2)); // 好像没太大区别
    PyObject* f = PyNumber_Float(PyNumber_InPlaceAdd(v1, v2));
    qDebug()<<"f is float? "<<(PyFloat_Check(f)==1);

    PyObject* hex =  PyNumber_ToBase(Py_BuildValue("i",2022),16);
    qDebug()<<"2022's hex = "<<(PyUnicode_AsUTF8(hex));
    Py_Finalize();
}

void test_seq_agreement()
{
        /*
            int PySequence_Check(PyObject *o);对象提供序列协议，函数返回 1 请注意它将为具有 __getitem__() 方法的 Python 类返回 1

            Py_ssize_t PySequence_Size(PyObject *o);
            Py_ssize_t PySequence_Length(PyObject *o); 相当于Python的len(o)表达式

            PyObject* PySequence_Concat(PyObject *o1, PyObject *o2)成功时返回 o1 和 o2 的拼接 等价于 Python 表达式 o1 + o2
            PyObject* PySequence_InPlaceConcat(PyObject *o1, PyObject *o2)等价于 Python 表达式 o1 += o2
            PyObject* PySequence_Repeat(PyObject *o, Py_ssize_t count)返回序列对象 o 重复 count 次的结果，等价于 Python 表达式 o * count
            PyObject* PySequence_InPlaceRepeat(PyObject *o, Py_ssize_t count)这等价于 Python 表达式 o *= count

            PyObject* PySequence_GetItem(PyObject *o, Py_ssize_t i)返回 o 中的第 i 号元素，这等价于 Python 表达式 o[i]
            int PySequence_SetItem(PyObject *o, Py_ssize_t i, PyObject *v)将对象 v 赋值给 o 的第 i 号元素。相当于 Python 语句 o[i] = v
            int PySequence_DelItem(PyObject *o, Py_ssize_t i)删除对象 o 的第 i 号元素相当于 Python 语句 del o[i]

            PyObject* PySequence_GetSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2)返回序列对象 o 的 i1 到 i2 的切片，这等价于 Python 表达式 o[i1:i2]
            int PySequence_SetSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2, PyObject *v)相当于 Python 语句 o[i1:i2] = v
            int PySequence_DelSlice(PyObject *o, Py_ssize_t i1, Py_ssize_t i2)相当于 Python 语句 del o[i1:i2]

            Py_ssize_t PySequence_Count(PyObject *o, PyObject *value)返回 value 在 o 中出现的次数，即返回使得 o[key] == value 的键的数量 相当于 Python 表达式 o.count(value)

            int PySequence_Contains(PyObject *o, PyObject *value); 是否包含 value。 如果包含返回 1否则返回 0。 出错时返回 -1。 相当于 Python 表达式 value in o

            Py_ssize_t PySequence_Index(PyObject *o, PyObject *value);返回第一个索引*i*,其中 o[i] == value.出错时,返回 -1.相当于Python的o.index(value)表达式.

            PyObject* PySequence_List(PyObject *o)返回一个列表对象，其内容与序列或可迭代对象 o 相同 这等价于 Python 表达式 list(o)
            PyObject* PySequence_Tuple(PyObject *o); 返回一个元组对象，等价于 Python 表达式 tuple(o)

        */

        Py_Initialize();

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('../pytest')");
        PyObject* pModule = PyImport_ImportModule("test_function");
        PyObject* nvoid_nvoid_il= PyObject_GetAttrString(pModule,"nvoid_nvoid_il");
        PyObject* nvoid_nvoid_il_ret= PyObject_CallFunction(nvoid_nvoid_il,"i",3);
        printf("nvoid_nvoid_il_ret is list? %d is tuple? %d is callable? %d is seq? %d\n",PyList_Check(nvoid_nvoid_il_ret),
                PyTuple_Check(nvoid_nvoid_il_ret),PyCallable_Check(nvoid_nvoid_il_ret),PySequence_Check(nvoid_nvoid_il_ret));
        // 从结果来看 ,python返回的[]对象不是list、tuple和callable，而是seq

        qDebug()<<"old length = "<<PySequence_Length(nvoid_nvoid_il_ret);

        double d1,d2,d3;
        // 一个方法是把返回的list对象利用PySequence_Tuple()转成tuple对象即可,再用PyArg_ParseTuple解析
        // 或者最笨的方法PySequence_GetItem获取每个项,再转为double类型即可
        PyObject* nvoid_nvoid_il_ret_tuple = PySequence_Tuple(nvoid_nvoid_il_ret); // 等价于tuple()
        printf("list to tuple successful? %d\n",nvoid_nvoid_il_ret_tuple!=NULL);
        int ok = PyArg_ParseTuple(nvoid_nvoid_il_ret_tuple,"ddd",&d1,&d2,&d3);
        printf("ok = %d nvoid_nvoid(int)->list = [%.8lf,%.8lf,%.8lf]\n",ok,d1,d2,d3);
        double d4 = PyFloat_AsDouble(PySequence_GetItem(nvoid_nvoid_il_ret,2));
        printf("nvoid_nvoid(int)->list list[2] = %.8lf\n",d4); // 直接获取项再转换也是ok的

        PyObject* seq = PySequence_Repeat(nvoid_nvoid_il_ret, 3); // 重复三次,cong'jie'guo'lai'kan 似乎没有成功
        qDebug()<<"After repeat length = "<<PySequence_Length(seq) <<"  "<<PyFloat_AsDouble(PySequence_GetItem(seq,7));
        Py_Finalize();
}

void test_map_agreement()
{
    /*
    int PyMapping_Check(PyObject *o);如果对象提供映射协议或支持切片则返回 1 注意它将为具有 __getitem__() 方法的 Python 类返回 1

    Py_ssize_t PyMapping_Size(PyObject *o)
    Py_ssize_t PyMapping_Length(PyObject *o);相当于 Python 表达式 len(o)

    PyObject* PyMapping_GetItemString(PyObject *o, const char *key) 返回 o 中对应于字符串 key 的元素，相当于 Python 表达式 o[key]
    int PyMapping_SetItemString(PyObject *o, const char *key, PyObject *v)相当于 Python 语句 o[key] = v
    int PyMapping_DelItem(PyObject *o, PyObject *key);相当于 Python 语句 del o[key]。 这是 PyObject_DelItem() 的一个别名
    int PyMapping_DelItemString(PyObject *o, const char *key);相当于 Python 语句 del o[key]

    int PyMapping_HasKey(PyObject *o, PyObject *key);如果映射对象具有键 key 则返回 1，相当于 Python 表达式 key in
    int PyMapping_HasKeyString(PyObject *o, const char *key);如果映射对象具有键 key 则返回 1，相当于 Python 表达式 key in o

    PyObject* PyMapping_Keys(PyObject *o); 成功时，返回对象 o 中的键的列表
    PyObject* PyMapping_Values(PyObject *o) ;成功时，返回对象 o 中的值的列表
    PyObject* PyMapping_Items(PyObject *o);成功时，返回对象 o 中条目的列表,每个条目是一个包含键值对的元组
    */
    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../pytest')");
    PyObject* pModule = PyImport_ImportModule("test_function");
    PyObject* func= PyObject_GetAttrString(pModule,"nvoid_nvoid_id");
    PyObject* ret = PyObject_CallFunction(func,"i",10);
    PyObject * key1 = PyDict_GetItemString(ret, "x");// 预期返回的字典key值是x和y
    PyObject * key2 = PyDict_GetItemString(ret, "y");
    double dct1,dct2;
    PyArg_Parse(key1,"d",&dct1); // 普通的值可以用PyArg_Parse解析
    PyArg_Parse(key2,"d",&dct2);
    printf("func(int)->dict = {'x': %.8lf ; 'y': %.8lf}\n",dct1,dct2);

    qDebug()<<"dict has key 'x'? "<<PyMapping_HasKeyString(ret, "x");
    PyObject* itemlist =  PyMapping_Items(ret);
    qDebug()<<"itemlist is seq? "<<PySequence_Check(itemlist);
    qDebug()<<"len(itemlist) = "<<PyMapping_Size(itemlist);
    qDebug()<<"tuple[1] = "<<PyFloat_AsDouble(PyMapping_GetItemString(ret,"y")); // 获取ret可以但是获取itemlist不行
    qDebug()<<"itemlist[1][1] = "<<PyFloat_AsDouble(PyTuple_GetItem(PySequence_GetItem(itemlist,1),1)); // 返回的是2个元组,第2个元素的第2个元素
    Py_Finalize();
}

void test_iter_agreement()
{
    /*
        int PyIter_Check(PyObject *o);返回 true ， 如果对象 o 支持迭代器协议的话
        PyObject* PyIter_Next(PyObject *o);返回迭代 o 的下一个值,对象必须是一个迭代器, 这个通过PyObject_GetIter获取对象的迭代器
        如果没有余下的值，则返回 NULL

    */
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../pytest')");
    PyObject* pModule = PyImport_ImportModule("test_function");
    PyObject* func= PyObject_GetAttrString(pModule,"nvoid_nvoid_il");
    PyObject* ret= PyObject_CallFunction(func,"i",3);
    PyObject *iter = PyObject_GetIter(ret); // seq是有迭代器的
    QList<double> vals; // 参考 https://www.coder.work/article/358631
   while (1) {
     PyObject *next = PyIter_Next(iter); // 这个函数是迭代器协议中的
     if (!next)break;
     vals.append(PyFloat_AsDouble(next)); // 每次迭代的项本身是double类型要转换
   }
   printf("func(int)->list = [%.8f,%.8f,%.8f]\n",vals[0],vals[1],vals[2]);

   Py_Finalize();
}
#endif // PYTEST_AGREEMENT_API_H
