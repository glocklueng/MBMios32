#include <mios32.h>

const u8 GLCD_FONT_TINY[] = {
								4, 1*8, 0, 4, // width, height, X0 offset, char offset
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,	// SPACE
								0x00,0x00,0x5C,0x00,	// !
								0x00,0x0C,0x00,0x0C,	// "
								0x00,0x7C,0x28,0x7C,	// #
								0x00,0x7C,0x44,0x7C,	// $
								0x00,0x24,0x10,0x48,	// %
								0x00,0x28,0x54,0x08,	// &
								0x00,0x00,0x0C,0x00,	// '
								0x00,0x38,0x44,0x00,	// (
								0x00,0x44,0x38,0x00,	// )  
								0x00,0x20,0x10,0x08,	// /
								0x00,0x10,0x38,0x10,	// +
								0x00,0x80,0x40,0x00,	// ,
								0x00,0x10,0x10,0x10,	// -
								0x00,0x00,0x40,0x00,	// .
								0x00,0x20,0x10,0x08,	// /
								0x00,0x38,0x44,0x38,	// 0
								0x00,0x00,0x7C,0x00,	// 1
								0x00,0x64,0x54,0x48,	// 2
								0x00,0x44,0x54,0x28,	// 3
								0x00,0x1C,0x10,0x7C,	// 4
								0x00,0x4C,0x54,0x24,	// 5
								0x00,0x38,0x54,0x20,	// 6
								0x00,0x04,0x74,0x0C,	// 7
								0x00,0x28,0x54,0x28,	// 8
								0x00,0x08,0x54,0x38,	// 9
								0x00,0x00,0x50,0x00,	// :
								0x00,0x80,0x50,0x00,	// ;
								0x00,0x10,0x28,0x44,	// <
								0x00,0x28,0x28,0x28,	// =
								0x00,0x44,0x28,0x10,	// >
								0x00,0x04,0x54,0x08,	// ?
								0x00,0x38,0x4C,0x5C,	// @
								0x00,0x78,0x14,0x78,	// A
								0x00,0x7C,0x54,0x28,	// B
								0x00,0x38,0x44,0x44,	// C
								0x00,0x7C,0x44,0x38,	// D
								0x00,0x7C,0x54,0x44,	// E
								0x00,0x7C,0x14,0x04,	// F
								0x00,0x38,0x44,0x34,	// G
								0x00,0x7C,0x10,0x7C,	// H
								0x00,0x00,0x7C,0x00,	// I
								0x00,0x20,0x40,0x3C,	// J
								0x00,0x7C,0x10,0x6C,	// K
								0x00,0x7C,0x40,0x40,	// L
								0x00,0x7C,0x08,0x7C,	// M
								0x00,0x7C,0x04,0x7C,	// N
								0x00,0x7C,0x44,0x7C,	// O
								0x00,0x7C,0x14,0x08,	// P
								0x00,0x38,0x44,0x78,	// Q
								0x00,0x7C,0x14,0x68,	// R
								0x00,0x48,0x54,0x24,	// S
								0x00,0x04,0x7C,0x04,	// T
								0x00,0x7C,0x40,0x7C,	// U
								0x00,0x3C,0x40,0x3C,	// V
								0x00,0x7C,0x20,0x7C,	// W
								0x00,0x6C,0x10,0x6C,	// X
								0x00,0x1C,0x60,0x1C,	// Y
								0x00,0x64,0x54,0x4C,	// Z
								0x00,0x7C,0x44,0x00,	// [
								0x00,0x08,0x10,0x20,	// Backslash
								0x00,0x44,0x7C,0x00,	// ]
								0x00,0x08,0x04,0x08,	// ^
								0x00,0x80,0x80,0x80,	// _
								0x00,0x04,0x08,0x00, 	// `
								0x00,0x78,0x14,0x78,	// A
								0x00,0x7C,0x54,0x28,	// B
								0x00,0x38,0x44,0x44,	// C
								0x00,0x7C,0x44,0x38,	// D
								0x00,0x7C,0x54,0x44,	// E
								0x00,0x7C,0x14,0x04,	// F
								0x00,0x38,0x44,0x34,	// G
								0x00,0x7C,0x10,0x7C,	// H
								0x00,0x00,0x7C,0x00,	// I
								0x00,0x20,0x40,0x3C,	// J
								0x00,0x7C,0x10,0x6C,	// K
								0x00,0x7C,0x40,0x40,	// L
								0x00,0x7C,0x08,0x7C,	// M
								0x00,0x7C,0x04,0x7C,	// N
								0x00,0x7C,0x44,0x7C,	// O
								0x00,0x7C,0x14,0x08,	// P
								0x00,0x38,0x44,0x78,	// Q
								0x00,0x7C,0x14,0x68,	// R
								0x00,0x48,0x54,0x24,	// S
								0x00,0x04,0x7C,0x04,	// T
								0x00,0x7C,0x40,0x7C,	// U
								0x00,0x3C,0x40,0x3C,	// V
								0x00,0x7C,0x20,0x7C,	// W
								0x00,0x6C,0x10,0x6C,	// X
								0x00,0x1C,0x60,0x1C,	// Y
								0x00,0x64,0x54,0x4C,	// Z
								0x00,0x7C,0x44,0x00,	// {
								0x00,0x00,0x7C,0x00,	// |
								0x00,0x44,0x7C,0x00,	// }
								0x00,0x10,0x10,0x10,	// ~
								0x00,0x00,0x00,0x00		// DEL
							};
