#ifndef CONCURRENT_TEST_H
#define CONCURRENT_TEST_H

#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QVector>
#include <QSet>
#include <QList>
#include <QTime>
#include <QThreadPool>
#include <QThread>
#include <QProgressDialog>
#include <QApplication>
#include <QMap>
#include  <random>
#include <tuple>
#include <functional>
#include "../core/list_files.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

using namespace QtConcurrent;

namespace ConCurrentTest {

    // 这是用于filter使用的测试数据
    QList<quint32> LessThanData = {2,12,9,20,33,8,16,3,15,7};
    QStringList LowerData = {"A","b","C","d","E","f","G","h","I","j"};
    QStringList ContainData = {"A","b","C","ad","aE","f","G","ah","I","ja"};
    QList<QString> filterData1 = {"3.17","陈","9","//","bei","10"};
    QVector<QVariant> filterData2 = {"3.17","陈",9,8.4,QString("//")};
    QList<QChar> BlockData = {'a','*','@','^','&','%','#','+','-','2','_','=','>','<','~','!','?','/','[','|','}','.',',','Z'};

   // 这是用于并行计算使用的过滤函数
    bool lessThan10(quint32 val){ qDebug()<<"lessThan10's current thread id = "<<QThread::currentThreadId(); return val < 10;  } // 普通函数形式
    class CLessThan10{ public:
            static bool lessThan10_1(quint32 val)  {qDebug()<<"lessThan10_1's current thread id = "<<QThread::currentThreadId();return val < 10;}// 静态成员函数形式(无this指针)
            bool lessThan10_2(quint32 val)  {qDebug()<<"lessThan10_2's current thread id = "<<QThread::currentThreadId();return val < 10;} // 普通成员函数形式
    };
    struct SLessThan10{bool operator()(quint32 val){ qDebug()<<"operator's current thread id = "<<QThread::currentThreadId();return val < 10;}};// 函数对象形式
    bool isLower(const QString& s) {qDebug()<<"isLower's current thread id = "<<QThread::currentThreadId();return s.toLower() == s;}
    bool lessThanThreshold(quint32 val, quint32 threshold) {qDebug()<<"lessThanThreshold's current thread id = "<<QThread::currentThreadId();return val < threshold;}; // 普通函数双参数
    bool strToint(const QString& var) {qDebug()<<"strToint's current thread id = "<<QThread::currentThreadId();return var.toInt() != 0;}
    bool varToint(QVariant var) {qDebug()<<"varToint's current thread id = "<<QThread::currentThreadId();return var.toInt() != 0;}
    bool lessThanChar(QChar c , QChar t) {qDebug()<<"lessThanChar's current thread id = "<<QThread::currentThreadId();return c < t;};
    void mergeCharUnicode(quint32& val, QChar c) {qDebug()<<"mergeCharUnicode's current thread id = "<<QThread::currentThreadId();val += c.unicode();};

    // 这是用于并行计算使用的reduce函数  void function(T &result, const U &intermediate)
    class Reduce {public: static void  reduce(QSet<quint32>& result_set, quint32 val){qDebug()<<"reduce's current thread id = "<<QThread::currentThreadId();result_set.insert(val);}};

    // 这是用于并行计算使用的测试函数
    void concurrent_filter_test()
    {
         // 原地过滤QFuture是void类型!

        qDebug()<<"原来的filterData1："<<filterData1;
         QFuture<void> f1 = QtConcurrent::filter(filterData1,strToint);
         // QFuture<void> f1_1 = QtConcurrent::filter(filterData1.begin(),filterData1.end(),strToint); // filter不支持迭代器使用
         f1.waitForFinished(); // 非阻塞类型,qDebug的速度可能很快,所以要等待完成打印是可以的
         qDebug()<<"原地过滤后的filterData1："<<filterData1; // 6个线程

         qDebug()<<"原来的filterData2："<<filterData2;
          //QFuture<void> f2  = QtConcurrent::filter(filterData2,varToint);
          //f2.waitForFinished(); // 非阻塞类型,qDebug的速度可能很快,所以要等待完成打印是可以的
          blockingFilter(filterData2,varToint); // 或者直接使用阻塞类型,完成以后才继续
          qDebug()<<"原地过滤后的filterData2："<<filterData2;  // 5个线程
    }

    void concurrent_filtered_test()
    {
        // （1）以quint32测试4类形式的函数
        QFuture<quint32> futureLessThan10 = QtConcurrent::filtered(LessThanData,lessThan10);
        qDebug()<<"filtered lessThan 10 [普通函数]："<<futureLessThan10.results();// 10个线程
        futureLessThan10 = QtConcurrent::filtered(LessThanData,[](quint32 val){
                qDebug()<<"lambda's current thread id = "<<QThread::currentThreadId();return val < 10;});
        qDebug()<<"filtered lessThan 10 [匿名函数]："<<futureLessThan10.results();// 10个线程
        futureLessThan10 = QtConcurrent::filtered(LessThanData,SLessThan10());
        qDebug()<<"filtered lessThan 10 [函数对象]："<<futureLessThan10.results();// 10个线程
        futureLessThan10 = QtConcurrent::filtered(LessThanData,CLessThan10::lessThan10_1);
        qDebug()<<"filtered lessThan 10 [静态成员函数]："<<futureLessThan10.results();// 10个线程
        futureLessThan10 = QtConcurrent::filtered(LessThanData,[](quint32 val){
                CLessThan10 obj; // 普通成员函数只能匿名函数中先构建出obj调用成员函数
                return obj.lessThan10_2(val); // 或者val不是基础类型而是类对象,那么就可以直接val的方法了
        });
        qDebug()<<"filtered lessThan 10 [普通成员函数]："<<futureLessThan10.results();// 10个线程
        futureLessThan10 = QtConcurrent::filtered(LessThanData.constBegin(),LessThanData.constEnd(),lessThan10);
        qDebug()<<"filtered lessThan 10 [使用迭代器]："<<futureLessThan10.results();// 10个线程
        // （2）测试QString类型的数据
        QFuture<QString> futureLower = QtConcurrent::filtered(LowerData,isLower);
        qDebug()<<"filtered isLower [普通函数]："<<futureLower.results();// 10个线程
        futureLower = QtConcurrent::filtered(LowerData,[](const QString& s){
            qDebug()<<"lambda's current thread id = "<<QThread::currentThreadId();
             return s == s.toLower(); // 这里toLower就是QString的成员函数但因为有this指针也就是双参所以不能直接调用
            // 但是可以借助匿名函数间接使用,但不能使用std::bind
        });
        qDebug()<<"filtered isLower [匿名函数]："<<futureLower.results();// 10个线程

        // （3）测试双参数函数: 其中普通成员函数的双参只能用匿名函数,上边已经测试过
        auto func = std::bind(ConCurrentTest::lessThanThreshold,std::placeholders::_1,15);
        QFuture<quint32> futureLessThanThre = QtConcurrent::filtered(LessThanData,func);
        qDebug()<<"filtered lessThanThreshold [双参普通函数-std::bind<15]："<<futureLessThanThre.results();// 10个线程
        futureLessThanThre = QtConcurrent::filtered(LessThanData,[](quint32 val){return lessThanThreshold(val,22);});
        qDebug()<<"filtered lessThanThreshold [双参成员函数-lambda<20]："<<futureLessThanThre.results();// 10个线程
        QFuture<QString> futureContain = QtConcurrent::filtered(ContainData,[](const QString&s){
            qDebug()<<"lambda's current thread id = "<<QThread::currentThreadId();
             return s.contains("a"); // 第1个参数this指针,第2个匿名函数来指定,返回带a的字符串
        });
        qDebug()<<"filtered contain [双参成员函数-lambda 'a']："<<futureContain.results();// 10个线程
    }

    void concurrent_filter_reduce_test()
    {
            QFuture<QSet<quint32>> future = QtConcurrent::filteredReduced(LessThanData.constBegin(),LessThanData.constEnd(),
                                                  lessThan10,Reduce::reduce,ReduceOption::SequentialReduce);// filter10个线程,reduce1个线程
            qDebug()<< "filter_reduce lessThan 10："<<future.results(); // 按顺序返回
    }

    void concurrent_filter_blocking_test()
    {
         // 对3个阻塞型的API进行测试
        auto func = std::bind(lessThanChar,std::placeholders::_1,',');

        QList<QChar> res = QtConcurrent::blockingFiltered(BlockData,func); // 不支持使用迭代器
        qDebug()<<"blockingFiltered："<<res; // 比','小的字符('*', '&', '%', '#', '+', '!') 17个线程

        quint32 total = QtConcurrent::blockingFilteredReduced(BlockData.constBegin(), // 可以使用迭代器
                                                              BlockData.constEnd(),func,mergeCharUnicode);
        qDebug()<<"blockingFilteredReduced："<<total; // 228 17个线程, merge1个线程

        QtConcurrent::blockingFilter(BlockData,func); // 不支持使用迭代器
        qDebug()<<"blockingFilter："<<BlockData; // ('*', '&', '%', '#', '+', '!') 17个线程
    }

    // 用于map的测试数据
    QList<QChar> LowerCharData = {'a','b','c'};
    QList<double> SerializeData  = {-M_2_PI,-M_2_PI*7/4,-M_2_PI*3/2,-M_2_PI*5/4,-M_PI,-M_PI*3/4,-M_PI/2,-M_PI/4,0,
                                   M_2_PI,M_2_PI*7/4,M_2_PI*3/2,M_2_PI*5/4,M_PI,M_PI*3/4,M_PI/2,M_PI/4,
                                   -5.0,-4.3,-3.6,-2.5,-1.7,-0.4,1.9,2.6,3.1,4.8,5.5};
    QList<int> random(int min = 0,int max = 1,quint32 size = 10){
            QList<int> res;
            std::uniform_real_distribution<double> dist(min, max);
            std::random_device rd;  // 将用于获得随机数引擎的种子
            std::mt19937 gen(rd()); // 以 rd() 播种的标准 mersenne_twister_engine
            for (quint32 n = 0; n <size; ++n) { res<< (int)dist(gen);}
            return res;
    }

    // 用于map的映射函数和reduce函数
    QChar toUpper(QChar c) {qDebug()<<"toUpper's current thread id = "<<QThread::currentThreadId();return  c.toUpper();};
    QString toString(int v,int base=10) {qDebug()<<"toString's current thread id = "<<QThread::currentThreadId();return QString::number(v,base);}
    double serialize(double val) {qDebug()<<"serialize's current thread id = "<<QThread::currentThreadId();return -1.0+2.0*sin((double)val)*cos((double)val)-tan(val)+exp(val);};
    void accumulate(double &res, double v){qDebug()<<"accumulate's current thread id = "<<QThread::currentThreadId(); res+=v;};
    struct Accumulate{
        static void accumulate_1(double &res, double v){qDebug()<<"accumulate's current thread id = "<<QThread::currentThreadId(); res+=v;}
        void accumulate_2(double &res, double v){qDebug()<<"accumulate's current thread id = "<<QThread::currentThreadId(); res+=v;}
        void operator()(double &res, double v){qDebug()<<"Accumulate's current thread id = "<<QThread::currentThreadId(); res+=v;}
    };


    void concurrent_map_test()
    {
            QFuture<void> f = QtConcurrent::map(LowerCharData.begin(),LowerCharData.end(),toUpper);// 3个线程执行
            f.waitForFinished();
            qDebug()<<"map toUpper [普通函数]："<<LowerCharData;// 没起作用 ('a', 'b', 'c')
    }

    void concurrent_mapped_test()
    {
         QFuture<QChar> f = QtConcurrent::mapped(LowerCharData.constBegin(),LowerCharData.constEnd(),toUpper); // 使用了3个不同的线程执行
         qDebug()<< "mapped toUpper [普通函数]："<<f.results(); // 这里使用普通函数,成员函数、函数对象、匿名函数都是可以的不再测试

         auto func  = std::bind(toString,std::placeholders::_1,2);
         QFuture<QString> f1 = QtConcurrent::mapped(random(0,100,5),func); // 使用了5个不同的线程执行
         qDebug()<< "mapped toString [双参函数]："<<f1.results();
    }

    void concurrent_map_reduce_test()
    {
        QFuture<double> f = QtConcurrent::mappedReduced(SerializeData,serialize,accumulate); //serialize使用了16个线程,reduce只用1个线程
        qDebug()<<"mappedReduced serialize [普通函数] reduce[普通函数]："<< f.results(); // 441.815
        double res = 5;
        Accumulate()(res,5); // 测试函数对象是否可用
        qDebug()<<"res = "<<res;

//        f = QtConcurrent::mappedReduced(SerializeData,serialize,[](double &res, const double& v){ // reduce函数不支持匿名函数
//            qDebug()<<"accumulate's current thread id = "<<QThread::currentThreadId();
//            res+=v;
//        });
//        QFuture<double> f1 = QtConcurrent::mappedReduced(SerializeData,serialize,[=](double &res, const double& v){ // 也不支持非静态成员函数
//             Accumulate a;
//             a.accumulate_2(res,v);
//        });

        //f = QtConcurrent::mappedReduced(SerializeData,serialize,Accumulate()); // reduce函数不支持函数对象
        QFuture<double> f1 = QtConcurrent::mappedReduced(SerializeData,serialize,Accumulate::accumulate_1); // 可以使用静态成员函数
        qDebug()<<"mappedReduced serialize [普通函数] reduce[静态成员函数]："<< f1.results(); // 441.815

        std::function<void(double&,const double &v)> reduce = [](double&res,const double& v)->void{
                qDebug()<<"accumulate's current thread id = "<<QThread::currentThreadId();
                res+=v;
        };
        // f1 = QtConcurrent::mappedReduced(SerializeData,serialize,reduce); 封装的函数也不支持
    }

    void concurrent_map_blocking_test()
    {
        // 对3个阻塞型的API进行测试
        QList<QChar> r1 = QtConcurrent::blockingMapped(LowerCharData,toUpper); // 3个线程
        qDebug()<<"blockingMapped："<<r1; //  ('A', 'B', 'C')
        quint32 total = QtConcurrent::blockingMappedReduced(SerializeData,serialize,accumulate); // 13个线程
        qDebug()<<"blockingMappedReduced："<<total; // 442
        QtConcurrent::blockingMap(LowerCharData,toUpper); // 3个线程
        qDebug()<<"blockingMap："<<LowerCharData; // ('a', 'b', 'c')没有改变？
    }


    // 用于run的测试函数
    void hello(QString name)
    {
        qDebug() << "Hello" << name << "from" << QThread::currentThread();
    }
    long callLoop(long rows = 5000,long cols = 5000){
        qDebug() << "callLoop's current thread id = " << QThread::currentThread();
        long sum = 0;
        QString s;
        for(long r = 0 ; r < rows ; ++r)
            for(long c = 0 ; c < cols; ++c)
            {
                s = QString::number(r+c+2,2);
            }
        sum = s.toLong(Q_NULLPTR,2);
        return sum;
    }
    class Loop{
        int mrows;
        int mcols;
        public:
            static long mtotal;
            Loop(int rows,int cols) : mrows(rows), mcols(cols){};
            long loop_1(){qDebug() << "loop_1's current thread id = " << QThread::currentThread();return callLoop(mrows,mcols);};
            static long loop_2(int row,int col) {qDebug() << "loop_2's current thread id = " << QThread::currentThread();mtotal = callLoop(row,col); return mtotal;};
            QStringList spilit(QString s) {
                qDebug() << "spilit's current thread id = " << QThread::currentThread();
                QStringList ret;
                for(long r = 0 ; r < mrows ; ++r)
                    for(long c = 0 ; c < mcols; ++c)
                    {
                       ret = s.split(',',QString::SkipEmptyParts);
                    }
                return ret;
            };
            void reset(int r, int c){mrows=r;mcols=c;}
    };
    long Loop::mtotal = 0; // 要给出定义

    void concurrent_run_test()
    {
        QThreadPool pool;
        QVector<QString> Names = {QString("A"),QString("B"),QString("C"),QString("D"),
                                                        QString("E"),QString("F"),QString("G"),QString("H"),
                                                        QString("I"),QString("J"),QString("K"),QString("L"),
                                                        QString("M"),QString("N"),QString("O"),QString("P"),
                                                        QString("Q"),QString("R"),QString("S"),QString("T")};
        QVector<QFuture<void>> futures;
        for(int i = 0; i < Names.size(); ++i ){
            futures.append(QtConcurrent::run(&pool,hello,Names.at(i)));
            //futures[i].waitForFinished(); // 如果顺序等待则都是1个线程执行的
        }
        for(int j = futures.size()-1; j>=0; j--) futures[j].waitForFinished(); // 倒序等待这样能看到很多都不是相同线程执行的 16个线程
        qDebug()<<"可用的线程数量："<<QThread::idealThreadCount();
//#define NonConcurrent
//#define ExternFuncConcurrent
//#define ExternFuncPoolConcurrent
//#define StaticMemberFuncConcurrent
#define NonStaticMemberFuncConcurrent
//#define LambdaFuncConcurrent
#ifdef NonConcurrent
         QTime t;
         t.start();
         long sum1 = callLoop(); // 1个线程
         qDebug()<<"without concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << sum1;
         // without concurrent cost time =  3.705  s and sum =  10000
#elif defined (ExternFuncConcurrent)
         // 调用普通函数
        QTime t;
        t.start();
         QFuture<long> sum2= QtConcurrent::run(callLoop,5000,5000); // 1个线程
         qDebug()<<"[普通函数] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << sum2.result();
         // [普通函数] with concurrent cost time =  3.736  s and sum =  10000
#elif defined (ExternFuncPoolConcurrent)
         // 带线程池参数的普通函数
        QTime t;
        t.start();
         QFuture<long> sum3= QtConcurrent::run(&pool,callLoop,5000,5000);// 1个线程
         qDebug()<<"[普通函数-pool] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << sum3.result();
         // [普通函数-pool] with concurrent cost time =  3.743  s and sum =  10000
 #elif defined (NonStaticMemberFuncConcurrent)
        QTime t;
        Loop loop(5000,5000);
        t.start();
        auto sum4 = QtConcurrent::run(&loop,&Loop::loop_1); // 使用成员函数 run第1个参数必须是类的实例的指针 1个线程
        qDebug()<<"[成员函数-实例指针] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << sum4.result();
        // [成员函数] with concurrent cost time =  3.772  s and sum =  10000
       QString text = "c++,python,html,css,javascript,java,mysql,c,rust";
       loop.reset(1000,1000);
        t.restart();
        auto splitArray = QtConcurrent::run(&loop,&Loop::spilit,text);// 1个线程
        qDebug()<<"[成员函数-实例指针] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and res = " << splitArray.result();

        QByteArray text1 = "c++,python,html,css,javascript,java,mysql,c,rust"; // 或者第1个参数是个常量也可以,QString不可以
        t.restart();
        QFuture<QList<QByteArray>> future1 = QtConcurrent::run(text1, &QByteArray::split, ',');
        qDebug()<<"[成员函数-常量引用] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and res = " << future1.result();

        //const QString text2 = "c++,python,html,css,javascript,java,mysql,c,rust"; // 不能通过编译
        //QFuture<QStringList> future2 = QtConcurrent::run(&text2, &QString::split, ',',QString::SkipEmptyParts,Qt::CaseInsensitive);
#elif defined (StaticMemberFuncConcurrent)
        QTime t;
        t.start();
        auto sum5 = QtConcurrent::run(Loop::loop_2,5000,5000);// 1个线程
        sum5.waitForFinished();
        qDebug()<<"[静态成员函数] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << Loop::mtotal;
        //[静态成员函数] with concurrent cost time =  3.759  s and sum =  10000
#elif defined ( LambdaFuncConcurrent)
        QTime t;
        t.start();
        auto sum6 = QtConcurrent::run([=]{ return callLoop();});// 1个线程
         qDebug()<<"[匿名函数] with concurrent cost time = "<<t.elapsed()/1000.0 <<" s and sum = " << sum6.results();
         // [匿名函数] with concurrent cost time =  3.733  s and sum =  (10000)
#endif
    }

    void concurrent_futurewatcher_test()
    {
              const int iterations = 100;
              QVector<int> vector;
              for (int i = 0; i < iterations; ++i)
                  vector.append(i);
               //qDebug()<<"vector = "<<vector;

              QProgressDialog dialog;

              dialog.setLabelText(QString("Progressing using %1 thread(s)...").arg(QThread::idealThreadCount())); // 返回可以在系统上运行的理想线程数(16个线程)

              dialog.resize(500,180);
              dialog.setFont(QFont("Times New Roman",12));

              QFutureWatcher<void> futureWatcher; // 监视器完成对应进度条的完成,如果autoClose=true(默认)就会隐藏
              QObject::connect(&futureWatcher, &QFutureWatcher<void>::finished, &dialog, &QProgressDialog::reset);
              QObject::connect(&futureWatcher,  &QFutureWatcher<void>::progressRangeChanged, &dialog, &QProgressDialog::setRange);// 监视器范围改变也会调整进度条的范围
              QObject::connect(&futureWatcher, &QFutureWatcher<void>::progressValueChanged,  &dialog, &QProgressDialog::setValue);// 更新进度条的值
              QObject::connect(&dialog, &QProgressDialog::canceled, &futureWatcher, &QFutureWatcher<void>::cancel);// 如果进度条人为点击取消那么结束任务,不过是异步的可以使用waitForFinshed同步

              std::function<void(int&)> spin = [](int&iteration) { // 一个匿名函数,参数int&,返回void被std::function打包成1个函数
                  const long long work = 1000 * 1000 * 1000; // spin作为map的映射函数,因为是原地映射,所以输入参数必须是引用且返回void
                  volatile long long v = 0;//内存可见性是指当一个线程修改了某个变量的值，其它线程总是能知道这个变量变化，可以使用加锁或者volatile关键字
                  for (long long j = 0; j < work; ++j) // iteration就是vector的每个值(不一定按照顺序)
                      ++v;

                  qDebug() << "iteration" << iteration << "in thread" << QThread::currentThreadId();
              };

              futureWatcher.setFuture(QtConcurrent::map(vector, spin));

              dialog.exec();

              futureWatcher.waitForFinished(); // 同步

              while (!futureWatcher.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);

              qDebug() << "Canceled?" << futureWatcher.future().isCanceled(); // 收到点击取消了才会true
    }

    typedef  QMap<QString,int> WordCount;

    WordCount countWords(const QString &file)
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        QTextStream textStream(&f);
        textStream.setAutoDetectUnicode(true);
        textStream.setCodec("utf-8"); // 不会乱码
        WordCount wordCount;

        while (!textStream.atEnd()) {
            const auto words =  textStream.readLine().split(' ');
            for (const QString &word : words)
                wordCount[word] += 1;
        }
        f.close();
        return wordCount;
    }

    void reduce(WordCount &result, const WordCount &w)
    {
        QMapIterator<QString, int> i(w);
        while (i.hasNext()) {
            i.next();
            result[i.key()] += i.value();
        }
    }

    void concurrent_example_test() // 一个使用并行技术和不使用花费时间的对比: 案例是计算所有过滤出的文件,不同单词出现的个数
    {

        //typedef  QMap<QString,int> WordCount; // 用于记录每个词出现次数的数据结构
        const char * path = "D:/"; // ".."  "D:/" "C:/Users/Lenovo/Desktop/myLeetCode"
        QStringList allFiles = listFiles(path,QStringList() << "*.cpp" << "*.h" <<"*.txt"
                                         <<"*.xml"<<"*.css"<<"*.html"<<"*.md"
                                         <<"*.py" <<"*.cmake");
        qDebug()<<"files's count = "<<allFiles.size();
        QTime time;

        // 一个单线程去计算所有文件出现所有词的频率
        qDebug()<<"single thread to calculate wordcount....";
        std::function<WordCount(const QStringList&)> singleThreadedWordCount = [&](const QStringList &files)->WordCount
        {
            WordCount wordCount;
            for (const QString &file : files) { // 计算很多文件
                QFile f(file);
                QTextStream textStream(&f);
                textStream.setAutoDetectUnicode(true);
                textStream.setCodec("utf-8"); // 不会乱码
                f.open(QIODevice::ReadOnly);
                while (!textStream.atEnd()) {
                    const auto words =  textStream.readLine().split(' ',QString::SkipEmptyParts); // 按空格区分不同词
                    //qDebug()<<"words = "<<words;
                    for (const QString &word : words)
                        wordCount[word] += 1;
                }
                f.close();
            }
             return wordCount;
        };


        time.start();
        WordCount total = singleThreadedWordCount(allFiles);
        double t1 = time.elapsed()/1000.0;
        qDebug() << "single thread cost time = " << t1 <<"s";
        // for(auto each: total.toStdMap()) qDebug()<<QString("word[%1]=%2").arg(each.first).arg(each.second);
        // foreach(auto key,total.keys()) qDebug()<<QString("word[%1]=%2").arg(key).arg(total[key]);
        qDebug()<<"total word's count = "<<total.keys().size();

        qDebug()<<"-----------------------------------------------------";
        qDebug()<<"multiple threads to calculate wordcount....";
        // 多线程计算
        // 单线程是一次性把所有文件都进行循环计算,多线程可以把1个文件的计算作为一个任务,使用单独线程进行
        // 所以首先把上述singleThreadedWordCount内部的计算代码去掉对文件的循环独立成为1个函数
        // 每个文件计算出来的结果的合并使用QtConcurrent::map的mappedReduced函数去合并
        std::function<WordCount(const QString&)> calOneFileWordCount = [](const QString& file) ->WordCount{
                QFile f(file); // 改为匿名函数也可以
                QTextStream textStream(&f);
                textStream.setAutoDetectUnicode(true);
                textStream.setCodec("utf-8"); // 不会乱码
                f.open(QIODevice::ReadOnly);
                WordCount wordCount;
                while (!textStream.atEnd()) {
                    const auto words =  textStream.readLine().split(' ');
                    for (const QString &word : words)
                        wordCount[word] += 1;
                }
                f.close();
                return wordCount;
        };
        auto mergeWordCount = [](WordCount &result, const WordCount &w)->void // 第1个参数是结果类型依然是WordCount
        { // 第2个参数是项类型也是WordCount
              QMapIterator<QString, int> i(w); // java风格迭代器
              while (i.hasNext()) {
                  i.next();
                  result[i.key()] += i.value();  // i就是WordCount,把每个文件的WordCount合并到整体的WordCount
              }
        };
        Q_UNUSED(mergeWordCount);

        time.restart();
        //total  = QtConcurrent::mappedReduced(allFiles.constBegin(),allFiles.constEnd(),countWords,reduce); // 可用
        total = QtConcurrent::mappedReduced(allFiles.constBegin(),allFiles.constEnd(),calOneFileWordCount,reduce); // 可用
        //total = QtConcurrent::mappedReduced(allFiles.constBegin(),allFiles.constEnd(),calOneFileWordCount,mergeWordCount); 不可用,因为reduce函数测试过不支持函数对象、匿名函数，非静态成员函数
        double t2 = time.elapsed()/1000.0;
        qDebug() << "multiple threads cost time = " << t2 <<"s";
        qDebug()<<"total word's count = "<<total.keys().size();

        qDebug()<<"-----------------------------------------------------";
        qDebug()<<"use concurrent speed "<< t1/t2 <<" multiples"; // 文件数特别少或者特别多比例是>1的,如果是十几个文件反而concurrent计算的更慢
    }
}
#endif // CONCURRENT_TEST_H
