#pragma once

#define USE_DEFAULT

#include <iterator>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <cstdarg>

#include <Windows.h>
#include <conio.h>
#include <crtdbg.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>

#include <d3d9.h>
#include <d3dx9.h>

using namespace std;

#define PI (2*acos(0.0f))

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define FRAMES_PER_BUFFER 256

#define SAFE_DELETE(p) {if(p){delete (p);(p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete[] (p);(p)=NULL;}}
#define SAFE_RELEASE(p) {if(p){p->Release();(p)=NULL;}}