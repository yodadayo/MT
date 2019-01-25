#### 2歩目出ている

<!-- Muscle, Phase| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7  -->
<!-- ------------- | --- | --- | --- | --- | --- | --- | --- | ---  -->
<!-- \#0 | S | E | E | S | S | S | S | S  -->
<!-- \#1 | S | S | E | E | S | S | S | S  -->
<!-- \#2 | E | E | S | E | E | E | E | E  -->
<!-- \#3 | S | S | S | S | S | E | E | S  -->
<!-- \#4 | S | S | S | S | S | S | E | S  -->
<!-- \#5 | E | E | E | E | E | S | S | E  -->
<!-- \#6 | S | S | E | E | S | S | S | S  -->
<!-- \#7 | S | S | S | S | S | S | E | E  -->

<!-- Moter ID, Phase| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7  -->
<!-- ------------- | --- | --- | --- | --- | --- | --- | --- | ---  -->
<!-- \#1 | 15 | 0 | 30 | 30 | 0 | -10 | -20 | -10  -->
<!-- \#2 | 15 | 20 | 30 | 0 | 0 | 0 | 30 | 30  -->
<!-- \#3 | 0 | -10 | -10 | 0 | 0 | 0 | -40 | -15  -->
<!-- \#4 | 0 | 10 | -45 | -35 | 0 | -10 | -10 | 10  -->

#### 前回との比較
	- 

#### 改善点
- 1步目状態からのパラメータ


uint16_t stance_v[V_NUM] = {S, S, E, S, S, E, S, S};
int16_t stance_m[ID_NUM] = {0, 0, 0, 0};

const int16_t firststep_m[ID_NUM][FIRST_PHASE] = // {deg, rpm}
  { { 15, 10, 40, 30}, // RIGHT_PITCH
    { 10, 10, 10, 0}, // LEFT_PITCH
    { 0, 0, 5, 5}, // RIGHT_ROLL
    { 0, 0, -20, -20} // LEFT_ROLL
  };
const uint16_t firststep_v[8][FIRST_PHASE] = { {S, E, S, S},
			      // right front shin (0)
			       {E, S, E, E},
			      // right back shin (1)
			       {E, S, S, E},
			      // right back femur (2)
			       {S, S, S, S},
			      // left front shin (3)
			       {E, E, S, E},
			      // left back shin (4)
			       {E, E, E, E},
			      // left back femur (5)
			       {S, S, E, E},
			      // right front femur(6)
			       {S, S, S, S}
			      // left front femur (9)
}; // if W_PHASE is 8

int16_t mstep[ID_NUM][W_PHASE] = // {deg, rpm}
  { { 5, -10, -15, -20, 5, 30, 40, 30}, // RIGHT_PITCH
    { 5, 30, 40, 30, 5, -10, -15, -20}, // LEFT_PITCH
    { -5, -15, -25, -20, -5, 0, -5, -10}, // RIGHT_ROLL
    { -5, 0, -5, -10, -5, -15, -25, -15} // LEFT_ROLL
  }; // if W_PHASE is 8
  uint16_t vstep[8][W_PHASE] = { {S, S, S, S, S, E, S, E},
			      // right front shin (0)
			       {E, E, S, E, E, S, E, E},
			      // right back shin (1)
			       {E, E, E, E, E, E, S, E},
			      // right back femur (2)
			       {E, S, S, E, S, S, S, S},
			      // left front shin (3)
			       {S, E, E, S, S, E, S, E},
			      // left back shin (4)
			       {S, S, S, E, E, E, E, E},
			      // left back femur (5)
			       {S, S, S, S, S, S, E, E},
			      // right front femur(6)
			       {E, E, E, S, S, S, S, S}
			      // left front femur (9)
}; // if W_PHASE is 8
