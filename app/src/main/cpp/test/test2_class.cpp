//
// Created by stevenhao on 4/7/24.
//

#include "test2_class.h"

int test2_class::testStatic1Int = 42;
int test2_class::testStatic2Int = 43;

///test2_class 构造函数
test2_class::test2_class(int temp1, int temp2) {
    testInt = temp1;
    testInt2 = temp2;


//    testStatic1Int = 1;
//    testStatic2Int = 2;
}
