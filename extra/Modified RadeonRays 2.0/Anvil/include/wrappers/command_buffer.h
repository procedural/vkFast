//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/** Defines a command buffer wrapper classes which simplify the following processes:
 *
 *  - Automatic object management (any object referred to from a command automatically
*                that object's reference counter).
 *  - Debugging (commands are stored internally in debug builds for quick investigation;
 *               cumbersome Vulkan descriptor arrays are converted to vectors;
 *               raw Vulkan object handles now refer to wrapper objects)
 *  - Embedded verification of command usage validity (eg. any attempt to set an event
 *               from within a render-pass will throw an assertion failure)
 *  - Life-time management
 *
 **/
#ifndef WRAPPERS_COMMAND_BUFFER_H
#define WRAPPERS_COMMAND_BUFFER_H


#include "misc/types.h"

namespace Anvil
{
    /* Forward declarations */
    struct BufferBarrier;

    /** Enumerates available Vulkan command buffer types */
    typedef enum
    {
        COMMAND_BUFFER_TYPE_PRIMARY,
    } CommandBufferType;

    /** Base structure for a Vulkan command.
     *
     *  Parent structure for all specialized Vulkan command structures which describe
     *  actual Vulkan commands.
     **/
    typedef struct Command
    {
        /** Constructor.
         *
         *  @param in_type Type of the command encapsulated by the structure. */
        Command()
        {
        }

        /** Stub destructor. */
        virtual ~Command()
        {
            /* Stub */
        }
    private:
        Command(const Command& in);
    } Command;

    /** Holds all arguments passed to a vkCmdPipelineBarrier() command.
     *
     *  Takes an array of Barrier descriptors instead of void* pointers, as is the case
     *  with the original Vulkan API. Each buffer in a buffer barrier, and each image in
     *  an image barrier, is retained.
     **/
    typedef struct PipelineBarrierCommand : public Command
    {
        std::vector<BufferBarrier> buffer_barriers;
        std::vector<MemoryBarrier> memory_barriers;

        VkBool32                   by_region;
        VkPipelineStageFlagBits    dst_stage_mask;
        VkPipelineStageFlagBits    src_stage_mask;

        /** Constructor.
         *
         *  Please see the general note for PipelineBarrierCommand structure for more details.
         *
         *  Arguments as per Vulkan API.
         **/
        explicit PipelineBarrierCommand(VkPipelineStageFlags       in_src_stage_mask,
                                        VkPipelineStageFlags       in_dst_stage_mask,
                                        VkBool32                   in_by_region,
                                        uint32_t                   in_memory_barrier_count,
                                        const MemoryBarrier* const in_memory_barriers_ptr_ptr,
                                        uint32_t                   in_buffer_memory_barrier_count,
                                        const BufferBarrier* const in_buffer_memory_barrier_ptr_ptr);
    } PipelineBarrierCommand;

    /** Implements base functionality of a command buffer object, such as common command registration
     *  support or validation. Also encapsulates command wrapper structure declarations.
     *
     *  The command buffer baking process is not deferred, which means you can use the wrapped Vulkan
     *  command buffer instance right after recording finishes without any additional performance cost.
     *
     *  Provides core functionality for the PrimaryCommandBuffer class.
     */
    class CommandBufferBase
    {
    public:
        /* Public functions */

        /* Disables internal command stashing which is enbled for builds created with
         * STORE_COMMAND_BUFFER_COMMANDS enabled.
         *
         * Nop for builds that were not built with the definition enabled.
         */
        static void disable_comand_stashing()
        {
            m_command_stashing_disabled = true;
        }

        /** Returns a handle to the raw Vulkan command buffer instance, encapsulated by the object */
        const VkCommandBuffer get_command_buffer() const
        {
            return m_command_buffer;
        }

        /** Returns a pointer to the handle to the raw Vulkan command buffer instance, as encapsulated
         *  by the object.
         **/
        const VkCommandBuffer* get_command_buffer_ptr() const
        {
            return &m_command_buffer;
        }

        /** Tells the type of the command buffer instance */
        CommandBufferType get_command_buffer_type() const
        {
            return m_type;
        }

        /** Issues a vkCmdBindDescriptorSets() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_bind_descriptor_sets(VkPipelineBindPoint    in_pipeline_bind_point,
                                         Anvil::PipelineLayout* in_layout_ptr,
                                         uint32_t               in_first_set,
                                         uint32_t               in_set_count,
                                         Anvil::DescriptorSet** in_descriptor_set_ptrs,
                                         uint32_t               in_dynamic_offset_count,
                                         const uint32_t*        in_dynamic_offset_ptrs);

        /** Issues a vkCmdBindPipeline() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_bind_pipeline(VkPipelineBindPoint in_pipeline_bind_point,
                                  Anvil::PipelineID   in_pipeline_id);

        /** Issues a vkCmdCopyBuffer() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  It is also illegal to call this function when recording renderpass commands. Doing so
         *  will also result in an assertion failure.
         *
         *  Any Vulkan object wrapper instances passed to this function are going to be retained,
         *  and will be released when the command buffer is released or resetted.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_copy_buffer(Anvil::Buffer*      in_src_buffer_ptr,
                                Anvil::Buffer*      in_dst_buffer_ptr,
                                uint32_t            in_region_count,
                                const VkBufferCopy* in_region_ptrs);

        /** Issues a vkCmdDispatch() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  It is also illegal to call this function when recording renderpass commands. Doing so
         *  will also result in an assertion failure.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_dispatch(uint32_t in_x,
                             uint32_t in_y,
                             uint32_t in_z);

        /** Issues a vkCmdPipelineBarrier() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  Any Vulkan object wrapper instances, passed implicitly to this function, are going to be retained,
         *  and will be released when the command buffer is released or resetted.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_pipeline_barrier(VkPipelineStageFlags       in_src_stage_mask,
                                     VkPipelineStageFlags       in_dst_stage_mask,
                                     VkBool32                   in_by_region,
                                     uint32_t                   in_memory_barrier_count,
                                     const MemoryBarrier* const in_memory_barriers_ptr,
                                     uint32_t                   in_buffer_memory_barrier_count,
                                     const BufferBarrier* const in_buffer_memory_barriers_ptr);

        /** Issues a vkCmdPushConstants() call and appends it to the internal vector of commands
         *  recorded for the specified command buffer (for builds with STORE_COMMAND_BUFFER_COMMANDS
         *  #define enabled).
         *
         *  Calling this function for a command buffer which has not been put into a recording mode
         *  (by issuing a start_recording() call earlier) will result in an assertion failure.
         *
         *  Argument meaning is as per Vulkan API specification.
         *
         *  @return true if successful, false otherwise.
         **/
        bool record_push_constants(Anvil::PipelineLayout* in_layout_ptr,
                                   VkShaderStageFlags     in_stage_flags,
                                   uint32_t               in_offset,
                                   uint32_t               in_size,
                                   const void*            in_values);

        /** Resets the underlying Vulkan command buffer and clears the internally managed vector of
         *  recorded commands, if STORE_COMMAND_BUFFER_COMMANDS has been defined for the build.
         *
         *  @param should_release_resources true if the vkResetCommandBuffer() should be made with the
         *                                  VK_CMD_BUFFER_RESET_RELEASE_RESOURCES_BIT flag set.
         *
         *  @return true if the request was handled successfully, false otherwise.
         **/
        bool reset(bool should_release_resources);

        /** Stops an ongoing command recording process.
         *
         *  It is an error to invoke this function if the command buffer has not been put
         *  into the recording mode by calling start_recording().
         *
         *  @return true if successful, false otherwise.
         **/
        bool stop_recording();

        VkCommandBuffer     m_command_buffer;
        Anvil::Device*      m_device_ptr;
        bool                m_is_renderpass_active;
        Anvil::CommandPool* m_parent_command_pool_ptr;
        bool                m_recording_in_progress;
        CommandBufferType   m_type;

        static bool         m_command_stashing_disabled;

    protected:
        /* Forward declarations */
        struct BindDescriptorSetsCommand;
        struct BindPipelineCommand;
        struct CopyBufferCommand;
        struct DispatchCommand;
        struct PushConstantsCommand;

        /* Protected type definitions */

        /** Holds all arguments passed to a vkCmdBindDescriptorSets() command. */
        typedef struct BindDescriptorSetsCommand : public Command
        {
            std::vector<Anvil::DescriptorSet*> descriptor_sets;
            std::vector<uint32_t>              dynamic_offsets;
            uint32_t                           first_set;
            Anvil::PipelineLayout*             layout_ptr;
            VkPipelineBindPoint                pipeline_bind_point;

            /** Constructor.
             *
             *  Retains the user-specified PipelineLayout instance.
             *
             *  Arguments as per Vulkan API.
             **/
            explicit BindDescriptorSetsCommand(VkPipelineBindPoint    in_pipeline_bind_point,
                                               Anvil::PipelineLayout* in_layout_ptr,
                                               uint32_t               in_first_set,
                                               uint32_t               in_set_count,
                                               Anvil::DescriptorSet** in_descriptor_set_ptrs,
                                               uint32_t               in_dynamic_offset_count,
                                               const uint32_t*        in_dynamic_offset_ptrs);

            /** Destructor.
             *
             *  Releases the encapsulated PipelineLayout instance */
            virtual ~BindDescriptorSetsCommand();
        } BindDescriptorSetsCommand;


        /** Holds all arguments passed to a vkCmdBindPipeline() command. */
        typedef struct BindPipelineCommand : public Command
        {
            VkPipelineBindPoint pipeline_bind_point;
            Anvil::PipelineID   pipeline_id;

            /** Constructor.
             *
             *  @param in_pipeline_bind_point As per Vulkan API.
             *  @param in_pipeline_id         ID of the pipeline. Can either be a compute pipeline ID, coming from
             *                                the device-specific compute pipeline manager initialized by the library,
             *                                or a graphics pipeline ID, coming from the device-specific graphics pipeline
             *                                manager. The type of the pipeline is deduced from @param in_pipeline_bind_point.
             **/
            explicit BindPipelineCommand(VkPipelineBindPoint in_pipeline_bind_point,
                                         Anvil::PipelineID   in_pipeline_id);
        } BindPipelineCommand;


        /** Holds all arguments passed to a vkCmdCopyBuffer() command.
         *
         *  Raw Vulkan object handles have been replaced with pointers to wrapper objects.
         *  These objects are retained at construction time, and released at descriptor
         *  destruction time.
         */
        typedef struct CopyBufferCommand : public Command
        {
            VkBuffer                  dst_buffer;
            Anvil::Buffer*            dst_buffer_ptr;
            std::vector<VkBufferCopy> regions;
            VkBuffer                  src_buffer;
            Anvil::Buffer*            src_buffer_ptr;

            /** Constructor.
             *
             *  Retains @param in_src_buffer_ptr and in_dst_buffer_ptr buffers.
             *
             *  Arguments as per Vulkan API.
             **/
            explicit CopyBufferCommand(Anvil::Buffer*      in_src_buffer_ptr,
                                       Anvil::Buffer*      in_dst_buffer_ptr,
                                       uint32_t            in_region_count,
                                       const VkBufferCopy* in_region_ptrs);

            /** Destructor.
             *
             *  Releases the encapsulated Buffer instances.
             **/
            virtual ~CopyBufferCommand();

            CopyBufferCommand(const CopyBufferCommand&);

        private:
            CopyBufferCommand& operator=(const CopyBufferCommand&);
        } CopyBufferCommand;


        /** Holds all arguments passed to a vkCmdDispatch() command. */
        typedef struct DispatchCommand : public Command
        {
            uint32_t x;
            uint32_t y;
            uint32_t z;

            /** Constructor.
             *
             *  Arguments as per Vulkan API.
             **/
            explicit DispatchCommand(uint32_t in_x,
                                     uint32_t in_y,
                                     uint32_t in_z);
        } DispatchCommand;


        /** Holds all arguments passed to a vkCmdPushConstants() command. */
        typedef struct PushConstantsCommand : public Command
        {
            Anvil::PipelineLayout* layout_ptr;
            uint32_t               offset;
            uint32_t               size;
            VkShaderStageFlags     stage_flags;
            const void*            values;

            /** Constructor.
             *
             *  Retains the user-specified PipelineLayout instance.
             *
             *  Arguments as per Vulkan API.
             **/
            explicit PushConstantsCommand(Anvil::PipelineLayout* in_layout_ptr,
                                          VkShaderStageFlags     in_stage_flags,
                                          uint32_t               in_offset,
                                          uint32_t               in_size,
                                          const void*            in_values);

            /** Destructor.
             *
             *  Releases the encapsulated PipelineLayout instance
             **/
            virtual ~PushConstantsCommand();
        } PushConstantsCommand;


        typedef std::vector<Command*> Commands;

        /* Protected functions */
        explicit CommandBufferBase(Anvil::Device*      device_ptr,
                                   Anvil::CommandPool* parent_command_pool_ptr,
                                   CommandBufferType   type);

        virtual ~CommandBufferBase();

        void on_parent_pool_released();

    private:
        /* Private type definitions */

        /* Private functions */
        CommandBufferBase           (const CommandBufferBase&);
        CommandBufferBase& operator=(const CommandBufferBase&);

        friend class Anvil::CommandPool;
    };

    /** Wrapper class for primary command buffers. */
    class PrimaryCommandBuffer : public CommandBufferBase
    {
    public:
        /* Public functions */

        /** Issues a vkBeginCommandBufer() call and clears the internally managed vector of recorded
         *  commands, if STORE_COMMAND_BUFFER_COMMANDS has been defined for the build.
         *
         *  It is an error to invoke this function if recording is already in progress.
         *
         *  @param one_time_submit          true if the VK_CMD_BUFFER_OPTIMIZE_ONE_TIME_SUBMIT_BIT flag should
         *                                  be used for the Vulkan API call.
         *  @param simultaneous_use_allowed true if the VK_CMD_BUFFER_OPTIMIZE_NO_SIMULTANEOUS_USE_BIT flag should
         *                                  be used for the Vulkan API call.
         *
         *  @return true if successful, false otherwise.
         **/
        bool start_recording(bool one_time_submit,
                             bool simultaneous_use_allowed);

    protected:
        /** Constructor. Should be used to instantiate primary-level command buffers.
         *
         *  NOTE: In order to create a command buffer, please call relevant alloc() functions
         *        from Anvil::CommandPool().
         *
         *  @param device_ptr              Device to use.
         *  @param parent_command_pool_ptr Command pool to use as a parent. Must not be nullptr.
         *
         **/
        PrimaryCommandBuffer(Anvil::Device* device_ptr,
                             CommandPool*   parent_command_pool_ptr);

    private:
        friend class Anvil::CommandPool;

        /* Private functions */
        PrimaryCommandBuffer           (const PrimaryCommandBuffer&);
        PrimaryCommandBuffer& operator=(const PrimaryCommandBuffer&);
    };
}; /* namespace Anvil */

#endif /* WRAPPERS_COMMAND_BUFFER_H */
