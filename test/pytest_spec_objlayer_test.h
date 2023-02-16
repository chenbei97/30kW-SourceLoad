#ifndef PYTEST_SPEC_OBJLAYER_TEST_H
#define PYTEST_SPEC_OBJLAYER_TEST_H

#include <Python.h>
#include <QDebug>
#include <QList>
#include "pytest_spec_datatype_api.h"
#include "pytest_spec_funcobj_api.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

enum class DATATYPE_TEST {LongTest,BoolTest,FloatTest,ByteTest,UnicodeTest,
                          TupleTest,ListTest,DictTest,SetTest,StructSeqTest,FuncTest};
// 对具体应用层的基本数据类型( 整型、布尔类型、浮点型、字符和字符串类型、元组列表字典 )进行测试
void test_specobj_layer(DATATYPE_TEST test)
{
    switch (test) {
        case DATATYPE_TEST::LongTest: test_long_obj();break;
        case DATATYPE_TEST::BoolTest: test_bool_obj();break;
        case DATATYPE_TEST::FloatTest: test_float_obj();break;
        case DATATYPE_TEST::ByteTest: test_byte_obj();break;
        case DATATYPE_TEST::UnicodeTest: test_unicode_obj();break;
        case DATATYPE_TEST::TupleTest: test_tuple_obj();break;
        case DATATYPE_TEST::ListTest: test_list_obj();break;
        case DATATYPE_TEST::DictTest: test_dict_obj();break;
        case DATATYPE_TEST::SetTest: test_set_obj();break;
        case DATATYPE_TEST::StructSeqTest: test_structseq_obj();break;
        case DATATYPE_TEST::FuncTest: test_func_obj();break; // 对具体应用层的函数对象进行测试
    }
}
#endif // PYTEST_SPEC_OBJLAYER_TEST_H
