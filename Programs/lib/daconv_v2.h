#ifdef DACONV_V2_H
#define DACONV_V2_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

int DA_Write(int pin, double V_OUT, int DA_SS);	//指定したpinの電圧をV_OUTに変更

#endif


/*メモ

Xレジスタを変更するプログラムしか作っていない．
時間があれば（必要があれば），C,Mレジスタを変更するプログラムを作成する．

	+----+----+-----------------------------------------------------------------+
	| M1 | M0 | Action                                                          |
	+----+----+-----------------------------------------------------------------+
	| 1  | 1  | Write DAC data (X) register                                     |
	| 1  | 0  | Write DAC offset (C) register                                   |
	| 0  | 1  | Write DAC gain (M) register                                     |
	| 0  | 0  | Special function, used in combination with other bits of a word |
	+----+----+-----------------------------------------------------------------+

	+---------------------------------+-----------------------------------------------------------------------------+
	|                                 | Address Bit A4 to Address Bit A3                                            |
	| Address Bit A2 to Adress Bit A0 +--------------------------+--------------------+--------------------+--------+
	|                                 | 00                       | 01                 | 10                 | 11     |
	+---------------------------------+--------------------------+--------------------+--------------------+--------+
	| 000                             | All groups, all channels | Group 0, Channel 0 | Group 1, Channel 0 | unused |
	| 001                             | group 0, all channels    | Group 0, Channel 1 | Group 1, Channel 1 | unused |
	| 010                             | group 1, all channels    | Group 0, Channel 2 | Group 1, Channel 2 | unused |
	| 011                             | unused                   | Group 0, Channel 3 | Group 1, Channel 3 | unused |
	| 100                             | unused                   | Group 0, Channel 4 | Group 1, Channel 4 | unused |
	| 101                             | unused                   | Group 0, Channel 5 | Group 1, Channel 5 | unused |
	| 110                             | unused                   | Group 0, Channel 6 | Group 1, Channel 6 | unused |
	| 111                             | unused                   | Group 0, Channel 7 | Group 1, Channel 7 | unused |
	+---------------------------------+--------------------------+--------------------+--------------------+--------+

	伝達関数
	---------------------------------------------------------------------
	DACコード
	DAC_CODE = INPUT_CODE * ( M + 1 ) / 2 ^ 16 + C - (2^15)
	----------------------------
	DAC出力電圧
	V_OUT = 4 * V_REF * ( DAC_CODE - ( OFFSET_CODE * 4 )) / ( 2^16 ) + V_SIGGND
	-----------------------------------------------------------------------------

	逆に言えば・・・
	----------------------------------------------
	入力コード
	INPUT_CODE = ( DAC_CODE - C + ( 2^15 )) * ( 2^16 ) / ( M + 1 )
	-------------------------------------------------------------------------------
	DACコード
	DAC_CODE = 4 * OFFSET_CODE + ( V_OUT - V_SIGGND ) * (2^16) / ( 4* V_REF )
	------------------------------------------------------------------------------
*/