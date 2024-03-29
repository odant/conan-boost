#include "test_values.hpp"

constexpr const char *test_subtraction_native_result[
	boost::mp11::mp_size<test_values>::value
] = {
//      0       0       0       0
//      012345670123456701234567012345670
//      012345678901234567890123456789012
/* 0*/ "..........x...x..........xxx.xxx.",
/* 1*/ "..........x...x..........xxx.xxx.",
/* 2*/ ".........x...x..........xxxxxxxx.",
/* 3*/ "........................xxxxxxxx.",
/* 4*/ "..........x...x..........xxx.xxx.",
/* 5*/ "..........x...x..........xxx.xxx.",
/* 6*/ ".........x...x..........xxxxxxxx.",
/* 7*/ "........................xxxxxxxx.",

/* 8*/ "..........x...x..........xxx.xxx.",
/* 9*/ "..xx..xx..xx..x...........xx.xxx.",
/*10*/ "xx..xx..xx...x..xxxxxxxxxxxxxxxx.",
/*11*/ "........................xxxxxxxx.",
/*12*/ "..............x..............xxx.",
/*13*/ "..xx..xx..xx..xx..............xx.",
/*14*/ "xx..xx..xx..xx..xxxxxxxxxxxxxxxx.",
/*15*/ "............................xxxx.",

//      0       0       0       0
//      012345670123456701234567012345670
//      012345678901234567890123456789012
/*16*/ "..........x...x..........xxx.xxx.",
/*17*/ "..........x...x..........xxx.xxx.",
/*18*/ "..........x...x..........xxx.xxx.",
/*19*/ "..........x...x..........xxx.xxx.",
/*20*/ "..........x...x..........xxx.xxx.",
/*21*/ "..........x...x..........xxx.xxx.",
/*22*/ "..........x...x..........xxx.xxx.",
/*23*/ "..........x...x..........xxx.xxx.",

/*24*/ ".xxx.xxx.xxx..x..xxx.xxx.xxx.xxx.",
/*25*/ "..xx..xx..xx..x...........xx.xxx.",
/*26*/ "..xx..xx..xx..x............x.xxx.",
/*27*/ "..xx..xx..xx..x..............xxx.",
/*28*/ ".xxx.xxx.xxx.xxx.xxx.xxx.xxx.xxx.",
/*29*/ "..xx..xx..xx..xx..............xx.",
/*30*/ "..xx..xx..xx..xx...............x.",
/*31*/ "..xx..xx..xx..xx.................",
/*32*/ "..........x...x.........xxxxxxxx."
};
