#include "myutils.h"
#include "dssparams.h"
#include "dss.h"
#include "sort.h"

void DSSParams::SetFromCmdLine()
	{
	if (optset_namedparams)
		SetNamedParams(opt_namedparams);
	else if (optset_paramsf)
		FromTsv(opt_paramsf);
	else 
		{
		if (int(opt_veryfast) +
			int(opt_fast) +
			int(opt_sensitive) +
			int(opt_verysensitive) != 1)
			Die("Must set exactly one of -veryfast -fast -sensitive -verysensitive");

		if (opt_veryfast)
			{
		// SEPQ1=0.0369 S1FP=0.0369 FF=0.4763 FF1=0.4763 UFil=0.0 CFil=89.1 secs=9 [6547e60]
			SetNamedParams("defaults");
			m_Desc = "veryfast";
			m_Omega = 7;
			m_MinU = 3;
			m_USort = true;
			}
		else if (opt_fast)
			{
		// SEPQ1=0.3708 S1FP=0.4257 FF=0.8672 FF1=0.8499 UFil=39.5 CFil=99.3 secs=37 [6547e60]
			SetNamedParams("defaults");
			m_Desc = "fast";
			m_Omega = 20;
			m_MinU = 10;
			m_USort = false;
			}
		else if (opt_sensitive)
			{
		// SEPQ1=0.3928 S1FP=0.4991 FF=0.9411 FF1=0.9127 UFil=0.0 CFil=96.1 secs=61 [6547e60]
			SetNamedParams("defaults");
			m_Desc = "sensitive";
			m_Omega = 11;
			m_MinU = 0;
			m_USort = false;
			}
		else if (opt_verysensitive)
			{
		// SEPQ1=0.3976 S1FP=0.5549 FF=1.0000 FF1=0.9504 UFil=0.0 CFil=0.0 secs=493 [6547e60]
			SetNamedParams("defaults");
			m_Desc = "verysensitive";
			m_Omega = 0;
			m_MinU = 0;
			m_USort = false;
			}
		}

	const int MINUS = -1; // for visual emphasis here
	if (optset_omega) m_Omega = (float) opt_omega;
	if (optset_daliw) m_DALIw = (float) opt_daliw;
	if (optset_lambda) m_Lambda = opt_lambda;
	if (optset_minfwdscore) m_MinFwdScore = float(opt_minfwdscore);
	if (optset_gapopen) m_GapOpen = MINUS*float(opt_gapopen);
	if (optset_gapopen) m_GapExt = MINUS*float(opt_gapext);
	if (optset_minu) m_MinU = opt_minu;
	if (optset_maxaccepts) m_MaxAccepts = opt_maxaccepts;
	if (optset_maxrejects) m_MaxRejects = opt_maxrejects;
	if (optset_usort) m_USort = true;

	if (m_GapOpen > 0 || m_GapExt > 0)
		Die("open=%.3g ext=%.3g, gap penalties must be >= 0",
		  -m_GapOpen, -m_GapExt);

	InitScoreMxs();
	WriteSummary(g_fLog);
	WriteSummary(stderr);
	}

void DSSParams::FromTsv(const string &FileName)
	{
	Clear();
	FILE *f = OpenStdioFile(FileName);
	string Line;
	while (ReadLineStdioFile(f, Line))
		{
		vector<string> Fields;
		Split(Line, Fields, '\t');
		asserta(SIZE(Fields) == 2);
		const string &Name = Fields[0];
		float Value = (float) StrToFloat(Fields[1]);
		SetParam(Name, Value, true);
		}
	CloseStdioFile(f);
	}

void DSSParams::ToFev(FILE *f) const
	{
	if (f == 0)
		return;
	const uint FeatureCount = GetFeatureCount();
	fprintf(f, "NF=%u", FeatureCount);
	for (uint i = 0; i < FeatureCount; ++i)
		{
		FEATURE F = m_Features[i];
		fprintf(f, "\t%s=%.6g", FeatureToStr(F), m_Weights[i]);
		}
	fprintf(f, "\tOpen %.4f", m_GapOpen);
	fprintf(f, "\tExt %.4f", m_GapExt);
	fprintf(f, "\tFwdMatch %.4f", m_FwdMatchScore);
	fprintf(f, "\tMinFwdScore %.4f", m_MinFwdScore);
	fprintf(f, "\tDALIw %.4f", m_DALIw);
	fprintf(f, "\tMAXFX %.4f", m_MAXFX);
	fprintf(f, "\tMinKmerScore %.4f", m_MinKmerScore);
	fprintf(f, "\tX %.4f", m_X);
	fprintf(f, "\tMAXNQNR %u", m_MAXNQNR);
	fprintf(f, "\tMinDiagSpacing %d", m_MinDiagSpacing);
	fprintf(f, "\tMaxDiagSpacing %d", m_MaxDiagSpacing);
	fprintf(f, "\n");
	}

void DSSParams::WriteSummary(FILE *f) const
	{
	if (f == 0)
		return;
	const uint FeatureCount = GetFeatureCount();
	fprintf(f, "Mode %s\n", m_Desc.c_str());
	for (uint i = 0; i < FeatureCount; ++i)
		{
		FEATURE F = m_Features[i];
		if (i > 0 && i%5 == 0)
			fprintf(f, "\n");
		fprintf(f, "%s:%u/%.3f ",
		  FeatureToStr(F), DSS::GetAlphaSize(F), m_Weights[i]);
		}
	fprintf(f, "\n");
	fprintf(f, "Open %.3f", -m_GapOpen);
	fprintf(f, " Ext %.3f", -m_GapExt);
	fprintf(f, " FwdMatch %.3f", m_FwdMatchScore);
	fprintf(f, " MinFwdSc %.3f", m_MinFwdScore);
	if (m_Omega != FLT_MAX)
		fprintf(f, " Omega %.3f", m_Omega);
	fprintf(f, " DALIw %.3f", m_DALIw);
	fprintf(f, "\n");
	}

uint DSSParams::GetFeatureCount() const
	{
	uint n = SIZE(m_Features);
	asserta(SIZE(m_Weights) == n);
	return n;
	}

float DSSParams::GetParam(const string &Name) const
	{
#define x(f)	if (Name == #f) { return m_##f; }
	x(GapOpen);
	x(GapExt);
	x(DALIw);
	x(FwdMatchScore);
	x(MinFwdScore);
#undef x
	for (uint F = 0; F < FEATURE_COUNT; ++F)
		{
		if (Name == FeatureToStr(F))
			{
			uint Idx = GetFeatureIdx(FEATURE(F));
			return m_Weights[Idx];
			}
		}
	Die("GetParam(%s)", Name.c_str());
	return FLT_MAX;
	}

void DSSParams::SetParam(const string &Name, float Value, bool AppendIfWeight)
	{
#define x(f)	if (Name == #f) { m_##f = Value; return; }
	x(GapOpen);
	x(GapExt);
	x(DALIw);
	x(FwdMatchScore);
	x(MinFwdScore);
	x(Omega);
#undef x
	if (AppendIfWeight)
		{
		FEATURE F = StrToFeature(Name.c_str());
		m_Features.push_back(F);
		m_Weights.push_back(Value);
		return;
		}
	else
		{
		for (uint Idx = 0; Idx < SIZE(m_Features); ++Idx)
			{
			FEATURE F = m_Features[Idx];
			if (Name == FeatureToStr(F))
				{
				m_Weights[Idx] = Value;
				return;
				}
			}
		}
	Die("SetParam(%s)", Name.c_str());
	}

uint DSSParams::GetFeatureIdx(FEATURE F) const
	{
	for (uint Idx = 0; Idx < SIZE(m_Features); ++Idx)
		if (m_Features[Idx] == F)
			return Idx;
	Die("GetFeatureIdx(%u)", F);
	return UINT_MAX;
	}

uint DSSParams::GetFeatureIdx_NoError(FEATURE F) const
	{
	for (uint Idx = 0; Idx < SIZE(m_Features); ++Idx)
		if (m_Features[Idx] == F)
			return Idx;
	return UINT_MAX;
	}

void DSSParams::NormalizeWeights()
	{
	float Sum = 0;
	const uint N = SIZE(m_Weights);
	for (uint Idx = 0; Idx < N; ++Idx)
		Sum += m_Weights[Idx];
	float Sum2 = 0;
	for (uint Idx = 0; Idx < N; ++Idx)
		{
		float w = m_Weights[Idx]/Sum;
		m_Weights[Idx] = w;
		Sum2 += w;
		}
	asserta(feq(Sum2, 1.0f));
	}

void DSSParams::InitScoreMxs()
	{
	asserta(m_ScoreMxs == 0);
	uint FeatureCount = GetFeatureCount();
	m_ScoreMxs = myalloc(float **, FEATURE_COUNT);
	for (uint i = 0; i < FEATURE_COUNT; ++i)
		m_ScoreMxs[i] = 0;
	for (uint Idx = 0; Idx < FeatureCount; ++Idx)
		{
		FEATURE F = m_Features[Idx];
		asserta(uint(F) < FEATURE_COUNT);
		uint AS = g_AlphaSizes2[F];
		m_ScoreMxs[F] = myalloc(float *, AS);
		for (uint Letter1 = 0; Letter1 < AS; ++Letter1)
			{
			m_ScoreMxs[F][Letter1] = myalloc(float, AS);
#if DEBUG
			for (uint Letter2 = 0; Letter2 < AS; ++Letter2)
				m_ScoreMxs[F][Letter1][Letter2] = FLT_MAX;
#endif
			}
		}
	ApplyWeights();
	}

float DSSParams::ScoreToEvalue(float Score, uint QL) const
	{
	asserta(m_DBSize != 0 && m_DBSize != FLT_MAX);
	const float Slope = -6.6f; // -7.3f;
	const float Intercept = 6.1f;
	float x = Score/(QL + m_Lambda);
	float logNF = Slope*x + Intercept;
	float NF = powf(10, logNF);
	float Evalue = NF*m_DBSize/1e8f;
	return Evalue;
	}

void DSSParams::ApplyWeights()
	{
	asserta(m_ScoreMxs != 0);
	uint FeatureCount = GetFeatureCount();
	for (uint Idx = 0; Idx < FeatureCount; ++Idx)
		{
		FEATURE F = m_Features[Idx];
		asserta(uint(F) < FEATURE_COUNT);
		float w = m_Weights[Idx];
		uint AS = g_AlphaSizes2[F];
		m_ScoreMxs[F] = myalloc(float *, AS);
		for (uint Letter1 = 0; Letter1 < AS; ++Letter1)
			{
			m_ScoreMxs[F][Letter1] = myalloc(float, AS);
			for (uint Letter2 = 0; Letter2 < AS; ++Letter2)
				m_ScoreMxs[F][Letter1][Letter2] = w*g_ScoreMxs2[F][Letter1][Letter2];
			}
		}
	}
