#include "ocean_models/fvcom/FVCOMStructure.h"
#include "ocean_models/fvcom/FVCOMChunk.h"
#include "ocean_models/fvcom/FVCOM.h"

#include "ocean_models/model_interface/ModelData.h"

#include <gtest/gtest.h>

using namespace ocean_models;

#define SECONDS_IN_DAY 86400

FVCOM fvcomMultiple("../../src/ocean_models/test_data/axial_data_test", 1000, 1000, 10, 3, 10);
FVCOMStructure structure("../../src/ocean_models/test_data/axial_data_test", 1000, 1000, 10, 3);

TEST(FVCOMTest, XYInterpolation)
{

    //Triangle: 9152
    //Nodes: 4664, 4663, 4728 (Index)

    //x: 12555.1604, 13046.3472, 11481.4646
    //y: -9989.55491, -8437.34084, -8810.83762
    //area = 1122788.05072229475
    //418.112538766, 1414.93039512, 1180.65915604
    //a1=271049.91463457,a2=810505.0664852,a3=41233.069582733

    //p1=0.72186826887,p2=0.03672382294,p3=0.24140790816


    //siglay: 0, time 0
    //temp: 3.8415392214337647, 3.841629457479984, 3.841782292651267
    //salinity: 34.3125037844578, 34.312490185816166, 34.31246715343819
    //dye: 0, 0, 0

    //siglay: 3, time: 0
    //temp: 3.801574550036353, 3.802206202359889, 3.8032760485588715
    //salinity: 34.31852649120459, 34.318431300713144, 34.31827007406733
    //dye: 0, 0, 0

    //siglay: 3, time: 3
    //temp: 3.801572406374122, 3.8022022400180635, 3.803271680277034
    //salinity: 34.31852681425481, 34.318431897839325, 34.31827073237096
    //dye: 0, 0, 0
    FVCOMStructure::Point p1;
    p1.x = 12314;
    p1.y = -9648;
    p1.h = 0;

    ModelData data1 = fvcomMultiple.getData(12314, -9648, 0, 0 * SECONDS_IN_DAY);
    FVCOMStructure::Plane plane1 = structure.getTriangleSiglayPlane(9152, 3);
    double height = (-plane1.d - plane1.a * p1.x - plane1.b * p1.y) / plane1.c;

    ModelData data2 = fvcomMultiple.getData(12314, -9648, height, 0 * SECONDS_IN_DAY);
    ModelData data3 = fvcomMultiple.getData(12314, -9648, height, 0.125 * SECONDS_IN_DAY);

    EXPECT_FLOAT_EQ(3.84160121445, data1.temp);
    EXPECT_FLOAT_EQ(34.312494441, data1.salt);
    EXPECT_FLOAT_EQ(0.0, data1.dye);
    EXPECT_FLOAT_EQ(1890.6309, data1.depth);
    
    EXPECT_FLOAT_EQ(3.80200850181, data2.temp);
    EXPECT_FLOAT_EQ(34.3184551724, data2.salt);
    EXPECT_FLOAT_EQ(0.0, data2.dye);
    EXPECT_FLOAT_EQ(1890.6309, data1.depth);

    EXPECT_FLOAT_EQ(3.80200575432, data3.temp);
    EXPECT_FLOAT_EQ(34.3184615073, data3.salt);
    EXPECT_FLOAT_EQ(0.0, data3.dye);
    EXPECT_FLOAT_EQ(1890.6309, data1.depth);
}

TEST(FVCOMTest, TimeInterpolation)
{
    //Node: 6783
    //x: -96.5869768
    //y: 50.2484645
    //t0 index = 2, t1 index = 3
    //t0 = 0.08333333333333333, t1 = 0.125
    //t0 percent = 0.36, t1 percent = 0.64

    //siglay 0
    //temp: 3.8427513686286203, 3.8427511613618037
    //salinity: 34.312321112942584, 34.312321144178256
    //dye: 0, 0

    //siglay 3
    //temp: 3.810064503770161, 3.8100655094815705}
    //salinity: 34.31724704864192, 34.31724689708151
    //dye: 0,0,0

    FVCOMStructure::Point p1;
    p1.x = -96.5869768;
    p1.y = 50.2484645;
    p1.h = 0;
    
    FVCOMStructure::Plane plane1 = structure.getTriangleSiglayPlane(13325, 3);
    double height = (-plane1.d - plane1.a * p1.x - plane1.b * p1.y) / plane1.c;
    ModelData data1 = fvcomMultiple.getData(-96.5869768, 50.2484645, 0, 0.11);
    ModelData data2 = fvcomMultiple.getData(-96.5869768, 50.2484645, height, 0.11);


    EXPECT_FLOAT_EQ(3.84275123598, data1.temp);
    EXPECT_FLOAT_EQ(34.3123211329, data1.salt);
    EXPECT_FLOAT_EQ(0.0, data1.dye);

    EXPECT_FLOAT_EQ(3.81006514743, data2.temp);
    EXPECT_FLOAT_EQ(34.3172469516, data2.salt);
    EXPECT_FLOAT_EQ(0.0, data2.dye);
}


TEST(FVCOMTest, DepthInterpolation)
{
    //Node: 6783
    //x: -96.5869768
    //y: 50.2484645
    //h: 1560.905469

    //siglay 5: -0.04330708645284176, -67.5982680907
    //siglay 6: -0.05118110217154026, -79.888862289

    //0.80458781157, 0.19541218843
    //temp: 3.788272445380895, 3.7773765263592396
    //salt: 34.32053112861692, 34.32217315200182
    //dye: 0, 0
    ModelData data1 = fvcomMultiple.getData(-96.5869768, 50.2484645, -70, 0);

    EXPECT_FLOAT_EQ(3.78614325, data1.temp);
    EXPECT_FLOAT_EQ(34.320852, data1.salt);
    EXPECT_FLOAT_EQ(0.0, data1.dye);

}


TEST(FVCOMTest, AllInterpolation)
{
    //Triangle: 9152
    //Nodes: 4664, 4663, 4728 (Index)

    //x: 12555.1604, 13046.3472, 11481.4646
    //y: -9989.55491, -8437.34084, -8810.83762
    //area = 1122788.05072229475
    //418.112538766, 1414.93039512, 1180.65915604
    //a1=271049.91463457,a2=810505.0664852,a3=41233.069582733
    //p1=0.72186826887,p2=0.03672382294,p3=0.24140790816

    //time interp:
    //t0 index = 2, t1 index = 3
    //t0 = 0.08333333333333333, t1 = 0.125
    //t0 percent = 0.36, t1 percent = 0.64

    //siglay interp:
    //siglay 5: -0.04330708645284176
    //siglay 6: -0.05118110217154026


    ModelData data1 = fvcomMultiple.getData(12314, -9648, -89, 0.11 * SECONDS_IN_DAY);

    //siglay: 5, time 2
    //temp: 3.7756120834702616
    //salt: 34.32243905385623

    //siglay: 6, time 2
    //temp: 3.762414280492864
    //salt: 34.32442797292545

    //siglay: 5, time 3
    //temp: 3.775609042581316
    //salt: 34.32243951212053

    //siglay: 6, time 3
    //temp: 3.762410686700008
    //salt: 34.32442851451244

    //siglay 5
    //temp: 3.7756101373013364
    //salt: 34.322439347145384

    //siglay 6
    //temp: 3.762411980465436
    //salt: 34.32442831954112

    //siglay 5 h: -81.8777  0.52157265784
    //siglay 6 h: -96.7646  0.47842734216

    EXPECT_FLOAT_EQ(3.7692957782, data1.temp);
    EXPECT_FLOAT_EQ(34.3233909259, data1.salt);
    EXPECT_FLOAT_EQ(0.0, data1.dye);
}

TEST(FVCOMTest, AllInterpolationOffset)
{
    //Same ass AllInterpolation just with offsets

    double offsetX = 100;
    double offsetY = -150;
    double offsetHeight = 15;
    double offsetTime = 0.2;
    fvcomMultiple.setOffsets(offsetX, offsetY, offsetHeight, offsetTime);

    ModelData data1 = fvcomMultiple.getData(12314 + offsetX, 
                                            -9648 + offsetY, 
                                            -89 + offsetHeight, 
                                            0.11 * SECONDS_IN_DAY + offsetTime);

    EXPECT_FLOAT_EQ(3.7692957782, data1.temp);
    EXPECT_FLOAT_EQ(34.3233909259, data1.salt);
    EXPECT_FLOAT_EQ(0.0, data1.dye);

    //Reset offsets to 0 for next tests
    fvcomMultiple.setOffsets(0, 0, 0, 0);
}

TEST(FVCOMTest, GetDataMultipleFiles) {

	//Test node with times in two files
	//Node: 1000
	//h: 2737.259485
	//siglay: 15
	//time: 4 in 0001_1
	ModelData data1 = fvcomMultiple.getData(8545.73568, -132697.938, 0, 0);
	ModelData data2 = fvcomMultiple.getData(8545.73568, -132697.938, -334.07498037, 0);
	ModelData data3 = fvcomMultiple.getData(8545.73568, -132697.938, -334.07498037, 0.375 * SECONDS_IN_DAY);

	//Test triangle with times in two files
	//Triangle: 1000
	//h: 3123.6463423333335
	//siglay: 15
	//time: 4 in 0001_1
	ModelData data4 = fvcomMultiple.getData(-138453.56466666667, -25886.79643333335, 0, 0);
	ModelData data5 = fvcomMultiple.getData(-138453.56466666667, -25886.79643333335, -381.232432006, 0);
	ModelData data6 = fvcomMultiple.getData(-138453.56466666667, -25886.79643333335, -381.232432006, 0.375 * SECONDS_IN_DAY);


	EXPECT_FLOAT_EQ(3.8386462639531507, data1.temp);
	EXPECT_FLOAT_EQ(34.3129397553773, data1.salt);
	EXPECT_FLOAT_EQ(0.0, data1.dye);

	EXPECT_FLOAT_EQ(3.552034178027694, data2.temp);
	EXPECT_FLOAT_EQ(34.3561324173774, data2.salt);
	EXPECT_FLOAT_EQ(0.0, data2.dye);

	EXPECT_FLOAT_EQ(3.5520324460792474, data3.temp);
	EXPECT_FLOAT_EQ(34.35613267838961, data3.salt);
	EXPECT_FLOAT_EQ(0.0, data3.dye);

	EXPECT_FLOAT_EQ(0.0, data4.u);
	EXPECT_FLOAT_EQ(0.0, data4.v);

	EXPECT_FLOAT_EQ(0.0, data5.u);
	EXPECT_FLOAT_EQ(0.0, data5.v);

	EXPECT_FLOAT_EQ(0.00038683573810392144, data6.u);
	EXPECT_FLOAT_EQ(-0.0019926347599095754, data6.v);


}

TEST(FVCOMTest, OutOfModelBounds)
{
	try {
        fvcomMultiple.getData(300000, 0, 0, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(-300000, 0, 0, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, 300000, 0, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, -300000, 0, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, 0, 1, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, 0, 10000, 0);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, 0, 0, -0.1 * SECONDS_IN_DAY);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }

    try {
        fvcomMultiple.getData(0, 0, 0, 1.0 * SECONDS_IN_DAY);
        FAIL() << "Expected std::out_of_range";
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("FVCOM request outside of model extent"));
    }
}

TEST(FVCOMTest, ModelEdge) {
	//Test node at edge of model
	//Node: 180
	ModelData data = fvcomMultiple.getData(150000, 150000, 0, 0);
	EXPECT_FLOAT_EQ(3.8387627679688694, data.temp);
	EXPECT_FLOAT_EQ(34.31292219813244, data.salt);
	EXPECT_FLOAT_EQ(0.0, data.dye);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
