#include "myutils.h"
#include "alpha.h"

double g_Blosum62[20][20] = 
	{
//               A        C        D        E        F        G        H        I        K        L        M        N        P        Q        R        S        T        V        W        Y
/* A */	{   1.9646, -0.2043, -0.8767, -0.4319, -1.1050,  0.0798, -0.8126, -0.6609, -0.3670, -0.7323, -0.4676, -0.7654, -0.4071, -0.4020, -0.7068,  0.5579, -0.0227, -0.0947, -1.2634, -0.8820, },  // A
/* C */	{  -0.2043,  4.2911, -1.7300, -1.8062, -1.1877, -1.2502, -1.4939, -0.6138, -1.5182, -0.6387, -0.7099, -1.3299, -1.3976, -1.4509, -1.6946, -0.4375, -0.4333, -0.4038, -1.1521, -1.2036, },  // C
/* D */	{  -0.8767, -1.7300,  2.8871,  0.7552, -1.7419, -0.6568, -0.5595, -1.5606, -0.3509, -1.8028, -1.5293,  0.6358, -0.7401, -0.1567, -0.8029, -0.1305, -0.5254, -1.5713, -2.1072, -1.5325, },  // D
/* E */	{  -0.4319, -1.8062,  0.7552,  2.4514, -1.5962, -1.0551, -0.0588, -1.5972,  0.3877, -1.4232, -0.9990, -0.1340, -0.5581,  0.9273, -0.0577, -0.0735, -0.4316, -1.2211, -1.4177, -1.0102, },  // E
/* F */	{  -1.1050, -1.1877, -1.7419, -1.5962,  3.0230, -1.5537, -0.6171, -0.0804, -1.5393,  0.2074,  0.0063, -1.4970, -1.7986, -1.5822, -1.3932, -1.1845, -1.0538, -0.4245,  0.4588,  1.4696, },  // F
/* G */	{   0.0798, -1.2502, -0.6568, -1.0551, -1.5537,  2.7816, -1.0204, -1.8624, -0.7640, -1.8135, -1.3383, -0.2114, -1.0668, -0.8926, -1.1521, -0.1462, -0.7877, -1.5694, -1.2457, -1.5199, },  // G
/* H */	{  -0.8126, -1.4939, -0.5595, -0.0588, -0.6171, -1.0204,  3.7555, -1.6158, -0.3605, -1.3934, -0.7756,  0.2892, -1.0805,  0.2240, -0.1249, -0.4408, -0.8429, -1.5587, -1.1711,  0.8463, },  // H
/* I */	{  -0.6609, -0.6138, -1.5606, -1.5972, -0.0804, -1.8624, -1.6158,  1.9993, -1.3351,  0.7608,  0.5634, -1.6085, -1.3783, -1.3848, -1.4951, -1.1741, -0.3588,  1.2735, -1.2903, -0.6657, },  // I
/* K */	{  -0.3670, -1.5182, -0.3509,  0.3877, -1.5393, -0.7640, -0.3605, -1.3351,  2.2523, -1.2234, -0.6774, -0.0895, -0.5068,  0.6363,  1.0544, -0.1017, -0.3348, -1.1312, -1.4782, -0.9100, },  // K
/* L */	{  -0.7323, -0.6387, -1.8028, -1.4232,  0.2074, -1.8135, -1.3934,  0.7608, -1.2234,  1.9247,  0.9959, -1.6895, -1.4300, -1.0670, -1.0773, -1.2213, -0.5987,  0.3942, -0.8159, -0.5310, },  // L
/* M */	{  -0.4676, -0.7099, -1.5293, -0.9990,  0.0063, -1.3383, -0.7756,  0.5634, -0.6774,  0.9959,  2.6963, -1.0754, -1.2382, -0.2105, -0.6836, -0.7404, -0.3331,  0.3436, -0.7124, -0.4974, },  // M
/* N */	{  -0.7654, -1.3299,  0.6358, -0.1340, -1.4970, -0.2114,  0.2892, -1.6085, -0.0895, -1.6895, -1.0754,  2.8266, -1.0002,  0.0008, -0.2199,  0.3005, -0.0230, -1.4382, -1.8480, -1.0409, },  // N
/* P */	{  -0.4071, -1.3976, -0.7401, -0.5581, -1.7986, -1.0668, -1.0805, -1.3783, -0.5068, -1.4300, -1.2382, -1.0002,  3.6823, -0.6410, -1.0543, -0.4045, -0.5376, -1.1744, -1.8271, -1.4599, },  // P
/* Q */	{  -0.4020, -1.4509, -0.1567,  0.9273, -1.5822, -0.8926,  0.2240, -1.3848,  0.6363, -1.0670, -0.2105,  0.0008, -0.6410,  2.6426,  0.4914, -0.0506, -0.3377, -1.0992, -0.9732, -0.7105, },  // Q
/* R */	{  -0.7068, -1.6946, -0.8029, -0.0577, -1.3932, -1.1521, -0.1249, -1.4951,  1.0544, -1.0773, -0.6836, -0.2199, -1.0543,  0.4914,  2.7367, -0.3824, -0.5612, -1.2513, -1.3397, -0.8469, },  // R
/* S */	{   0.5579, -0.4375, -0.1305, -0.0735, -1.1845, -0.1462, -0.4408, -1.1741, -0.1017, -1.2213, -0.7404,  0.3005, -0.4045, -0.0506, -0.3824,  1.9422,  0.6906, -0.8231, -1.3759, -0.8429, },  // S
/* T */	{  -0.0227, -0.4333, -0.5254, -0.4316, -1.0538, -0.7877, -0.8429, -0.3588, -0.3348, -0.5987, -0.3331, -0.0230, -0.5376, -0.3377, -0.5612,  0.6906,  2.2727, -0.0278, -1.2145, -0.8030, },  // T
/* V */	{  -0.0947, -0.4038, -1.5713, -1.2211, -0.4245, -1.5694, -1.5587,  1.2735, -1.1312,  0.3942,  0.3436, -1.4382, -1.1744, -1.0992, -1.2513, -0.8231, -0.0278,  1.8845, -1.4171, -0.6038, },  // V
/* W */	{  -1.2634, -1.1521, -2.1072, -1.4177,  0.4588, -1.2457, -1.1711, -1.2903, -1.4782, -0.8159, -0.7124, -1.8480, -1.8271, -0.9732, -1.3397, -1.3759, -1.2145, -1.4171,  5.2520,  1.0771, },  // W
/* Y */	{  -0.8820, -1.2036, -1.5325, -1.0102,  1.4696, -1.5199,  0.8463, -0.6657, -0.9100, -0.5310, -0.4974, -1.0409, -1.4599, -0.7105, -0.8469, -0.8429, -0.8030, -0.6038,  1.0771,  3.2975, },  // Y
	};

// P(xy|x), so probs sum to 1 one each row.
double g_BLOSUM62_JP_Rows[20][20] = 
	{
//              A       C       D       E       F       G       H       I       K       L       M       N       P       Q       R       S       T       V       W       Y
/* A */	{  0.2901, 0.0216, 0.0297, 0.0405, 0.0216, 0.0783, 0.0148, 0.0432, 0.0445, 0.0594, 0.0175, 0.0256, 0.0297, 0.0256, 0.0310, 0.0850, 0.0499, 0.0688, 0.0054, 0.0175, },  // A sum = 1.00
/* C */	{  0.0650, 0.4837, 0.0163, 0.0163, 0.0203, 0.0325, 0.0081, 0.0447, 0.0203, 0.0650, 0.0163, 0.0163, 0.0163, 0.0122, 0.0163, 0.0407, 0.0366, 0.0569, 0.0041, 0.0122, },  // C sum = 1.00
/* D */	{  0.0410, 0.0075, 0.3974, 0.0914, 0.0149, 0.0466, 0.0187, 0.0224, 0.0448, 0.0280, 0.0093, 0.0690, 0.0224, 0.0299, 0.0299, 0.0522, 0.0354, 0.0243, 0.0037, 0.0112, },  // D sum = 1.00
/* E */	{  0.0552, 0.0074, 0.0902, 0.2965, 0.0166, 0.0350, 0.0258, 0.0221, 0.0755, 0.0368, 0.0129, 0.0405, 0.0258, 0.0645, 0.0497, 0.0552, 0.0368, 0.0313, 0.0055, 0.0166, },  // E sum = 1.00
/* F */	{  0.0338, 0.0106, 0.0169, 0.0190, 0.3869, 0.0254, 0.0169, 0.0634, 0.0190, 0.1142, 0.0254, 0.0169, 0.0106, 0.0106, 0.0190, 0.0254, 0.0254, 0.0550, 0.0169, 0.0888, },  // F sum = 1.00
/* G */	{  0.0783, 0.0108, 0.0337, 0.0256, 0.0162, 0.5101, 0.0135, 0.0189, 0.0337, 0.0283, 0.0094, 0.0391, 0.0189, 0.0189, 0.0229, 0.0513, 0.0297, 0.0243, 0.0054, 0.0108, },  // G sum = 1.00
/* H */	{  0.0420, 0.0076, 0.0382, 0.0534, 0.0305, 0.0382, 0.3550, 0.0229, 0.0458, 0.0382, 0.0153, 0.0534, 0.0191, 0.0382, 0.0458, 0.0420, 0.0267, 0.0229, 0.0076, 0.0573, },  // H sum = 1.00
/* I */	{  0.0471, 0.0162, 0.0177, 0.0177, 0.0442, 0.0206, 0.0088, 0.2710, 0.0236, 0.1679, 0.0368, 0.0147, 0.0147, 0.0133, 0.0177, 0.0250, 0.0398, 0.1767, 0.0059, 0.0206, },  // I sum = 1.00
/* K */	{  0.0570, 0.0086, 0.0415, 0.0708, 0.0155, 0.0432, 0.0207, 0.0276, 0.2781, 0.0432, 0.0155, 0.0415, 0.0276, 0.0535, 0.1071, 0.0535, 0.0397, 0.0328, 0.0052, 0.0173, },  // K sum = 1.00
/* L */	{  0.0445, 0.0162, 0.0152, 0.0202, 0.0547, 0.0213, 0.0101, 0.1154, 0.0253, 0.3755, 0.0496, 0.0142, 0.0142, 0.0162, 0.0243, 0.0243, 0.0334, 0.0962, 0.0071, 0.0223, },  // L sum = 1.00
/* M */	{  0.0522, 0.0161, 0.0201, 0.0281, 0.0482, 0.0281, 0.0161, 0.1004, 0.0361, 0.1968, 0.1606, 0.0201, 0.0161, 0.0281, 0.0321, 0.0361, 0.0402, 0.0924, 0.0080, 0.0241, },  // M sum = 1.00
/* N */	{  0.0427, 0.0090, 0.0831, 0.0494, 0.0180, 0.0652, 0.0315, 0.0225, 0.0539, 0.0315, 0.0112, 0.3169, 0.0202, 0.0337, 0.0449, 0.0697, 0.0494, 0.0270, 0.0045, 0.0157, },  // N sum = 1.00
/* P */	{  0.0568, 0.0103, 0.0310, 0.0362, 0.0129, 0.0362, 0.0129, 0.0258, 0.0413, 0.0362, 0.0103, 0.0233, 0.4935, 0.0207, 0.0258, 0.0439, 0.0362, 0.0310, 0.0026, 0.0129, },  // P sum = 1.00
/* Q */	{  0.0559, 0.0088, 0.0471, 0.1029, 0.0147, 0.0412, 0.0294, 0.0265, 0.0912, 0.0471, 0.0206, 0.0441, 0.0235, 0.2147, 0.0735, 0.0559, 0.0412, 0.0353, 0.0059, 0.0206, },  // Q sum = 1.00
/* R */	{  0.0446, 0.0078, 0.0310, 0.0523, 0.0174, 0.0329, 0.0233, 0.0233, 0.1202, 0.0465, 0.0155, 0.0388, 0.0194, 0.0484, 0.3450, 0.0446, 0.0349, 0.0310, 0.0058, 0.0174, },  // R sum = 1.00
/* S */	{  0.1099, 0.0175, 0.0489, 0.0524, 0.0209, 0.0663, 0.0192, 0.0297, 0.0541, 0.0419, 0.0157, 0.0541, 0.0297, 0.0332, 0.0401, 0.2199, 0.0820, 0.0419, 0.0052, 0.0175, },  // S sum = 1.00
/* T */	{  0.0730, 0.0178, 0.0375, 0.0394, 0.0237, 0.0434, 0.0138, 0.0533, 0.0454, 0.0651, 0.0197, 0.0434, 0.0276, 0.0276, 0.0355, 0.0927, 0.2465, 0.0710, 0.0059, 0.0178, },  // T sum = 1.00
/* V */	{  0.0700, 0.0192, 0.0178, 0.0233, 0.0357, 0.0247, 0.0082, 0.1646, 0.0261, 0.1303, 0.0316, 0.0165, 0.0165, 0.0165, 0.0219, 0.0329, 0.0494, 0.2689, 0.0055, 0.0206, },  // V sum = 1.00
/* W */	{  0.0303, 0.0076, 0.0152, 0.0227, 0.0606, 0.0303, 0.0152, 0.0303, 0.0227, 0.0530, 0.0152, 0.0152, 0.0076, 0.0152, 0.0227, 0.0227, 0.0227, 0.0303, 0.4924, 0.0682, },  // W sum = 1.00
/* Y */	{  0.0405, 0.0093, 0.0187, 0.0280, 0.1308, 0.0249, 0.0467, 0.0436, 0.0312, 0.0685, 0.0187, 0.0218, 0.0156, 0.0218, 0.0280, 0.0312, 0.0280, 0.0467, 0.0280, 0.3178, },  // Y sum = 1.00
	};

double g_AAFreqs[20] =
        {
        0.07410, // A
        0.02460, // C
        0.05360, // D
        0.05430, // E
        0.04730, // F
        0.07410, // G
        0.02620, // H
        0.06790, // I
        0.05790, // K
        0.09880, // L
        0.02490, // M
        0.04450, // N
        0.03870, // P
        0.03400, // Q
        0.05160, // R
        0.05730, // S
        0.05070, // T
        0.07290, // V
        0.01320, // W
        0.03210, // Y
        }; // Sum = 0.99870
