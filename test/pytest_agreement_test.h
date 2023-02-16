#ifndef PYTEST_AGREEMENT_TEST_H
#define PYTEST_AGREEMENT_TEST_H

#include "pytest_agreement_api.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// 这里测试协议层: 包括对象协议、数字协议、序列协议、映射协议、迭代器协议
enum class AGREEMENT_TEST {ObjAgreement,DigitAgreement,SeqAgreement,MapAgreement,IterAgreement};
void test_agreement(AGREEMENT_TEST test)
{
    switch (test) {
            case AGREEMENT_TEST::ObjAgreement: test_obj_agreement();break;
            case AGREEMENT_TEST::DigitAgreement: test_digit_agreement();break;
            case AGREEMENT_TEST::SeqAgreement: test_seq_agreement();break;
            case AGREEMENT_TEST::MapAgreement: test_map_agreement();break;
            case AGREEMENT_TEST::IterAgreement: test_iter_agreement();break;
    }
}
#endif // PYTEST_AGREEMENT_TEST_H
