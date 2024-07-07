#pragma once

class DALIScorer
	{
public:
	string m_Name;
	const SeqDB *m_MSA = 0;
	bool m_DoCore = false;
	vector<PDBChain *> m_Chains;
	map<string, uint> m_SeqToChainIdx;

	vector<uint> m_SeqIdxToChainIdx;
	set<string> m_NotFoundLabels;
	vector<bool> m_ColIsCore;
	uint m_CoreColCount = 0;

	vector<vector<uint> > m_ColToPosVec;
	vector<double> m_ColScores;

public:
	void ClearMSA()
		{
		m_MSA = 0;
		m_Name.clear();
		m_SeqIdxToChainIdx.clear();
		m_NotFoundLabels.clear();
		m_ColIsCore.clear();
		}

	void LoadChains(const string &FN);
	void SetMSA(const string &Name, const SeqDB &MSA,
	  bool DoCore, bool MissingSeqOk);
	double GetZ() const;
	double GetZ_Rows() const;
	double GetSumScore_Cols() const;
	void SetSeqIdxToChainIdx(bool MissingSeqOk);
	void SetCore();
	void SetColToPosVec(bool Core);
	void GetColToPos(uint SeqIdx, vector<uint> &ColToPos, bool Core);
	uint GetColCount() const { return m_MSA->GetColCount(); }
	uint GetSeqCount() const { return m_MSA->GetSeqCount(); }
	bool GetDALIRowPair(uint SeqIdx1, uint SeqIdx2,
	  double &Score, double &Z) const;
	double GetDALIScoreColPair(uint Col1, uint Col2) const;
	double GetDALIPosPairScore(
	  const PDBChain &ChainX, uint PosX1, uint PosX2,
	  const PDBChain &ChainY, uint PosY1, uint PosY2) const;
	double GetDiagScore() const;
	double GetDiagScoreSeqPair(uint SeqIdx1, uint SeqIdx2) const;
	};

double DALI_dpscorefun(double a, double b);
double GetDALIScore(const PDBChain &Q, const PDBChain &T,
	const vector<uint> &PosQs, const vector<uint> &PosTs);
double GetDALIZFromScoreAndLengths(double DALIScore, uint QL, uint TL);
extern float g_DALI_Theta;