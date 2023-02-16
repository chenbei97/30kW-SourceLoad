#ifndef LIST_FILES_H
#define LIST_FILES_H
#include <QStringList>
#include <QDir>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// 依据指定的过滤条件列出一个路径下的所有文件名
QStringList listFiles(const QString &startDir, const QStringList &filters)
 {
     QStringList names;
     QDir dir(startDir);

     const auto files = dir.entryList(filters, QDir::Files); // 列出dir目录的所有文件类型
     for (const QString &file : files)
         names += startDir + '/' + file; // 文件路径添加进

     const auto subdirs =  dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot); // 子目录且不要列出特殊的带..和.的条目
     for (const QString &subdir : subdirs)
         names += listFiles(startDir + '/' + subdir, filters); // 递归计算
     return names;
 }
#endif // LIST_FILES_H
