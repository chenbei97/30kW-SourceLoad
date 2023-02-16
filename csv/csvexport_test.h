#ifndef CSVEXPORT_TEST_H
#define CSVEXPORT_TEST_H
#include <QString>
#include <QDebug>
#include "csvexport.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// 使用inline防止多重定义
inline QString construct_writeHeader() // 用于测试的构造表头的函数
{
    return QString("姓名,性别,年龄,学历,专业,籍贯,手机号\n");
}

inline QString construct_writeContent() // 用于测试的构造内容的函数
{
        //姓名,性别,年龄,学历,专业,籍贯,手机号
     QString  content = QString(
                    "A,男,25,硕士,电气,北京,19801279791\n"
                    "B,女,26,大专,建筑,上海,19801279792\n"
                    "C,女,27,本科,财会,南京,19801279793\n"
                    "D,男,28,博士,机电,重庆,19801279794\n"
                    "E,男,29,中专,政治,天津,19801279795\n"
                    "F,男,20,小学,计算机,石家庄,19801279796\n"
                    "G,女,30,中专,地质,苏州,19801279797\n"
                    "H,男,40,教授,物流,杭州,19801279798\n"
                    "K,女,50,博士后,艺术,厦门,19801279799\n"
                    "L,男,50,研究员,教育,大连,19801279790\n");
     return content;
}

// 用于测试的函数
inline void test_export_import_csv()
{
    CSVExport  csv;
    // c++版本
//    csv.writeTable(construct_writeHeader(),construct_writeContent);
//    csv.writeTable(construct_writeHeader(),construct_writeContent());
//    qDebug()<<"rows："<<csv.row();
//    qDebug()<<"cols："<<csv.col();
//    qDebug()<<"header："<<csv.tableHeader();
//    qDebug()<<"content："<<csv.tableContent();
    //csv.importCsv(); // 似乎python和c++版本的导入导出必须成对使用否则会存在异常

    // 任意执行1个都对,但是只能执行1个,还可以直接传入const char*,不过这里不再测试
//    csv.writeTablePy(construct_writeHeader(),construct_writeContent());
//    csv.writeTablePy(QStringLiteral("姓名,性别,年龄,学历,专业,籍贯,手机号"),construct_writeContent()); // 可以不要\n
    csv.writeTablePy(construct_writeHeader(),construct_writeContent); // 自动创建csv
    qDebug()<<"rows："<<csv.row();
    qDebug()<<"cols："<<csv.col();
    qDebug()<<"header："<<csv.tableHeader();
    qDebug()<<"content："<<csv.tableContent();

    //csv.importCsvPy(); // 导入和导出共用了python程序,也不能同时执行,注释掉一段运行另一段
    exit(0); // 为了退出程序,还能重复运行
}
#endif // CSVEXPORT_TEST_H
