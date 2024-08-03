#include "myutils.h"
#include "seqdb.h"
#include "pdbchain.h"
#include "daliscorer.h"

void cmd_daliscore_msas()
	{
	asserta(optset_input);
	asserta(optset_testdir);
	FILE* fOut = CreateStdioFile(opt_output);
	const bool MissingSeqOk = !opt_missingtestseqok;

	string TestDir = string(opt_testdir);
	Dirize(TestDir);

	DALIScorer DS;
	DS.LoadChains(opt_input);

	vector<string> FNs;
	ReadLinesFromFile(g_Arg1, FNs);

	const bool DoCore = opt_core;

	const uint N = SIZE(FNs);
	double SumZ = 0;
	double MeanZ = 0;
	for (uint i = 0; i < N; ++i)
		{
		const string &FN = FNs[i];

		SeqDB MSA;
		MSA.FromFasta(TestDir + FN, true);

		DS.SetMSA(FN, MSA, true, MissingSeqOk);
		double Z = DS.GetZ();
		double Score = DS.GetSumScore_Rows();

#if DEBUG
		{
		double Z2 = DS.GetZ_Rows();
		double Score2 = DS.GetSumScore_Cols();
		asserta(feq(Z, Z2));
		asserta(feq(Score, Score2));
		}
#endif

		uint CoreColCount = DS.m_CoreColCount;

		SumZ += Z;
		MeanZ = SumZ/(i+1);
		ProgressStep(i, N, "n %u Z %.2f", i+1, MeanZ);
		if (fOut != 0)
			{
			fprintf(fOut, "aln=%s", FN.c_str());
			fprintf(fOut, "\tZ=%.1f", Z);
			fprintf(fOut, "\tscore=%.1f", Score);
			fprintf(fOut, "\tnr_core=%u", CoreColCount);
			fprintf(fOut, "\n");
			}
		}

	if (fOut != 0)
		{
		fprintf(fOut, "testdir=%s", TestDir.c_str());
		fprintf(fOut, "\tZ=%.1f", MeanZ);
		fprintf(fOut, "\n");
		}
	  
	CloseStdioFile(fOut);
	}