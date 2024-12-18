#!/bin/bash -e

mkdir -p ../test_output
cd ../test_output

ls ../test_structures/* \
	> test_structures.files

$reseek \
	-convert test_structures.files \
	-fasta files.fa \
	-cal files.cal \
	-bca files.bca \
	-chainsep : \
	-log convert_files.log

$reseek \
	-convert ../test_structures/ \
	-fasta dir.fa \
	-cal dir.cal \
	-bca dir.bca \
	-chainsep : \
	-log convert_dir.log

function cvt() {
	from=$1
	opt=$2
	to=$3
	$reseek \
		-convert $from \
		-$opt $to \
		-log $to.log
}

cvt dir.cal fasta cvt_dir_cal.fa
cvt dir.cal bca cvt_dir_cal.bca
cvt dir.bca cal cvt_dir_bca.cal

rm -f scop40.cal*
cp ../test_data/scop40.cal.gz .
gunzip scop40.cal.gz
$reseek \
	-convert scop40.cal \
	-minchainlength 5 \
	-bca scop40.bca
