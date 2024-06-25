#include "myutils.h"
#include "scop40bench.h"
#include "mx.h"
#include "pdbchain.h"
#include "xdpmem.h"
#include "cigar.h"
#include "timing.h"
#include "sort.h"
#include <algorithm>
#include <random>
#include <thread>
#include <set>

void GetPathCounts(const string &Path, uint &M, uint &D, uint &I);
uint GetU(const vector<uint> &KmersQ, const vector<uint> &KmersR);
uint GetUBits(const vector<uint> &KmerBitsQ, const vector<uint> &KmerBitsR);

void SCOP40Bench::GetDomFromLabel(const string &Label, string &Dom)
	{
	vector<string> Fields;
	Split(Label, Fields, '/');
	Dom = Fields[0];
	}

void SCOP40Bench::ParseScopLabel(const string &Label, string &Dom,
 string &Cls, string &Fold, string &SF, string &Fmy, bool MissingOk)
	{
	vector<string> Fields;
	Split(Label, Fields, '/');
	if (SIZE(Fields) == 1)
		{
		if (!MissingOk)
			Die("ParseScopLabel, SCOP id missing >%s\n", Label.c_str());
		Dom = Fields[0];
		Cls = "-";
		Fold = "-";
		SF = "-";
		Fmy = "-";
		}
	else if (SIZE(Fields) == 2)
		{
		Dom = Fields[0];
		const string &ScopId = Fields[1];
	// Scop identifier looks like a.1.2.3 
	//     a    1           2      3
	// class.fold.superfamily.family
		vector<string> Fields2;
		Split(ScopId, Fields2, '.');
		if (SIZE(Fields2) != 4)
			Die("ParseScopLabel, bad SCOP id >%s\n", Label.c_str());
		Cls = Fields2[0];
		Fold = Fields2[0] + "." + Fields2[1];
		SF = Fields2[0] + "." + Fields2[1] + "." + Fields2[2];
		Fmy = Fields2[0] + "." + Fields2[1] + "." +  Fields2[2] + "." + Fields2[3];
		}
	else
		Die("ParseScopLabel, bad format >%s\n", Label.c_str());
	}

//void SCOP40Bench::SetSFSizes()
//	{
//	GetSFSizes(m_SFSizes);
//	}

uint SCOP40Bench::GetDomIdx(const string &Dom_or_DomSlashId,
  bool FailOnErr) const
	{
	vector<string> Fields;
	Split(Dom_or_DomSlashId, Fields, '/');
	const string &Dom = Fields[0];
	map<string, uint>::const_iterator iter = m_DomToIdx.find(Dom);
	if (iter == m_DomToIdx.end())
		{
		if (FailOnErr)
			Die("GetDomIdx(%s)", Dom_or_DomSlashId.c_str());
		return UINT_MAX;
		}
	uint DomIdx = iter->second;
	return DomIdx;
	}

const vector<vector<byte> > &SCOP40Bench::GetProfileByDomIdx(uint DomIdx) const
	{
	asserta(DomIdx < SIZE(m_Profiles));
	asserta(DomIdx < SIZE(m_DomIdxToChainIdx));
	uint ChainIdx = m_DomIdxToChainIdx[DomIdx];
	asserta(ChainIdx < SIZE(m_Profiles));
	return m_Profiles[ChainIdx];
	}

const PDBChain &SCOP40Bench::GetChainByDomIdx(uint DomIdx) const
	{
	asserta(DomIdx < SIZE(m_DomIdxToChainIdx));
	uint ChainIdx = m_DomIdxToChainIdx[DomIdx];
	asserta(ChainIdx < SIZE(m_Chains));
	return *m_Chains[ChainIdx];
	}

void SCOP40Bench::OnSetup()
	{
	if (optset_benchlevel)
		m_Level = opt_benchlevel;

	asserta(m_QuerySelf);
	asserta(m_ChainCount == m_QueryChainCount);
	asserta(m_DBChainCount == 0);
	if (opt_scores_are_not_evalues)
		m_ScoresAreEvalues = false;
	else
		m_ScoresAreEvalues = true;
	BuildDomSFIndexesFromQueryChainLabels();
	}

void SCOP40Bench::AddDom(const string &Dom, const string &Fold, const string &SF,
  uint ChainIndex)
	{
	uint SFIdx = UINT_MAX;
	if (m_SFToIdx.find(SF) == m_SFToIdx.end())
		{
		SFIdx = SIZE(m_SFs);
		m_SFs.push_back(SF);
		m_SFToIdx[SF] = SFIdx;
		}
	else
		SFIdx = m_SFToIdx[SF];

	uint FoldIdx = UINT_MAX;
	if (m_FoldToIdx.find(Fold) == m_FoldToIdx.end())
		{
		FoldIdx = SIZE(m_Folds);
		m_Folds.push_back(Fold);
		m_FoldToIdx[Fold] = FoldIdx;
		}
	else
		FoldIdx = m_FoldToIdx[Fold];
 
	uint DomIdx = UINT_MAX;
	if (m_DomToIdx.find(Dom) != m_DomToIdx.end())
		{
		Die("Duplicate dom >%s", Dom.c_str());
		DomIdx = m_DomToIdx[Dom];
		}
	DomIdx = SIZE(m_Doms);
	m_Doms.push_back(Dom + "/" + SF);
	m_DomToIdx[Dom] = DomIdx;
	m_DomIdxToSFIdx.push_back(SFIdx);
	m_DomIdxToFoldIdx.push_back(FoldIdx);

	m_DomIdxs.push_back(DomIdx);
	m_DomToChainIdx[Dom] = ChainIndex;
	if (ChainIndex != UINT_MAX)
		{
		asserta(DomIdx < SIZE(m_DomIdxToChainIdx));
		m_DomIdxToChainIdx[DomIdx] = ChainIndex;
		}
	}

void SCOP40Bench::ReadLookup(const string &FileName)
	{
	FILE *f = OpenStdioFile(FileName);
	string Line;
	vector<string> Fields;
	vector<string> Fields2;
	while (ReadLineStdioFile(f, Line))
		{
		Split(Line, Fields, '\t');
		asserta(SIZE(Fields) == 2);
		const string &Dom = Fields[0];
		const string &ScopId = Fields[1];
		Split(ScopId, Fields2, '.');
		asserta(SIZE(Fields2) == 4);
		const string &Cls_NOTUSED = Fields2[0];
		const string &Fold = Fields2[0] + "." + Fields2[1];
		const string &SF = Fields2[0] + "." + Fields2[1] + "." + Fields2[2];
		const string &Fmy_NOTUSED = Fields2[0] + "." + Fields2[1] + "." +  Fields2[2] + "." + Fields2[3];
		AddDom(Dom, Fold, SF);
		}
	CloseStdioFile(f);
	}

void SCOP40Bench::BuildDomSFIndexesFromQueryChainLabels()
	{
	m_DomIdxToChainIdx.clear();
	m_DomIdxToChainIdx.resize(m_ChainCount, UINT_MAX);
	for (uint ChainIndex = 0; ChainIndex < m_QueryChainCount; ++ChainIndex)
		{
		ProgressStep(ChainIndex, m_ChainCount, "Index chain labels");
		const PDBChain &Chain = *m_Chains[ChainIndex];

		string Dom;
		string Cls;
		string SF;
		string Fmy;
		string Fold;
		ParseScopLabel(Chain.m_Label, Dom, Cls, Fold, SF, Fmy);
		AddDom(Dom, Fold, SF, ChainIndex);
		}

	asserta(SIZE(m_DomIdxs) == m_QueryChainCount);
	asserta(SIZE(m_DomIdxToSFIdx) == m_QueryChainCount);
	asserta(SIZE(m_DomIdxToFoldIdx) == m_QueryChainCount);
	}

void SCOP40Bench::LogFirstFewDoms() const
	{
	for (uint i = 0; i < 10; ++i)
		Log("[%5u]  %s\n", i, m_Doms[i].c_str());
	}

void SCOP40Bench::LogFirstFewHits() const
	{
	for (uint i = 0; i < 10; ++i)
		{
		uint Dom1 = m_DomIdx1s[i];
		uint Dom2 = m_DomIdx2s[i];
		Log("[%5u]  ", i);
		Log("  Dom1=%s/%5u", m_Doms[Dom1].c_str(), Dom1);
		Log("  Dom2=%s/%5u", m_Doms[Dom2].c_str(), Dom2);
		Log("  Score=%.3g\n", m_Scores[i]);
		}
	}

void SCOP40Bench::StoreScore(uint ChainIndex1, uint ChainIndex2,
  float ScoreAB, float TSAB)
	{
	if (TSAB <= 0)
		return;
	uint DomIdx1 = m_DomIdxs[ChainIndex1];
	uint DomIdx2 = m_DomIdxs[ChainIndex2];
	asserta(TSAB > 0);
	m_Scores.push_back(ScoreAB);
	m_TSs.push_back(TSAB);
	m_DomIdx1s.push_back(DomIdx1);
	m_DomIdx2s.push_back(DomIdx2);
	}

float SCOP40Bench::AlignDomPair(uint ThreadIndex,
  uint Dom1, uint Dom2, uint &Lo1, uint &Lo2, string &Path)
	{
	Lo1 = UINT_MAX;
	Lo2 = UINT_MAX;
	Path.clear();

	const PDBChain &Chain1 = GetChainByDomIdx(Dom1);
	const PDBChain &Chain2 = GetChainByDomIdx(Dom2);

	const vector<vector<byte> > &Profile1 = GetProfileByDomIdx(Dom1);
	const vector<vector<byte> > &Profile2 = GetProfileByDomIdx(Dom2);

	asserta(ThreadIndex < SIZE(m_DAs));
	DSSAligner &DA = *m_DAs[ThreadIndex];
	DA.SetQuery(Chain1, &Profile1, 0, 0, FLT_MAX, FLT_MAX);
	DA.SetTarget(Chain2, &Profile2, 0, 0, FLT_MAX, FLT_MAX);
	DA.Align_NoAccel();
	Lo1 = DA.m_LoA;
	Lo2 = DA.m_LoB;
	Path = DA.m_PathA;
	return DA.m_EvalueA;
	}

void SCOP40Bench::OnAln(uint ChainIndex1, uint ChainIndex2, DSSAligner &DA)
	{
	StoreScore(ChainIndex1, ChainIndex2, DA.m_EvalueA, DA.m_TestStatisticA);
	}

void SCOP40Bench::OnAlnBA(uint ChainIndex1, uint ChainIndex2, DSSAligner &DA)
	{
	StoreScore(ChainIndex2, ChainIndex1, DA.m_EvalueB, DA.m_TestStatisticB);
	}

void SCOP40Bench::SetSFIdxToDomIdxs()
	{
	uint SFCount = GetSFCount();
	uint DomCount = GetDomCount();
	m_SFIdxToDomIdxs.clear();
	m_SFIdxToDomIdxs.resize(SFCount);
	for (uint DomIdx = 0; DomIdx < DomCount; ++DomIdx)
		{
		uint SFIdx = m_DomIdxToSFIdx[DomIdx];
		m_SFIdxToDomIdxs[SFIdx].push_back(DomIdx);
		}
	}

void SCOP40Bench::SetDomIdxToL()
	{
	m_DomIdxToL.clear();
	uint DomCount = GetDomCount();
	for (uint DomIdx = 0; DomIdx < DomCount; ++DomIdx)
		{
		const string &Label = m_Doms[DomIdx];
		vector<string> Fields;
		Split(Label, Fields, '/');
		const string &Dom = Fields[0];
		map<string, uint>::const_iterator iter =
		  m_DomToChainIdx.find(Dom);
		asserta(iter != m_DomToChainIdx.end());
		const PDBChain &Chain = *m_Chains[iter->second];
		m_DomIdxToL.push_back(Chain.GetSeqLength());
		}
	}

void SCOP40Bench::SetDomIdxToHitIdxs()
	{
	uint DomCount = GetDomCount();
	uint HitCount = GetHitCount();
	m_DomIdxToHitIdxs.clear();
	m_DomIdxToHitIdxs.resize(DomCount);
	for (uint HitIdx = 0; HitIdx < HitCount; ++HitIdx)
		{
		uint DomIdx1 = m_DomIdx1s[HitIdx];
		m_DomIdxToHitIdxs[DomIdx1].push_back(HitIdx);
		}
	}

void SCOP40Bench::ClearHits()
	{
	m_Scores.clear();
	m_DomIdx1s.clear();
	m_DomIdx2s.clear();
	}

float SCOP40Bench::GetVeryGoodScore() const
	{
	if (m_ScoresAreEvalues)
		return 0;
	else
		return 999999.9f;
	}

float SCOP40Bench::GetVeryBadScore() const
	{
	if (m_ScoresAreEvalues)
		return 999999.9f;
	else
		return -999999.9f;
	}

bool SCOP40Bench::HitIsBetter(uint HitIdx1, uint HitIdx2) const
	{
	float Score1 = m_Scores[HitIdx1];
	float Score2 = m_Scores[HitIdx2];
	return ScoreIsBetter(Score1, Score2);
	}

bool SCOP40Bench::ScoreIsBetter(float Score1, float Score2) const
	{
	if (m_ScoresAreEvalues)
		return Score1 < Score2;
	else
		return Score1 > Score2;
	}

float SCOP40Bench::GetMeanLength(uint SFIdx) const
	{
	float Sum = 0;
	const vector<uint> &DomIdxs = m_SFIdxToDomIdxs[SFIdx];
	const uint N = SIZE(DomIdxs);
	asserta(N > 0);
	for (uint i = 0; i < N; ++i)
		{
		uint DomIdx = DomIdxs[i];
		const string &Label = m_Doms[DomIdx];
		vector<string> Fields;
		Split(Label, Fields, '/');
		const uint n = SIZE(Fields);
		asserta(n < 3);
		string Dom = Fields[0];
		map<string, uint>::const_iterator iter =
		  m_DomToChainIdx.find(Dom);
		asserta(iter != m_DomToChainIdx.end());
		uint ChainIdx = iter->second;
		const PDBChain &Chain = *m_Chains[ChainIdx];
		Sum += Chain.GetSeqLength();
		}
	return Sum/N;
	}

void SCOP40Bench::ScanDomHits()
	{
	m_ConsideredHitCount = 0;
	m_IgnoredHitCount = 0;
	m_DomIdxToScoreLastTP.clear();
	m_DomIdxToScoreFirstFP.clear();
	m_DomIdxToHitIdxFirstFP.clear();
	m_DomIdxToSens1FP.clear();

	const uint DomCount = GetDomCount();
	const uint HitCount = GetHitCount();

	m_DomIdxToSens1FP.resize(DomCount, 0);
	m_DomIdxToHitIdxLastTP.resize(DomCount, UINT_MAX);
	m_DomIdxToHitIdxFirstFP.resize(DomCount, UINT_MAX);

	m_DomIdxToScoreLastTP.resize(DomCount, GetVeryGoodScore());
	m_DomIdxToScoreFirstFP.resize(DomCount, GetVeryBadScore());

	for (uint HitIdx = 0; HitIdx < HitCount; ++HitIdx)
		{
		uint Dom1 = m_DomIdx1s[HitIdx];
		uint Dom2 = m_DomIdx2s[HitIdx];
		int T = IsT(Dom1, Dom2);
		if (T == -1)
			{
			++m_IgnoredHitCount;
			continue;
			}

		float Score = m_Scores[HitIdx];
		++m_ConsideredHitCount;
		if (T == 0)
			{
			if (ScoreIsBetter(Score, m_DomIdxToScoreFirstFP[Dom1]))
				{
				m_DomIdxToScoreFirstFP[Dom1] = Score;
				m_DomIdxToHitIdxFirstFP[Dom1] = HitIdx;
				}
			}
		}

	for (uint HitIdx = 0; HitIdx < HitCount; ++HitIdx)
		{
		uint Dom1 = m_DomIdx1s[HitIdx];
		uint Dom2 = m_DomIdx2s[HitIdx];
		int T = IsT(Dom1, Dom2);
		if (T == -1)
			continue;

		float Score = m_Scores[HitIdx];
		if (T == 1)
			{
			if (ScoreIsBetter(Score, m_DomIdxToScoreFirstFP[Dom1]))
				{
				m_DomIdxToSens1FP[Dom1] += 1;
				if (!ScoreIsBetter(Score, m_DomIdxToScoreLastTP[Dom1]))
					{
					m_DomIdxToScoreLastTP[Dom1] = Score;
					m_DomIdxToHitIdxLastTP[Dom1] = HitIdx;
					}
				}
			}
		}
	}

void SCOP40Bench::GetTPs1FP(vector<uint> &Doms1, vector<uint> &Doms2)
	{
	Doms1.clear();
	Doms2.clear();
	ScanDomHits();
	const uint HitCount = GetHitCount();

	uint GoodCount = 0;
	for (uint i = 0; i < HitCount; ++i)
		{
		uint Dom1 = m_DomIdx1s[i];
		uint Dom2 = m_DomIdx2s[i];
		float Score = m_Scores[i];
		uint SF1 = m_DomIdxToSFIdx[Dom1];
		uint SF2 = m_DomIdxToSFIdx[Dom2];
		if (Dom1 != Dom2 && SF1 == SF2)
			{
			Doms1.push_back(Dom1);
			Doms2.push_back(Dom2);
			}
		}
	}

uint SCOP40Bench::GetSens1stFP()
	{
	ScanDomHits();
	const uint HitCount = GetHitCount();

	uint GoodCount = 0;
	for (uint i = 0; i < HitCount; ++i)
		{
		uint Dom1 = m_DomIdx1s[i];
		uint Dom2 = m_DomIdx2s[i];
		if (Dom1 != Dom2 && IsT(Dom1, Dom2) == 1)
			{
			float Score = m_Scores[i];
			if (ScoreIsBetter(Score, m_DomIdxToScoreFirstFP[Dom1]))
				++GoodCount;
			}
		}
	return GoodCount;
	}

void SCOP40Bench::WriteBit(const string &FileName) const
	{
	if (FileName == "")
		return;
	const uint DomCount = SIZE(m_DomIdxToSFIdx);
	const uint HitCount = SIZE(m_DomIdx1s);
	ProgressLog("Write %u doms %s hits to %s\n",
	  DomCount, IntToStr(HitCount), FileName.c_str());
	asserta(SIZE(m_DomIdx2s) == HitCount);
	asserta(SIZE(m_Scores) == HitCount);

	FILE *f = CreateStdioFile(FileName);
	WriteStdioFile(f, &DomCount, sizeof(DomCount));
	WriteStdioFile(f, &HitCount, sizeof(HitCount));
	WriteStdioFile(f, m_DomIdx1s.data(), HitCount*sizeof(uint));
	WriteStdioFile(f, m_DomIdx2s.data(), HitCount*sizeof(uint));
	if (opt_writebitts)
		WriteStdioFile(f, m_TSs.data(), HitCount*sizeof(float));
	else
		WriteStdioFile(f, m_Scores.data(), HitCount*sizeof(float));
	CloseStdioFile(f);
	}

void SCOP40Bench::SetStats(float MaxFPR, bool UseTS)
	{
	SetTFs();

	GetROCSteps(m_ROCStepScores, m_ROCStepNTPs, m_ROCStepNFPs, UseTS);

	SmoothROCSteps(m_ROCStepScores, m_ROCStepNTPs, m_ROCStepNFPs, 100, MaxFPR,
	  m_SmoothScores, m_SmoothNTPs, m_SmoothNFPs, m_SmoothTPRs, m_SmoothFPRs);

	ROCStepsToTsv(opt_roc, m_SmoothScores, m_SmoothNTPs, m_SmoothNFPs,
	  m_SmoothTPRs, m_SmoothFPRs);

	m_nt_epq0_1 = GetNTPAtEPQThreshold(m_ROCStepNTPs, m_ROCStepNFPs, 0.1f);
	m_nt_epq1 = GetNTPAtEPQThreshold(m_ROCStepNTPs, m_ROCStepNFPs, 1);
	m_nt_epq10 = GetNTPAtEPQThreshold(m_ROCStepNTPs, m_ROCStepNFPs, 10);
	m_nt_firstfp = GetSens1stFP();
	}

void SCOP40Bench::WriteSummary()
	{
	uint AlnCount = DSSAligner::m_AlnCount;
	uint HitCount = GetHitCount();
	uint UFilterCount = DSSAligner::m_UFilterCount;
	uint ComboFilterCount = DSSAligner::m_ComboFilterCount;
	uint Secs = m_Secs;
	float AlnsPerThreadPerSec = m_AlnsPerThreadPerSec;
	uint nt_firstfp = m_nt_firstfp;
	float SensFirstFP = float(nt_firstfp)/m_NT;
	float SensEPQ0_1 = float(m_nt_epq0_1)/m_NT;
	float SensEPQ1 = float(m_nt_epq1)/m_NT;
	float SensEPQ10 = float(m_nt_epq10)/m_NT;
	uint TotalFilterCount = DSSAligner::m_UFilterCount + DSSAligner::m_ComboFilterCount;
	double FilterPct = GetPct(TotalFilterCount, AlnCount);
	double UFilterPct = GetPct(DSSAligner::m_UFilterCount, AlnCount);
	uint ComboFilterInputCount = AlnCount - DSSAligner::m_UFilterCount;
	double ComboFilterPct =
	  GetPct(DSSAligner::m_ComboFilterCount, ComboFilterInputCount);

	ProgressLog("SEPQ0.1=%.4f", SensEPQ0_1);
	ProgressLog(" SEPQ1=%.4f", SensEPQ1);
	ProgressLog(" SEPQ10=%.4f", SensEPQ10);
	ProgressLog(" S1FP=%.4f", SensFirstFP);
	ProgressLog(" N1FP=%u", nt_firstfp);
	if (Secs != UINT_MAX)
		ProgressLog(" secs=%u", Secs);
	ProgressLog(" level=%s", m_Level.c_str());
	if (optset_fast)
		ProgressLog(" fast [%s]", g_GitVer);
	else if (optset_sensitive)
		ProgressLog(" sensitive [%s]", g_GitVer);
	ProgressLog("\n");

	Log("ufil=%.1f", UFilterPct);
	Log(" cfil=%.1f", ComboFilterPct);
	Log(" sat=%u", DSSAligner::m_ParasailSaturateCount);
	Log(" qp cache hits %u, misses %u",
	  m_QPCacheHits.load(), m_QPCacheMisses.load());
	Log("\n");

	Log("NT %u NF %u NI %u considered %u ignored %u\n",
	  m_NT, m_NF, m_NI, m_ConsideredHitCount, m_IgnoredHitCount);
	}

void SCOP40Bench::WriteOutput()
	{
	float MaxFPR = 0.01f;
	if (optset_maxfpr)
		MaxFPR = (float) opt_maxfpr;
	FILE *fSVE = CreateStdioFile(opt_sens_vs_err);
	if (optset_benchlevel)
		{
		m_Level = opt_benchlevel;
		SetStats(MaxFPR);
		WriteSensVsErr(fSVE, 100);
		WriteSummary();
		}
	else
		{
		vector<string> Modes;
		Modes.push_back("sf");
		Modes.push_back("fold");
		for (uint Modei = 0; Modei < SIZE(Modes); ++Modei)
			{
			m_Level = Modes[Modei];
			SetStats(MaxFPR);
			WriteSensVsErr(fSVE, 100);
			WriteSummary();
			}
		}
	CloseStdioFile(fSVE);
	}

void SCOP40Bench::LogSens1FPReport_Dom(uint DomIdx) const
	{
	vector<uint> Dom2s;
	vector<float> Scores;
	vector<bool> TFs;
	const uint HitCount = GetHitCount();
	for (uint HitIdx = 0; HitIdx < HitCount; ++HitIdx)
		{
		uint Dom1 = m_DomIdx1s[HitIdx];
		if (Dom1 != DomIdx)
			continue;
		Dom2s.push_back(m_DomIdx2s[HitIdx]);
		Scores.push_back(m_Scores[HitIdx]);
		TFs.push_back(m_TFs[HitIdx]);
		}
	const uint N = SIZE(Dom2s);
	vector<uint> Order(N);
	QuickSortOrder(Scores.data(), N, Order.data());
	Log("____________________________________\n");
	const string &Name = m_Doms[DomIdx];
	Log("%u:%s\n", DomIdx, Name.c_str());
	uint nfp = 0;
	for (uint k = 0; k < N; ++k)
		{
		uint i = Order[k];
		uint Dom2 = Dom2s[i];
		double Score = Scores[i];
		bool TF = TFs[i];
		Log("  [%3u]  %u:%s(%.3g)\n", k, Dom2, m_Doms[Dom2].c_str(), Score);
		if (!TF)
			{
			++nfp;
			if (nfp == 2)
				break;
			}
		}
	}

void SCOP40Bench::WriteSens1FPReport(FILE *f) const
	{
	if (f == 0)
		return;

	const uint DomCount = GetDomCount();
	for (uint DomIdx = 0; DomIdx < DomCount; ++DomIdx)
		{
		const string &Name = m_Doms[DomIdx];
		fprintf(f, "%s", Name.c_str());

		uint HitIdxTP = m_DomIdxToHitIdxLastTP[DomIdx];
		uint HitIdxFP = m_DomIdxToHitIdxFirstFP[DomIdx];
		if (HitIdxTP != UINT_MAX)
			{
			asserta(m_TFs[HitIdxTP] == 1);
			float ScoreTP = m_DomIdxToScoreLastTP[DomIdx];
			uint Dom1TP = m_DomIdx1s[HitIdxTP];
			uint Dom2TP = m_DomIdx2s[HitIdxTP];
			asserta(Dom1TP == DomIdx);
			const string &NameTP = m_Doms[Dom2TP];
			float Score2TP = m_Scores[HitIdxTP];
			float TS_TP = m_TSs[HitIdxTP];
			asserta(Score2TP == ScoreTP);
			fprintf(f, "\t%s\t%.3g\t%.3g",
			  NameTP.c_str(), TS_TP, ScoreTP);
			}
		else
			fprintf(f, "\t.\t.\t.");

		if (HitIdxFP != UINT_MAX)
			{
			asserta(m_TFs[HitIdxFP] == 0);
			float ScoreFP = m_DomIdxToScoreFirstFP[DomIdx];
			uint Dom1FP = m_DomIdx1s[HitIdxFP];
			uint Dom2FP = m_DomIdx2s[HitIdxFP];
			const string &NameFP = m_Doms[Dom2FP];
			float Score2FP = m_Scores[HitIdxFP];
			float TS_FP = m_TSs[HitIdxFP];
			asserta(Score2FP == ScoreFP);
			asserta(Dom1FP == DomIdx);
			fprintf(f, "\t%s\t%.3g\t%.3g",
			  NameFP.c_str(), TS_FP, ScoreFP);
			}
		else
			fprintf(f, "\t.\t.\t.");
		fprintf(f, "\n");
		}
	//LogSens1FPReport_Dom(3);
	}

void cmd_scop40bench()
	{
	string CalFN;
	if (g_Arg1 == ".")
#ifdef _MSC_VER
		CalFN = "c:/src/reseek_scop40/reseek_db/scop40_family.cal";
#else
		CalFN = "/c/src/reseek_scop40/reseek_db/scop40_family.cal";
#endif
	else
		CalFN = g_Arg1;

	SCOP40Bench SB;
	SB.LoadChains(CalFN, "");

	DSSParams Params;
	Params.SetFromCmdLine(SB.GetDBSize());
	SB.Setup(Params);
	
	float MaxFPR = 0.005f;
	if (optset_maxfpr)
		MaxFPR = (float) opt_maxfpr;

	SB.m_fTsv = CreateStdioFile(opt_output);
	SB.m_fAln = CreateStdioFile(opt_aln);
	SB.m_fFasta2 = CreateStdioFile(opt_fasta2);

	ResetTimers();
	SB.m_QuerySelf = true;
	SB.m_ScoresAreEvalues = true;
	if (opt_scores_are_not_evalues)
		SB.m_ScoresAreEvalues = false;
	SB.Run();
	SB.WriteOutput();
	SB.WriteBit(opt_savebit);
	//SB.LogFirstFewDoms();
	//SB.LogFirstFewHits();
	if (optset_sens1fp_report)
		{
		FILE *f = CreateStdioFile(opt_sens1fp_report);
		SB.WriteSens1FPReport(f);
		CloseStdioFile(f);
		}
#if SCORE_DIST
	DSSAligner::ReportScoreDist();
#endif
	}
