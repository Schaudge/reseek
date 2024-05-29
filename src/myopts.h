#ifndef MY_VERSION
#define MY_VERSION	"1.0"
#endif

#define C(x)	STR_OPT(x)
#include "cmds.h"

STR_OPT(search_aa_top)
STR_OPT(search_nt_top)
STR_OPT(search_nt_all)
STR_OPT(search_nt_topx)
STR_OPT(search_pair_nt)
STR_OPT(search_pair_g)
STR_OPT(search_pair_aa)
STR_OPT(otumaps)
STR_OPT(alnqc)
STR_OPT(msaqc)
STR_OPT(msaqc3)
STR_OPT(logpssms)
STR_OPT(agreefastaout)
STR_OPT(qhitfastaout)
STR_OPT(rex)
STR_OPT(align_msas)
STR_OPT(test_pssms)
STR_OPT(ppp)
STR_OPT(tree)
STR_OPT(splitb)
STR_OPT(shapes)
STR_OPT(loaddir)
STR_OPT(alphastr)
STR_OPT(param)
STR_OPT(params)
STR_OPT(namedparams)
STR_OPT(paramsf)
STR_OPT(roc)
STR_OPT(epqx)
STR_OPT(fev)
STR_OPT(benchmode)

STR_OPT(train_cal)
STR_OPT(test_cal)
STR_OPT(report)
STR_OPT(scoredist)

STR_OPT(ref)
STR_OPT(label)
STR_OPT(labels)
STR_OPT(fastaout)
STR_OPT(alnout)
STR_OPT(triout)
STR_OPT(calin)
STR_OPT(pdboutx)
STR_OPT(chain)
STR_OPT(db)
STR_OPT(freqsout)
STR_OPT(relabel)
STR_OPT(fevout)
STR_OPT(bedout)
STR_OPT(model)
STR_OPT(ppout)
STR_OPT(ppout_nt)
STR_OPT(motifs_fastaout)
STR_OPT(motifs_fastaout2)
STR_OPT(motifs_fastaout3)
STR_OPT(nohit_fastaout)
STR_OPT(pssm_alnout)
STR_OPT(msa2)
STR_OPT(pssmdir)
STR_OPT(pdbout)
STR_OPT(xxx)
STR_OPT(centroids)
STR_OPT(exclude)
STR_OPT(include)
STR_OPT(seg_fasta_prefix)
STR_OPT(motif_coords)
STR_OPT(feature)
STR_OPT(features)
STR_OPT(weights)
STR_OPT(weightstr)

STR_OPT(log)
STR_OPT(input)
STR_OPT(input1)
STR_OPT(input2)
STR_OPT(input3)
STR_OPT(output)
STR_OPT(output2)
STR_OPT(output3)
STR_OPT(output4)
STR_OPT(psm)
STR_OPT(psm1)
STR_OPT(psm2)
STR_OPT(spec)
STR_OPT(logodds)
STR_OPT(myss3)
STR_OPT(pattern)

STR_OPT(fw_name)
STR_OPT(psm_fw)

STR_OPT(sep)
STR_OPT(fieldnrs)
STR_OPT(png)
STR_OPT(subchain)
STR_OPT(searchmfs)
STR_OPT(requiremfs)
STR_OPT(savebit)
STR_OPT(tps1fp)
STR_OPT(aln)
STR_OPT(parasail)

UNS_OPT(threads,			8,			0,			UINT_MAX)
UNS_OPT(alpha_size,			8,			0,			UINT_MAX)
UNS_OPT(band,				16,			0,			UINT_MAX)
UNS_OPT(hspw,				0,			1,			UINT_MAX)
UNS_OPT(minhsp,				32,			1,			UINT_MAX)
UNS_OPT(iddrop,				8,			1,			UINT_MAX)
UNS_OPT(cluster_maxdiffs,	1,			0,			UINT_MAX)
UNS_OPT(tsv_topn,			32,			1,			UINT_MAX)
UNS_OPT(report_topn,		32,			1,			UINT_MAX)
UNS_OPT(maxx,				10,			1,			UINT_MAX)
UNS_OPT(refn,				10,			1,			UINT_MAX)
UNS_OPT(minflanklen,		10,			1,			UINT_MAX)
UNS_OPT(scoretype,			10,			1,			UINT_MAX)
UNS_OPT(minlen,				10,			1,			UINT_MAX)
UNS_OPT(lo,					10,			1,			UINT_MAX)
UNS_OPT(hi,					10,			1,			UINT_MAX)
UNS_OPT(pos,				10,			1,			UINT_MAX)
UNS_OPT(minsize,				10,			1,			UINT_MAX)
UNS_OPT(minu,				10,			1,			UINT_MAX)
UNS_OPT(maxaccepts,			10,			1,			UINT_MAX)
UNS_OPT(maxrejects,			10,			1,			UINT_MAX)

UNS_OPT(secs,				60,			1,			UINT_MAX)
UNS_OPT(flanks,				60,			1,			UINT_MAX)

UNS_OPT(maxseqlength,		500000000,		1,			UINT_MAX)
UNS_OPT(sfasta_buff_bytes,	512*1024*1024,1024,		UINT_MAX)
UNS_OPT(randseed,			0,			0,			UINT_MAX)
UNS_OPT(usort_w,			6,			1,			UINT_MAX)
UNS_OPT(mingap,				0,			1,			UINT_MAX)
UNS_OPT(maxgap,				999,		1,			UINT_MAX)
UNS_OPT(hiw,				45,			1,			UINT_MAX)
UNS_OPT(low,				16,			1,			UINT_MAX)
UNS_OPT(sample_size,		16,			1,			UINT_MAX)
UNS_OPT(topn,				16,			1,			UINT_MAX)
UNS_OPT(k,					5,			0,			UINT_MAX)
UNS_OPT(n,					5,			0,			UINT_MAX)
UNS_OPT(nbrw,				5,			0,			UINT_MAX)
UNS_OPT(densw,				5,			0,			UINT_MAX)
UNS_OPT(ssdensw,			5,			0,			UINT_MAX)
UNS_OPT(dpgaps,				10,			1,			UINT_MAX)
UNS_OPT(biassteps,			10,			1,			UINT_MAX)
UNS_OPT(opensteps,			10,			1,			UINT_MAX)
UNS_OPT(extsteps,			10,			1,			UINT_MAX)
UNS_OPT(scorefieldnr,		10,			1,			UINT_MAX)
UNS_OPT(nfp,			10,			1,			UINT_MAX)
UNS_OPT(lambda,			10,			1,			UINT_MAX)

FLT_OPT(maxgapfract,				0.6,		0.0,			1.0)
FLT_OPT(mintm,				0.6,		0.0,			1.0)
FLT_OPT(maxtm,				0.8,		0.0,			1.0)
FLT_OPT(minscore,			10.0,		-9e9,		+9e9)
FLT_OPT(minppscore,			10.0,		-9e9,		+9e9)
FLT_OPT(minpalmscore,		10.0,		-9e9,		+9e9)
//FLT_OPT(maxlefppm,			10.0,		-9e9,		+9e9)
FLT_OPT(maxscore,			10.0,		-9e9,		+9e9)
FLT_OPT(motif_cluster_minscore,			0.0,		-9e9,		+9e9)
FLT_OPT(minscore1,			0.0,		-9e9,		+9e9)
FLT_OPT(minscore2,			0.0,		-9e9,		+9e9)
FLT_OPT(minscore_pair,		0.0,		-9e9,		+9e9)
FLT_OPT(stop_score,			-10.0,		-9e9,		+9e9)
FLT_OPT(minscore_fw,		0.0,		-9e9,		+9e9)
FLT_OPT(fastq_maxee,		1.0,		0.0,		9e9)
FLT_OPT(threshold,			1.0,		0.0,		9e9)
FLT_OPT(minselfscorenonpp,	1.0,		0.0,		9e9)
FLT_OPT(minselfscorepp,		1.0,		0.0,		9e9)
FLT_OPT(maxfpr,				1.0,		0.0,		9e9)
FLT_OPT(thresh,				1.0,		0.0,		9e9)
FLT_OPT(minfwdscore,				1.0,		0.0,		9e9)
FLT_OPT(omega,				1.0,		0.0,		9e9)
FLT_OPT(daliw,				1.0,		0.0,		9e9)

FLT_OPT(match,				1.0,		0.0,		DBL_MAX)
FLT_OPT(mismatch,			-2.0,		0.0,		DBL_MAX)
FLT_OPT(xdrop_u,			16.0,		0.0,		DBL_MAX)
FLT_OPT(xdrop_g,			32.0,		0.0,		DBL_MAX)
FLT_OPT(xdrop_nw,			16.0,		0.0,		DBL_MAX)
FLT_OPT(lopen,				10.0,		0.0,		DBL_MAX)
FLT_OPT(lext,				1.0,		0.0,		DBL_MAX)
FLT_OPT(mincscore,			0.0,		0.0,		DBL_MAX)
FLT_OPT(rmsd,				0.0,		0.0,		DBL_MAX)
FLT_OPT(train_pct,			0.0,		0.0,		DBL_MAX)
FLT_OPT(min_palm_score,			0.0,		0.0,		DBL_MAX)
FLT_OPT(radius,			0.0,		0.0,		DBL_MAX)
FLT_OPT(bias,			0.0,		0.0,		DBL_MAX)
FLT_OPT(minw, 0.0, 0.0, DBL_MAX)
FLT_OPT(maxw, 0.0, 0.0, DBL_MAX)
FLT_OPT(minbias,			0.0,		0.0,		DBL_MAX)
FLT_OPT(maxbias,			0.0,		0.0,		DBL_MAX)
FLT_OPT(minval,			0.0,		0.0,		DBL_MAX)
FLT_OPT(maxval,			0.0,		0.0,		DBL_MAX)
FLT_OPT(minopen,			0.0,		0.0,		DBL_MAX)
FLT_OPT(maxopen,			0.0,		0.0,		DBL_MAX)
FLT_OPT(minext,			0.0,		0.0,		DBL_MAX)
FLT_OPT(maxext,			0.0,		0.0,		DBL_MAX)
FLT_OPT(maxfx,			0.0,		0.0,		DBL_MAX)
FLT_OPT(minseedscore,			0.0,		0.0,		DBL_MAX)
FLT_OPT(min_timer_pct,		1.0,		0.0,		100.0)
FLT_OPT(evalue,		1.0,		0.0,		100.0)
FLT_OPT(gapopen,		1.0,		0.0,		100.0)
FLT_OPT(gapext,		1.0,		0.0,		100.0)
FLT_OPT(dbsize,		1.0,		0.0,		100.0)

FLAG_OPT(trunclabels)
FLAG_OPT(notrunclabels)
FLAG_OPT(compilerinfo)
FLAG_OPT(quiet)
FLAG_OPT(logmemgrows)
FLAG_OPT(fulldp)
FLAG_OPT(logquery)
FLAG_OPT(verbose)
FLAG_OPT(coords)
FLAG_OPT(permuted)
FLAG_OPT(notpermuted)
FLAG_OPT(trace)
FLAG_OPT(self)
FLAG_OPT(leave_one_out)
FLAG_OPT(label_substr_match)
FLAG_OPT(pdb_ent)
FLAG_OPT(dgd)
FLAG_OPT(top_hit_only)
FLAG_OPT(refs)
FLAG_OPT(first_only)
FLAG_OPT(norows)
FLAG_OPT(calctps)
FLAG_OPT(compact)
FLAG_OPT(dotproduct)
FLAG_OPT(subchainc)
FLAG_OPT(delete_hetatm)
FLAG_OPT(misses)
FLAG_OPT(pml_savepse)
FLAG_OPT(traceabc)
FLAG_OPT(testabc)
FLAG_OPT(calibrate)
//FLAG_OPT(sensitive)
//FLAG_OPT(lowerrors)
FLAG_OPT(pmlrevmotifs)
FLAG_OPT(noappendchain)
FLAG_OPT(besthit)
FLAG_OPT(scores_are_evalues)
FLAG_OPT(scores_are_not_evalues)
FLAG_OPT(allpairs)
FLAG_OPT(nodali)
FLAG_OPT(symm)
FLAG_OPT(skipzeros)
FLAG_OPT(usort)
FLAG_OPT(veryfast)
FLAG_OPT(fast)
FLAG_OPT(sensitive)
FLAG_OPT(verysensitive)

#undef FLAG_OPT
#undef UNS_OPT
#undef FLT_OPT
#undef STR_OPT
