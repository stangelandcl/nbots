#ifndef NB_GRAPHICS_DRAWING_TOOLS_PIX_FONT_BITMAP_H_
#define NB_GRAPHICS_DRAWING_TOOLS_PIX_FONT_BITMAP_H_

/*
 * This header contains:
 *     font8x8_basic[128]
 *     font8x8_control[32]
 *     font8x8_latin[96]
 *     font8x8_greek[58]
 *     font8x8_block[32]
 *     font8x8_misc[10]
 *     font8x8_hiragana[96]
 *     font8x8_sga[26]
 */

/* Contains an 8x8 font map for unicode points U+0000 - U+007F (basic latin) */
static char font8x8_basic[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

/* Contains an 8x8 font map for unicode points U+0080 - U+009F (C1/C2 control) */
static char font8x8_control[32][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0080
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0081
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0082
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0083
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0084
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0085
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0086
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0087
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0088
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0089
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+008F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0090
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0091
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0092
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0093
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0094
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0095
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0096
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0097
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0098
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0099
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+009A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+009B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+009C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+009D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+009E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+009F
};

/* Contains an 8x8 font map for unicode points U+00A0 - U+00FF (extended latin) */
static char font8x8_latin[96][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+00A0 (no break space)
    { 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00},   // U+00A1 (inverted !)
    { 0x18, 0x18, 0x7E, 0x03, 0x03, 0x7E, 0x18, 0x18},   // U+00A2 (dollarcents)
    { 0x1C, 0x36, 0x26, 0x0F, 0x06, 0x67, 0x3F, 0x00},   // U+00A3 (pound sterling)
    { 0x00, 0x00, 0x63, 0x3E, 0x36, 0x3E, 0x63, 0x00},   // U+00A4 (currency mark)
    { 0x33, 0x33, 0x1E, 0x3F, 0x0C, 0x3F, 0x0C, 0x0C},   // U+00A5 (yen)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+00A6 (broken pipe)
    { 0x7C, 0xC6, 0x1C, 0x36, 0x36, 0x1C, 0x33, 0x1E},   // U+00A7 (paragraph)
    { 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+00A8 (diaeresis)
    { 0x3C, 0x42, 0x99, 0x85, 0x85, 0x99, 0x42, 0x3C},   // U+00A9 (copyright symbol)
    { 0x3C, 0x36, 0x36, 0x7C, 0x00, 0x00, 0x00, 0x00},   // U+00AA (superscript a)
    { 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00},   // U+00AB (<<)
    { 0x00, 0x00, 0x00, 0x3F, 0x30, 0x30, 0x00, 0x00},   // U+00AC (gun pointing left)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+00AD (soft hyphen)
    { 0x3C, 0x42, 0x9D, 0xA5, 0x9D, 0xA5, 0x42, 0x3C},   // U+00AE (registered symbol)
    { 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+00AF (macron)
    { 0x1C, 0x36, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00},   // U+00B0 (degree)
    { 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00},   // U+00B1 (plusminus)
    { 0x1C, 0x30, 0x18, 0x0C, 0x3C, 0x00, 0x00, 0x00},   // U+00B2 (superscript 2)
    { 0x1C, 0x30, 0x18, 0x30, 0x1C, 0x00, 0x00, 0x00},   // U+00B2 (superscript 3)
    { 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+00B2 (aigu)
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x03},   // U+00B5 (mu)
    { 0xFE, 0xDB, 0xDB, 0xDE, 0xD8, 0xD8, 0xD8, 0x00},   // U+00B6 (pilcrow)
    { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},   // U+00B7 (central dot)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x1E},   // U+00B8 (cedille)
    { 0x08, 0x0C, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00},   // U+00B9 (superscript 1)
    { 0x1C, 0x36, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00},   // U+00BA (superscript 0)
    { 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00},   // U+00BB (>>)
    { 0xC3, 0x63, 0x33, 0xBD, 0xEC, 0xF6, 0xF3, 0x03},   // U+00BC (1/4)
    { 0xC3, 0x63, 0x33, 0x7B, 0xCC, 0x66, 0x33, 0xF0},   // U+00BD (1/2)
    { 0x03, 0xC4, 0x63, 0xB4, 0xDB, 0xAC, 0xE6, 0x80},   // U+00BE (3/4)
    { 0x0C, 0x00, 0x0C, 0x06, 0x03, 0x33, 0x1E, 0x00},   // U+00BF (inverted ?)
    { 0x07, 0x00, 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x00},   // U+00C0 (A grave)
    { 0x70, 0x00, 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x00},   // U+00C1 (A aigu)
    { 0x1C, 0x36, 0x00, 0x3E, 0x63, 0x7F, 0x63, 0x00},   // U+00C2 (A circumflex)
    { 0x6E, 0x3B, 0x00, 0x3E, 0x63, 0x7F, 0x63, 0x00},   // U+00C3 (A ~)
    { 0x63, 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x63, 0x00},   // U+00C4 (A umlaut)
    { 0x0C, 0x0C, 0x00, 0x1E, 0x33, 0x3F, 0x33, 0x00},   // U+00C5 (A ring)
    { 0x7C, 0x36, 0x33, 0x7F, 0x33, 0x33, 0x73, 0x00},   // U+00C6 (AE)
    { 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x18, 0x30, 0x1E},   // U+00C7 (C cedille)
    { 0x07, 0x00, 0x3F, 0x06, 0x1E, 0x06, 0x3F, 0x00},   // U+00C8 (E grave)
    { 0x38, 0x00, 0x3F, 0x06, 0x1E, 0x06, 0x3F, 0x00},   // U+00C9 (E aigu)
    { 0x0C, 0x12, 0x3F, 0x06, 0x1E, 0x06, 0x3F, 0x00},   // U+00CA (E circumflex)
    { 0x36, 0x00, 0x3F, 0x06, 0x1E, 0x06, 0x3F, 0x00},   // U+00CB (E umlaut)
    { 0x07, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00CC (I grave)
    { 0x38, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00CD (I aigu)
    { 0x0C, 0x12, 0x00, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+00CE (I circumflex)
    { 0x33, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00CF (I umlaut)
    { 0x3F, 0x66, 0x6F, 0x6F, 0x66, 0x66, 0x3F, 0x00},   // U+00D0 (Eth)
    { 0x3F, 0x00, 0x33, 0x37, 0x3F, 0x3B, 0x33, 0x00},   // U+00D1 (N ~)
    { 0x0E, 0x00, 0x18, 0x3C, 0x66, 0x3C, 0x18, 0x00},   // U+00D2 (O grave)
    { 0x70, 0x00, 0x18, 0x3C, 0x66, 0x3C, 0x18, 0x00},   // U+00D3 (O aigu)
    { 0x3C, 0x66, 0x18, 0x3C, 0x66, 0x3C, 0x18, 0x00},   // U+00D4 (O circumflex)
    { 0x6E, 0x3B, 0x00, 0x3E, 0x63, 0x63, 0x3E, 0x00},   // U+00D5 (O ~)
    { 0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00},   // U+00D6 (O umlaut)
    { 0x00, 0x36, 0x1C, 0x08, 0x1C, 0x36, 0x00, 0x00},   // U+00D7 (multiplicative x)
    { 0x5C, 0x36, 0x73, 0x7B, 0x6F, 0x36, 0x1D, 0x00},   // U+00D8 (O stroke)
    { 0x0E, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},   // U+00D9 (U grave)
    { 0x70, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},   // U+00DA (U aigu)
    { 0x3C, 0x66, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x00},   // U+00DB (U circumflex)
    { 0x33, 0x00, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+00DC (U umlaut)
    { 0x70, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x00},   // U+00DD (Y aigu)
    { 0x0F, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+00DE (Thorn)
    { 0x00, 0x1E, 0x33, 0x1F, 0x33, 0x1F, 0x03, 0x03},   // U+00DF (beta)
    { 0x07, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},   // U+00E0 (a grave)
    { 0x38, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},   // U+00E1 (a aigu)
    { 0x7E, 0xC3, 0x3C, 0x60, 0x7C, 0x66, 0xFC, 0x00},   // U+00E2 (a circumflex)
    { 0x6E, 0x3B, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},   // U+00E3 (a ~)
    { 0x33, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},   // U+00E4 (a umlaut)
    { 0x0C, 0x0C, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00},   // U+00E5 (a ring)
    { 0x00, 0x00, 0xFE, 0x30, 0xFE, 0x33, 0xFE, 0x00},   // U+00E6 (ae)
    { 0x00, 0x00, 0x1E, 0x03, 0x03, 0x1E, 0x30, 0x1C},   // U+00E7 (c cedille)
    { 0x07, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00},   // U+00E8 (e grave)
    { 0x38, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00},   // U+00E9 (e aigu)
    { 0x7E, 0xC3, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00},   // U+00EA (e circumflex)
    { 0x33, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00},   // U+00EB (e umlaut)
    { 0x07, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00EC (i grave)
    { 0x1C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00ED (i augu)
    { 0x3E, 0x63, 0x1C, 0x18, 0x18, 0x18, 0x3C, 0x00},   // U+00EE (i circumflex)
    { 0x33, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+00EF (i umlaut)
    { 0x1B, 0x0E, 0x1B, 0x30, 0x3E, 0x33, 0x1E, 0x00},   // U+00F0 (eth)
    { 0x00, 0x1F, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x00},   // U+00F1 (n ~)
    { 0x00, 0x07, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+00F2 (o grave)
    { 0x00, 0x38, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+00F3 (o aigu)
    { 0x1E, 0x33, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+00F4 (o circumflex)
    { 0x6E, 0x3B, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+00F5 (o ~)
    { 0x00, 0x33, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+00F6 (o umlaut)
    { 0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00},   // U+00F7 (division)
    { 0x00, 0x60, 0x3C, 0x76, 0x7E, 0x6E, 0x3C, 0x06},   // U+00F8 (o stroke)
    { 0x00, 0x07, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00},   // U+00F9 (u grave)
    { 0x00, 0x38, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00},   // U+00FA (u aigu)
    { 0x1E, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00},   // U+00FB (u circumflex)
    { 0x00, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00},   // U+00FC (u umlaut)
    { 0x00, 0x38, 0x00, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+00FD (y aigu)
    { 0x00, 0x00, 0x06, 0x3E, 0x66, 0x3E, 0x06, 0x00},   // U+00FE (thorn)
    { 0x00, 0x33, 0x00, 0x33, 0x33, 0x3E, 0x30, 0x1F}    // U+00FF (y umlaut)
};

/* Contains an 8x8 font map for unicode points U+0390 - U+03C9 (greek characters) */
static char font8x8_greek[58][8] = {
    { 0x2D, 0x00, 0x0C, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0390 (iota with tonos and diaeresis)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0391 (Alpha)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0392 (Beta)
    { 0x3F, 0x33, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00},   // U+0393 (Gamma)
    { 0x08, 0x1C, 0x1C, 0x36, 0x36, 0x63, 0x7F, 0x00},   // U+0394 (Delta)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0395 (Epsilon)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+0396 (Zeta)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0397 (Eta)
    { 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x36, 0x1C, 0x00},   // U+0398 (Theta)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0399 (Iota)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+039A (Kappa)
    { 0x08, 0x1C, 0x1C, 0x36, 0x36, 0x63, 0x63, 0x00},   // U+039B (Lambda)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+039C (Mu)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+039D (Nu)
    { 0x7F, 0x63, 0x00, 0x3E, 0x00, 0x63, 0x7F, 0x00},   // U+039E (Xi)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+039F (Omikron)
    { 0x7F, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00},   // U+03A0 (Pi)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+03A1 (Rho)
    { 0x00, 0x01, 0x02, 0x04, 0x4F, 0x90, 0xA0, 0x40},   // U+03A2
    { 0x7F, 0x63, 0x06, 0x0C, 0x06, 0x63, 0x7F, 0x00},   // U+03A3 (Sigma 2)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+03A4 (Tau)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+03A5 (Upsilon)
    { 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x00},   // U+03A6 (Phi)
    { 0x63, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x63, 0x00},   // U+03A7 (Chi)
    { 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x18, 0x3C, 0x00},   // U+03A8 (Psi)
    { 0x3E, 0x63, 0x63, 0x63, 0x36, 0x36, 0x77, 0x00},   // U+03A9 (Omega)
    { 0x33, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0399 (Iota with diaeresis)
    { 0x33, 0x00, 0x33, 0x33, 0x1E, 0x0C, 0x1E, 0x00},   // U+03A5 (Upsilon with diaeresis)
    { 0x70, 0x00, 0x6E, 0x3B, 0x13, 0x3B, 0x6E, 0x00},   // U+03AC (alpha aigu)
    { 0x38, 0x00, 0x1E, 0x03, 0x0E, 0x03, 0x1E, 0x00},   // U+03AD (epsilon aigu)
    { 0x38, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x30},   // U+03AE (eta aigu)
    { 0x38, 0x00, 0x0C, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+03AF (iota aigu)
    { 0x2D, 0x00, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+03B0 (upsilon with tonos and diaeresis)
    { 0x00, 0x00, 0x6E, 0x3B, 0x13, 0x3B, 0x6E, 0x00},   // U+03B1 (alpha)
    { 0x00, 0x1E, 0x33, 0x1F, 0x33, 0x1F, 0x03, 0x03},   // U+03B2 (beta)
    { 0x00, 0x00, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x00},   // U+03B3 (gamma)
    { 0x38, 0x0C, 0x18, 0x3E, 0x33, 0x33, 0x1E, 0x00},   // U+03B4 (delta)
    { 0x00, 0x00, 0x1E, 0x03, 0x0E, 0x03, 0x1E, 0x00},   // U+03B5 (epsilon)
    { 0x00, 0x3F, 0x06, 0x03, 0x03, 0x1E, 0x30, 0x1C},   // U+03B6 (zeta)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x30},   // U+03B7 (eta)
    { 0x00, 0x00, 0x1E, 0x33, 0x3F, 0x33, 0x1E, 0x00},   // U+03B8 (theta)
    { 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+03B9 (iota)
    { 0x00, 0x00, 0x33, 0x1B, 0x0F, 0x1B, 0x33, 0x00},   // U+03BA (kappa)
    { 0x00, 0x03, 0x06, 0x0C, 0x1C, 0x36, 0x63, 0x00},   // U+03BB (lambda)
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x03},   // U+03BC (mu)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+03BD (nu)
    { 0x1E, 0x03, 0x0E, 0x03, 0x03, 0x1E, 0x30, 0x1C},   // U+03BE (xi)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+03BF (omikron)
    { 0x00, 0x00, 0x7F, 0x36, 0x36, 0x36, 0x36, 0x00},   // U+03C0 (pi)
    { 0x00, 0x00, 0x3C, 0x66, 0x66, 0x36, 0x06, 0x06},   // U+03C1 (rho)
    { 0x00, 0x00, 0x3E, 0x03, 0x03, 0x1E, 0x30, 0x1C},   // U+03C2 (sigma 1)
    { 0x00, 0x00, 0x7E, 0x1B, 0x1B, 0x1B, 0x0E, 0x00},   // U+03C3 (sigma 2)
    { 0x00, 0x00, 0x7E, 0x18, 0x18, 0x58, 0x30, 0x00},   // U+03C4 (tau)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+03C5 (upsilon)
    { 0x00, 0x00, 0x76, 0xDB, 0xDB, 0x7E, 0x18, 0x00},   // U+03C6 (phi)
    { 0x00, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+03C7 (chi)
    { 0x00, 0x00, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x00},   // U+03C8 (psi)
    { 0x00, 0x00, 0x36, 0x63, 0x6B, 0x7F, 0x36, 0x00}    // U+03C9 (omega)
};

/* Contains an 8x8 font map for unicode points U+2580 - U+259F (block elements) */
static char font8x8_block[32][8] = {
    { 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00},   // U+2580 (top half)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+2581 (box 1/8)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF},   // U+2582 (box 2/8)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF},   // U+2583 (box 3/8)
    { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2584 (bottom half)
    { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2585 (box 5/8)
    { 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2586 (box 6/8)
    { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2587 (box 7/8)
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2588 (solid)
    { 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F},   // U+2589 (box 7/8)
    { 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F},   // U+258A (box 6/8)
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},   // U+258B (box 5/8)
    { 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F},   // U+258C (left half)
    { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07},   // U+258D (box 3/8)
    { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03},   // U+258E (box 2/8)
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},   // U+258F (box 1/8)
    { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0},   // U+2590 (right half)
    { 0x55, 0x00, 0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00},   // U+2591 (25% solid)
    { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA},   // U+2592 (50% solid)
    { 0xFF, 0xAA, 0xFF, 0x55, 0xFF, 0xAA, 0xFF, 0x55},   // U+2593 (75% solid)
    { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+2594 (box 1/8)
    { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},   // U+2595 (box 1/8)
    { 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F},   // U+2596 (box bottom left)
    { 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0},   // U+2597 (box bottom right)
    { 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00},   // U+2598 (box top left)
    { 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF},   // U+2599 (boxes left and bottom)
    { 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0},   // U+259A (boxes top-left and bottom right)
    { 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F},   // U+259B (boxes top and left)
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0},   // U+259C (boxes top and right)
    { 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00},   // U+259D (box top right)
    { 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F},   // U+259E (boxes top right and bottom left)
    { 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF},   // U+259F (boxes right and bottom)
};

static char font8x8_misc[10][8] = {
    { 0x1F, 0x33, 0x33, 0x5F, 0x63, 0xF3, 0x63, 0xE3},   // U+20A7 (Spanish Pesetas/Pt)
    { 0x70, 0xD8, 0x18, 0x3C, 0x18, 0x18, 0x1B, 0x0E},   // U+0192 (dutch florijn)
    { 0x3C, 0x36, 0x36, 0x7C, 0x00, 0x7E, 0x00, 0x00},   // U+ (underlined superscript a)
    { 0x1C, 0x36, 0x36, 0x1C, 0x00, 0x3E, 0x00, 0x00},   // U+ (underlined superscript 0)
    { 0x00, 0x00, 0x00, 0x3F, 0x03, 0x03, 0x00, 0x00},   // U+2310 (gun pointing right)
    { 0x30, 0x18, 0x0C, 0x18, 0x30, 0x00, 0x7E, 0x00},   // U+ (less than or equal)
    { 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x00, 0x7E, 0x00},   // U+ (greater than or equal)
    { 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+ (grave)
    { 0x0E, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x00},   // U+ (Y grave)
    { 0x00, 0x07, 0x00, 0x33, 0x33, 0x3E, 0x30, 0x1F}    // U+ (y grave)
};

/* Contains an 8x8 font map for unicode points U+3040 - U+309F (Hiragana) */
static char font8x8_hiragana[96][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3040
    { 0x04, 0x3F, 0x04, 0x3C, 0x56, 0x4D, 0x26, 0x00},   // U+3041 (Hiragana a)
    { 0x04, 0x3F, 0x04, 0x3C, 0x56, 0x4D, 0x26, 0x00},   // U+3042 (Hiragana A)
    { 0x00, 0x00, 0x00, 0x11, 0x21, 0x25, 0x02, 0x00},   // U+3043 (Hiragana i)
    { 0x00, 0x01, 0x11, 0x21, 0x21, 0x25, 0x02, 0x00},   // U+3044 (Hiragana I)
    { 0x00, 0x1C, 0x00, 0x1C, 0x22, 0x20, 0x18, 0x00},   // U+3045 (Hiragana u)
    { 0x3C, 0x00, 0x3C, 0x42, 0x40, 0x20, 0x18, 0x00},   // U+3046 (Hiragana U)
    { 0x1C, 0x00, 0x3E, 0x10, 0x38, 0x24, 0x62, 0x00},   // U+3047 (Hiragana e)
    { 0x1C, 0x00, 0x3E, 0x10, 0x38, 0x24, 0x62, 0x00},   // U+3048 (Hiragana E)
    { 0x24, 0x4F, 0x04, 0x3C, 0x46, 0x45, 0x22, 0x00},   // U+3049 (Hiragana o)
    { 0x24, 0x4F, 0x04, 0x3C, 0x46, 0x45, 0x22, 0x00},   // U+304A (Hiragana O)
    { 0x04, 0x24, 0x4F, 0x54, 0x52, 0x12, 0x09, 0x00},   // U+304B (Hiragana KA)
    { 0x44, 0x24, 0x0F, 0x54, 0x52, 0x52, 0x09, 0x00},   // U+304C (Hiragana GA)
    { 0x08, 0x1F, 0x08, 0x3F, 0x1C, 0x02, 0x3C, 0x00},   // U+304D (Hiragana KI)
    { 0x44, 0x2F, 0x04, 0x1F, 0x0E, 0x01, 0x1E, 0x00},   // U+304E (Hiragana GI)
    { 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00},   // U+304F (Hiragana KU)
    { 0x28, 0x44, 0x12, 0x21, 0x02, 0x04, 0x08, 0x00},   // U+3050 (Hiragana GU)
    { 0x00, 0x22, 0x79, 0x21, 0x21, 0x22, 0x10, 0x00},   // U+3051 (Hiragana KE)
    { 0x40, 0x22, 0x11, 0x3D, 0x11, 0x12, 0x08, 0x00},   // U+3052 (Hiragana GE)
    { 0x00, 0x00, 0x3C, 0x00, 0x02, 0x02, 0x3C, 0x00},   // U+3053 (Hiragana KO)
    { 0x20, 0x40, 0x16, 0x20, 0x01, 0x01, 0x0E, 0x00},   // U+3054 (Hiragana GO)
    { 0x10, 0x7E, 0x10, 0x3C, 0x02, 0x02, 0x1C, 0x00},   // U+3055 (Hiragana SA)
    { 0x24, 0x4F, 0x14, 0x2E, 0x01, 0x01, 0x0E, 0x00},   // U+3056 (Hiragana ZA)
    { 0x00, 0x02, 0x02, 0x02, 0x42, 0x22, 0x1C, 0x00},   // U+3057 (Hiragana SI)
    { 0x20, 0x42, 0x12, 0x22, 0x02, 0x22, 0x1C, 0x00},   // U+3058 (Hiragana ZI)
    { 0x10, 0x7E, 0x18, 0x14, 0x18, 0x10, 0x0C, 0x00},   // U+3059 (Hiragana SU)
    { 0x44, 0x2F, 0x06, 0x05, 0x06, 0x04, 0x03, 0x00},   // U+305A (Hiragana ZU)
    { 0x20, 0x72, 0x2F, 0x22, 0x1A, 0x02, 0x1C, 0x00},   // U+305B (Hiragana SE)
    { 0x80, 0x50, 0x3A, 0x17, 0x1A, 0x02, 0x1C, 0x00},   // U+305C (Hiragana ZE)
    { 0x1E, 0x08, 0x04, 0x7F, 0x08, 0x04, 0x38, 0x00},   // U+305D (Hiragana SO)
    { 0x4F, 0x24, 0x02, 0x7F, 0x08, 0x04, 0x38, 0x00},   // U+305E (Hiragana ZO)
    { 0x02, 0x0F, 0x02, 0x72, 0x02, 0x09, 0x71, 0x00},   // U+305F (Hiragana TA)
    { 0x42, 0x2F, 0x02, 0x72, 0x02, 0x09, 0x71, 0x00},   // U+3060 (Hiragana DA)
    { 0x08, 0x7E, 0x08, 0x3C, 0x40, 0x40, 0x38, 0x00},   // U+3061 (Hiragana TI)
    { 0x44, 0x2F, 0x04, 0x1E, 0x20, 0x20, 0x1C, 0x00},   // U+3062 (Hiragana DI)
    { 0x00, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x1C, 0x00},   // U+3063 (Hiragana tu)
    { 0x00, 0x1C, 0x22, 0x41, 0x40, 0x20, 0x1C, 0x00},   // U+3064 (Hiragana TU)
    { 0x40, 0x20, 0x1E, 0x21, 0x20, 0x20, 0x1C, 0x00},   // U+3065 (Hiragana DU)
    { 0x00, 0x3E, 0x08, 0x04, 0x04, 0x04, 0x38, 0x00},   // U+3066 (Hiragana TE)
    { 0x00, 0x3E, 0x48, 0x24, 0x04, 0x04, 0x38, 0x00},   // U+3067 (Hiragana DE)
    { 0x04, 0x04, 0x08, 0x3C, 0x02, 0x02, 0x3C, 0x00},   // U+3068 (Hiragana TO)
    { 0x44, 0x24, 0x08, 0x3C, 0x02, 0x02, 0x3C, 0x00},   // U+3069 (Hiragana DO)
    { 0x32, 0x02, 0x27, 0x22, 0x72, 0x29, 0x11, 0x00},   // U+306A (Hiragana NA)
    { 0x00, 0x02, 0x7A, 0x02, 0x0A, 0x72, 0x02, 0x00},   // U+306B (Hiragana NI)
    { 0x08, 0x09, 0x3E, 0x4B, 0x65, 0x55, 0x22, 0x00},   // U+306C (Hiragana NU)
    { 0x04, 0x07, 0x34, 0x4C, 0x66, 0x54, 0x24, 0x00},   // U+306D (Hiragana NE)
    { 0x00, 0x00, 0x3C, 0x4A, 0x49, 0x45, 0x22, 0x00},   // U+306E (Hiragana NO)
    { 0x00, 0x22, 0x7A, 0x22, 0x72, 0x2A, 0x12, 0x00},   // U+306F (Hiragana HA)
    { 0x80, 0x51, 0x1D, 0x11, 0x39, 0x15, 0x09, 0x00},   // U+3070 (Hiragana BA)
    { 0x40, 0xB1, 0x5D, 0x11, 0x39, 0x15, 0x09, 0x00},   // U+3071 (Hiragana PA)
    { 0x00, 0x00, 0x13, 0x32, 0x51, 0x11, 0x0E, 0x00},   // U+3072 (Hiragana HI)
    { 0x40, 0x20, 0x03, 0x32, 0x51, 0x11, 0x0E, 0x00},   // U+3073 (Hiragana BI)
    { 0x40, 0xA0, 0x43, 0x32, 0x51, 0x11, 0x0E, 0x00},   // U+3074 (Hiragana PI)
    { 0x1C, 0x00, 0x08, 0x2A, 0x49, 0x10, 0x0C, 0x00},   // U+3075 (Hiragana HU)
    { 0x4C, 0x20, 0x08, 0x2A, 0x49, 0x10, 0x0C, 0x00},   // U+3076 (Hiragana BU)
    { 0x4C, 0xA0, 0x48, 0x0A, 0x29, 0x48, 0x0C, 0x00},   // U+3077 (Hiragana PU)
    { 0x00, 0x00, 0x04, 0x0A, 0x11, 0x20, 0x40, 0x00},   // U+3078 (Hiragana HE)
    { 0x20, 0x40, 0x14, 0x2A, 0x11, 0x20, 0x40, 0x00},   // U+3079 (Hiragana BE)
    { 0x20, 0x50, 0x24, 0x0A, 0x11, 0x20, 0x40, 0x00},   // U+307A (Hiragana PE)
    { 0x7D, 0x11, 0x7D, 0x11, 0x39, 0x55, 0x09, 0x00},   // U+307B (Hiragana HO)
    { 0x9D, 0x51, 0x1D, 0x11, 0x39, 0x55, 0x09, 0x00},   // U+307C (Hiragana BO)
    { 0x5D, 0xB1, 0x5D, 0x11, 0x39, 0x55, 0x09, 0x00},   // U+307D (Hiragana PO)
    { 0x7E, 0x08, 0x3E, 0x08, 0x1C, 0x2A, 0x04, 0x00},   // U+307E (Hiragana MA)
    { 0x00, 0x07, 0x24, 0x24, 0x7E, 0x25, 0x12, 0x00},   // U+307F (Hiragana MI)
    { 0x04, 0x0F, 0x64, 0x06, 0x05, 0x26, 0x3C, 0x00},   // U+3080 (Hiragana MU)
    { 0x00, 0x09, 0x3D, 0x4A, 0x4B, 0x45, 0x2A, 0x00},   // U+3081 (Hiragana ME)
    { 0x02, 0x0F, 0x02, 0x0F, 0x62, 0x42, 0x3C, 0x00},   // U+3082 (Hiragana MO)
    { 0x00, 0x00, 0x12, 0x1F, 0x22, 0x12, 0x04, 0x00},   // U+3083 (Hiragana ya)
    { 0x00, 0x12, 0x3F, 0x42, 0x42, 0x34, 0x04, 0x00},   // U+3084 (Hiragana YA)
    { 0x00, 0x00, 0x11, 0x3D, 0x53, 0x39, 0x11, 0x00},   // U+3085 (Hiragana yu)
    { 0x00, 0x11, 0x3D, 0x53, 0x51, 0x39, 0x11, 0x00},   // U+3086 (Hiragana YU)
    { 0x00, 0x08, 0x38, 0x08, 0x1C, 0x2A, 0x04, 0x00},   // U+3087 (Hiragana yo)
    { 0x08, 0x08, 0x38, 0x08, 0x1C, 0x2A, 0x04, 0x00},   // U+3088 (Hiragana YO)
    { 0x1E, 0x00, 0x02, 0x3A, 0x46, 0x42, 0x30, 0x00},   // U+3089 (Hiragana RA)
    { 0x00, 0x20, 0x22, 0x22, 0x2A, 0x24, 0x10, 0x00},   // U+308A (Hiragana RI)
    { 0x1F, 0x08, 0x3C, 0x42, 0x49, 0x54, 0x38, 0x00},   // U+308B (Hiragana RU)
    { 0x04, 0x07, 0x04, 0x0C, 0x16, 0x55, 0x24, 0x00},   // U+308C (Hiragana RE)
    { 0x3F, 0x10, 0x08, 0x3C, 0x42, 0x41, 0x30, 0x00},   // U+308D (Hiragana RO)
    { 0x00, 0x00, 0x08, 0x0E, 0x38, 0x4C, 0x2A, 0x00},   // U+308E (Hiragana wa)
    { 0x04, 0x07, 0x04, 0x3C, 0x46, 0x45, 0x24, 0x00},   // U+308F (Hiragana WA)
    { 0x0E, 0x08, 0x3C, 0x4A, 0x69, 0x55, 0x32, 0x00},   // U+3090 (Hiragana WI)
    { 0x06, 0x3C, 0x42, 0x39, 0x04, 0x36, 0x49, 0x00},   // U+3091 (Hiragana WE)
    { 0x04, 0x0F, 0x04, 0x6E, 0x11, 0x08, 0x70, 0x00},   // U+3092 (Hiragana WO)
    { 0x08, 0x08, 0x04, 0x0C, 0x56, 0x52, 0x21, 0x00},   // U+3093 (Hiragana N)
    { 0x40, 0x2E, 0x00, 0x3C, 0x42, 0x40, 0x38, 0x00},   // U+3094 (Hiragana VU)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3095
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3096
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3097
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3098
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+3099 (voiced combinator mark)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+309A (semivoiced combinator mark)
    { 0x40, 0x80, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00},   // U+309B (Hiragana voiced mark)
    { 0x40, 0xA0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+309C (Hiragana semivoiced mark)
    { 0x00, 0x00, 0x08, 0x08, 0x10, 0x30, 0x0C, 0x00},   // U+309D (Hiragana iteration mark)
    { 0x20, 0x40, 0x14, 0x24, 0x08, 0x18, 0x06, 0x00},   // U+309E (Hiragana voiced iteration mark)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+309F
};

/* Standard Galactic Alphabet */
static char font8x8_sga[26][8] = {
    { 0x00, 0x00, 0x38, 0x66, 0x06, 0x06, 0x07, 0x00},   // U+E541 (SGA A)
    { 0x00, 0x00, 0x0C, 0x0C, 0x18, 0x30, 0x7F, 0x00},   // U+E542 (SGA B)
    { 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x30, 0x30, 0x00},   // U+E543 (SGA C)
    { 0x00, 0x00, 0x7F, 0x00, 0x03, 0x1C, 0x60, 0x00},   // U+E544 (SGA D)
    { 0x00, 0x00, 0x63, 0x03, 0x03, 0x03, 0x7F, 0x00},   // U+E545 (SGA E)
    { 0x00, 0x00, 0x00, 0xFF, 0x00, 0xDB, 0x00, 0x00},   // U+E546 (SGA F)
    { 0x00, 0x00, 0x30, 0x30, 0x3E, 0x30, 0x30, 0x00},   // U+E547 (SGA G)
    { 0x00, 0x00, 0x7E, 0x00, 0x7E, 0x18, 0x18, 0x00},   // U+E548 (SGA H)
    { 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00},   // U+E549 (SGA I)
    { 0x00, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00},   // U+E54A (SGA J)
    { 0x00, 0x00, 0x18, 0x18, 0x5A, 0x18, 0x18, 0x00},   // U+E54B (SGA K)
    { 0x00, 0x00, 0x03, 0x33, 0x03, 0x33, 0x03, 0x00},   // U+E54C (SGA L)
    { 0x00, 0x00, 0x63, 0x60, 0x60, 0x60, 0x7F, 0x00},   // U+E54D (SGA M)
    { 0x00, 0x00, 0x66, 0x60, 0x30, 0x18, 0x0C, 0x00},   // U+E54E (SGA N)
    { 0x00, 0x00, 0x3C, 0x60, 0x30, 0x18, 0x0C, 0x00},   // U+E54F (SGA O)
    { 0x00, 0x00, 0x66, 0x60, 0x66, 0x06, 0x66, 0x00},   // U+E550 (SGA P)
    { 0x00, 0x00, 0x18, 0x00, 0x7E, 0x60, 0x7E, 0x00},   // U+E551 (SGA Q)
    { 0x00, 0x00, 0x00, 0x66, 0x00, 0x66, 0x00, 0x00},   // U+E552 (SGA R)
    { 0x00, 0x00, 0x0C, 0x0C, 0x3C, 0x30, 0x30, 0x00},   // U+E553 (SGA S)
    { 0x00, 0x00, 0x3C, 0x30, 0x30, 0x00, 0x30, 0x00},   // U+E554 (SGA T)
    { 0x00, 0x00, 0x00, 0x36, 0x00, 0x7F, 0x00, 0x00},   // U+E555 (SGA U)
    { 0x00, 0x00, 0x18, 0x18, 0x7E, 0x00, 0x7E, 0x00},   // U+E556 (SGA V)
    { 0x00, 0x00, 0x00, 0x18, 0x00, 0x66, 0x00, 0x00},   // U+E557 (SGA W)
    { 0x00, 0x00, 0x66, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+E558 (SGA X)
    { 0x00, 0x00, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00},   // U+E559 (SGA Y)
    { 0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x66, 0x00}    // U+E55A (SGA Z)
};

#endif
