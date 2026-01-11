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

#include <memory>
#include <functional>
#include <queue>

#include "bvhstrategy.h"
#include "bvh2lstrategy.h"

namespace RadeonRays
{
    class Strategy;
    struct CalcEventHolder;

    ///< The class represents Calc based intersection device.
    ///< It uses Calc::Device abstraction to implement intersection algorithm.
    ///<
    class CalcIntersectionDevice
    {
    public:
        //
        CalcIntersectionDevice(Calc::Device* device);
        ~CalcIntersectionDevice();

        void Preprocess(World const& world);

        Buffer* CreateBuffer(size_t size, void* initdata) const;
        Buffer* CreateBufferReadback(size_t size, void* initdata) const;

        void DeleteBuffer(Buffer* const) const;

        void DeleteEvent(Event* const) const;

        void MapBuffer(Buffer* buffer, MapType type, size_t offset, size_t size, void** data, Event** event) const;
        void MapBufferReadback(Buffer* buffer, MapType type, size_t offset, size_t size, void** data) const;

        void UnmapBuffer(Buffer* buffer, void* ptr, Event** event) const;
        void UnmapBufferReadback(Buffer* buffer, void* ptr) const;

        void QueryIntersection(Buffer const* rays, int numrays, Buffer* hitinfos, Event const* waitevent, Event** event) const;

        void QueryOcclusion(Buffer const* rays, int numrays, Buffer* hitresults, Event const* waitevent, Event** event) const;

        void QueryIntersection(Buffer const* rays, Buffer const* numrays, int maxrays, Buffer* hitinfos, Event const* waitevent, Event** event) const;

        void QueryOcclusion(Buffer const* rays, Buffer const* numrays, int maxrays, Buffer* hitresults, Event const* waitevent, Event** event) const;

        CalcEventHolder* CreateEventHolder() const;
        void      ReleaseEventHolder(CalcEventHolder* e) const;

        std::unique_ptr<Calc::Device, std::function<void(Calc::Device*)>> m_device;
        std::unique_ptr<BvhStrategy> m_intersector;
        std::unique_ptr<Bvh2lStrategy> m_intersector2l;
        std::string m_intersector_string;

        // Initial number of events in the pool
        static const std::size_t EVENT_POOL_INITIAL_SIZE = 100;
        // Event pool
        mutable std::queue<CalcEventHolder*> m_event_pool;
    };
}

