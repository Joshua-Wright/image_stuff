//
// Created by j0sh on 11/5/15.
//

#ifndef IMAGE_STUFF_HOT_COLORMAP_H
#define IMAGE_STUFF_HOT_COLORMAP_H

#include "image_utils.h"

using namespace image_utils;

RGB* get_hot_colormap() {
    RGB *hot_colormap = new RGB[256];
    hot_colormap[0].r = 3;
    hot_colormap[0].g = 0;
    hot_colormap[0].b = 0;
    hot_colormap[1].r = 5;
    hot_colormap[1].g = 0;
    hot_colormap[1].b = 0;
    hot_colormap[2].r = 8;
    hot_colormap[2].g = 0;
    hot_colormap[2].b = 0;
    hot_colormap[3].r = 11;
    hot_colormap[3].g = 0;
    hot_colormap[3].b = 0;
    hot_colormap[4].r = 13;
    hot_colormap[4].g = 0;
    hot_colormap[4].b = 0;
    hot_colormap[5].r = 16;
    hot_colormap[5].g = 0;
    hot_colormap[5].b = 0;
    hot_colormap[6].r = 19;
    hot_colormap[6].g = 0;
    hot_colormap[6].b = 0;
    hot_colormap[7].r = 21;
    hot_colormap[7].g = 0;
    hot_colormap[7].b = 0;
    hot_colormap[8].r = 24;
    hot_colormap[8].g = 0;
    hot_colormap[8].b = 0;
    hot_colormap[9].r = 27;
    hot_colormap[9].g = 0;
    hot_colormap[9].b = 0;
    hot_colormap[10].r = 29;
    hot_colormap[10].g = 0;
    hot_colormap[10].b = 0;
    hot_colormap[11].r = 32;
    hot_colormap[11].g = 0;
    hot_colormap[11].b = 0;
    hot_colormap[12].r = 35;
    hot_colormap[12].g = 0;
    hot_colormap[12].b = 0;
    hot_colormap[13].r = 37;
    hot_colormap[13].g = 0;
    hot_colormap[13].b = 0;
    hot_colormap[14].r = 40;
    hot_colormap[14].g = 0;
    hot_colormap[14].b = 0;
    hot_colormap[15].r = 43;
    hot_colormap[15].g = 0;
    hot_colormap[15].b = 0;
    hot_colormap[16].r = 45;
    hot_colormap[16].g = 0;
    hot_colormap[16].b = 0;
    hot_colormap[17].r = 48;
    hot_colormap[17].g = 0;
    hot_colormap[17].b = 0;
    hot_colormap[18].r = 51;
    hot_colormap[18].g = 0;
    hot_colormap[18].b = 0;
    hot_colormap[19].r = 53;
    hot_colormap[19].g = 0;
    hot_colormap[19].b = 0;
    hot_colormap[20].r = 56;
    hot_colormap[20].g = 0;
    hot_colormap[20].b = 0;
    hot_colormap[21].r = 59;
    hot_colormap[21].g = 0;
    hot_colormap[21].b = 0;
    hot_colormap[22].r = 61;
    hot_colormap[22].g = 0;
    hot_colormap[22].b = 0;
    hot_colormap[23].r = 64;
    hot_colormap[23].g = 0;
    hot_colormap[23].b = 0;
    hot_colormap[24].r = 67;
    hot_colormap[24].g = 0;
    hot_colormap[24].b = 0;
    hot_colormap[25].r = 69;
    hot_colormap[25].g = 0;
    hot_colormap[25].b = 0;
    hot_colormap[26].r = 72;
    hot_colormap[26].g = 0;
    hot_colormap[26].b = 0;
    hot_colormap[27].r = 75;
    hot_colormap[27].g = 0;
    hot_colormap[27].b = 0;
    hot_colormap[28].r = 77;
    hot_colormap[28].g = 0;
    hot_colormap[28].b = 0;
    hot_colormap[29].r = 80;
    hot_colormap[29].g = 0;
    hot_colormap[29].b = 0;
    hot_colormap[30].r = 83;
    hot_colormap[30].g = 0;
    hot_colormap[30].b = 0;
    hot_colormap[31].r = 85;
    hot_colormap[31].g = 0;
    hot_colormap[31].b = 0;
    hot_colormap[32].r = 88;
    hot_colormap[32].g = 0;
    hot_colormap[32].b = 0;
    hot_colormap[33].r = 91;
    hot_colormap[33].g = 0;
    hot_colormap[33].b = 0;
    hot_colormap[34].r = 93;
    hot_colormap[34].g = 0;
    hot_colormap[34].b = 0;
    hot_colormap[35].r = 96;
    hot_colormap[35].g = 0;
    hot_colormap[35].b = 0;
    hot_colormap[36].r = 99;
    hot_colormap[36].g = 0;
    hot_colormap[36].b = 0;
    hot_colormap[37].r = 101;
    hot_colormap[37].g = 0;
    hot_colormap[37].b = 0;
    hot_colormap[38].r = 104;
    hot_colormap[38].g = 0;
    hot_colormap[38].b = 0;
    hot_colormap[39].r = 107;
    hot_colormap[39].g = 0;
    hot_colormap[39].b = 0;
    hot_colormap[40].r = 109;
    hot_colormap[40].g = 0;
    hot_colormap[40].b = 0;
    hot_colormap[41].r = 112;
    hot_colormap[41].g = 0;
    hot_colormap[41].b = 0;
    hot_colormap[42].r = 115;
    hot_colormap[42].g = 0;
    hot_colormap[42].b = 0;
    hot_colormap[43].r = 117;
    hot_colormap[43].g = 0;
    hot_colormap[43].b = 0;
    hot_colormap[44].r = 120;
    hot_colormap[44].g = 0;
    hot_colormap[44].b = 0;
    hot_colormap[45].r = 123;
    hot_colormap[45].g = 0;
    hot_colormap[45].b = 0;
    hot_colormap[46].r = 125;
    hot_colormap[46].g = 0;
    hot_colormap[46].b = 0;
    hot_colormap[47].r = 128;
    hot_colormap[47].g = 0;
    hot_colormap[47].b = 0;
    hot_colormap[48].r = 131;
    hot_colormap[48].g = 0;
    hot_colormap[48].b = 0;
    hot_colormap[49].r = 133;
    hot_colormap[49].g = 0;
    hot_colormap[49].b = 0;
    hot_colormap[50].r = 136;
    hot_colormap[50].g = 0;
    hot_colormap[50].b = 0;
    hot_colormap[51].r = 139;
    hot_colormap[51].g = 0;
    hot_colormap[51].b = 0;
    hot_colormap[52].r = 141;
    hot_colormap[52].g = 0;
    hot_colormap[52].b = 0;
    hot_colormap[53].r = 144;
    hot_colormap[53].g = 0;
    hot_colormap[53].b = 0;
    hot_colormap[54].r = 147;
    hot_colormap[54].g = 0;
    hot_colormap[54].b = 0;
    hot_colormap[55].r = 149;
    hot_colormap[55].g = 0;
    hot_colormap[55].b = 0;
    hot_colormap[56].r = 152;
    hot_colormap[56].g = 0;
    hot_colormap[56].b = 0;
    hot_colormap[57].r = 155;
    hot_colormap[57].g = 0;
    hot_colormap[57].b = 0;
    hot_colormap[58].r = 157;
    hot_colormap[58].g = 0;
    hot_colormap[58].b = 0;
    hot_colormap[59].r = 160;
    hot_colormap[59].g = 0;
    hot_colormap[59].b = 0;
    hot_colormap[60].r = 163;
    hot_colormap[60].g = 0;
    hot_colormap[60].b = 0;
    hot_colormap[61].r = 165;
    hot_colormap[61].g = 0;
    hot_colormap[61].b = 0;
    hot_colormap[62].r = 168;
    hot_colormap[62].g = 0;
    hot_colormap[62].b = 0;
    hot_colormap[63].r = 171;
    hot_colormap[63].g = 0;
    hot_colormap[63].b = 0;
    hot_colormap[64].r = 173;
    hot_colormap[64].g = 0;
    hot_colormap[64].b = 0;
    hot_colormap[65].r = 176;
    hot_colormap[65].g = 0;
    hot_colormap[65].b = 0;
    hot_colormap[66].r = 179;
    hot_colormap[66].g = 0;
    hot_colormap[66].b = 0;
    hot_colormap[67].r = 181;
    hot_colormap[67].g = 0;
    hot_colormap[67].b = 0;
    hot_colormap[68].r = 184;
    hot_colormap[68].g = 0;
    hot_colormap[68].b = 0;
    hot_colormap[69].r = 187;
    hot_colormap[69].g = 0;
    hot_colormap[69].b = 0;
    hot_colormap[70].r = 189;
    hot_colormap[70].g = 0;
    hot_colormap[70].b = 0;
    hot_colormap[71].r = 192;
    hot_colormap[71].g = 0;
    hot_colormap[71].b = 0;
    hot_colormap[72].r = 195;
    hot_colormap[72].g = 0;
    hot_colormap[72].b = 0;
    hot_colormap[73].r = 197;
    hot_colormap[73].g = 0;
    hot_colormap[73].b = 0;
    hot_colormap[74].r = 200;
    hot_colormap[74].g = 0;
    hot_colormap[74].b = 0;
    hot_colormap[75].r = 203;
    hot_colormap[75].g = 0;
    hot_colormap[75].b = 0;
    hot_colormap[76].r = 205;
    hot_colormap[76].g = 0;
    hot_colormap[76].b = 0;
    hot_colormap[77].r = 208;
    hot_colormap[77].g = 0;
    hot_colormap[77].b = 0;
    hot_colormap[78].r = 211;
    hot_colormap[78].g = 0;
    hot_colormap[78].b = 0;
    hot_colormap[79].r = 213;
    hot_colormap[79].g = 0;
    hot_colormap[79].b = 0;
    hot_colormap[80].r = 216;
    hot_colormap[80].g = 0;
    hot_colormap[80].b = 0;
    hot_colormap[81].r = 219;
    hot_colormap[81].g = 0;
    hot_colormap[81].b = 0;
    hot_colormap[82].r = 221;
    hot_colormap[82].g = 0;
    hot_colormap[82].b = 0;
    hot_colormap[83].r = 224;
    hot_colormap[83].g = 0;
    hot_colormap[83].b = 0;
    hot_colormap[84].r = 227;
    hot_colormap[84].g = 0;
    hot_colormap[84].b = 0;
    hot_colormap[85].r = 229;
    hot_colormap[85].g = 0;
    hot_colormap[85].b = 0;
    hot_colormap[86].r = 232;
    hot_colormap[86].g = 0;
    hot_colormap[86].b = 0;
    hot_colormap[87].r = 235;
    hot_colormap[87].g = 0;
    hot_colormap[87].b = 0;
    hot_colormap[88].r = 237;
    hot_colormap[88].g = 0;
    hot_colormap[88].b = 0;
    hot_colormap[89].r = 240;
    hot_colormap[89].g = 0;
    hot_colormap[89].b = 0;
    hot_colormap[90].r = 243;
    hot_colormap[90].g = 0;
    hot_colormap[90].b = 0;
    hot_colormap[91].r = 245;
    hot_colormap[91].g = 0;
    hot_colormap[91].b = 0;
    hot_colormap[92].r = 248;
    hot_colormap[92].g = 0;
    hot_colormap[92].b = 0;
    hot_colormap[93].r = 251;
    hot_colormap[93].g = 0;
    hot_colormap[93].b = 0;
    hot_colormap[94].r = 253;
    hot_colormap[94].g = 0;
    hot_colormap[94].b = 0;
    hot_colormap[95].r = 255;
    hot_colormap[95].g = 0;
    hot_colormap[95].b = 0;
    hot_colormap[96].r = 255;
    hot_colormap[96].g = 3;
    hot_colormap[96].b = 0;
    hot_colormap[97].r = 255;
    hot_colormap[97].g = 5;
    hot_colormap[97].b = 0;
    hot_colormap[98].r = 255;
    hot_colormap[98].g = 8;
    hot_colormap[98].b = 0;
    hot_colormap[99].r = 255;
    hot_colormap[99].g = 11;
    hot_colormap[99].b = 0;
    hot_colormap[100].r = 255;
    hot_colormap[100].g = 13;
    hot_colormap[100].b = 0;
    hot_colormap[101].r = 255;
    hot_colormap[101].g = 16;
    hot_colormap[101].b = 0;
    hot_colormap[102].r = 255;
    hot_colormap[102].g = 19;
    hot_colormap[102].b = 0;
    hot_colormap[103].r = 255;
    hot_colormap[103].g = 21;
    hot_colormap[103].b = 0;
    hot_colormap[104].r = 255;
    hot_colormap[104].g = 24;
    hot_colormap[104].b = 0;
    hot_colormap[105].r = 255;
    hot_colormap[105].g = 27;
    hot_colormap[105].b = 0;
    hot_colormap[106].r = 255;
    hot_colormap[106].g = 29;
    hot_colormap[106].b = 0;
    hot_colormap[107].r = 255;
    hot_colormap[107].g = 32;
    hot_colormap[107].b = 0;
    hot_colormap[108].r = 255;
    hot_colormap[108].g = 35;
    hot_colormap[108].b = 0;
    hot_colormap[109].r = 255;
    hot_colormap[109].g = 37;
    hot_colormap[109].b = 0;
    hot_colormap[110].r = 255;
    hot_colormap[110].g = 40;
    hot_colormap[110].b = 0;
    hot_colormap[111].r = 255;
    hot_colormap[111].g = 43;
    hot_colormap[111].b = 0;
    hot_colormap[112].r = 255;
    hot_colormap[112].g = 45;
    hot_colormap[112].b = 0;
    hot_colormap[113].r = 255;
    hot_colormap[113].g = 48;
    hot_colormap[113].b = 0;
    hot_colormap[114].r = 255;
    hot_colormap[114].g = 51;
    hot_colormap[114].b = 0;
    hot_colormap[115].r = 255;
    hot_colormap[115].g = 53;
    hot_colormap[115].b = 0;
    hot_colormap[116].r = 255;
    hot_colormap[116].g = 56;
    hot_colormap[116].b = 0;
    hot_colormap[117].r = 255;
    hot_colormap[117].g = 59;
    hot_colormap[117].b = 0;
    hot_colormap[118].r = 255;
    hot_colormap[118].g = 61;
    hot_colormap[118].b = 0;
    hot_colormap[119].r = 255;
    hot_colormap[119].g = 64;
    hot_colormap[119].b = 0;
    hot_colormap[120].r = 255;
    hot_colormap[120].g = 67;
    hot_colormap[120].b = 0;
    hot_colormap[121].r = 255;
    hot_colormap[121].g = 69;
    hot_colormap[121].b = 0;
    hot_colormap[122].r = 255;
    hot_colormap[122].g = 72;
    hot_colormap[122].b = 0;
    hot_colormap[123].r = 255;
    hot_colormap[123].g = 75;
    hot_colormap[123].b = 0;
    hot_colormap[124].r = 255;
    hot_colormap[124].g = 77;
    hot_colormap[124].b = 0;
    hot_colormap[125].r = 255;
    hot_colormap[125].g = 80;
    hot_colormap[125].b = 0;
    hot_colormap[126].r = 255;
    hot_colormap[126].g = 83;
    hot_colormap[126].b = 0;
    hot_colormap[127].r = 255;
    hot_colormap[127].g = 85;
    hot_colormap[127].b = 0;
    hot_colormap[128].r = 255;
    hot_colormap[128].g = 88;
    hot_colormap[128].b = 0;
    hot_colormap[129].r = 255;
    hot_colormap[129].g = 91;
    hot_colormap[129].b = 0;
    hot_colormap[130].r = 255;
    hot_colormap[130].g = 93;
    hot_colormap[130].b = 0;
    hot_colormap[131].r = 255;
    hot_colormap[131].g = 96;
    hot_colormap[131].b = 0;
    hot_colormap[132].r = 255;
    hot_colormap[132].g = 99;
    hot_colormap[132].b = 0;
    hot_colormap[133].r = 255;
    hot_colormap[133].g = 101;
    hot_colormap[133].b = 0;
    hot_colormap[134].r = 255;
    hot_colormap[134].g = 104;
    hot_colormap[134].b = 0;
    hot_colormap[135].r = 255;
    hot_colormap[135].g = 107;
    hot_colormap[135].b = 0;
    hot_colormap[136].r = 255;
    hot_colormap[136].g = 109;
    hot_colormap[136].b = 0;
    hot_colormap[137].r = 255;
    hot_colormap[137].g = 112;
    hot_colormap[137].b = 0;
    hot_colormap[138].r = 255;
    hot_colormap[138].g = 115;
    hot_colormap[138].b = 0;
    hot_colormap[139].r = 255;
    hot_colormap[139].g = 117;
    hot_colormap[139].b = 0;
    hot_colormap[140].r = 255;
    hot_colormap[140].g = 120;
    hot_colormap[140].b = 0;
    hot_colormap[141].r = 255;
    hot_colormap[141].g = 123;
    hot_colormap[141].b = 0;
    hot_colormap[142].r = 255;
    hot_colormap[142].g = 125;
    hot_colormap[142].b = 0;
    hot_colormap[143].r = 255;
    hot_colormap[143].g = 128;
    hot_colormap[143].b = 0;
    hot_colormap[144].r = 255;
    hot_colormap[144].g = 131;
    hot_colormap[144].b = 0;
    hot_colormap[145].r = 255;
    hot_colormap[145].g = 133;
    hot_colormap[145].b = 0;
    hot_colormap[146].r = 255;
    hot_colormap[146].g = 136;
    hot_colormap[146].b = 0;
    hot_colormap[147].r = 255;
    hot_colormap[147].g = 139;
    hot_colormap[147].b = 0;
    hot_colormap[148].r = 255;
    hot_colormap[148].g = 141;
    hot_colormap[148].b = 0;
    hot_colormap[149].r = 255;
    hot_colormap[149].g = 144;
    hot_colormap[149].b = 0;
    hot_colormap[150].r = 255;
    hot_colormap[150].g = 147;
    hot_colormap[150].b = 0;
    hot_colormap[151].r = 255;
    hot_colormap[151].g = 149;
    hot_colormap[151].b = 0;
    hot_colormap[152].r = 255;
    hot_colormap[152].g = 152;
    hot_colormap[152].b = 0;
    hot_colormap[153].r = 255;
    hot_colormap[153].g = 155;
    hot_colormap[153].b = 0;
    hot_colormap[154].r = 255;
    hot_colormap[154].g = 157;
    hot_colormap[154].b = 0;
    hot_colormap[155].r = 255;
    hot_colormap[155].g = 160;
    hot_colormap[155].b = 0;
    hot_colormap[156].r = 255;
    hot_colormap[156].g = 163;
    hot_colormap[156].b = 0;
    hot_colormap[157].r = 255;
    hot_colormap[157].g = 165;
    hot_colormap[157].b = 0;
    hot_colormap[158].r = 255;
    hot_colormap[158].g = 168;
    hot_colormap[158].b = 0;
    hot_colormap[159].r = 255;
    hot_colormap[159].g = 171;
    hot_colormap[159].b = 0;
    hot_colormap[160].r = 255;
    hot_colormap[160].g = 173;
    hot_colormap[160].b = 0;
    hot_colormap[161].r = 255;
    hot_colormap[161].g = 176;
    hot_colormap[161].b = 0;
    hot_colormap[162].r = 255;
    hot_colormap[162].g = 179;
    hot_colormap[162].b = 0;
    hot_colormap[163].r = 255;
    hot_colormap[163].g = 181;
    hot_colormap[163].b = 0;
    hot_colormap[164].r = 255;
    hot_colormap[164].g = 184;
    hot_colormap[164].b = 0;
    hot_colormap[165].r = 255;
    hot_colormap[165].g = 187;
    hot_colormap[165].b = 0;
    hot_colormap[166].r = 255;
    hot_colormap[166].g = 189;
    hot_colormap[166].b = 0;
    hot_colormap[167].r = 255;
    hot_colormap[167].g = 192;
    hot_colormap[167].b = 0;
    hot_colormap[168].r = 255;
    hot_colormap[168].g = 195;
    hot_colormap[168].b = 0;
    hot_colormap[169].r = 255;
    hot_colormap[169].g = 197;
    hot_colormap[169].b = 0;
    hot_colormap[170].r = 255;
    hot_colormap[170].g = 200;
    hot_colormap[170].b = 0;
    hot_colormap[171].r = 255;
    hot_colormap[171].g = 203;
    hot_colormap[171].b = 0;
    hot_colormap[172].r = 255;
    hot_colormap[172].g = 205;
    hot_colormap[172].b = 0;
    hot_colormap[173].r = 255;
    hot_colormap[173].g = 208;
    hot_colormap[173].b = 0;
    hot_colormap[174].r = 255;
    hot_colormap[174].g = 211;
    hot_colormap[174].b = 0;
    hot_colormap[175].r = 255;
    hot_colormap[175].g = 213;
    hot_colormap[175].b = 0;
    hot_colormap[176].r = 255;
    hot_colormap[176].g = 216;
    hot_colormap[176].b = 0;
    hot_colormap[177].r = 255;
    hot_colormap[177].g = 219;
    hot_colormap[177].b = 0;
    hot_colormap[178].r = 255;
    hot_colormap[178].g = 221;
    hot_colormap[178].b = 0;
    hot_colormap[179].r = 255;
    hot_colormap[179].g = 224;
    hot_colormap[179].b = 0;
    hot_colormap[180].r = 255;
    hot_colormap[180].g = 227;
    hot_colormap[180].b = 0;
    hot_colormap[181].r = 255;
    hot_colormap[181].g = 229;
    hot_colormap[181].b = 0;
    hot_colormap[182].r = 255;
    hot_colormap[182].g = 232;
    hot_colormap[182].b = 0;
    hot_colormap[183].r = 255;
    hot_colormap[183].g = 235;
    hot_colormap[183].b = 0;
    hot_colormap[184].r = 255;
    hot_colormap[184].g = 237;
    hot_colormap[184].b = 0;
    hot_colormap[185].r = 255;
    hot_colormap[185].g = 240;
    hot_colormap[185].b = 0;
    hot_colormap[186].r = 255;
    hot_colormap[186].g = 243;
    hot_colormap[186].b = 0;
    hot_colormap[187].r = 255;
    hot_colormap[187].g = 245;
    hot_colormap[187].b = 0;
    hot_colormap[188].r = 255;
    hot_colormap[188].g = 248;
    hot_colormap[188].b = 0;
    hot_colormap[189].r = 255;
    hot_colormap[189].g = 251;
    hot_colormap[189].b = 0;
    hot_colormap[190].r = 255;
    hot_colormap[190].g = 253;
    hot_colormap[190].b = 0;
    hot_colormap[191].r = 255;
    hot_colormap[191].g = 255;
    hot_colormap[191].b = 0;
    hot_colormap[192].r = 255;
    hot_colormap[192].g = 255;
    hot_colormap[192].b = 4;
    hot_colormap[193].r = 255;
    hot_colormap[193].g = 255;
    hot_colormap[193].b = 8;
    hot_colormap[194].r = 255;
    hot_colormap[194].g = 255;
    hot_colormap[194].b = 12;
    hot_colormap[195].r = 255;
    hot_colormap[195].g = 255;
    hot_colormap[195].b = 16;
    hot_colormap[196].r = 255;
    hot_colormap[196].g = 255;
    hot_colormap[196].b = 20;
    hot_colormap[197].r = 255;
    hot_colormap[197].g = 255;
    hot_colormap[197].b = 24;
    hot_colormap[198].r = 255;
    hot_colormap[198].g = 255;
    hot_colormap[198].b = 28;
    hot_colormap[199].r = 255;
    hot_colormap[199].g = 255;
    hot_colormap[199].b = 32;
    hot_colormap[200].r = 255;
    hot_colormap[200].g = 255;
    hot_colormap[200].b = 36;
    hot_colormap[201].r = 255;
    hot_colormap[201].g = 255;
    hot_colormap[201].b = 40;
    hot_colormap[202].r = 255;
    hot_colormap[202].g = 255;
    hot_colormap[202].b = 44;
    hot_colormap[203].r = 255;
    hot_colormap[203].g = 255;
    hot_colormap[203].b = 48;
    hot_colormap[204].r = 255;
    hot_colormap[204].g = 255;
    hot_colormap[204].b = 52;
    hot_colormap[205].r = 255;
    hot_colormap[205].g = 255;
    hot_colormap[205].b = 56;
    hot_colormap[206].r = 255;
    hot_colormap[206].g = 255;
    hot_colormap[206].b = 60;
    hot_colormap[207].r = 255;
    hot_colormap[207].g = 255;
    hot_colormap[207].b = 64;
    hot_colormap[208].r = 255;
    hot_colormap[208].g = 255;
    hot_colormap[208].b = 68;
    hot_colormap[209].r = 255;
    hot_colormap[209].g = 255;
    hot_colormap[209].b = 72;
    hot_colormap[210].r = 255;
    hot_colormap[210].g = 255;
    hot_colormap[210].b = 76;
    hot_colormap[211].r = 255;
    hot_colormap[211].g = 255;
    hot_colormap[211].b = 80;
    hot_colormap[212].r = 255;
    hot_colormap[212].g = 255;
    hot_colormap[212].b = 84;
    hot_colormap[213].r = 255;
    hot_colormap[213].g = 255;
    hot_colormap[213].b = 88;
    hot_colormap[214].r = 255;
    hot_colormap[214].g = 255;
    hot_colormap[214].b = 92;
    hot_colormap[215].r = 255;
    hot_colormap[215].g = 255;
    hot_colormap[215].b = 96;
    hot_colormap[216].r = 255;
    hot_colormap[216].g = 255;
    hot_colormap[216].b = 100;
    hot_colormap[217].r = 255;
    hot_colormap[217].g = 255;
    hot_colormap[217].b = 104;
    hot_colormap[218].r = 255;
    hot_colormap[218].g = 255;
    hot_colormap[218].b = 108;
    hot_colormap[219].r = 255;
    hot_colormap[219].g = 255;
    hot_colormap[219].b = 112;
    hot_colormap[220].r = 255;
    hot_colormap[220].g = 255;
    hot_colormap[220].b = 116;
    hot_colormap[221].r = 255;
    hot_colormap[221].g = 255;
    hot_colormap[221].b = 120;
    hot_colormap[222].r = 255;
    hot_colormap[222].g = 255;
    hot_colormap[222].b = 124;
    hot_colormap[223].r = 255;
    hot_colormap[223].g = 255;
    hot_colormap[223].b = 128;
    hot_colormap[224].r = 255;
    hot_colormap[224].g = 255;
    hot_colormap[224].b = 132;
    hot_colormap[225].r = 255;
    hot_colormap[225].g = 255;
    hot_colormap[225].b = 136;
    hot_colormap[226].r = 255;
    hot_colormap[226].g = 255;
    hot_colormap[226].b = 140;
    hot_colormap[227].r = 255;
    hot_colormap[227].g = 255;
    hot_colormap[227].b = 144;
    hot_colormap[228].r = 255;
    hot_colormap[228].g = 255;
    hot_colormap[228].b = 148;
    hot_colormap[229].r = 255;
    hot_colormap[229].g = 255;
    hot_colormap[229].b = 152;
    hot_colormap[230].r = 255;
    hot_colormap[230].g = 255;
    hot_colormap[230].b = 156;
    hot_colormap[231].r = 255;
    hot_colormap[231].g = 255;
    hot_colormap[231].b = 160;
    hot_colormap[232].r = 255;
    hot_colormap[232].g = 255;
    hot_colormap[232].b = 164;
    hot_colormap[233].r = 255;
    hot_colormap[233].g = 255;
    hot_colormap[233].b = 168;
    hot_colormap[234].r = 255;
    hot_colormap[234].g = 255;
    hot_colormap[234].b = 172;
    hot_colormap[235].r = 255;
    hot_colormap[235].g = 255;
    hot_colormap[235].b = 176;
    hot_colormap[236].r = 255;
    hot_colormap[236].g = 255;
    hot_colormap[236].b = 180;
    hot_colormap[237].r = 255;
    hot_colormap[237].g = 255;
    hot_colormap[237].b = 184;
    hot_colormap[238].r = 255;
    hot_colormap[238].g = 255;
    hot_colormap[238].b = 188;
    hot_colormap[239].r = 255;
    hot_colormap[239].g = 255;
    hot_colormap[239].b = 192;
    hot_colormap[240].r = 255;
    hot_colormap[240].g = 255;
    hot_colormap[240].b = 196;
    hot_colormap[241].r = 255;
    hot_colormap[241].g = 255;
    hot_colormap[241].b = 200;
    hot_colormap[242].r = 255;
    hot_colormap[242].g = 255;
    hot_colormap[242].b = 204;
    hot_colormap[243].r = 255;
    hot_colormap[243].g = 255;
    hot_colormap[243].b = 208;
    hot_colormap[244].r = 255;
    hot_colormap[244].g = 255;
    hot_colormap[244].b = 212;
    hot_colormap[245].r = 255;
    hot_colormap[245].g = 255;
    hot_colormap[245].b = 216;
    hot_colormap[246].r = 255;
    hot_colormap[246].g = 255;
    hot_colormap[246].b = 220;
    hot_colormap[247].r = 255;
    hot_colormap[247].g = 255;
    hot_colormap[247].b = 224;
    hot_colormap[248].r = 255;
    hot_colormap[248].g = 255;
    hot_colormap[248].b = 228;
    hot_colormap[249].r = 255;
    hot_colormap[249].g = 255;
    hot_colormap[249].b = 232;
    hot_colormap[250].r = 255;
    hot_colormap[250].g = 255;
    hot_colormap[250].b = 236;
    hot_colormap[251].r = 255;
    hot_colormap[251].g = 255;
    hot_colormap[251].b = 240;
    hot_colormap[252].r = 255;
    hot_colormap[252].g = 255;
    hot_colormap[252].b = 244;
    hot_colormap[253].r = 255;
    hot_colormap[253].g = 255;
    hot_colormap[253].b = 248;
    hot_colormap[254].r = 255;
    hot_colormap[254].g = 255;
    hot_colormap[254].b = 252;
    hot_colormap[255].r = 255;
    hot_colormap[255].g = 255;
    hot_colormap[255].b = 255;
    return hot_colormap;
};

#endif //IMAGE_STUFF_HOT_COLORMAP_H
