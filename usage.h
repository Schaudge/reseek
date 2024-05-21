
const char *usage_txt[] =
    {
"All-vs-all alignment (excluding self-hits)\n"
"    reseek -search STRUCTS -output hits.txt -mode MODE\n"
"\n"
"Search query against database\n"
"    reseek -search Q_STRUCTS -db DB_STRUCTS -mode MODE -output hits.txt\n"
"\n"
"Align two structures\n"
"    reseek -search NAME1.pdb -db NAME2.pdb -aln aln.txt\n"
"\n"
"Output options for -search\n"
"   -aln FILE     # Alignments in human-readable format\n"
"   -output FILE  # Hits in tabbed text format with 8 fields:\n"
"                 #   Evalue Query Target Qstart Qend Tstart Tend CIGAR\n"
"                 # (More output formats coming soon)\n"
"\n"
"Search and alignment options\n"
"  -mode MODE     # veryfast|fast|sensitive|verysensitive (required)\n"
"  -evalue E      # Max E-value (default 10)\n"
"  -omega X       # Omega accelerator (floating-point)\n"
"  -minu U        # K-mer accelerator (integer)\n"
"  -gapopen X     # Gap-open penalty (floating-point >= 0, default 1.1)\n"
"  -gapext X      # Gap-extend penalty (floating-point >= 0, default 0.14)\n"
"  -dbsize D      # Effective database size for E-value (default actual size)\n"
"  -usort         # U-sort accelerator (default off)\n"
"  -maxaccepts N  # If U-sort, max hits <= E-value (default 1)\n"
"  -maxrejects N  # If U-sort, max hits > E-value (default 32)\n"
"\n"
"Convert PDB file(s) to .cal (C-alpha) format\n"
"    reseek -pdb2cal STRUCTS -output structs.cal\n"
"\n"
"STRUCTS argument is one of:\n"
"   NAME.pdb      # PDB file (mmCIF support will be added soon)\n"
"   NAME.files    # Text file with PDB file/pathnames, one per line\n"
"   NAME.cal      # C-alpha (.cal) file, recommended for databases\n"
    };
