#include "myutils.h"

static const uint feature_count = 34;
static const uint sample_count = 19;

// y = x W^T + b
//  W^T = transpose of weight matrix
static const double linear_one_weight[2][34] = {
	{  15.5764,    1.5172,   -0.8334,   -0.5254,   -0.0555,   -0.3513,   -0.3427,    0.0166,   -0.2262,   -0.2856,   -0.5742,   -0.7330,   -0.1665,    0.4181,   -0.2544,   -0.4851,   -0.0166,    0.4540,   -0.3925,    0.5741,   -3.0492,   -0.2624,   -1.4809,    0.6170,    0.0886,    2.1249,   -1.0097,   -2.9409,    1.4552,   -3.9386,   -0.9709,   -3.0962,   -0.2577,   -0.2298},
	{ -15.5837,   -1.5757,    0.6378,    0.4783,    0.1607,    0.3676,    0.3474,    0.0490,    0.2467,    0.4125,    0.4966,    0.6708,    0.1538,   -0.5176,    0.1997,    0.4294,    0.0956,   -0.7383,    0.4517,   -0.5977,    3.0914,    0.1762,    1.2789,   -0.3606,   -0.0715,   -2.0539,    1.0915,    3.1529,   -1.5675,    4.0048,    0.7075,    2.9155,    0.2396,    0.3444},
};
static const double linear_one_bias[2] = {  -1.0953,    1.0740 };

static const double sample_data[19][34] = {
	{   0.2050,    0.1083,    0.1592,    0.4603,    0.9208,    0.9957,    0.6108,    0.7445,    0.5042,    0.5871,    0.6619,    0.4624,    0.5148,    0.3284,    0.3223,    0.2399,    0.2576,    0.1445,    0.0874,    0.0893,    0.0684,    0.1402,    0.1394,    0.1149,    0.1281,    0.0704,    0.0902,    0.1426,    0.0723,    0.0682,    0.0634,    0.0710,    0.0938,    1.0000},
	{   0.1270,    0.1113,    0.4138,    0.9640,    1.0000,    0.5412,    0.2149,    0.1796,    0.1004,    0.0150,    0.0192,    0.0101,    0.0044,    0.0048,    0.0043,    0.0066,    0.0023,    0.0047,    0.0000,    0.0051,    0.0000,    0.0076,    0.0028,    0.0027,    0.0027,    0.0000,    0.0000,    0.0000,    0.0000,    0.0022,    0.0000,    0.0000,    0.0000,    0.0023},
	{   0.1630,    0.1206,    0.2968,    0.9764,    0.8808,    0.7607,    0.4707,    0.3043,    0.2264,    0.1057,    0.0795,    0.0430,    0.0178,    0.0080,    0.0049,    0.0062,    0.0104,    0.0059,    0.0231,    0.0086,    0.0127,    0.0039,    0.0038,    0.0023,    0.0116,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0022},
	{   0.2480,    0.0186,    0.0000,    0.0609,    0.1433,    0.2666,    0.7138,    0.7206,    0.6038,    0.7399,    0.7047,    0.9705,    0.5271,    0.6130,    0.4787,    0.3263,    0.3889,    0.3131,    0.1386,    0.1987,    0.1528,    0.0480,    0.0000,    0.0345,    0.0164,    0.0687,    0.0000,    0.0147,    0.0364,    0.0570,    0.0548,    0.0682,    0.0138,    0.2798},
	{   0.2790,    0.0000,    0.0000,    0.0701,    0.1241,    0.2104,    0.3713,    0.7740,    0.9379,    0.8880,    0.9565,    0.7185,    0.6070,    0.6535,    0.5318,    0.3387,    0.3808,    0.3576,    0.2766,    0.2368,    0.1250,    0.0643,    0.1002,    0.0540,    0.1642,    0.0996,    0.0564,    0.0142,    0.0278,    0.0169,    0.0436,    0.0186,    0.0157,    0.2781},
	{   0.1150,    0.6588,    0.8789,    0.8476,    0.4563,    0.1261,    0.0132,    0.0033,    0.0031,    0.0000,    0.0039,    0.0081,    0.0043,    0.0034,    0.0000,    0.0000,    0.0065,    0.0210,    0.0033,    0.0000,    0.0000,    0.0034,    0.0165,    0.0040,    0.0000,    0.0032,    0.0076,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0220},
	{   0.2790,    0.0297,    0.0575,    0.1596,    0.2389,    0.4221,    0.6428,    1.0000,    0.9747,    0.7923,    0.6804,    0.2730,    0.2274,    0.1388,    0.0532,    0.0392,    0.0200,    0.0406,    0.0147,    0.0215,    0.0059,    0.0151,    0.0108,    0.0000,    0.0000,    0.0149,    0.0167,    0.0101,    0.0215,    0.0057,    0.0101,    0.0135,    0.0062,    0.2166},
	{   0.1990,    0.2615,    0.5701,    0.9127,    0.9675,    0.6416,    0.5124,    0.3898,    0.3617,    0.2155,    0.1583,    0.1282,    0.0878,    0.0726,    0.0741,    0.0743,    0.0923,    0.0950,    0.0968,    0.1241,    0.0933,    0.0385,    0.0487,    0.0424,    0.0177,    0.0141,    0.0000,    0.0135,    0.0065,    0.0126,    0.0074,    0.0068,    0.0131,    0.1021},
	{   0.2860,    0.0000,    0.0000,    0.0111,    0.0000,    0.0393,    0.0847,    0.0915,    0.2313,    0.3379,    0.6954,    0.8717,    1.0000,    0.8661,    0.8531,    0.7184,    0.6884,    0.4750,    0.5022,    0.3556,    0.3532,    0.2750,    0.1051,    0.2302,    0.0761,    0.0350,    0.0344,    0.0327,    0.0087,    0.0109,    0.0248,    0.0092,    0.0000,    0.0770},
	{   0.1620,    0.0734,    0.2552,    0.6672,    0.9101,    1.0000,    0.7151,    0.5048,    0.6087,    0.5942,    0.7946,    0.8126,    0.4406,    0.5371,    0.5015,    0.3586,    0.1694,    0.1349,    0.2513,    0.1304,    0.1091,    0.1058,    0.0624,    0.0982,    0.0418,    0.0332,    0.0298,    0.0150,    0.0086,    0.0000,    0.0000,    0.0065,    0.0073,    0.6293},
	{   0.1130,    0.0687,    0.2882,    0.5941,    0.9803,    0.9229,    1.0000,    0.6676,    0.5150,    0.3283,    0.2472,    0.1157,    0.0786,    0.0451,    0.0249,    0.0040,    0.0037,    0.0043,    0.0082,    0.0037,    0.0000,    0.0038,    0.0097,    0.0000,    0.0066,    0.0000,    0.0073,    0.0000,    0.0106,    0.0038,    0.0000,    0.0165,    0.0000,    0.0354},
	{   0.1420,    0.0119,    0.0766,    0.1329,    0.2808,    0.5573,    0.8316,    0.9997,    1.0000,    0.7779,    0.7992,    0.8141,    0.4794,    0.3940,    0.3015,    0.2004,    0.1993,    0.0770,    0.0850,    0.0318,    0.0074,    0.0000,    0.0159,    0.0000,    0.0000,    0.0000,    0.0000,    0.0063,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0389},
	{   0.1670,    0.0091,    0.0599,    0.1295,    0.2540,    0.4340,    0.6961,    1.0000,    0.8350,    0.9697,    0.9312,    0.5686,    0.5090,    0.5850,    0.3182,    0.1981,    0.1303,    0.1122,    0.0720,    0.0453,    0.0258,    0.0183,    0.0000,    0.0237,    0.0220,    0.0000,    0.0105,    0.0098,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0104},
	{   0.1270,    0.0373,    0.0513,    0.3083,    0.4054,    0.8377,    1.0000,    0.9708,    0.8619,    0.5679,    0.2828,    0.2259,    0.0834,    0.0702,    0.0362,    0.0207,    0.0137,    0.0000,    0.0000,    0.0044,    0.0000,    0.0000,    0.0000,    0.0049,    0.0046,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0458},
	{   0.1410,    0.2422,    0.4479,    1.0000,    0.9792,    0.8277,    0.8593,    0.7508,    0.7908,    0.5377,    0.2366,    0.2169,    0.1770,    0.0635,    0.0924,    0.0660,    0.0582,    0.0648,    0.0339,    0.0160,    0.0327,    0.0474,    0.0145,    0.0159,    0.0099,    0.0075,    0.0066,    0.0041,    0.0000,    0.0000,    0.0032,    0.0000,    0.0000,    0.0255},
	{   0.1570,    0.0055,    0.0436,    0.1092,    0.1492,    0.4084,    0.4944,    0.6023,    1.0000,    0.9114,    0.9537,    0.7767,    0.7217,    0.5251,    0.3420,    0.2427,    0.1841,    0.0784,    0.0733,    0.0320,    0.0041,    0.0138,    0.0226,    0.0226,    0.0162,    0.0000,    0.0086,    0.0238,    0.0053,    0.0317,    0.0222,    0.0274,    0.0305,    0.0556},
	{   0.1100,    0.0000,    0.0044,    0.0287,    0.0511,    0.1108,    0.2204,    0.4276,    0.9178,    1.0000,    0.7942,    0.7616,    0.3683,    0.2803,    0.1862,    0.1098,    0.0755,    0.0295,    0.0477,    0.0120,    0.0083,    0.0080,    0.0054,    0.0085,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0041},
	{   0.1970,    0.0000,    0.0315,    0.1021,    0.1609,    0.4230,    0.6122,    1.0000,    0.8719,    1.0000,    0.9468,    1.0000,    0.4836,    0.4243,    0.2654,    0.0646,    0.0796,    0.0244,    0.0079,    0.0000,    0.0289,    0.0190,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0000,    0.0080,    0.0000,    0.0000,    0.0000,    0.1343},
	{   0.2190,    0.2199,    0.6345,    0.8571,    1.0000,    0.6230,    0.6740,    0.4359,    0.3300,    0.3277,    0.2048,    0.1433,    0.0511,    0.0916,    0.0479,    0.0595,    0.0625,    0.0307,    0.0966,    0.0682,    0.1021,    0.0370,    0.0597,    0.0601,    0.1228,    0.1066,    0.0365,    0.0751,    0.0802,    0.0423,    0.0191,    0.0279,    0.0199,    0.1131},
};
static const int ys[19] = {
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

// y = x W^T + b
//  W^T = transpose of weight matrix
static void forward(const vector<double> &x, vector<double> &yhat)
	{
	yhat.clear();
	asserta(SIZE(x) == feature_count);
	for (uint i = 0; i < 2; ++i)
		{
		double Sum = 0;
		for (uint j = 0; j < feature_count; ++j)
			Sum += x[j]*linear_one_weight[i][j];
		Sum += linear_one_bias[i];
		yhat.push_back(Sum);
		}
	}

static void softmax(const vector<double> &x, vector<double> &y)
	{
	y.clear();
	const uint n = SIZE(x);
	double Sum = 0;
	for (uint i = 0; i < n; ++i)
		Sum += exp(x[i]);
	if (Sum == 0)
		Sum = 1;
	for (uint i = 0; i < n; ++i)
		y.push_back(exp(x[i])/Sum);
	}

static void log_softmax(const vector<double> &x, vector<double> &y)
	{
	y.clear();
	const uint n = SIZE(x);
	double Sum = 0;
	for (uint i = 0; i < n; ++i)
		Sum += exp(x[i]);
	if (Sum == 0)
		Sum = 1;
	for (uint i = 0; i < n; ++i)
		y.push_back(x[i] - log(Sum));
	}

double GetPNN(uint Idx, double TS)
	{
	asserta(Idx < sample_count);
	vector<double> x;
	x.push_back(TS);
	for (uint j = 1; j < feature_count; ++j)
		x.push_back(sample_data[Idx][j]);
	vector<double> yhat;
	forward(x, yhat);

	vector<double> ysm;
	softmax(yhat, ysm);
	return ysm[0];
	}

void cmd_testmodel()
	{
	for (uint i = 0; i < sample_count; ++i)
		{
		vector<double> x;
		for (uint j = 0; j < feature_count; ++j)
			x.push_back(sample_data[i][j]);

		vector<double> yhat;
		forward(x, yhat);

		vector<double> ysm;
		softmax(yhat, ysm);

		vector<double> ylsm;
		log_softmax(yhat, ylsm);
		Log("Sample %3u", i);
		Log("  y=%u", ys[i]);
		Log("  yhat=(%8.3g, %8.3g)", yhat[0], yhat[1]);
		Log("  ysm=(%8.3g, %8.3g)", ysm[0], ysm[1]);
		Log("  ylsm=(%8.3g, %8.3g)", ylsm[0], ylsm[1]);
		Log("  x=");
		for (uint j = 0; j < feature_count; ++j)
			{
			if (j > 0)
				Log(",");
			Log(" %.3g", x[j]);
			}
		Log("\n");
		}
	}
