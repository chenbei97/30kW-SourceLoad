#ifndef PYTEST_TOOL_API_H
#define PYTEST_TOOL_API_H

#include <Python.h>
#include <QDir>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// 工具层测试:导入模块+解析参数并构建值变量+字符串转换与格式化
// 其他的工具如操作系统实用程序、系统功能、过程控制、数据 marshal 操作支持、
// 反射、编解码器注册与支持功能等是不常用的不进行测试

// https://docs.python.org/zh-cn/3.7/c-api/import.html
// https://docs.python.org/zh-cn/3.7/c-api/arg.html
// https://docs.python.org/zh-cn/3.7/c-api/conversion.html

void test_import_module()
{
    /*
        PyObject* PyImport_ImportModule(const char *name); 使用绝对路径 PyImport_ImportModuleEx()的简化接口
        PyObject* PyImport_Import(PyObject *name);
        PyObject* PyImport_GetModule(PyObject *name)
        返回具有给定名称的已导入模块。如果模块尚未导入，则返回NULL，但不设置错误。返回NULL并在查找失败时设置错误

        PyObject* PyImport_ImportModuleEx(const char *name,
                        PyObject *globals, PyObject *locals, PyObject *fromlist);
        这个就是python的内置函数def __import__(name, globals=None, locals=None, fromlist=(), level=0);但是没有level参数
        name: 要导入的文件名称
        globals: 仅用于确定上下文,它们不会被修改
        locals: 参数未使用
        fromlist: 模拟"from name import ..." 或 "import name"
        level: 0=绝对导入(默认), 正数是相对于当前模块要搜索的父目录数
        return: 元组列表
        注意: 对于表达式__import__('a.B'，…),如果fromlist为空则返回包a,不为空时则返回其子模块B
        __import__('tips',None,None,(['algolist','algotuple']),0) 从tips文件夹导入2个文件
        __import__('tips.algolist',None,None,(),0) 指定导入algolist文件

        PyObject* PyImport_ImportModuleLevelObject(PyObject *name,
                    PyObject *globals, PyObject *locals, PyObject *fromlist, int level); // 区别是name是PyObject *,并可以设置level
        PyObject* PyImport_ImportModuleLevel(const char *name, PyObject *globals, PyObject *locals, PyObject *fromlist, int level); 区别是传入const char*


        其它不常用:
        PyObject* PyImport_ReloadModule(PyObject *m);
        重新加载模块。返回对重新加载的模块的新引用，或者返回NULL，并在失败时设置异常（在这种情况下，模块仍然存在）

        PyObject* PyImport_GetModuleDict(); 返回用于模块管理的字典（也称为sys.modules）。请注意，这是每个解释器的变量
*/
    Py_Initialize();

    PyRun_SimpleString("import os,sys");
    PyRun_SimpleString("os.chdir('../pytest')"); // 改变当前工作路径
    PyRun_SimpleString("print(os.getcwd())"); // 30kW-SourceLoad-abridged\pytest
    printf("cur path = %s\n",QDir::currentPath().toStdString().c_str()); // 30kW-SourceLoad-abridged\pytest
    //PyRun_SimpleString("print(sys.path)");
    PyRun_SimpleString("sys.path.append('../pytest')"); // 这行还是要加的
    //PyRun_SimpleString("print(sys.path)");
    printf("import module ret = %d %d\n",PyImport_ImportModule("test_python")!=NULL,PyImport_Import(Py_BuildValue("s","test_python"))!=NULL);
    printf("test_python has import? %d\n",PyImport_GetModule(Py_BuildValue("s","test_python"))!=NULL);

    // 这个要执行成功,必须在debug目录下创建importtest文件夹,然后再创建2个py文件,看起来切换工作目录没有用
    PyImport_ImportModuleEx("importtest",NULL,NULL,Py_BuildValue("[s,s]","a","b")); // ①
    PyImport_ImportModuleEx("importtest.c",NULL,NULL,NULL); // ② 两种导入方式都可
    printf("__import__ module ret = %d\n",PyImport_ImportModuleEx("importtest",NULL,NULL,NULL)!=NULL); // 只导入包也可以

    int level = 0; //  1会出现KeyError: '__name__' not in globals
    // 0的话,如果是pytest就会找不到,看起来切换工作目录没有用
    PyImport_ImportModuleLevelObject(Py_BuildValue("s","importtest"),NULL,NULL,Py_BuildValue("[s]","d"),level);
    PyImport_ImportModuleLevel("importtest" ,NULL,NULL,Py_BuildValue("[s]","e"),level);
    PyImport_ImportModuleLevel("importtest.f" ,NULL,NULL,NULL,level);

    Py_Finalize();
}


void test_analytic_arg()
{
        /*
                https://docs.python.org/zh-cn/3.7/c-api/arg.html#c.PyArg_ParseTuple

                创建你自己的扩展函数和方法时,主要涉及3个函数PyArg_ParseTuple(),PyArg_ParseTupleAndKeywords()以及 PyArg_Parse()
                注意任何由调用者提供的Python对象引用是 借来的 引用；不要递减它们的引用计数
                为了转换成功，arg 对象必须匹配格式并且格式必须用尽。成功的话，PyArg_Parse*() 函数返回true，反之它们返回false并且引发一个合适的异常
                当 PyArg_Parse*() 函数因为某一个格式单元转化失败而失败时，对应的以及后续的格式单元地址内的变量都不会被使用

                左边是返回的Python参数类型,右边是会被转换的C类型,总结如下：

                数字类型：
                b (int) [unsigned char] 非负Python整型
                B (int) [unsigned char] Python整型
                h (int) [short] Python整型
                H (int) [unsigned short] Python整型
                i (int) [int] Python整型
                I (int) [unsigned int] Python整型
                l (int) [long] Python整型
                L (int) [long long] Python整型
                k (int) [unsigned long] Python整型
                K (int) [unsigned long long] Python整型
                n (int) [Py_ssize_t] Python整型转化成一个C Py_ssize_t Python元大小类型
                c (bytes 或者 bytearray 长度为1) [char] Python字节类型(一个长度为1的 bytes 或者 bytearray 对象)
                C (str 长度为1) [int] Python字符(一个长度为1的 str 字符串对象)
                f (float) [float] Python浮点数
                d (float) [double] Python浮点数
                D (complex) [Py_complex] Python复数类型转化成一个C Py_complex Python复数类型
                p (bool) [int] 测试传入的值是否为真(一个布尔判断)并且将结果转化为相对应的C true/false整型值
                (items) (tuple) [matching-items] 对象必须是Python序列,它的长度是 items 中格式单元的数量,C参数必须对应 items 中每一个独立的格式单元

                字符串类型：
                s (str) [const char *]   将一个Unicode对象转换成一个指向字符串的C指针
                s* (str or bytes-like object) [Py_buffer] 接受Unicode对象也接受类字节类型对象,转换为调用者提供的 Py_buffer 结构
                s# (str, 只读 bytes-like object) [const char *, int or Py_ssize_t] 存储在两个C 变量中,第一个是指向C字符串的指针,第二个是它的长度
                S (bytes) [PyBytesObject *] 要求Python对象是一个 bytes 类型对象

                z (str or None) [const char *] 和s相同,区别是如果Python传入None,则C返回NULL
                z* (str, bytes-like object or None) [Py_buffer]
                z# (str, 只读 bytes-like object 或 None) [const char *, int or Py_ssize_t]

                y (read-only bytes-like object) [const char *] 区别是不接受Unicode对象
                y* (bytes-like object) [Py_buffer]
                y# (只读 bytes-like object) [const char *, int or Py_ssize_t]
                Y (bytearray) [PyByteArrayObject *] 要求Python对象是一个 bytearray 类型对象

                u (str) [const Py_UNICODE *] 将Unicode对象转化成指向一个以空终止的Unicode字符缓冲区的指针，必须传入 Py_UNICODE 指针变量地址
                u# (str) [const Py_UNICODE *, int 或 Py_ssize_t] 存储两个C变量，第一个指针指向一个Unicode数据缓存区，第二个是它的长度
                U (str) [PyObject *] 要求Python对象是一个Unicode对象 (不常用)

                Z (str 或 None) [const Py_UNICODE *] 和u类似,区别是Python对象可能为 None，此时Py_UNICODE 指针设置为 NULL
                Z# (str 或 None) [const Py_UNICODE *, int 或 Py_ssize_t]  (不常用)

               w* (可读写 bytes-like object) [Py_buffer] (非常不常用)
               es (str) [const char *encoding, char **buffer] (非常不常用)
               et (str, bytes or bytearray) [const char *encoding, char **buffer] (非常不常用)
               es# (str) [const char *encoding, char **buffer, int 或 Py_ssize_t *buffer_length] (非常不常用)
               et# (str, bytes 或 bytearray) [const char *encoding, char **buffer, int 或 Py_ssize_t *buffer_length] (非常不常用)

                其它类型:
                O (object) [PyObject *] 将 Python 对象（不进行任何转换）存储在 C 对象指针中
                O! (object) [typeobject, PyObject *] 将一个Python对象存入一个C指针。和 O 类似但需两个C参数：Python类型对象的地址和存储对象指针的C变量( PyObject* 变量)的地址
                O& (object) [converter, anything] 通过converter 函数将Python对象转换成一个C变量。需两个参数：一个函数和一个C变量的地址(任意类型的)，转化为 void * 类型
                例子: status = converter(object, address);
                object*是待转化的Python对象并且 *address 是传入 PyArg_Parse*() 函数的 void* 类型参数。返回的 status 是1代表转换成功，0代表转换失败

                特殊字符:
                格式化字符串中还有一些其他的字符具有特殊的涵义
                '|' : 表明Python参数列表中剩下的参数都是可选的。PyArg_ParseTuple(args, "s|si", &file, &mode, &bufsize);f('spam'),f('spam', 'w'),f('spam', 'wb', 100000)
                '$': 仅用于PyArg_ParseTupleAndKeywords(),表明Python参数列表中剩下的参数都是强制关键字参数。当前，所有强制关键字参数都必须也是可选参数，所以格式化字符串中 | 必须一直在 $ 前面
                ':' :格式单元的列表结束标志,冒号后的字符串被用来作为错误消息中的函数名(PyArg_ParseTuple() 函数引发的“关联值”异常)
                ';': 格式单元的列表结束标志,分号后的字符串被用来作为错误消息取代默认的错误消息,分号和冒号相互排斥

                解析API函数说明：
                int PyArg_ParseTuple(PyObject *args, const char *format, ...);
                解析一个函数的参数,表达式中的参数按参数位置顺序存入局部变量中,成功返回true

                int PyArg_ParseTupleAndKeywords(PyObject *args, PyObject *kw, const char *format, char *keywords[], ...);
                将位置参数和关键字参数同时转换为局部变量的函数的参数. keywords 参数是关键字参数名称的 NULL 终止数组. 空名称表示 positional-only parameters,成功时返回 true

                int PyArg_Parse(PyObject *args, const char *format, ...);
                函数被用来解析旧类型函数的参数列表,不被推荐用于新代码的参数解析,只是兼容以前的代码
        */
        Py_Initialize();

        PyObject * arglist1 = Py_BuildValue("(ii)",1,2);
        int a, b, ok;
        ok = PyArg_Parse(arglist1,"(ii)",&a,&b); // 简单类型可以使用PyArg_Parse
        printf("ok = %d, a = %d, b = %d\n",ok,a,b);

//        arglist1 = Py_BuildValue("((ii)(ii)) (ii)",1,2,3,4,5,6);
//        int arg11,arg12,arg13,arg14,arg15,arg16;
        //ok = PyArg_Parse(arglist1,"((ii)(ii)) (ii)",&arg11,&arg12,&arg13,&arg14,&arg15,&arg16);// old style getargs format uses new features 这里使用旧时风格就会出错
        //ok = PyArg_ParseTuple(arglist1,"((ii)(ii)) (ii)",&arg11,&arg12,&arg13,&arg14,&arg15,&arg16);// SystemError: argument 2 (impossible<bad format char>) 直接改为Tuple也不行,参数给定错误
        //printf("ok = %d, arg11 = %d, arg12 = %d, arg13 = %d, arg14 = %d, arg15 = %d, arg16=%d\n",ok,arg11,arg12,arg13,arg14,arg15,arg16);
        //        PyObject *arglist = Py_BuildValue("(i)", arg);
        //        PyObject *result = PyObject_CallObject(my_callback, arglist);

        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('../pytest')");
        PyObject* pModule = PyImport_ImportModule("test_function");

        // （1）调用无输入参数无返回值的函数
        PyObject* void_void= PyObject_GetAttrString(pModule,"void_void");
        PyObject_CallFunction(void_void,Q_NULLPTR);
        PyObject_CallFunctionObjArgs(void_void); // 如果不需要参数,这个是更快的选择

        // （2）调用无输入参数有返回值的函数（普通浮点数返回值）
        PyObject* nvoid_void= PyObject_GetAttrString(pModule,"nvoid_void");
        PyObject* nvoid_void_ret = PyObject_CallFunction(nvoid_void,NULL); // 这里不能使用PyObject_CallFunctionObjArgs否则解析不出来
        double nvoid_void_ret_val = 0.2;
        ok = PyArg_Parse(nvoid_void_ret,"d",&nvoid_void_ret_val); // 把python的float变为double类型传给接收者,简单类型使用PyArg_Parse解析
        printf("ok = %d, nvoid_void's ret = %lf\n",ok,nvoid_void_ret_val);

         // （3）调用有输入参数无返回值的函数（普通整型参数）
        // （3.1）可以直接PyObject_CallFunction传递参数
        PyObject* void_nvoid= PyObject_GetAttrString(pModule,"void_nvoid");
        PyObject_CallFunction(void_nvoid,"i",5);// 计算5×5的,传递参数size=5
        // （3.2）可以使用PyObject_CallObject,但这个要求必须传递元组
        PyObject_CallObject(void_nvoid, Py_BuildValue("(i)",5)); // 不能使用Py_BuildValue("i",5)

        // （4）调用有输入参数有返回值的函数
        // （4.1）int ->float
        PyObject* nvoid_nvoid_if= PyObject_GetAttrString(pModule,"nvoid_nvoid_if");
        PyObject* nvoid_nvoid_if_ret= PyObject_CallFunction(nvoid_nvoid_if,"i",5); // 输入int=5
        float nvoid_nvoid_if_ret_val = 0.1f;
        PyArg_Parse(nvoid_nvoid_if_ret,"f",&nvoid_nvoid_if_ret_val);
        printf("nvoid_nvoid(int)->float =>float = %.15f\n",nvoid_nvoid_if_ret_val);
        // 同理使用PyObject_CallObject也可以,但是要传递元组参数
        nvoid_nvoid_if_ret = PyObject_CallObject(nvoid_nvoid_if, Py_BuildValue("(i)",5));
        double nvoid_nvoid_if_ret_val_ = 0.;
        PyArg_Parse(nvoid_nvoid_if_ret,"d",&nvoid_nvoid_if_ret_val_);
        printf("nvoid_nvoid(int)->float =>double = %.15lf\n",nvoid_nvoid_if_ret_val_);

        // （4.2） int->str
        PyObject* nvoid_nvoid_is= PyObject_GetAttrString(pModule,"nvoid_nvoid_is");
        PyObject* nvoid_nvoid_is_ret= PyObject_CallFunction(nvoid_nvoid_is,"i",5);
        char * nvoid_nvoid_is_ret_val;
        PyArg_Parse(nvoid_nvoid_is_ret,"s",&nvoid_nvoid_is_ret_val);
        printf("nvoid_nvoid(int)->str => ret = %s\n",nvoid_nvoid_is_ret_val); // 返回str类型可以转为const char*类型

        // （4.3）int ->tuple
        PyObject* nvoid_nvoid_it= PyObject_GetAttrString(pModule,"nvoid_nvoid_it");
        PyObject* nvoid_nvoid_it_ret = PyObject_CallFunction(nvoid_nvoid_it,"i",5);
        //double nvoid_nvoid_it_ret1 ; int nvoid_nvoid_it_ret2;
        //PyArg_Parse(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret1,&nvoid_nvoid_it_ret2); // PyArg_Parse可能解析不了元组
        //printf("nvoid_nvoid(int)->tuple = (%.15lf,%d)\n",nvoid_nvoid_it_ret1,nvoid_nvoid_it_ret2); // 第一个值是准确的,但是第2个值不正确
        double  nvoid_nvoid_it_ret3 = 1.2;int nvoid_nvoid_it_ret4 = 11;
        ok = PyArg_ParseTuple(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret3,&nvoid_nvoid_it_ret4);
        printf("ok = %d nvoid_nvoid(int)->tuple = (%.15f,%d)\n",ok,nvoid_nvoid_it_ret3,nvoid_nvoid_it_ret4);//用PyArg_ParseTuple可以解析正确
        // 使用PyObject_CallObject依然可以
        nvoid_nvoid_it_ret = PyObject_CallObject(nvoid_nvoid_it, Py_BuildValue("(i)",5));
        ok = PyArg_ParseTuple(nvoid_nvoid_it_ret,"di",&nvoid_nvoid_it_ret3,&nvoid_nvoid_it_ret4);
        printf("ok = %d nvoid_nvoid(int)->tuple = (%.15f,%d)\n",ok,nvoid_nvoid_it_ret3,nvoid_nvoid_it_ret4);

        // （4.4）int ->np.ndarray/list
        PyObject* nvoid_nvoid_il= PyObject_GetAttrString(pModule,"nvoid_nvoid_il");
        PyObject* nvoid_nvoid_il_ret= PyObject_CallFunction(nvoid_nvoid_il,"i",3);
        printf("nvoid_nvoid_il_ret is list? %d is tuple? %d is callable? %d is seq? %d\n",PyList_Check(nvoid_nvoid_il_ret),
                PyTuple_Check(nvoid_nvoid_il_ret),PyCallable_Check(nvoid_nvoid_il_ret),PySequence_Check(nvoid_nvoid_il_ret));
        // 从结果来看 ,python返回的[]对象不是list、tuple和callable，而是seq
        double d1,d2,d3;
        //PyArg_Parse(nvoid_nvoid_il_ret,"[ddd]",&d1,&d2,&d3); //PyArg_Parse肯定解析不出来
        //printf("%lf,  %lf,  %lf\n",d1,d2,d3);

        // ① 第一个方法,把返回的list对象利用PySequence_Tuple()转成tuple对象即可,再用PyArg_ParseTuple解析
        // 或者最笨的方法PySequence_GetItem获取每个项,再转为double类型即可
        PyObject* nvoid_nvoid_il_ret_tuple = PySequence_Tuple(nvoid_nvoid_il_ret); // 等价于tuple()
        printf("list to tuple successful? %d\n",nvoid_nvoid_il_ret_tuple!=NULL);
        ok = PyArg_ParseTuple(nvoid_nvoid_il_ret_tuple,"ddd",&d1,&d2,&d3);
        printf("ok = %d nvoid_nvoid(int)->list = [%.8lf,%.8lf,%.8lf]\n",ok,d1,d2,d3);
        double d4 = PyFloat_AsDouble(PySequence_GetItem(nvoid_nvoid_il_ret,2));
        printf("nvoid_nvoid(int)->list list[2] = %.8lf\n",d4); // 直接获取项再转换也是ok的

        // ② 第二个方法PyObject_GetIter获取迭代器,相当于python的iter(obj) 在对象协议中
         PyObject *nvoid_nvoid_iter = PyObject_GetIter(nvoid_nvoid_il_ret); // seq是有迭代器的
         QList<double> nvoid_nvoid_list; // 参考 https://www.coder.work/article/358631
        while (1) {
          PyObject *next = PyIter_Next(nvoid_nvoid_iter); // 这个函数是迭代器协议中的
          if (!next)break;
          nvoid_nvoid_list.append(PyFloat_AsDouble(next)); // 每次迭代的项本身是double类型要转换
        }
        printf("nvoid_nvoid(int)->list = [%.8f,%.8f,%.8f]\n",nvoid_nvoid_list[0],nvoid_nvoid_list[1],nvoid_nvoid_list[2]);
        // 使用PyObject_CallObject依然可以,这里不再验证

         //（4.5）int ->dict
        PyObject* nvoid_nvoid_id= PyObject_GetAttrString(pModule,"nvoid_nvoid_id");
        PyObject* nvoid_nvoid_id_ret = PyObject_CallFunction(nvoid_nvoid_id,"i",10);
        printf("nvoid_nvoid(int)->dict is dict? %id size = %I64d\n",PyDict_Check(nvoid_nvoid_id_ret),PyDict_Size(nvoid_nvoid_id_ret));

        // ① 既然是字典就可以直接解析字典,使用PyDict_GetItemString来获取返回的字典指定key对应的val
        PyObject * key1 = PyDict_GetItemString(nvoid_nvoid_id_ret, "x");// 预期返回的字典key值是x和y
        PyObject * key2 = PyDict_GetItemString(nvoid_nvoid_id_ret, "y");
        double dct1,dct2;
        PyArg_Parse(key1,"d",&dct1); // 普通的值可以用PyArg_Parse解析
        PyArg_Parse(key2,"d",&dct2);
        printf("nvoid_nvoid(int)->dict = {'x': %.8lf ; 'y': %.8lf}\n",dct1,dct2);

        // ② 使用PyArg_ParseTupleAndKeywords来解析
        // (PyObject *args, PyObject *kw, const char *format, char *keywords[], ...);
        // 这里返回的参数没有元组,args=NULL,kw的格式是sdsd,定义在keywords内
        // 参数 kwlist 是一个NULL结尾的字符串，定义了可以接受的参数名，并从左到右与format中各个变量对应
        // 参考https://www.codenong.com/35068114/ 和 https://zhidao.baidu.com/question/319332190.html 这个问题暂时没解决
        char cc1[] = {'s'};char cc2[] = {'d'};char cc3[] = {'s'};char cc4[] = {'d'};
        char *keywords[] = {cc1,cc2,cc3,cc4,NULL};// 一维指针数组
        char * kx = new char(1) ; char  *ky = new char(1) ;//这个是需要的否则程序异常
        double kxv , kyv;
        ok = PyArg_ParseTupleAndKeywords(NULL,nvoid_nvoid_id_ret,"sdsd",keywords,&kx,&kxv,&ky,&kyv);
        printf("nvoid_nvoid(int)->dict = {%s: %.8lf ; %s: %.8lf}\n",kx,kxv,ky,kyv); // 虽然可以执行但是这个结果不对
        // 使用PyObject_CallObject依然可以,这里不再验证

        //（4.6）str -> int
        PyObject* nvoid_nvoid_si= PyObject_GetAttrString(pModule,"nvoid_nvoid_si");
        const char  * mstr =    "A,男,25,硕士,电气,江西,19801279791\n"
                                            "B,男,25,硕士,电气,江西,19801279791\n"
                                            "C,男,25,硕士,电气,江西,19801279791\n"
                                            "D,男,25,硕士,电气,江西,19801279791\n"
                                            "E,男,25,硕士,电气,江西,19801279791\n"
                                            "F,男,25,硕士,电气,江西,19801279791\n"
                                            "G,男,25,硕士,电气,江西,19801279791\n"
                                            "H,男,25,硕士,电气,江西,19801279791\n"
                                            "K,男,25,硕士,电气,江西,19801279791\n"
                                            "L,男,25,硕士,电气,江西,19801279791\n"; // 280个字符,包含换行符
        PyObject* nvoid_nvoid_si_ret = PyObject_CallFunction(nvoid_nvoid_si,"s",mstr);
        int mstr_len = 0;
        ok = PyArg_Parse(nvoid_nvoid_si_ret,"i",&mstr_len); // 简单类型可以使用PyArg_Parse
        printf("ok = %d, mstr_len = %d\n",ok,mstr_len); //结果显示可以传递str参数

        // （4.7）tuple -> int
        PyObject *TupleArgs1 = PyTuple_New(5);
        PyTuple_SetItem(TupleArgs1, 0, Py_BuildValue("i", 1));
        PyTuple_SetItem(TupleArgs1, 1, Py_BuildValue("i", 2));
        PyTuple_SetItem(TupleArgs1, 2, Py_BuildValue("i", 3));
        PyTuple_SetItem(TupleArgs1, 3, Py_BuildValue("i", 4));
        PyTuple_SetItem(TupleArgs1, 4, Py_BuildValue("i", 5));
        PyObject *TupleArgs2 = Py_BuildValue("(iii)", 4,5,6);
        printf("TupleArgs1 and Tuple Args2's type = tuple? %d %d\n ",PyTuple_Check(TupleArgs1),PyTuple_Check(TupleArgs2));
        //PyObject * SeqArgs3 = PySequence_New(); // 不存在类似的创建序列的函数 ,不需要再进行传入序列参数测试

        printf("nvoid_nvoid_ti func is exist? %d\n",PyObject_HasAttrString(pModule,"nvoid_nvoid_ti"));
        printf("nvoid_nvoid_di func is exist? %d\n",PyObject_HasAttrString(pModule,"nvoid_nvoid_di"));
        printf("nvoid_nvoid_li func is exist? %d\n",PyObject_HasAttrString(pModule,"nvoid_nvoid_li"));
        //PyObject * func= PyObject_GetAttrString(pModule,"nvoid_nvoid_ti");
        //PyObject * func= PyObject_GetAttrString(pModule,"nvoid_nvoid_di");
        //PyObject * func= PyObject_GetAttrString(pModule,"nvoid_nvoid_li");
        // 从测试结果来看,如果python指定的参数类型是tuple,dict,list都不行

        // 4种调用方式都返回的是空指针,已经没有其它可用的调用方式,所以只能寻求更改
        // python的函数不要定义类型为tuple,拆成3个int,内部转为tuple类型计算
        // 估计dict/list可能是类似的不能传递 2022/11/11
        PyObject * func= PyObject_GetAttrString(pModule,"nvoid_nvoid_iii");

        PyObject* func_ret = PyObject_CallFunction(func,"(iii)",10,1,1);
        if (func_ret == Q_NULLPTR) printf("input tuple is failed1\n");
        else printf("(1) len((10,1,1)) + sum((10,1,1)) = %ld\n",PyLong_AsLong(func_ret));
        func_ret = PyObject_CallFunctionObjArgs(func,TupleArgs2,NULL);
        if (func_ret == Q_NULLPTR) printf("input tuple is failed2\n");
        else printf("(2) len((10,1,1)) + sum((10,1,1)) = %ld\n",PyLong_AsLong(func_ret));
        func_ret  = PyObject_CallObject(func,Py_BuildValue("iii",2,3,4));
        if (func_ret == Q_NULLPTR) printf("input tuple is failed3\n");
        else printf("(3) len((10,1,1)) + sum((10,1,1)) = %ld\n",PyLong_AsLong(func_ret));
        func_ret  = PyObject_Call(func,Py_BuildValue("iii",2,3,4),NULL);
        if (func_ret == Q_NULLPTR) printf("input tuple is failed4\n");
        else printf("(4) len((10,1,1)) + sum((10,1,1)) = %ld\n",PyLong_AsLong(func_ret));

         Py_Finalize();
}

void test_str_conversion()
{
        /*
                https://docs.python.org/zh-cn/3.7/c-api/conversion.html
                用于数字转换和格式化字符串输出的函数

                int PyOS_snprintf(char *str, size_t size, const char *format, ...);
                根据格式字符串 format 和额外参数，输出不超过 size 字节到 str
                包装器确保 str*[*size-1] 在返回时始终是 '\0' 需要满足 str != NULL , size > 0 和 format != NULL


                double PyOS_string_to_double(const char *s, char **endptr, PyObject *overflow_exception);
                将字符串 s 转换为 double 类型，失败时引发Python异常。接受的字符串的集合对应于被 Python 的 float() 构造函数接受的字符串的集合
                如果 endptr 是 NULL ，转换整个字符串,如果字符串不是浮点数的有效的表达方式,引发 ValueError 并且 返回 -1.0
                如果 overflow_exception 是 NULL 返回 Py_HUGE_VAL

                char* PyOS_double_to_string(double val, char format_code, int precision, int flags, int *ptype);
                转换 double val 为一个使用 format_code， precision 和 flags 的字符串
                format_code: 'e', 'E', 'f', 'F', 'g', 'G' 或者 'r'。对于 'r' , 提供的精度必须是0,'r' 格式码指定了标准函数 repr() 格式
                flags: 可以为零或者其他值 Py_DTSF_SIGN, Py_DTSF_ADD_DOT_0 或 Py_DTSF_ALT 或其组合
                            Py_DTSF_SIGN 表示总是在返回的字符串前附加一个符号字符，即使 val 为非负数
                            Py_DTSF_ADD_DOT_0 表示确保返回的字符串看起来不像是一个整数
                            Py_DTSF_ALT 表示应用 "替代的" 格式化规则。 相关细节请参阅 PyOS_snprintf() '#' 定义文档
               ptype 不为 NULL，则它指向的值将被设为 Py_DTST_FINITE, Py_DTST_INFINITE 或 Py_DTST_NAN 中的一个
                                    分别表示 val 是一个有限数字、无限数字或非数字
               返回值是一个指向包含转换后字符串的 buffer 的指针，如果转换失败则为 NULL。 调用方要负责调用 PyMem_Free() 来释放返回的字符串

               int PyOS_stricmp(const char *s1, const char *s2);字符串不区分大小写。该函数几乎与 strcmp() 的工作方式相同，只是它忽略了大小写
               int PyOS_strnicmp(const char *s1, const char *s2, Py_ssize_t  size); n指定要比较的最大字符数,几乎与 strncmp() 的工作方式相同，只是它忽略了大小写
        */
        Py_Initialize();
        char* str = new char;
        int ok = PyOS_snprintf(str,6,"%i",532722);
        printf("ok= %d ret = %s\n", ok,str);
        delete str; str = new char;
        ok = PyOS_snprintf(str,7,"%s","chenbei");
        printf("ok= %d ret = %s\n", ok,str);
        delete str; str = new char;
        ok = PyOS_snprintf(str,8,"%f",3.1415926535f);
        printf("ok= %d ret = %s\n", ok,str);

        double d = PyOS_string_to_double("6.284334534556",NULL,NULL);
        printf("double ret = %.15lf\n", d);
        const char * num = "345.34389239434";
        char * addr = new char;
        char ** paddr = &addr;
        d = PyOS_string_to_double(num,paddr,NULL);
        printf("double ret = %.15lf addr = %s\n", d,*paddr); // *paddr=addr没有东西


        char format_code = 'e'; // 'E' 3.453438923943400e+02 或 3.453438923943400E+02
        int precision = 15;
        int flags = 0;
        int ptype = Py_DTST_FINITE;
        char * mystr = PyOS_double_to_string(d, format_code, precision, flags, &ptype);
        printf("e/E char ret = %s\n",mystr);

        format_code = 'F'; // f和F没区别 都是345.343892394340003
        mystr = PyOS_double_to_string(d, format_code, precision, flags, &ptype);
        printf("f/F char ret = %s\n",mystr);

        format_code = 'G'; // g和G没区别 345.34389239434 用原本的精度恢复
        mystr = PyOS_double_to_string(d, format_code, precision, flags, &ptype);
        printf("g/G char ret = %s\n",mystr);

        format_code = 'r'; // 345.34389239434
        mystr = PyOS_double_to_string(d, format_code, 0, flags, &ptype);
        printf("r char ret = %s\n",mystr);

        flags = Py_DTSF_SIGN; // +345.34389239434
        mystr = PyOS_double_to_string(d, format_code, 0, flags, &ptype);
        printf("r char ret = %s\n",mystr);

        PyMem_Free(mystr);

        char str1[15];
        char str2[15];
        int ret;
        strcpy(str1, "abcdef");
        strcpy(str2, "ABCDEF");
        //ret = strcmp(str1, str2);
        ret = PyOS_stricmp(str1, str2);
        //ret = strncmp(str1, str2, 4);
        ret = PyOS_strnicmp(str1, str2, 4);
        if(ret < 0) printf("str1 < str2\n");
        else if(ret > 0) printf("str1 > str2\n");
        else printf("str1 = str2\n");
        Py_Finalize();
}
#endif // PYTEST_TOOL_API_H
