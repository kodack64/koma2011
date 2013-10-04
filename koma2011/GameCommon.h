
#pragma once

#include "Common.h"
#include "GraphicApi.h"
#include "InputApi.h"
#include <Box2D\\Box2D.h>

#define MAX_MAGNET 1000
#define MAX_TRACE 10
#define EXTENDS 100
#define MAX_LIGHT 180
#define MAXDARK 180
#define FIRSTCOUNT 30
#define FIRSTBALL 15
#define MAX_PARTICLE 3
#define AMPL (100.0)
#define M2P(x) ((int)(((float)x)*AMPL))
#define P2M(x) ((float)(((float)(x))/AMPL))
#define EPS 5

#pragma warning(disable:4244)