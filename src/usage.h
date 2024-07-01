
const char *usage_txt[] =
    {
"Commands\n"
"  -search        # Alignment (e.g. DB search, pairwise, all-vs-all)\n"
"  -convert       # Convert file formats (e.g. create DB)\n"
"\n"
"Search against database\n"
"    reseek -search STRUCTS -db STRUCTS -output hits.txt\n"
"                 # STRUCTS specifies structure(s), see below\n"
"\n"
"Recommended format for large database is .bca, e.g.\n"
"    reseek -convert /data/PDB_mirror/ -bca PDB.bca\n"
"\n"
"Align two structures\n"
"    reseek -search 1XYZ.pdb -db 2ABC.pdb -aln aln.txt\n"
"\n"
"All-vs-all alignment (excluding self-hits)\n"
"    reseek -search STRUCTS -output hits.txt\n"
"\n"
"Output options for -search\n"
"   -aln FILE     # Alignments in human-readable format\n"
"   -output FILE  # Hits in tabbed text format\n"
"   -columns name1+name2+name3...\n"
"                 # Output columns, names are\n"
"                 #   query   Query label\n"
"                 #   target  Target label\n"
"                 #   qlo     Start of aligment in query\n"
"                 #   qhi     End of aligment in query\n"
"                 #   tlo     Start of aligment in target\n"
"                 #   thi     End of aligment in target\n"
"                 #   ql      Query length\n"
"                 #   tl      Target length\n"
"                 #   pctid   Percent identity of alignment\n"
"                 #   cigar   CIGAR string\n"
"                 #   evalue  You can guess this one\n"
"                 #   qrow    Aligned query sequence with gaps (local)\n"
"                 #   trow    Aligned target sequence with gaps (local)\n"
"                 #   qrowg   Aligned query sequence with gaps (global)\n"
"                 #   trowg   Aligned target sequence with gaps (global)\n"
"                 #   std     query+target+qlo+qhi+ql+tlo+thi+tl+pctid+evalue\n"
"                 # default evalue+query+target\n"
"\n"
"Search and alignment options\n"
"  -sensitive     # Try harder (~2x slower)\n"
"  -evalue E      # Max E-value (default 10)\n"
"  -omega X       # Omega accelerator (floating-point)\n"
"  -minu U        # K-mer accelerator (integer)\n"
"  -gapopen X     # Gap-open penalty (floating-point >= 0)\n"
"  -gapext X      # Gap-extend penalty (floating-point >= 0)\n"
"  -dbsize D      # DB size (nr. chains) for E-value (default actual size)\n"
"\n"
"Convert between file formats\n"
"    reseek -convert STRUCTS [one or more output options]\n"
"           -cal FILENAME    # .cal format, text with a.a. and C-alpha x,y,z\n"
"           -bca FILENAME    # .bca format, binary .cal, recommended for DBs\n"
"           -fasta FILENAME  # FASTA format\n"
"\n"
"STRUCTS argument is one of:\n"
"   NAME.cif or NAME.mmcif     # PDBx/mmCIF file\n"
"   NAME.pdb                   # Legacy format PDB file\n"
"   NAME.cal                   # C-alpha tabbed text format with chain(s)\n"
"   NAME.bca                   # Binary C-alpha, recommended for larger DBs\n"
"   NAME.files                 # Text file with one STRUCT per line,\n"
"                              #   may be filename, directory or .files\n"
"   DIRECTORYNAME              # Directory (and its sub-directories) is searched\n"
"                              #   for known file types including .pdb, .files etc.\n"
"\n"
"   -log FILENAME              # Log file with errors, warnings, time and memory.\n"
"   -threads N                 # Number of threads, default number of CPU cores.\n"
    };
