#include "myutils.h"
#include "dssparams.h"

void DSSParams::FromParamStr(const string &Str)
	{
	Clear();
	m_Desc = "FromParamStr";
	vector<string> Fields;
	Split(Str, Fields, '_');

	m_GapOpen = -1.5f;
	m_GapExt = -0.42f;
	m_DALIw = 0;
	m_FwdMatchScore = 0;
	m_MinFwdScore = 0;
	m_MinComboFwdScore = 0;
	m_Omega = 0;
	m_Lambda = 0;
	m_MinU = 0;
	m_USort = false;
	m_MaxAccepts = UINT_MAX;
	m_MaxRejects = UINT_MAX;
	m_PatternStr = "*";

	const uint N = SIZE(Fields);
	for (uint i = 0; i < N; ++i)
		{
		const string &Field = Fields[i];
		vector<string> Fields2;
		Split(Field, Fields2, ':');
		asserta(SIZE(Fields2) == 2);
		const string &Name = Fields2[0];
		double w = StrToFloat(Fields2[1]);
		FEATURE F = StrToFeature(Name.c_str());
		AddFeature(F, w);
		}
	}

void DSSParams::SetNamedParams(const string &Name)
	{
	Clear();
	m_Desc = Name;

	if (Name == "aa")
		{
		m_AAOnly = true;

		AddFeature(FEATURE_AA,			1);

		m_GapOpen = -1.5f;
		m_GapExt = -0.42f;
		m_DALIw = 0;
		m_FwdMatchScore = 0;
		m_MinFwdScore = 0;
		m_MinComboFwdScore = 0;
		m_Omega = 0;
		m_Lambda = 0;
		m_MinU = 0;
		m_USort = false;
		m_MaxAccepts = UINT_MAX;
		m_MaxRejects = UINT_MAX;
		m_PatternStr = "*";
		return;
		}

	if (Name == "defaults")
		{
		m_Desc.clear();
		AddFeature(FEATURE_AA,			0.398145);
		AddFeature(FEATURE_NbrDist,		0.129367);
		AddFeature(FEATURE_MySS,		0.202354);
		AddFeature(FEATURE_NbrMySS,		0.149383);
		AddFeature(FEATURE_RevNbrDist,	0.0937677);
		AddFeature(FEATURE_DstNxtHlx,	0.00475462);
		AddFeature(FEATURE_StrandDens,	0.0183853);
		AddFeature(FEATURE_NormDens,	0.00384384);

		m_GapOpen = -0.685533f;
		m_GapExt = -0.051881f;
		m_DALIw = 3.0f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_MinComboFwdScore = 7.0f;
		m_Omega = 29;
		m_OmegaFwd = 29;
		m_Lambda = 32;
		m_MinU = 0;
		m_USort = false;
		m_MaxAccepts = 1;
		m_MaxRejects = 32;
		m_PatternStr = "10000000001";

//		if (0) // opt_fast
//			{
////SEPQ0.1=0.1444 SEPQ1=0.2553 SEPQ10=0.3504 S1FP=0.3197 N1FP=145363 secs=36 level=sf fast [d1307e2] uncommented
////SEPQ0.1=0.1557 SEPQ1=0.2522 SEPQ10=0.2522 S1FP=0.2460 N1FP=111877 secs=35 level=sf fast [d1307e2] commented
//			//m_GapOpen = -0.325218;
//			//m_GapExt = -0.0165677;
//			//m_FwdMatchScore = 0.537963;
//			//m_DALIw = 5.47414;
//			//SetParam("NbrDist", 0.154403, false);
//			//SetParam("MySS", 0.240193, false);
//			//SetParam("NbrMySS", 0.147806, false);
//			//SetParam("RevNbrDist", 0.102264, false);
//			//SetParam("DstNxtHlx", 0.0110055, false);
//			//SetParam("StrandDens", 0.0136859, false);
//			//SetParam("NormDens", 0.00594393, false);
//			}
//		else if (opt_veryfast)
//			{
//			m_Desc += "-veryfast";
//			m_Omega = 16;
//			m_MinU = 4;
//			m_USort = true;
//			}
//		else if (opt_sensitive)
//			{
//			m_Desc += "-sensitive";
//			m_Omega = 0;
//			m_MinU = 0;
//			m_USort = false;
//			}
		return;
		}

	if (Name == "old_defaults")
		{
		AddFeature(FEATURE_AA,			0.561938);
		AddFeature(FEATURE_NbrDist,		0.183907);
		AddFeature(FEATURE_MySS,		0.112387);
		AddFeature(FEATURE_NbrMySS,		0.033476);
		AddFeature(FEATURE_RevNbrDist,	0.0415593);
		AddFeature(FEATURE_DstNxtHlx,	0.00674395);
		AddFeature(FEATURE_StrandDens,	0.0209877);
		AddFeature(FEATURE_NormDens,	0.0390013);

		m_GapOpen = -1.099f;
		m_GapExt = -0.143f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_MinComboFwdScore = 7.0f;
		m_Omega = 11;
		m_Lambda = 32;
		m_MinU = 15;
		m_USort = false;
		m_MaxAccepts = 1;
		m_MaxRejects = 32;
		m_PatternStr = "10000000001";
		return;
		}

// SensEPQ1 0.4936 Sens1FP 57259 (0.6017) time 02:00 mode cf SW 4.6%
	if (Name == "test_5w")
		{
		AddFeature(FEATURE_AA,			0.617453);
		AddFeature(FEATURE_NbrDist,		0.125706);
		AddFeature(FEATURE_MySS,		0.127637);
		AddFeature(FEATURE_NbrMySS,		0.0672284);
		AddFeature(FEATURE_RevNbrDist,	0.0619757);

		m_GapOpen = -0.945997f;
		m_GapExt = -0.12f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_Omega = 10;
		return;
		}

// SensEPQ1 0.4923 Sens1FP 57080 (0.5998) time 01:54 mode cf SW 4.6%
	if (Name == "test_4w")
		{
		AddFeature(FEATURE_AA,			0.637597);
		AddFeature(FEATURE_NbrDist,		0.157105);
		AddFeature(FEATURE_MySS,		0.159157);
		AddFeature(FEATURE_NbrMySS,		0.0610495);

		m_GapOpen = -0.947013f;
		m_GapExt = -0.131341f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_Omega = 10;
		return;
		}

// SensEPQ1 0.4930 Sens1FP 57015 (0.5991) time 01:56 mode cf SW 4.6% [f7f5af4]
	if (Name == "test_3w")
		{
		AddFeature(FEATURE_AA,			0.688014);
		AddFeature(FEATURE_NbrDist,		0.140244);
		AddFeature(FEATURE_MySS,		0.171742);

		m_GapOpen = -0.861921f;
		m_GapExt = -0.156609f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_Omega = 10;
		return;
		}

// SensEPQ1 0.4885 Sens1FP 56050 (0.5890) time 01:55 mode cf SW 4.6% [27e8bd9]
	if (Name == "test_2w")
		{
		AddFeature(FEATURE_AA,			0.734078);
		AddFeature(FEATURE_MySS,		0.265922);

		m_GapOpen = -0.861921f;
		m_GapExt = -0.156609f;
		m_DALIw = 2.78145f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_Omega = 10;
		return;
		}


// SensEPQ1 0.4928 Sens1FP 57445 (0.6037) time 02:00 mode cf SW 4.6%  [daebbfb]
	if (Name == "57445_6w")
		{
		AddFeature(FEATURE_AA,			0.560703);
		AddFeature(FEATURE_NbrDist,		0.166875);
		AddFeature(FEATURE_MySS,		0.115906);
		AddFeature(FEATURE_NbrMySS,		0.0610495);
		AddFeature(FEATURE_RevNbrDist,	0.0562796);
		AddFeature(FEATURE_NormDens,	0.0391864);

		m_GapOpen = -1.09f;
		m_GapExt = -0.12f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_PatternStr = "10000000001";
		m_Omega = 10;
		return;
		}

	if (Name == "62107_7w") // verified in [24a4011]
		{
		AddFeature(FEATURE_AA,			0.564048);//+
		AddFeature(FEATURE_NbrDist,		0.181301);//+
		AddFeature(FEATURE_MySS,		0.110795);//+
		AddFeature(FEATURE_NbrMySS,		0.0456908);//+
		AddFeature(FEATURE_RevNbrDist,	0.0402891);//+
		AddFeature(FEATURE_StrandDens,	0.0175873);//++
		AddFeature(FEATURE_NormDens,	0.0402891);//++

		m_GapOpen = -1.1f;
		m_GapExt = -0.143f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_PatternStr = "10000000001";
		m_Omega = 7.5f;
		return;
		}

	if (Name == "norev_62250")
		{
		AddFeature(FEATURE_AA,			0.562541);
		AddFeature(FEATURE_NbrDist,		0.120009);
		AddFeature(FEATURE_MySS,		0.132363);
		AddFeature(FEATURE_NbrMySS,		0.0451798);
		AddFeature(FEATURE_RevNbrDist,	0.0661813);
		AddFeature(FEATURE_RevNbrMySS,	0.0254136);
		AddFeature(FEATURE_DstNxtHlx,	0.0141187);
		AddFeature(FEATURE_StrandDens,	0.0176484);
		AddFeature(FEATURE_NormDens,	0.0165453);

		m_GapOpen = -0.8832f;
		m_GapExt = -0.1348f;
		m_DALIw = 2.4f;
		m_FwdMatchScore = 0.1f;
		m_MinFwdScore = 7.0f;
		m_PatternStr = "10000000001";
		return;
		}

	Die("SetNamedParams(%s)", Name.c_str());
	}
