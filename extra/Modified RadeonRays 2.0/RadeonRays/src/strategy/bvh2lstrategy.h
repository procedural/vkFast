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

#include "calc.h"
#include "device.h"
#include "radeon_rays.h"
#include "event.h"
#include "executable.h"
#include "../src/world/world.h"
#include "../src/translator/plain_bvh_translator.h"
#include <memory>

namespace RadeonRays
{
    struct Bvh2lStrategyShapeData
    {
        // Shape ID
        Id id;
        // Index of root bvh node
        int bvhidx;
        int mask;
        int padding1;
        // Transform
        matrix minv;
        // Motion blur data
        float3 linearvelocity;
        // Angular veocity (quaternion)
        quaternion angularvelocity;
    };

    struct Bvh2lStrategyFace
    {
        int idx[3];
        int shadeidx;
        // Primitive ID within the mesh
        int id;
        // Idx count
        int cnt;
        int padding[2];
    };

    struct Bvh2lStrategyGpuData
    {
        // Device
        Calc::Device* device;
        // BVH nodes
        Calc::Buffer* bvh;
        // Vertex positions
        Calc::Buffer* vertices;
        // Indices
        Calc::Buffer* faces;
        // Shape IDs
        Calc::Buffer* shapes;

        int bvhrootidx;

        Calc::Executable* executable;
        Calc::Function* isect_func;
        Calc::Function* occlude_func;
        Calc::Function* isect_indirect_func;
        Calc::Function* occlude_indirect_func;

        Bvh2lStrategyGpuData(Calc::Device* d)
            : device(d)
            , bvh(nullptr)
            , vertices(nullptr)
            , faces(nullptr)
            , shapes(nullptr)
            , bvhrootidx(-1)
            , executable(nullptr)
            , isect_func(nullptr)
            , occlude_func(nullptr)
            , isect_indirect_func(nullptr)
            , occlude_indirect_func(nullptr)
        {
        }

        ~Bvh2lStrategyGpuData()
        {
            device->DeleteBuffer(bvh);
            device->DeleteBuffer(vertices);
            device->DeleteBuffer(faces);
            device->DeleteBuffer(shapes);
            if(executable != nullptr)
            {
                executable->DeleteFunction(isect_func);
                executable->DeleteFunction(occlude_func);
                executable->DeleteFunction(isect_indirect_func);
                executable->DeleteFunction(occlude_indirect_func);
                device->DeleteExecutable(executable);
            }
        }
    };


    struct Bvh2lStrategyCpuData
    {
        std::vector<int> mesh_vertices_start_idx;
        std::vector<int> mesh_faces_start_idx;
        std::vector<Bvh const*> bvhptrs;
        std::vector<Bvh2lStrategyShapeData> shapedata;
        std::vector<bbox> bounds;

        PlainBvhTranslator translator;
    };
}

namespace RadeonRays
{
    class Bvh;

    class Bvh2lStrategy
    {
    public:
        Bvh2lStrategy(Calc::Device* device);
        
        void Preprocess(World const& world);
        
        void QueryIntersection(std::uint32_t queueidx,
                               Calc::Buffer const* rays,
                               std::uint32_t numrays,
                               Calc::Buffer* hits,
                               Calc::Event const* waitevent,
                               Calc::Event** event) const;
        
        void QueryOcclusion(std::uint32_t queueidx,
                            Calc::Buffer const* rays,
                            std::uint32_t numrays,
                            Calc::Buffer* hits,
                            Calc::Event const* waitevent,
                            Calc::Event** event) const;
        
        void QueryIntersection(std::uint32_t queueidx,
                               Calc::Buffer const* rays,
                               Calc::Buffer const* numrays,
                               std::uint32_t maxrays,
                               Calc::Buffer* hits,
                               Calc::Event const* waitevent,
                               Calc::Event** event) const;
        
        void QueryOcclusion(std::uint32_t queueidx,
                            Calc::Buffer const* rays,
                            Calc::Buffer const* numrays,
                            std::uint32_t maxrays,
                            Calc::Buffer* hits,
                            Calc::Event const* waitevent,
                            Calc::Event** event) const;

        Calc::Device* m_device;

    private:
        std::unique_ptr<Bvh2lStrategyGpuData> m_gpudata;
        std::unique_ptr<Bvh2lStrategyCpuData> m_cpudata;
        std::vector<std::unique_ptr<Bvh> > m_bvhs;
    };
}

