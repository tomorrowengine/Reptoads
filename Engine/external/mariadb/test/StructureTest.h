







#ifndef MARIADBCLIENTPP_STRUCTURETEST_H
#define MARIADBCLIENTPP_STRUCTURETEST_H

#include "SkeletonTest.h"

class StructureTest : public SkeletonTest {
   protected:
    virtual void CreateTestTable() override {}
};

#endif  
