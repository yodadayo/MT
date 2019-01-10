#### 1歩目出た改善版

Muscle, Phase| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 
------------- | --- | --- | --- | --- | --- | --- | --- | --- 
\#0 | S | E | E | S | S | S | S | S 
\#1 | S | S | E | E | S | S | S | S 
\#2 | E | E | S | E | E | E | E | E 
\#3 | S | S | S | S | S | E | E | S 
\#4 | S | S | S | S | S | S | E | S 
\#5 | E | E | E | E | E | S | S | E 
\#6 | S | S | E | E | S | S | S | S 
\#7 | S | S | S | S | S | S | E | E 

Moter ID, Phase| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 
------------- | --- | --- | --- | --- | --- | --- | --- | --- 
\#1 | 10 | 0 | 30 | 30 | 0 | -10 | -20 | -10 
\#2 | 10 | 20 | 30 | 0 | 0 | 0 | 30 | 30 
\#3 | 0 | -10 | -10 | 20 | 0 | 0 | -40 | -15 
\#4 | 0 | 10 | -45 | -35 | 0 | -10 | -10 | 10 

#### 前回との比較
	- 前回より遊脚期伸びた？(遊脚が伸び切る直前くらいまで浮いてる)

#### 改善点
- 遊脚期もう少し長く
- 片脚支持から両脚支持になるとき遊脚側がつく地点が内側過ぎ

uint16_t vstep[8][W_PHASE] = { {S, E, E, S, S, S, S, S},
			      // right front shin (0)
			       {S, S, E, E, S, S, S, S},
			      // right back shin (1)
			       {E, E, S, E, E, E, E, E},
			      // right back femur (2)
			       {S, S, S, S, S, E, E, S},
			      // left front shin (3)
			       {S, S, S, S, S, S, E, S},
			      // left back shin (4)
			       {E, E, E, E, E, S, S, E},
			      // left back femur (5)
			       {S, S, E, E, S, S, S, S},
			      // right front femur(6)
			       {S, S, S, S, S, S, E, E}
			      // left front femur (9)
}; // if W_PHASE is 8

int16_t mstep[ID_NUM][W_PHASE] = // {deg, rpm}
  { { 10, 0, 30, 30, 0, -10, -20, -10}, // RIGHT_PITCH
    { 10, 20, 30, 0, 0, 0, 30, 30}, // LEFT_PITCH
    { 0, -10, -10, 20, 0, 0, -40, -15}, // RIGHT_ROLL
    { 0, 10, -45, -35, 0, -10, -10, 10} // LEFT_ROLL
  }; // if W_PHASE is 8
