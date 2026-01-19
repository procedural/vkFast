/**********************************************************************
Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/
#pragma once

#include "float3.h"
#include "float2.h"
#include "quaternion.h"
#include "matrix.h"
#include "ray.h"
#include "bbox.h"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

#define PI 3.14159265358979323846f
#define OFFSETOF(struc,member) (&(((struc*)0)->member))

namespace RadeonRays
{
    /// Clamp float value to [a, b) range
    inline float clamp(float x, float a, float b)
    {
        return x < a ? a : (x > b ? b : x);
    }

    inline unsigned clamp(unsigned x, unsigned a, unsigned b)
    {
        return x < a ? a : (x > b ? b : x);
    }
    
    /// Clamp each component of the vector to [a, b) range
    inline float3 clamp(float3 const& v, float3 const& v1, float3 const& v2)
    {
        float3 res;
        res.x = clamp(v.x, v1.x, v2.x);
        res.y = clamp(v.y, v1.y, v2.y);
        res.z = clamp(v.z, v1.z, v2.z);
        return res;
    }

    /// Clamp each component of the vector to [a, b) range
    inline float2 clamp(float2 const& v, float2 const& v1, float2 const& v2)
    {
        float2 res;
        res.x = clamp(v.x, v1.x, v2.x);
        res.y = clamp(v.y, v1.y, v2.y);
        return res;
    }

    /// Transform a point using a matrix
    inline float3 transform_point(float3 const& p, matrix const& m)
    {
        float3 res = m * p;
        res.x += m.m03;
        res.y += m.m13;
        res.z += m.m23;
        return res;
    }
    
    /// Transform bounding box
    inline bbox transform_bbox(bbox const& b, matrix const& m)
    {
        // Get extents
        float3 extents = b.extents();
        
        // Transform the box to correct instance space
        bbox newbox(transform_point(b.pmin, m));
        newbox.grow(transform_point(b.pmin + float3(extents.x, 0, 0), m));
        newbox.grow(transform_point(b.pmin + float3(extents.x, extents.y, 0), m));
        newbox.grow(transform_point(b.pmin + float3(0, extents.y, 0), m));
        newbox.grow(transform_point(b.pmin + float3(extents.x, 0, extents.z), m));
        newbox.grow(transform_point(b.pmin + float3(extents.x, extents.y, extents.z), m));
        newbox.grow(transform_point(b.pmin + float3(0, extents.y, extents.z), m));
        newbox.grow(transform_point(b.pmin + float3(0, 0, extents.z), m));
        
        return newbox;
    }

    // Checks if the float value is IEEE FP NaN
    inline bool is_nan(float val)
    {
        return val != val;
    }
}