
#include "GLUU/Shapes.h"
#include "GLUU/Basic.h"


// 8 Stacks.
// 32 Slices. 

static const int num_vertices = 257;
static const int num_normals = 256;
static const int num_uvs = 0;
static const int num_faces = 256;

static double vertices[257*3] = {
-0.19509, 0.980785, 0,   -0.382683, 0.92388, 0,   -0.55557, 0.83147, 0,   
-0.707107, 0.707107, 0,   -0.83147, 0.55557, 0,   -0.92388, 0.382683, 0,   
-0.980785, 0.19509, 0,   -1, 0, 0,   -0.191342, 0.980785, -0.03806,   
-0.37533, 0.92388, -0.074658,   -0.544895, 0.83147, -0.108386,   -0.69352, 0.707107, -0.13795,   
-0.815493, 0.55557, -0.162212,   -0.906127, 0.382683, -0.18024,   -0.96194, 0.19509, -0.191342,   
-0.980785, 0, -0.19509,   -0.18024, 0.980785, -0.074658,   -0.353553, 0.92388, -0.146447,   
-0.51328, 0.83147, -0.212608,   -0.653281, 0.707107, -0.270598,   -0.768178, 0.55557, -0.31819,   
-0.853553, 0.382683, -0.353553,   -0.906127, 0.19509, -0.37533,   -0.92388, 0, -0.382684,   
-0.162212, 0.980785, -0.108387,   -0.31819, 0.92388, -0.212608,   -0.46194, 0.83147, -0.308658,   
-0.587938, 0.707107, -0.392848,   -0.691342, 0.55557, -0.46194,   -0.768178, 0.382683, -0.51328,   
-0.815493, 0.19509, -0.544895,   -0.83147, 0, -0.55557,   -0.13795, 0.980785, -0.13795,   
-0.270598, 0.92388, -0.270598,   -0.392847, 0.83147, -0.392848,   -0.5, 0.707107, -0.5,   
-0.587938, 0.55557, -0.587938,   -0.653281, 0.382683, -0.653282,   -0.69352, 0.19509, -0.69352,   
-0.707107, 0, -0.707107,   -0.108386, 0.980785, -0.162212,   -0.212607, 0.92388, -0.31819,   
-0.308658, 0.83147, -0.46194,   -0.392847, 0.707107, -0.587938,   -0.461939, 0.55557, -0.691342,   
-0.51328, 0.382683, -0.768178,   -0.544895, 0.19509, -0.815493,   -0.55557, 0, -0.83147,   
-0.074658, 0.980785, -0.18024,   -0.146446, 0.92388, -0.353554,   -0.212607, 0.83147, -0.51328,   
-0.270598, 0.707107, -0.653282,   -0.318189, 0.55557, -0.768178,   -0.353553, 0.382683, -0.853554,   
-0.37533, 0.19509, -0.906128,   -0.382683, 0, -0.92388,   -0.03806, 0.980785, -0.191342,   
-0.074658, 0.92388, -0.375331,   -0.108386, 0.83147, -0.544895,   -0.137949, 0.707107, -0.69352,   
-0.162211, 0.55557, -0.815493,   -0.18024, 0.382683, -0.906128,   -0.191341, 0.19509, -0.96194,   
-0.19509, 0, -0.980785,   0, 0.980785, -0.195091,   0, 0.92388, -0.382684,   
0, 0.83147, -0.55557,   0, 0.707107, -0.707107,   0, 0.55557, -0.83147,   
0, 0.382683, -0.92388,   0, 0.19509, -0.980785,   0, 0, -1,   
0.038061, 0.980785, -0.191342,   0.074658, 0.92388, -0.37533,   0.108387, 0.83147, -0.544895,   
0.13795, 0.707107, -0.69352,   0.162212, 0.55557, -0.815493,   0.18024, 0.382683, -0.906128,   
0.191342, 0.19509, -0.96194,   0.195091, 0, -0.980785,   0.074658, 0.980785, -0.18024,   
0.146447, 0.92388, -0.353554,   0.212608, 0.83147, -0.51328,   0.270599, 0.707107, -0.653282,   
0.31819, 0.55557, -0.768178,   0.353554, 0.382683, -0.853553,   0.375331, 0.19509, -0.906127,   
0.382684, 0, -0.92388,   0.108387, 0.980785, -0.162212,   0.212608, 0.92388, -0.31819,   
0.308659, 0.83147, -0.46194,   0.392848, 0.707107, -0.587938,   0.46194, 0.55557, -0.691342,   
0.51328, 0.382683, -0.768178,   0.544895, 0.19509, -0.815493,   0.555571, 0, -0.83147,   
0.13795, 0.980785, -0.13795,   0.270599, 0.92388, -0.270598,   0.392848, 0.83147, -0.392848,   
0.5, 0.707107, -0.5,   0.587938, 0.55557, -0.587938,   0.653282, 0.382683, -0.653281,   
0.69352, 0.19509, -0.69352,   0.707107, 0, -0.707107,   0.162212, 0.980785, -0.108386,   
0.31819, 0.92388, -0.212608,   0.46194, 0.83147, -0.308658,   0.587938, 0.707107, -0.392847,   
0.691342, 0.55557, -0.46194,   0.768178, 0.382683, -0.51328,   0.815493, 0.19509, -0.544895,   
0.83147, 0, -0.55557,   0.18024, 0.980785, -0.074658,   0.353554, 0.92388, -0.146447,   
0.51328, 0.83147, -0.212608,   0.653282, 0.707107, -0.270598,   0.768178, 0.55557, -0.31819,   
0.853554, 0.382683, -0.353553,   0.906128, 0.19509, -0.37533,   0.92388, 0, -0.382683,   
0.191342, 0.980785, -0.03806,   0.375331, 0.92388, -0.074658,   0.544896, 0.83147, -0.108386,   
0.69352, 0.707107, -0.13795,   0.815493, 0.55557, -0.162212,   0.906128, 0.382683, -0.18024,   
0.96194, 0.19509, -0.191342,   0.980785, 0, -0.19509,   0.195091, 0.980785, 0,   
0.382684, 0.92388, 0,   0.555571, 0.83147, 0,   0.707107, 0.707107, 0,   
0.83147, 0.55557, 0,   0.92388, 0.382683, 0,   0.980785, 0.19509, 0,   
1, 0, 0,   0.191342, 0.980785, 0.03806,   0.375331, 0.92388, 0.074658,   
0.544896, 0.83147, 0.108386,   0.69352, 0.707107, 0.13795,   0.815493, 0.55557, 0.162212,   
0.906128, 0.382683, 0.18024,   0.96194, 0.19509, 0.191342,   0.980785, 0, 0.19509,   
0.18024, 0.980785, 0.074658,   0.353554, 0.92388, 0.146447,   0.51328, 0.83147, 0.212608,   
0.653282, 0.707107, 0.270598,   0.768178, 0.55557, 0.31819,   0.853554, 0.382683, 0.353554,   
0.906128, 0.19509, 0.37533,   0.92388, 0, 0.382683,   0.162212, 0.980785, 0.108387,   
0.31819, 0.92388, 0.212608,   0.46194, 0.83147, 0.308658,   0.587938, 0.707107, 0.392848,   
0.691342, 0.55557, 0.46194,   0.768178, 0.382683, 0.51328,   0.815493, 0.19509, 0.544895,   
0.83147, 0, 0.55557,   0.13795, 0.980785, 0.13795,   0.270598, 0.92388, 0.270598,   
0.392848, 0.83147, 0.392848,   0.5, 0.707107, 0.5,   0.587938, 0.55557, 0.587938,   
0.653282, 0.382683, 0.653282,   0.69352, 0.19509, 0.69352,   0.707107, 0, 0.707107,   
0.108387, 0.980785, 0.162212,   0.212608, 0.92388, 0.31819,   0.308659, 0.83147, 0.46194,   
0.392848, 0.707107, 0.587938,   0.46194, 0.55557, 0.691342,   0.51328, 0.382683, 0.768178,   
0.544895, 0.19509, 0.815493,   0.55557, 0, 0.831469,   0.074658, 0.980785, 0.18024,   
0.146447, 0.92388, 0.353554,   0.212608, 0.83147, 0.51328,   0.270598, 0.707107, 0.653281,   
0.31819, 0.55557, 0.768178,   0.353554, 0.382683, 0.853553,   0.37533, 0.19509, 0.906127,   
0.382683, 0, 0.923879,   0.038061, 0.980785, 0.191342,   0.074658, 0.92388, 0.37533,   
0.108387, 0.83147, 0.544895,   0.13795, 0.707107, 0.69352,   0.162212, 0.55557, 0.815493,   
0.18024, 0.382683, 0.906127,   0.191342, 0.19509, 0.96194,   0.19509, 0, 0.980785,   
0, 0.980785, 0.19509,   0, 0.92388, 0.382684,   0, 0.83147, 0.55557,   
0, 0.707107, 0.707107,   0, 0.55557, 0.831469,   0, 0.382683, 0.923879,   
0, 0.19509, 0.980785,   0, 0, 1,   -0.03806, 0.980785, 0.191342,   
-0.074658, 0.92388, 0.37533,   -0.108386, 0.83147, 0.544895,   -0.137949, 0.707107, 0.69352,   
-0.162211, 0.55557, 0.815493,   -0.18024, 0.382683, 0.906127,   -0.191342, 0.19509, 0.961939,   
-0.19509, 0, 0.980785,   -0.074658, 0.980785, 0.18024,   -0.146446, 0.92388, 0.353553,   
-0.212607, 0.83147, 0.51328,   -0.270598, 0.707107, 0.653281,   -0.318189, 0.55557, 0.768177,   
-0.353553, 0.382683, 0.853553,   -0.37533, 0.19509, 0.906127,   -0.382683, 0, 0.923879,   
-0.108386, 0.980785, 0.162212,   -0.212607, 0.92388, 0.31819,   -0.308658, 0.83147, 0.46194,   
-0.392847, 0.707107, 0.587938,   -0.461939, 0.55557, 0.691341,   -0.51328, 0.382683, 0.768178,   
-0.544895, 0.19509, 0.815493,   -0.55557, 0, 0.831469,   -0.137949, 0.980785, 0.13795,   
-0.270598, 0.92388, 0.270598,   -0.392847, 0.83147, 0.392847,   -0.5, 0.707107, 0.5,   
-0.587937, 0.55557, 0.587937,   -0.653281, 0.382683, 0.653281,   -0.693519, 0.19509, 0.693519,   
-0.707106, 0, 0.707106,   0, 1, 0,   -0.162211, 0.980785, 0.108386,   
-0.318189, 0.92388, 0.212608,   -0.461939, 0.83147, 0.308658,   -0.587937, 0.707107, 0.392847,   
-0.691341, 0.55557, 0.461939,   -0.768177, 0.382683, 0.51328,   -0.815493, 0.19509, 0.544895,   
-0.831469, 0, 0.55557,   -0.18024, 0.980785, 0.074658,   -0.353553, 0.92388, 0.146447,   
-0.51328, 0.83147, 0.212607,   -0.653281, 0.707107, 0.270598,   -0.768177, 0.55557, 0.318189,   
-0.853553, 0.382683, 0.353553,   -0.906127, 0.19509, 0.37533,   -0.923879, 0, 0.382683,   
-0.191341, 0.980785, 0.03806,   -0.37533, 0.92388, 0.074658,   -0.544895, 0.83147, 0.108386,   
-0.69352, 0.707107, 0.13795,   -0.815492, 0.55557, 0.162211,   -0.906127, 0.382683, 0.18024,   
-0.961939, 0.19509, 0.191341,   -0.980784, 0, 0.19509};   

static double normals[256*3] = {
-0.47089, 0.880972, -0.046379,   -0.633158, 0.771506, -0.062361,   -0.77078, 0.632563, -0.075915,   
-0.878613, 0.469628, -0.086536,   -0.952718, 0.289004, -0.093835,   -0.990438, 0.09755, -0.09755,   
-0.290166, 0.956549, -0.028579,   -0.741159, 0.632562, -0.224828,   -0.844848, 0.469628, -0.256282,   
-0.916106, 0.289004, -0.277898,   -0.952376, 0.09755, -0.2889,   -0.279015, 0.956549, -0.084638,   
-0.452794, 0.880972, -0.137354,   -0.608826, 0.771506, -0.184686,   -0.417297, 0.880972, -0.22305,   
-0.561098, 0.771506, -0.299913,   -0.683056, 0.632562, -0.365101,   -0.778617, 0.469628, -0.416179,   
-0.844288, 0.289004, -0.451281,   -0.877715, 0.097551, -0.469149,   -0.257142, 0.956549, -0.137445,   
-0.740024, 0.289003, -0.607323,   -0.769323, 0.097551, -0.631368,   -0.225387, 0.956549, -0.18497,   
-0.365764, 0.880972, -0.300175,   -0.491806, 0.771506, -0.403615,   -0.598704, 0.632562, -0.491344,   
-0.682463, 0.469628, -0.560083,   -0.491344, 0.632562, -0.598704,   -0.560083, 0.469629, -0.682463,   
-0.607322, 0.289003, -0.740025,   -0.631367, 0.097551, -0.769324,   -0.18497, 0.95655, -0.225387,   
-0.300175, 0.880972, -0.365764,   -0.403615, 0.771506, -0.491806,   -0.22305, 0.880972, -0.417297,   
-0.299913, 0.771506, -0.561098,   -0.365101, 0.632562, -0.683057,   -0.416179, 0.469629, -0.778617,   
-0.451281, 0.289003, -0.844288,   -0.469148, 0.097551, -0.877715,   -0.137445, 0.956549, -0.257142,   
-0.277898, 0.289003, -0.916106,   -0.2889, 0.097551, -0.952376,   -0.084638, 0.956549, -0.279015,   
-0.137353, 0.880972, -0.452794,   -0.184685, 0.771506, -0.608826,   -0.224828, 0.632562, -0.74116,   
-0.256282, 0.469629, -0.844848,   -0.075915, 0.632562, -0.770781,   -0.086535, 0.469629, -0.878613,   
-0.093834, 0.289003, -0.952718,   -0.097549, 0.097551, -0.990438,   -0.028579, 0.956549, -0.290166,   
-0.046378, 0.880972, -0.47089,   -0.062361, 0.771506, -0.633158,   0.028579, 0.956549, -0.290166,   
0.046379, 0.880972, -0.47089,   0.062361, 0.771506, -0.633158,   0.075915, 0.632562, -0.770781,   
0.086536, 0.469629, -0.878613,   0.093835, 0.289003, -0.952718,   0.09755, 0.09755, -0.990438,   
0.256282, 0.469629, -0.844848,   0.277898, 0.289003, -0.916106,   0.2889, 0.09755, -0.952376,   
0.084639, 0.956549, -0.279015,   0.137354, 0.880972, -0.452793,   0.184686, 0.771506, -0.608826,   
0.224829, 0.632562, -0.74116,   0.299913, 0.771506, -0.561098,   0.365102, 0.632562, -0.683057,   
0.416179, 0.469629, -0.778616,   0.451282, 0.289003, -0.844288,   0.469149, 0.09755, -0.877715,   
0.137445, 0.95655, -0.257142,   0.22305, 0.880972, -0.417297,   0.631368, 0.09755, -0.769324,   
0.18497, 0.956549, -0.225387,   0.300175, 0.880972, -0.365764,   0.403615, 0.771506, -0.491806,   
0.491344, 0.632562, -0.598704,   0.560083, 0.469629, -0.682463,   0.607323, 0.289003, -0.740025,   
0.682463, 0.469629, -0.560083,   0.740025, 0.289003, -0.607322,   0.769324, 0.09755, -0.631367,   
0.225387, 0.956549, -0.18497,   0.365764, 0.880972, -0.300174,   0.491807, 0.771506, -0.403615,   
0.598704, 0.632562, -0.491344,   0.561098, 0.771506, -0.299913,   0.683057, 0.632562, -0.365101,   
0.778616, 0.469629, -0.416179,   0.844288, 0.289003, -0.451281,   0.877715, 0.09755, -0.469148,   
0.257142, 0.956549, -0.137445,   0.417297, 0.880972, -0.22305,   0.952376, 0.09755, -0.2889,   
0.279015, 0.956549, -0.084638,   0.452794, 0.880972, -0.137353,   0.608827, 0.771506, -0.184686,   
0.74116, 0.632562, -0.224828,   0.844848, 0.469629, -0.256282,   0.916106, 0.289003, -0.277898,   
0.878612, 0.469629, -0.086536,   0.952718, 0.289003, -0.093834,   0.990438, 0.09755, -0.097549,   
0.290166, 0.956549, -0.028579,   0.47089, 0.880972, -0.046378,   0.633159, 0.771506, -0.06236,   
0.77078, 0.632562, -0.075915,   0.633159, 0.771506, 0.062361,   0.77078, 0.632562, 0.075915,   
0.878612, 0.469629, 0.086536,   0.952718, 0.289003, 0.093835,   0.990438, 0.09755, 0.09755,   
0.290166, 0.956549, 0.028579,   0.47089, 0.880972, 0.046379,   0.952376, 0.09755, 0.2889,   
0.279015, 0.956549, 0.084638,   0.452794, 0.880972, 0.137354,   0.608827, 0.771506, 0.184686,   
0.74116, 0.632562, 0.224829,   0.844848, 0.469629, 0.256282,   0.916106, 0.289003, 0.277898,   
0.683057, 0.632562, 0.365101,   0.778616, 0.469629, 0.416179,   0.844288, 0.289003, 0.451282,   
0.877715, 0.097549, 0.469149,   0.257142, 0.956549, 0.137445,   0.417297, 0.880972, 0.22305,   
0.561098, 0.771506, 0.299913,   0.365764, 0.880972, 0.300175,   0.491806, 0.771506, 0.403615,   
0.598704, 0.632562, 0.491344,   0.682463, 0.469629, 0.560083,   0.740025, 0.289003, 0.607323,   
0.769324, 0.097549, 0.631368,   0.225387, 0.956549, 0.18497,   0.607322, 0.289003, 0.740025,   
0.631368, 0.097549, 0.769324,   0.18497, 0.956549, 0.225387,   0.300175, 0.880972, 0.365764,   
0.403615, 0.771506, 0.491807,   0.491344, 0.632562, 0.598704,   0.560083, 0.469629, 0.682463,   
0.365101, 0.632562, 0.683057,   0.416179, 0.469629, 0.778616,   0.451281, 0.289003, 0.844288,   
0.469148, 0.097549, 0.877715,   0.137445, 0.956549, 0.257142,   0.22305, 0.880972, 0.417297,   
0.299913, 0.771506, 0.561098,   0.137353, 0.880972, 0.452794,   0.184685, 0.771506, 0.608827,   
0.224828, 0.632562, 0.74116,   0.256282, 0.469629, 0.844848,   0.277897, 0.289003, 0.916106,   
0.2889, 0.097549, 0.952376,   0.084638, 0.956549, 0.279015,   0.093834, 0.289003, 0.952718,   
0.097549, 0.097549, 0.990438,   0.028579, 0.956549, 0.290166,   0.046378, 0.880972, 0.47089,   
0.06236, 0.771506, 0.633158,   0.075915, 0.632562, 0.770781,   0.086536, 0.469629, 0.878612,   
-0.075915, 0.632562, 0.770781,   -0.086536, 0.469629, 0.878612,   -0.093835, 0.289003, 0.952718,   
-0.09755, 0.097549, 0.990438,   -0.028579, 0.956549, 0.290166,   -0.046379, 0.880972, 0.47089,   
-0.062361, 0.771506, 0.633158,   -0.137354, 0.880972, 0.452794,   -0.184686, 0.771506, 0.608826,   
-0.224829, 0.632562, 0.74116,   -0.256282, 0.469629, 0.844848,   -0.277898, 0.289003, 0.916106,   
-0.288901, 0.097549, 0.952376,   -0.084638, 0.956549, 0.279015,   -0.451282, 0.289003, 0.844288,   
-0.469149, 0.097549, 0.877715,   -0.137445, 0.956549, 0.257142,   -0.22305, 0.880972, 0.417297,   
-0.299913, 0.771506, 0.561098,   -0.365102, 0.632562, 0.683057,   -0.416179, 0.469629, 0.778616,   
-0.403615, 0.771506, 0.491806,   -0.491344, 0.632561, 0.598704,   -0.560083, 0.469629, 0.682463,   
-0.607323, 0.289003, 0.740025,   -0.631368, 0.097549, 0.769324,   -0.18497, 0.956549, 0.225387,   
-0.300175, 0.880972, 0.365764,   -0.769324, 0.097549, 0.631367,   -0.225387, 0.956549, 0.18497,   
-0.365764, 0.880972, 0.300175,   -0.491806, 0.771506, 0.403615,   -0.598705, 0.632561, 0.491344,   
-0.682463, 0.469629, 0.560083,   -0.740025, 0.289003, 0.607322,   -0.778616, 0.469629, 0.416179,   
-0.844288, 0.289002, 0.451281,   -0.877715, 0.097549, 0.469148,   -0.257142, 0.956549, 0.137445,   
-0.417297, 0.880972, 0.22305,   -0.561098, 0.771506, 0.299913,   -0.683057, 0.632561, 0.365101,   
-0.608827, 0.771506, 0.184685,   -0.74116, 0.632561, 0.224828,   -0.844848, 0.46963, 0.256282,   
-0.916106, 0.289002, 0.277897,   -0.952377, 0.097549, 0.2889,   -0.279015, 0.956549, 0.084638,   
-0.452794, 0.880972, 0.137353,   -0.098013, 0.995138, -0.009653,   -0.094246, 0.995138, -0.028589,   
-0.086858, 0.995138, -0.046426,   -0.076131, 0.995138, -0.062479,   -0.062479, 0.995138, -0.076131,   
-0.046426, 0.995138, -0.086858,   -0.028589, 0.995138, -0.094246,   -0.009653, 0.995138, -0.098013,   
0.009653, 0.995138, -0.098012,   0.028589, 0.995138, -0.094246,   0.046426, 0.995138, -0.086857,   
0.062479, 0.995138, -0.076131,   0.076131, 0.995138, -0.062479,   0.086858, 0.995138, -0.046426,   
0.094246, 0.995138, -0.028589,   0.098012, 0.995138, -0.009653,   0.098012, 0.995138, 0.009653,   
0.094246, 0.995138, 0.028589,   0.086858, 0.995138, 0.046426,   0.076131, 0.995138, 0.062479,   
0.062479, 0.995138, 0.076131,   0.046426, 0.995138, 0.086858,   0.028589, 0.995138, 0.094246,   
0.009653, 0.995138, 0.098013,   -0.009653, 0.995138, 0.098013,   -0.028589, 0.995138, 0.094246,   
-0.046426, 0.995138, 0.086858,   -0.06248, 0.995138, 0.076132,   -0.076132, 0.995138, 0.06248,   
-0.086858, 0.995138, 0.046426,   -0.094246, 0.995138, 0.028589,   -0.990438, 0.097549, 0.097554,   
-0.290166, 0.956549, 0.02858,   -0.470889, 0.880972, 0.046381,   -0.633158, 0.771506, 0.062363,   
-0.77078, 0.632562, 0.075918,   -0.878612, 0.469629, 0.086539,   -0.952718, 0.289003, 0.093838,   
-0.098013, 0.995138, 0.009654};   

static char* group_name = "";
static int face_sizes[256] = {
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 
3}; 

static int face_vertex_ids[256][4] = {
{2,1,9,10}, {3,2,10,11}, {4,3,11,12}, 
{5,4,12,13}, {6,5,13,14}, {7,6,14,15}, 
{1,0,8,9}, {12,11,19,20}, {13,12,20,21}, 
{14,13,21,22}, {15,14,22,23}, {9,8,16,17}, 
{10,9,17,18}, {11,10,18,19}, {18,17,25,26}, 
{19,18,26,27}, {20,19,27,28}, {21,20,28,29}, 
{22,21,29,30}, {23,22,30,31}, {17,16,24,25}, 
{30,29,37,38}, {31,30,38,39}, {25,24,32,33}, 
{26,25,33,34}, {27,26,34,35}, {28,27,35,36}, 
{29,28,36,37}, {36,35,43,44}, {37,36,44,45}, 
{38,37,45,46}, {39,38,46,47}, {33,32,40,41}, 
{34,33,41,42}, {35,34,42,43}, {42,41,49,50}, 
{43,42,50,51}, {44,43,51,52}, {45,44,52,53}, 
{46,45,53,54}, {47,46,54,55}, {41,40,48,49}, 
{54,53,61,62}, {55,54,62,63}, {49,48,56,57}, 
{50,49,57,58}, {51,50,58,59}, {52,51,59,60}, 
{53,52,60,61}, {60,59,67,68}, {61,60,68,69}, 
{62,61,69,70}, {63,62,70,71}, {57,56,64,65}, 
{58,57,65,66}, {59,58,66,67}, {65,64,72,73}, 
{66,65,73,74}, {67,66,74,75}, {68,67,75,76}, 
{69,68,76,77}, {70,69,77,78}, {71,70,78,79}, 
{77,76,84,85}, {78,77,85,86}, {79,78,86,87}, 
{73,72,80,81}, {74,73,81,82}, {75,74,82,83}, 
{76,75,83,84}, {83,82,90,91}, {84,83,91,92}, 
{85,84,92,93}, {86,85,93,94}, {87,86,94,95}, 
{81,80,88,89}, {82,81,89,90}, {95,94,102,103}, 
{89,88,96,97}, {90,89,97,98}, {91,90,98,99}, 
{92,91,99,100}, {93,92,100,101}, {94,93,101,102}, 
{101,100,108,109}, {102,101,109,110}, {103,102,110,111}, 
{97,96,104,105}, {98,97,105,106}, {99,98,106,107}, 
{100,99,107,108}, {107,106,114,115}, {108,107,115,116}, 
{109,108,116,117}, {110,109,117,118}, {111,110,118,119}, 
{105,104,112,113}, {106,105,113,114}, {119,118,126,127}, 
{113,112,120,121}, {114,113,121,122}, {115,114,122,123}, 
{116,115,123,124}, {117,116,124,125}, {118,117,125,126}, 
{125,124,132,133}, {126,125,133,134}, {127,126,134,135}, 
{121,120,128,129}, {122,121,129,130}, {123,122,130,131}, 
{124,123,131,132}, {131,130,138,139}, {132,131,139,140}, 
{133,132,140,141}, {134,133,141,142}, {135,134,142,143}, 
{129,128,136,137}, {130,129,137,138}, {143,142,150,151}, 
{137,136,144,145}, {138,137,145,146}, {139,138,146,147}, 
{140,139,147,148}, {141,140,148,149}, {142,141,149,150}, 
{148,147,155,156}, {149,148,156,157}, {150,149,157,158}, 
{151,150,158,159}, {145,144,152,153}, {146,145,153,154}, 
{147,146,154,155}, {154,153,161,162}, {155,154,162,163}, 
{156,155,163,164}, {157,156,164,165}, {158,157,165,166}, 
{159,158,166,167}, {153,152,160,161}, {166,165,173,174}, 
{167,166,174,175}, {161,160,168,169}, {162,161,169,170}, 
{163,162,170,171}, {164,163,171,172}, {165,164,172,173}, 
{172,171,179,180}, {173,172,180,181}, {174,173,181,182}, 
{175,174,182,183}, {169,168,176,177}, {170,169,177,178}, 
{171,170,178,179}, {178,177,185,186}, {179,178,186,187}, 
{180,179,187,188}, {181,180,188,189}, {182,181,189,190}, 
{183,182,190,191}, {177,176,184,185}, {190,189,197,198}, 
{191,190,198,199}, {185,184,192,193}, {186,185,193,194}, 
{187,186,194,195}, {188,187,195,196}, {189,188,196,197}, 
{196,195,203,204}, {197,196,204,205}, {198,197,205,206}, 
{199,198,206,207}, {193,192,200,201}, {194,193,201,202}, 
{195,194,202,203}, {202,201,209,210}, {203,202,210,211}, 
{204,203,211,212}, {205,204,212,213}, {206,205,213,214}, 
{207,206,214,215}, {201,200,208,209}, {214,213,221,222}, 
{215,214,222,223}, {209,208,216,217}, {210,209,217,218}, 
{211,210,218,219}, {212,211,219,220}, {213,212,220,221}, 
{219,218,226,227}, {220,219,227,228}, {221,220,228,229}, 
{222,221,229,230}, {223,222,230,231}, {217,216,224,225}, 
{218,217,225,226}, {231,230,239,240}, {225,224,233,234}, 
{226,225,234,235}, {227,226,235,236}, {228,227,236,237}, 
{229,228,237,238}, {230,229,238,239}, {238,237,245,246}, 
{239,238,246,247}, {240,239,247,248}, {234,233,241,242}, 
{235,234,242,243}, {236,235,243,244}, {237,236,244,245}, 
{244,243,251,252}, {245,244,252,253}, {246,245,253,254}, 
{247,246,254,255}, {248,247,255,256}, {242,241,249,250}, 
{243,242,250,251}, {0,232,8,-1}, {8,232,16,-1}, 
{16,232,24,-1}, {24,232,32,-1}, {32,232,40,-1}, 
{40,232,48,-1}, {48,232,56,-1}, {56,232,64,-1}, 
{64,232,72,-1}, {72,232,80,-1}, {80,232,88,-1}, 
{88,232,96,-1}, {96,232,104,-1}, {104,232,112,-1}, 
{112,232,120,-1}, {120,232,128,-1}, {128,232,136,-1}, 
{136,232,144,-1}, {144,232,152,-1}, {152,232,160,-1}, 
{160,232,168,-1}, {168,232,176,-1}, {176,232,184,-1}, 
{184,232,192,-1}, {192,232,200,-1}, {200,232,208,-1}, 
{208,232,216,-1}, {216,232,224,-1}, {224,232,233,-1}, 
{233,232,241,-1}, {241,232,249,-1}, {256,255,6,7}, 
{250,249,0,1}, {251,250,1,2}, {252,251,2,3}, 
{253,252,3,4}, {254,253,4,5}, {255,254,5,6}, 
{249,232,0,-1}}; 

static int face_normal_ids[256][4] = {
{0,0,0,0}, {1,1,1,1}, {2,2,2,2}, 
{3,3,3,3}, {4,4,4,4}, {5,5,5,5}, 
{6,6,6,6}, {7,7,7,7}, {8,8,8,8}, 
{9,9,9,9}, {10,10,10,10}, {11,11,11,11}, 
{12,12,12,12}, {13,13,13,13}, {14,14,14,14}, 
{15,15,15,15}, {16,16,16,16}, {17,17,17,17}, 
{18,18,18,18}, {19,19,19,19}, {20,20,20,20}, 
{21,21,21,21}, {22,22,22,22}, {23,23,23,23}, 
{24,24,24,24}, {25,25,25,25}, {26,26,26,26}, 
{27,27,27,27}, {28,28,28,28}, {29,29,29,29}, 
{30,30,30,30}, {31,31,31,31}, {32,32,32,32}, 
{33,33,33,33}, {34,34,34,34}, {35,35,35,35}, 
{36,36,36,36}, {37,37,37,37}, {38,38,38,38}, 
{39,39,39,39}, {40,40,40,40}, {41,41,41,41}, 
{42,42,42,42}, {43,43,43,43}, {44,44,44,44}, 
{45,45,45,45}, {46,46,46,46}, {47,47,47,47}, 
{48,48,48,48}, {49,49,49,49}, {50,50,50,50}, 
{51,51,51,51}, {52,52,52,52}, {53,53,53,53}, 
{54,54,54,54}, {55,55,55,55}, {56,56,56,56}, 
{57,57,57,57}, {58,58,58,58}, {59,59,59,59}, 
{60,60,60,60}, {61,61,61,61}, {62,62,62,62}, 
{63,63,63,63}, {64,64,64,64}, {65,65,65,65}, 
{66,66,66,66}, {67,67,67,67}, {68,68,68,68}, 
{69,69,69,69}, {70,70,70,70}, {71,71,71,71}, 
{72,72,72,72}, {73,73,73,73}, {74,74,74,74}, 
{75,75,75,75}, {76,76,76,76}, {77,77,77,77}, 
{78,78,78,78}, {79,79,79,79}, {80,80,80,80}, 
{81,81,81,81}, {82,82,82,82}, {83,83,83,83}, 
{84,84,84,84}, {85,85,85,85}, {86,86,86,86}, 
{87,87,87,87}, {88,88,88,88}, {89,89,89,89}, 
{90,90,90,90}, {91,91,91,91}, {92,92,92,92}, 
{93,93,93,93}, {94,94,94,94}, {95,95,95,95}, 
{96,96,96,96}, {97,97,97,97}, {98,98,98,98}, 
{99,99,99,99}, {100,100,100,100}, {101,101,101,101}, 
{102,102,102,102}, {103,103,103,103}, {104,104,104,104}, 
{105,105,105,105}, {106,106,106,106}, {107,107,107,107}, 
{108,108,108,108}, {109,109,109,109}, {110,110,110,110}, 
{111,111,111,111}, {112,112,112,112}, {113,113,113,113}, 
{114,114,114,114}, {115,115,115,115}, {116,116,116,116}, 
{117,117,117,117}, {118,118,118,118}, {119,119,119,119}, 
{120,120,120,120}, {121,121,121,121}, {122,122,122,122}, 
{123,123,123,123}, {124,124,124,124}, {125,125,125,125}, 
{126,126,126,126}, {127,127,127,127}, {128,128,128,128}, 
{129,129,129,129}, {130,130,130,130}, {131,131,131,131}, 
{132,132,132,132}, {133,133,133,133}, {134,134,134,134}, 
{135,135,135,135}, {136,136,136,136}, {137,137,137,137}, 
{138,138,138,138}, {139,139,139,139}, {140,140,140,140}, 
{141,141,141,141}, {142,142,142,142}, {143,143,143,143}, 
{144,144,144,144}, {145,145,145,145}, {146,146,146,146}, 
{147,147,147,147}, {148,148,148,148}, {149,149,149,149}, 
{150,150,150,150}, {151,151,151,151}, {152,152,152,152}, 
{153,153,153,153}, {154,154,154,154}, {155,155,155,155}, 
{156,156,156,156}, {157,157,157,157}, {158,158,158,158}, 
{159,159,159,159}, {160,160,160,160}, {161,161,161,161}, 
{162,162,162,162}, {163,163,163,163}, {164,164,164,164}, 
{165,165,165,165}, {166,166,166,166}, {167,167,167,167}, 
{168,168,168,168}, {169,169,169,169}, {170,170,170,170}, 
{171,171,171,171}, {172,172,172,172}, {173,173,173,173}, 
{174,174,174,174}, {175,175,175,175}, {176,176,176,176}, 
{177,177,177,177}, {178,178,178,178}, {179,179,179,179}, 
{180,180,180,180}, {181,181,181,181}, {182,182,182,182}, 
{183,183,183,183}, {184,184,184,184}, {185,185,185,185}, 
{186,186,186,186}, {187,187,187,187}, {188,188,188,188}, 
{189,189,189,189}, {190,190,190,190}, {191,191,191,191}, 
{192,192,192,192}, {193,193,193,193}, {194,194,194,194}, 
{195,195,195,195}, {196,196,196,196}, {197,197,197,197}, 
{198,198,198,198}, {199,199,199,199}, {200,200,200,200}, 
{201,201,201,201}, {202,202,202,202}, {203,203,203,203}, 
{204,204,204,204}, {205,205,205,205}, {206,206,206,206}, 
{207,207,207,207}, {208,208,208,208}, {209,209,209,209}, 
{210,210,210,210}, {211,211,211,211}, {212,212,212,212}, 
{213,213,213,213}, {214,214,214,214}, {215,215,215,215}, 
{216,216,216,216}, {217,217,217,-1}, {218,218,218,-1}, 
{219,219,219,-1}, {220,220,220,-1}, {221,221,221,-1}, 
{222,222,222,-1}, {223,223,223,-1}, {224,224,224,-1}, 
{225,225,225,-1}, {226,226,226,-1}, {227,227,227,-1}, 
{228,228,228,-1}, {229,229,229,-1}, {230,230,230,-1}, 
{231,231,231,-1}, {232,232,232,-1}, {233,233,233,-1}, 
{234,234,234,-1}, {235,235,235,-1}, {236,236,236,-1}, 
{237,237,237,-1}, {238,238,238,-1}, {239,239,239,-1}, 
{240,240,240,-1}, {241,241,241,-1}, {242,242,242,-1}, 
{243,243,243,-1}, {244,244,244,-1}, {245,245,245,-1}, 
{246,246,246,-1}, {247,247,247,-1}, {248,248,248,248}, 
{249,249,249,249}, {250,250,250,250}, {251,251,251,251}, 
{252,252,252,252}, {253,253,253,253}, {254,254,254,254}, 
{255,255,255,-1}}; 


namespace mg
{

void 
mgluHemisphere(double r)
{
	glPushMatrix();
	glScaled(r, r, r);
	for ( int i=0; i<num_faces; i++ )
	{
		glBegin(GL_POLYGON);
		for ( int j=0; j<face_sizes[i]; j++ )
		{
			glNormal3dv( normals+ 3*face_normal_ids[i][j] );
			glVertex3dv( vertices+ 3*face_vertex_ids[i][j] );
		}
		glEnd();
	}
	glPopMatrix();
}


void
mgluHemisphere(math::vector pos, double r)
{
	glPushMatrix();
	glVertex3d(pos[0], pos[1], pos[2]);
	mgluHemisphere(r);
	glPopMatrix();
}


}