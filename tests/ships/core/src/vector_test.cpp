/*
 * File:   VectorTest.cpp
 * Author: fqhuy
 *
 * Created on Feb 23, 2011, 9:32:30 PM
 */
#include "vector_test.hpp"
//CPPUNIT_TEST_SUITE_REGISTRATION(VectorTest);

VectorTest::VectorTest() {
	//LOG4CXX_INFO(Sp::core_logger, "vector_test: creating vector...");
	Sp::MemoryModel<float>* mm = new Sp::MemoryModel<float>(1, true, true, 2,
			Sp::READ_WRITE);
	uint32_t size = 7;
	Sp::SampleModel<float, float>* sm = new Sp::PixelInterleavedSampleModel<
			float>(1, 1, &size);
	vector_ = new Sp::Vector<float, float>(size, mm, sm);
	//LOG4CXX_INFO(Sp::core_logger, "vector_test: array size is: " << vector_->GetArray()->GetSize() << '\n');
}

VectorTest::~VectorTest() {
}

void VectorTest::setUp() {

}

void VectorTest::tearDown() {
	//if(vector_!=NULL)
	//	delete vector_;
}

void VectorTest::testSet() {
	//LOG4CXX_INFO(Sp::core_logger, "vector_test: testSet()...");
	for (uint32_t i = 0; i < vector_->GetSize(); i++) {
		float val = 1.33f;
		vector_->Set(val*i, i);
	}
}

void VectorTest::testGet() {
	{
		this->testSet();
		for (uint32_t i = 0; i < vector_->GetSize(); i++) {
			CPPUNIT_ASSERT_EQUAL(vector_->Get(i),(float)i*1.33f);
		}


	}
}

