/*
 * aco_ptv_test.hpp
 *
 *  Created on: Mar 3, 2011
 *      Author: fqhuy
 */

#ifndef ACO_PTV_TEST_HPP_
#define ACO_PTV_TEST_HPP_

#include <Magick++.h>

class ACOPTVEstimatorTest: public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE( ACOPTVEstimatorTest);
	//CPPUNIT_TEST( testStandard);
	//CPPUNIT_TEST( testOrigin );
	//CPPUNIT_TEST( testEstimate );
	//CPPUNIT_TEST( testRelaxationLength);
	//CPPUNIT_TEST( testDistances );
	//CPPUNIT_TEST( testSort );
	//CPPUNIT_TEST( testCluster );
	CPPUNIT_TEST( testAll );

	CPPUNIT_TEST_SUITE_END();

private:
	int SIZE; //*******
	static const int DIM = 2;
	Sp::Matrix<float, float>* frame0_;
	Sp::Matrix<float, float>* frame1_;
	Sp::Matrix<float, float>* result_;
	Sp::Matrix<int, int>* iresult_;
	Sp::ACOPTVEstimator* est_;
	//std::string file0_, file1_;
	libconfig::Config cfg_;

	float alpha;
	float beta;
	float rho;
	float tau0;
	int cluster_size;
	int cluster_max;
	int num_particles;
	int num_ants;
	int num_loops;

	int zoom;
public:
	ACOPTVEstimatorTest() :
		frame0_(NULL), frame1_(NULL), result_(NULL), iresult_(NULL), est_(NULL) {
		cfg_.readFile(
				"/home/fqhuy/Documents/Projects/ships-project-build/bin/aco_ptv_test.cfg");
		/*
		 alpha = 1.0f;
		 beta = 5.0f;
		 num_loops = 5; //5;
		 num_particles = SIZE; //1024, 2048, 3072, 4096
		 num_ants = 16;
		 rho = 0.5f;
		 cluster_size = 6; //4 ********
		 tau0 = 5.0f;
		 */
		libconfig::Setting& ships = cfg_.lookup("aco_ptv_test");
		alpha = ships["alpha"];
		beta = ships["beta"];
		num_loops = ships["num_loops"]; //5;
		num_particles = ships["num_particles"]; //1024, 2048, 3072, 4096
		SIZE = num_particles;
		num_ants = ships["num_ants"];
		rho = ships["rho"];
		cluster_size = ships["cluster_size"]; //4 ********
		cluster_max = ships["cluster_max"];
		tau0 = ships["tau0"];
		zoom = ships["zoom"];

	}
	virtual ~ACOPTVEstimatorTest() {
		if (frame0_)
			delete frame0_;
		if (frame1_)
			delete frame1_;
		if (result_)
			delete result_;
		if (iresult_)
			delete iresult_;
		if (est_)
			delete est_;
	}
	void setUp() {
		libconfig::Setting& ships = cfg_.lookup("aco_ptv_test");
		std::string file0_ = ships["first_frame"];
		std::string file1_ = ships["second_frame"];

		Sp::OctaveTextMatrixReader reader_;
		frame0_ = new Sp::HostMatrix<float, float>(DIM, SIZE);
		frame1_ = new Sp::HostMatrix<float, float>(DIM, SIZE);
		reader_.ReadAsFloatMatrix(file0_, frame0_);

		reader_.ReadAsFloatMatrix(file1_, frame1_);

		est_ = new Sp::ACOPTVEstimator(alpha, beta, rho, tau0, cluster_size, cluster_max,
				num_particles, num_ants, num_loops);


	}

	void tearDown() {

	}

	void testAll(){
		libconfig::Setting& ships = cfg_.lookup("aco_ptv_test");
		std::string test = ships["test"];

		if(test == "testCluster")
			testCluster();
		else if( test == "testEstimate")
			testEstimate();
		else if( test == "testEstimateN")
			testEstimateN();
		else if( test == "testRelaxationLength")
			testRelaxationLength();
		else if( test == "testStandard")
			testStandard();
		else if( test == "testOrigin")
			testOrigin();
	}

	void testDistances() {

		result_ = est_->Distances(frame0_, frame1_);
		//LOG4CXX_INFO(Sp::video_logger, frame0_->ToString());
		//LOG4CXX_INFO(Sp::video_logger, frame1_->ToString());
		//LOG4CXX_INFO(Sp::video_logger, result_->ToString());
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		est_->Estimate();
		LOG4CXX_INFO(Sp::video_logger, "(" << frame0_->Get(511, 0) << "-"
				<< frame0_->Get(511, 1) << ") --> (" << frame1_->Get(113, 0)
				<< "-" << frame1_->Get(113, 1) << ") = " << est_->Cf0f1()->Get(
				511, 113) << "    " << result_->Get(511, 113));
	}

	void testSort() {
		//Sp::ACOPTVEstimator est;
		Sp::HostMatrix<int, int> indices(SIZE, SIZE);
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				indices.Set(j, i, j);

		result_ = est_->Distances(frame0_, frame1_);
		est_->Sort(result_, &indices);

		LOG4CXX_INFO(Sp::video_logger, result_->ToString());
		LOG4CXX_INFO(Sp::video_logger, indices.ToString());
	}
	void testEstimateN() {
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		for(int i=0;i<10;i++){
			iresult_ = est_->Estimate();
			int ok = 0;
			for (int i = 0; i < iresult_->GetHeight(); i++) {
				if (iresult_->Get(i, 0) == iresult_->Get(i, 1)) {
					ok++;
				}
			}
			LOG4CXX_INFO(Sp::video_logger, "correct rate: "<< ok << "/" << SIZE);
		}
	}
	void testEstimate() {
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		iresult_ = est_->Estimate();
		//LOG4CXX_INFO(Sp::video_logger,"estimating the motion vectors..");
		CPPUNIT_ASSERT(iresult_ != NULL);
		if (iresult_) {
			int ok = 0, nok =0, id = 0;

			try {
				Magick::Image image("800x800", "white");
				//std::ostringstream oss, oss1;
				char str[256], str1[256];
			    image.fontPointsize( 12 );
				image.strokeWidth(0.5f);
				for (int i = 0; i < iresult_->GetHeight(); i++) {
					image.strokeColor("pink");
					image.draw(Magick::DrawableCircle(frame0_->Get( \
							iresult_->Get(i, 0), 0) * zoom, frame0_->Get( \
							iresult_->Get(i, 0), 1) * zoom, frame0_->Get( \
							iresult_->Get(i, 0), 0) * zoom-1, frame0_->Get( \
							iresult_->Get(i, 0), 1) * zoom \
							) \
					);
					image.strokeColor("green");
					image.draw(Magick::DrawableCircle(frame1_->Get( \
							iresult_->Get(i, 1), 0) * zoom, frame1_->Get( \
							iresult_->Get(i, 1), 1) * zoom, frame1_->Get( \
							iresult_->Get(i, 1), 0) * zoom-1, frame1_->Get( \
							iresult_->Get(i, 1), 1) * zoom \
							) \
					);

					if (iresult_->Get(i, 0) == iresult_->Get(i, 1)) {
						ok++;
						image.strokeColor("blue");
						image.draw(Magick::DrawableLine(frame0_->Get( \
								iresult_->Get(i, 0), 0) * zoom, frame0_->Get( \
								iresult_->Get(i, 0), 1) * zoom, frame1_->Get( \
								iresult_->Get(i, 1), 0) * zoom, frame1_->Get( \
								iresult_->Get(i, 1), 1) * zoom));
					} else {
						nok++;
						image.strokeColor("red");
						if(nok<20){
							sprintf(str, "%d",iresult_->Get(i, 0));
							sprintf(str1,"+%d+%d",(int)frame0_->Get(iresult_->Get(i, 0), 0)*zoom,(int)frame0_->Get(iresult_->Get(i, 0), 1)*zoom);
							image.annotate(str,str1);
							sprintf(str, "%d",iresult_->Get(i, 1));
							sprintf(str1,"+%d+%d",(int)frame1_->Get(iresult_->Get(i, 1), 0)*zoom,(int)frame1_->Get(iresult_->Get(i, 1), 1)*zoom);
							image.annotate(str,str1);

						image.draw(Magick::DrawableLine(frame0_->Get( \
								iresult_->Get(i, 0), 0) * zoom, frame0_->Get( \
								iresult_->Get(i, 0), 1) * zoom, frame1_->Get( \
								iresult_->Get(i, 1), 0) * zoom, frame1_->Get( \
								iresult_->Get(i, 1), 1) * zoom));
						}
						LOG4CXX_INFO(Sp::video_logger, "wrong pair: "
								<< iresult_->Get(i, 0) << "-" << iresult_->Get(
								i, 1) << "---" << est_->Cf0f1()->Get(
								iresult_->Get(i, 0), iresult_->Get(i, 1))

						);
					}
				}
				image.depth(8);
				image.compressType(Magick::RLECompression);
				image.write("result.miff");

				// cout << "Display image..." << endl;
				image.display();
			} catch (exception &error_) {
				cout << "Caught exception: " << error_.what() << endl;
				return;
			}
			LOG4CXX_INFO(Sp::video_logger, "the number of correct pair is: "
					<< ok << "/" << SIZE);

		}
		//	LOG4CXX_INFO(Sp::video_logger, iresult_->ToString());
	}
	void testStandard() {
		try {
			Magick::Image image("800x800", "white");

			image.strokeWidth(0.5f);
			for (int i = 0; i < num_particles; i++) {

				image.strokeColor("pink");
				image.draw(Magick::DrawableCircle(frame0_->Get( \
						i, 0) * zoom, frame0_->Get( \
						i, 1) * zoom, frame0_->Get( \
						i, 0) * zoom-1, frame0_->Get( \
						i, 1) * zoom \
						) \
				);
				image.strokeColor("green");
				image.draw(Magick::DrawableCircle(frame1_->Get( \
						i, 0) * zoom, frame1_->Get( \
						i, 1) * zoom, frame1_->Get( \
						i, 0) * zoom-1, frame1_->Get( \
						i, 1) * zoom \
						) \
				);

				image.strokeColor("blue");
				image.draw(Magick::DrawableLine(frame0_->Get(i, 0) * zoom,
						frame0_->Get(i, 1) * zoom, frame1_->Get(i, 0) * zoom,
						frame1_->Get(i, 1) * zoom));
			}
			image.depth(8);
			image.compressType(Magick::RLECompression);
			image.write("standard.miff");

			// cout << "Display image..." << endl;
			image.display();
		} catch (exception &error_) {
			cout << "Caught exception: " << error_.what() << endl;
			return;
		}
	}

	void testOrigin(){
		try {
			Magick::Image image("800x800", "white");

			image.strokeWidth(0.5f);
			for (int i = 0; i < num_particles; i++) {
				image.strokeColor("blue");
				image.draw(Magick::DrawableCircle(frame0_->Get(i, 0) * zoom,
						frame0_->Get(i, 1) * zoom,frame0_->Get(i, 0) * zoom-2,
						frame0_->Get(i, 1) * zoom));
				image.strokeColor("red");
				image.draw(Magick::DrawableCircle(frame1_->Get(i, 0) * zoom,
						frame1_->Get(i, 1) * zoom,frame1_->Get(i, 0) * zoom-2,
						frame1_->Get(i, 1) * zoom));
			}
			image.depth(8);
			image.compressType(Magick::RLECompression);
			image.write("standard.miff");

			// cout << "Display image..." << endl;
			image.display();
		} catch (exception &error_) {
			cout << "Caught exception: " << error_.what() << endl;
			return;
		}
	}
	void testRelaxationLength() {
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		est_->Estimate();
		int a = 325, b = 400;
		float rlength1 = est_->RelaxationLength(a, a);
		float rlength2 = est_->RelaxationLength(a, b);

		float length1 = est_->Cf0f1()->Get(a, a);
		float length2 = est_->Cf0f1()->Get(a, b);
		LOG4CXX_INFO(Sp::video_logger, length1 << " vs " << length2 << " ---- "
				<< rlength1 << " vs " << rlength2);
	}

	void testCluster() {
		est_->AddFrame(frame0_);
		est_->AddFrame(frame1_);
		est_->Estimate();
		//std::ostringstream oss;
		Sp::Matrix<int,int>* clusters01 = est_->Clusters01();
		bool flag = false;
		int i, j ;
		for(i=0;i<SIZE;i++){
			flag = false;
			for(j=0;j<cluster_max;j++){
				if(clusters01->Get(i,j)==i){
					flag = true;
					break;
				}
			}
			if(flag==false){
				std::cout << "bad cluster: (" << i << ") ";
				for(j=0;j<cluster_max;j++){
					std::cout << clusters01->Get(i,j) << " ";
				}
				std::cout << std::endl;
			}

		}
	}

	void testNext() {

	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( ACOPTVEstimatorTest);

#endif /* ACO_PTV_TEST_HPP_ */
