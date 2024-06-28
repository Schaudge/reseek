#include "myutils.h"
#include "chainreader2.h"

uint ChainReader2::m_DupeSeqCount = 0;

void ChainReader2::Close()
	{
	if (m_Trace) Log("ChainReader2::Close()\n");
	m_State = STATE_Closed;
	}

void ChainReader2::Open(PDBFileScanner &FS)
	{
	m_ptrFS = &FS;
	m_Trace = opt_trace_chainreader2;
	if (m_Trace) Log("ChainReader2::Open()\n");
	m_State = STATE_PendingFile;
	m_DeleteDupeSeqs = true;
	if (opt_keep_dupe_seqs)
		m_DeleteDupeSeqs = false;
	m_ChainCount = 0;
	}

// Files first, then directories to reduce queue
PDBChain *ChainReader2::GetFirst(const string &FN)
	{
	m_CurrentFN = FN;

	string Ext;
	GetExtFromPathName(FN, Ext);
	ToLower(Ext);
	if (m_Trace) Log("  FN=%s Ext=%s\n", FN.c_str(), Ext.c_str());
	if (Ext == "cal")
		{
		m_State = STATE_ReadingCALFile;
		PDBChain *Chain = GetFirst_CAL(FN);
		if (Chain != 0)
			return Chain;
		}
	else if (Ext == "pdb" || Ext == "pdb.gz" || Ext == "ent" || Ext == "ent.gz")
		{
		m_State = STATE_ReadingPDBFile;
		PDBChain *Chain = GetFirst_PDB(FN);
		if (Chain != 0)
			return Chain;
		}
	else if (Ext == "cif" || Ext == "cif.gz" || Ext == "mmcif" || Ext == "mmcif.gz")
		{
		m_State = STATE_ReadingCIFFile;
		PDBChain *Chain = GetFirst_CIF(FN);
		if (Chain != 0)
			return Chain;
		}
	else
		Die("ChainReader2::GetNext(%s), unknown extension", FN.c_str());
	return 0;
	}

PDBChain *ChainReader2::GetNext()
	{
	m_Lock.lock();
	PDBChain *Chain = GetNextLo1();
	m_Lock.unlock();
	return Chain;
	}

PDBChain *ChainReader2::GetNextLo1()
	{
	for (uint SanityCounter = 0; SanityCounter < 100; ++SanityCounter)
		{
		if (m_Trace) Log("GetNextLo2() state=%d\n", m_State);
		switch (m_State)
			{
		case STATE_Closed:
			return 0;

		case STATE_PendingFile:
			{
			string FN;
			bool Ok = m_ptrFS->GetNext(FN);
			if (!Ok)
				{
				Close();
				return 0;
				}
			PDBChain *Chain = GetFirst(FN);
			if (m_Trace) Log("PendingFile()=0, Close() and stop.\n");
			if (Chain != 0)
				return Chain;
			continue;
			}

		case STATE_ReadingCALFile:
			{
			PDBChain *Chain = GetNext_CAL();
			if (Chain != 0)
				return Chain;
			if (m_Trace) Log("GetNext_CAL()=0, state->PendingFile\n");
			m_State = STATE_PendingFile;
			continue;
			}

		case STATE_ReadingPDBFile:
			{
			PDBChain *Chain = GetNext_PDB();
			if (Chain != 0)
				return Chain;
			if (m_Trace) Log("GetNext_PDB()=0, state->PendingFile\n");
			m_State = STATE_PendingFile;
			continue;
			}

		case STATE_ReadingCIFFile:
			{
			PDBChain *Chain = GetNext_CIF();
			if (Chain != 0)
				return Chain;
			if (m_Trace) Log("GetNext_CIF()=0, state->PendingFile\n");
			m_State = STATE_PendingFile;
			continue;
			}

		default:
			asserta(false);
			}
		}
	Die("Excessive looping in ChainReader2::GetNextLo2()");
	return 0;
	}

void ChainReader2::GetFallbackLabelFromFN(const string &FN, string &Label)
	{
	GetStemName(FN, Label);
	string Ext;
	GetExtFromPathName(FN, Ext);
	ToLower(Ext);

// Special-case for downloaded PDB files e.g. pdb1iv1.ent
	if (Ext == "pdb" || Ext == "ent")
		{
		if (Label.size() == 7 && Label[0] == 'p' && Label[1] == 'd' && Label[2] == 'b')
			{
			Label = Label.substr(3, string::npos);
			ToUpper(Label);
			}
		}
	}

PDBChain *ChainReader2::GetFirst_CAL(const string &FN)
	{
	m_LR.Open(FN);
	bool Ok = m_LR.ReadLine(m_Line);
	if (!Ok)
		Die("Failed to read first line of CAL file '%s'",
		  FN.c_str());
	return GetNext_CAL();
	}

PDBChain *ChainReader2::GetNext_CAL()
	{
	if (m_LR.m_EOF)
		{
		m_LR.Close();
		return 0;
		}
	if (m_Line.empty() || m_Line[0] != '>')
		Die("%s: Expected '>' in CAL file",
		  m_CurrentFN.c_str());

	const string Label = m_Line.substr(1);
	if (m_Trace) Log("ChainReader2::GetNext_CAL() Label=%s\n", Label.c_str());
	m_Lines.clear();
	while (m_LR.ReadLine(m_Line))
		{
		if (m_Line.c_str()[0] == '>')
			break;
		m_Lines.push_back(m_Line);
		}

	PDBChain *Chain = new PDBChain;
	Chain->m_Label = Label;

/***
>102l
M       43.619  -1.924  8.869
N       40.445  -0.876  10.670
I       38.254  2.240   11.220
F       40.340  3.621   14.036
***/
	const uint N = SIZE(m_Lines);
	vector<string> Fields;
	for (uint LineNr = 0; LineNr < N; ++LineNr)
		{
		const string &Line = m_Lines[LineNr];
		if (Line.empty())
			continue;
		Split(Line, Fields, '\t');
		if (Fields.size() != 4 || Fields[0].size() != 1)
			Die("%s: Invalid CAL record '%s'",
			  m_CurrentFN.c_str(), Line.c_str());

		char aa = Fields[0][0];
		double X = StrToFloat(Fields[1]);
		double Y = StrToFloat(Fields[2]);
		double Z = StrToFloat(Fields[3]);

		Chain->m_Seq.push_back(aa);
		Chain->m_Xs.push_back(X);
		Chain->m_Ys.push_back(Y);
		Chain->m_Zs.push_back(Z);
		}
	return Chain;
	}

PDBChain *ChainReader2::GetFirst_PDB(const string &FN)
	{
	ReadLinesFromFile(FN, m_Lines);
	string FallbackLabel;
	GetFallbackLabelFromFN(FN, FallbackLabel);
	ChainsFromLines_PDB(m_Lines, m_Chains_PDB, FallbackLabel);
	if (m_DeleteDupeSeqs)
		DeleteDuplicateSeqs(m_Chains_PDB);
	m_ChainIdx_PDB = 0;
	return GetNext_PDB();
	}

PDBChain *ChainReader2::GetFirst_CIF(const string &FN)
	{
	ReadLinesFromFile(FN, m_Lines);
	string FallbackLabel;
	GetFallbackLabelFromFN(FN, FallbackLabel);
	ChainsFromLines_CIF(m_Lines, m_Chains_CIF, FallbackLabel);
	if (m_DeleteDupeSeqs)
		DeleteDuplicateSeqs(m_Chains_CIF);
	m_ChainIdx_CIF = 0;
	return GetNext_CIF();
	}

PDBChain *ChainReader2::GetNext_PDB()
	{
	const uint N = SIZE(m_Chains_PDB);
	if (m_ChainIdx_PDB == N)
		return 0;
	asserta(m_ChainIdx_PDB < N);
	PDBChain *Chain = m_Chains_PDB[m_ChainIdx_PDB++];
	if (m_Trace) Log("ChainReader2::GetNext_PDB() %u/%u Label=%s\n", m_ChainIdx_PDB, N, Chain->m_Label.c_str());
	return Chain;
	}

PDBChain *ChainReader2::GetNext_CIF()
	{
	const uint N = SIZE(m_Chains_CIF);
	if (m_ChainIdx_CIF == N)
		return 0;
	asserta(m_ChainIdx_CIF < N);
	PDBChain *Chain = m_Chains_CIF[m_ChainIdx_CIF++];
	if (m_Trace) Log("ChainReader2::GetNext_CIF() %u/%u Label=%s\n", m_ChainIdx_CIF, N, Chain->m_Label.c_str());
	return Chain;
	}

void ChainReader2::ChainsFromLines_PDB(const vector<string> &Lines,
  vector<PDBChain *> &Chains, const string &FallbackLabel) const
	{
	string Label = FallbackLabel;
	Chains.clear();
	const uint N = SIZE(Lines);
	vector<string> ChainLines;
	char CurrChainChar = 0;
	bool AnyAtoms = false;
	for (uint i = 0; i < N; ++i)
		{
		const string &Line = Lines[i];
		if (StartsWith(Line, "HEADER "))
			{
			vector<string> Fields;
			SplitWhite(Line, Fields);
			uint n = SIZE(Fields);
			if (n > 1)
				{
				Label = Fields[n-1];
				if (Label == "")
					Label = FallbackLabel;
				}
			}

		if (IsATOMLine_PDB(Line))
			{
			if (Line.size() < 57)
				continue;
			char ChainChar = Line[21];
			if (ChainChar != CurrChainChar)
				{
				if (AnyAtoms && !ChainLines.empty())
					{
					PDBChain *Chain = new PDBChain;
					char ChainChar = Chain->FromPDBLines(Label, ChainLines);
					if (ChainChar == 0)
						delete Chain;
					else
						Chains.push_back(Chain);
					ChainLines.clear();
					AnyAtoms = false;
					}
				CurrChainChar = ChainChar;
				}
			ChainLines.push_back(Line);
			AnyAtoms = true;
			}
		}

	if (!ChainLines.empty() && AnyAtoms)
		{
		PDBChain *Chain = new PDBChain;
		Chain->FromPDBLines(Label, ChainLines);
		ChainLines.clear();
		Chains.push_back(Chain);
		}
	}

PDBChain *ChainReader2::ChainFromLines_CAL(const vector<string> &Lines) const
	{
	PDBChain *Chain = new PDBChain;
	Chain->FromCalLines(Lines);
	return Chain;
	}

bool ChainReader2::IsATOMLine_PDB(const string &Line) const
	{
	if (SIZE(Line) < 27)
		return false;
	if (strncmp(Line.c_str(), "ATOM  ", 6) == 0)
		return true;
	return false;
	}

void ChainReader2::DeleteDuplicateSeqs(vector<PDBChain *> &Chains)
	{
	if (Chains.size() <= 1)
		return;

	set<string> Seqs;
	vector<PDBChain *> UpdatedChains;
	for (uint i = 0; i < SIZE(Chains); ++i)
		{
		PDBChain *ptrChain = Chains[i];
		const string &Seq = ptrChain->m_Seq;
		if (Seqs.find(Seq) != Seqs.end())
			{
			delete ptrChain;
			++m_DupeSeqCount;
			continue;
			}
		Seqs.insert(Seq);
		UpdatedChains.push_back(ptrChain);
		}
	Chains = UpdatedChains;
	}
