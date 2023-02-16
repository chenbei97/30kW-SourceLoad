#ifndef TEST_H
#define TEST_H

#include "pytest_intro_test.h" // 引入python和package测试
#include "pytest_useful_macro_test.h" // Python.h提供的宏测试
#include "pytest_tool_test.h" // 工具层测试:导入模块+解析参数并构建值变量+字符串转换与格式化
#include "pytest_agreement_test.h" // 协议层测试: 对象协议+数字协议+序列协议+映射协议+迭代器协议
#include "pytest_spec_objlayer_test.h" // 具体对象层测试
#include "../csv/csvexport_test.h" // CSV导入导出测试
#include "concurrent_test.h" // 并行计算测试
#include "listfiles_test.h" // 查找指定路径下所有文件名的测试
#include "animation_test.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

namespace AllTest {
    /*-----------------------------------------*/
    void listFiles_test(){list_files_test();}

    /*-----------------------------------------*/
    void pytest()
    {
            test_intro(INTRO_TEST::PythonTest);
            test_intro(INTRO_TEST::PackageTest);

            test_useful_macro(USEFULLMACRO_TEST::UsefullMacroTest);

            test_tool(TOOL_TEST::ArgTest);
            test_tool(TOOL_TEST::ImportTest);
            test_tool(TOOL_TEST::ConversionTest);

            // 以下测试函数不能同时运行,否则异常
            test_agreement(AGREEMENT_TEST::IterAgreement);
//            test_agreement(AGREEMENT_TEST::MapAgreement);
//            test_agreement(AGREEMENT_TEST::SeqAgreement);
//            test_agreement(AGREEMENT_TEST::DigitAgreement);
//            test_agreement(AGREEMENT_TEST::ObjAgreement);

            test_specobj_layer(DATATYPE_TEST::TupleTest);
    }
    /*-----------------------------------------*/
    void csv_test(){test_export_import_csv();}


    /*-----------------------------------------*/
    void concurrent_test()
    {
         ConCurrentTest::concurrent_filtered_test();
         ConCurrentTest::concurrent_filter_reduce_test();
         ConCurrentTest::concurrent_filter_test();
         ConCurrentTest::concurrent_filter_blocking_test();

         ConCurrentTest::concurrent_mapped_test();
         ConCurrentTest::concurrent_map_reduce_test();
         ConCurrentTest::concurrent_map_test();
         ConCurrentTest::concurrent_map_blocking_test();

         ConCurrentTest::concurrent_run_test();

         ConCurrentTest::concurrent_futurewatcher_test();

         ConCurrentTest::concurrent_example_test();
    }

    void animation_test()
    {
        AnimationTest::pushbutton_animation();
    }

}

#endif // TEST_H
