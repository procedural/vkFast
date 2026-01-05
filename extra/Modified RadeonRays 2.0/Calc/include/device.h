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

#include "wrappers/command_buffer.h"
#include "wrappers/device.h"
#include "wrappers/fence.h"
#include <atomic>
#include <array>
#include <memory>

namespace Calc
{
    class Function;
    class Buffer;
    class Executable;
    class Event;

    class Executable;
    class Function;
    class Event;
    class Primitives;

    // Calc device specification
    struct DeviceSpec
    {
        char const* name;
        char const* vendor;

        std::uint32_t min_alignment;
        std::uint32_t max_num_queues;

        std::size_t global_mem_size;
        std::size_t local_mem_size;
        std::size_t max_alloc_size;
        std::size_t max_local_size;
    };

    class Device
    {
    public:
        static const unsigned int NUM_FENCE_TRACKERS = 16;

        Device( Anvil::Device* inDevice, bool in_use_compute_pipe );
        ~Device();

        // Return specification of the device
        void GetSpec( DeviceSpec& spec );

        // Buffer creation and deletion
        Buffer* CreateBuffer( std::size_t size, std::uint32_t flags );
        Buffer* CreateBuffer( std::size_t size, std::uint32_t flags, void* initdata );
        Buffer* CreateBufferReadback( std::size_t size, std::uint32_t flags );
        Buffer* CreateBufferReadback( std::size_t size, std::uint32_t flags, void* initdata );
        void DeleteBuffer( Buffer* buffer );

        // Data movement
        void ReadBuffer( Buffer const* buffer, std::uint32_t queue, std::size_t offset, std::size_t size, void* dst, Event** e ) const;
        void WriteBuffer( Buffer const* buffer, std::uint32_t queue, std::size_t offset, std::size_t size, void* src, Event** e );

        // Buffer mapping 
        void MapBuffer( Buffer const* buffer, std::uint32_t queue, std::size_t offset, std::size_t size, std::uint32_t map_type, void** mapdata, Event** e );
        void MapBufferReadback( Buffer const* buffer, std::uint32_t queue, std::size_t offset, std::size_t size, std::uint32_t map_type, void** mapdata );
        void UnmapBuffer( Buffer const* buffer, std::uint32_t queue, void* mapdata, Event** e );
        void UnmapBufferReadback( Buffer const* buffer, std::uint32_t queue, void* mapdata );

        // Kernel compilation
        Executable* CompileExecutable( );
        Executable* CompileExecutable( char const* source_code, std::size_t size, char const* options );
        Executable* CompileExecutable( std::uint8_t const* binary_code, std::size_t size, char const* options );
        Executable* CompileExecutable( char const* filename,
                                       char const** headernames,
                                       int numheaders, char const* options);

        void DeleteExecutable( Executable* executable );

        // Executable management
        size_t GetExecutableBinarySize( Executable const* executable ) const;
        void GetExecutableBinary( Executable const* executable, std::uint8_t* binary ) const;

        // Execution
        void Execute( Function const* func, std::uint32_t queue, size_t global_size, size_t local_size, Event** e );

        // Events handling
        void WaitForEvent( Event* e );
        void WaitForMultipleEvents( Event** e, std::size_t num_events );
        void DeleteEvent( Event* e );

        // Queue management functions
        void Flush( std::uint32_t queue );
        void Finish( std::uint32_t queue );

        // Parallel prims handling
        bool HasBuiltinPrimitives() const;
        Primitives* CreatePrimitives() const;
        void DeletePrimitives( Primitives* prims );

        bool InitializeVulkanResources();
        bool InitializeVulkanCommandBuffer(Anvil::CommandPool* cmd_pool);
        
        Anvil::PrimaryCommandBuffer* GetCommandBuffer() const { return m_command_buffer.get(); }

        // returns true if the compute pipeline should be used
        bool GetUseComputePipe() const { return m_use_compute_pipe; }

        uint64_t GetFenceId() const { return m_cpu_fence_id; }

        bool HasFenceBeenPassed(uint64_t id) const { return m_gpu_known_fence_id > id; }

        void WaitForFence( uint64_t id ) const;

        typedef std::unique_ptr<Anvil::PrimaryCommandBuffer> PrimaryCommandBuffer;
        typedef std::array<std::unique_ptr<Anvil::Fence>, NUM_FENCE_TRACKERS> FenceArray;

        uint64_t AllocNextFenceId();

        // Managing CommandBuffer to record Vulkan commands
        void StartRecording();
        void EndRecording( bool in_wait_till_completed, Event** out_event );
        void CommitCommandBuffer( bool in_wait_till_completed );

        Anvil::Fence* GetFence( uint64_t id ) const { return m_anvil_fences[id%NUM_FENCE_TRACKERS].get(); }

        Anvil::Queue* GetQueue() const;

        // Anvil device
        Anvil::Device* m_anvil_device;

        // Main CommandBuffer to record Vulkan commands
        PrimaryCommandBuffer m_command_buffer;

        // To indicate whether recording is already in progress
        bool m_is_command_buffer_recording;

        // Whether to use compute pipe
        bool m_use_compute_pipe;

        // Fences for synchronization
        FenceArray    m_anvil_fences;
        std::atomic<uint64_t> m_cpu_fence_id;
        mutable std::atomic<uint64_t> m_gpu_known_fence_id;

    };

}
