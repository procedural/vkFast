#include "C:/Users/Constantine/Desktop/vkfast/vkfast.h"
#include "C:/Users/Constantine/Desktop/vkfast/vkfast_ex.h"
#pragma comment(lib, "C:/Users/Constantine/Desktop/vkfast/extra/Build vkFast as a library/vkFast.lib")

void buildCommandBuffer()
{
  VkCommandBuffer vkcmdBuffer = drawCmdBuffers[currentBuffer];
  VkImage presentImage = swapChain.images[currentImageIndex];

  // NOTE(Constantine): Recording nothing for validation layers to chill :D
  VkCommandBufferBeginInfo vkcmdBufInfo = vks::initializers::commandBufferBeginInfo();
  VK_CHECK_RESULT(vkBeginCommandBuffer(vkcmdBuffer, &vkcmdBufInfo));
  VK_CHECK_RESULT(vkEndCommandBuffer(vkcmdBuffer));

  static std::map<VkCommandBuffer, uint64_t> batches;

  {
    #define FF __FILE__
    #define LL __LINE__

    static gpu_handle_context_t ctx = NULL;

    const unsigned array65536[2] = {65536, 65536};
    static gpu_thread_t gpu_thread = NULL;

    static int once = 1;
    if (once == 1) {
      once = 0;

      gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
      memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024) - 64; // NOTE(Constantine)(Mar 20, 2026): '- 64' added for Intel iGPUs which can allocate not 1073741824, but 1073741820 max, lol.
      memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
      memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
      memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
      gpu_context_optional_parameters_t optional_parameters = {0};
      optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

      gpu_context_ex2_parameters_t ex2_parameters = {0};
      ex2_parameters.external_VkInstance       = (uint64_t)instance;
      ex2_parameters.external_VkPhysicalDevice = (uint64_t)physicalDevice;
      ex2_parameters.external_VkDevice         = (uint64_t)device;
      ex2_parameters.exposeOnlyOneGpu          = 1;
      ex2_parameters.exposeOnlyOneQueue        = 1;

      ctx = vfContextInitEx2(1, 0, &optional_parameters, &ex2_parameters, FF, LL);

      vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);
    }

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 2;
    bindings_info.max_storage_binds_count     = 2;
    bindings_info.max_texture_rw_binds_count  = 1;
    batches[vkcmdBuffer] = vfBatchBegin(ctx, batches[vkcmdBuffer], &bindings_info, NULL, FF, LL);
    VkCommandBuffer cmdBuffer = (VkCommandBuffer)(void *)vfBatchGetRawHandle(ctx, batches[vkcmdBuffer], FF, LL);

    {
      VkClearValue clearValues[2]{};
      clearValues[0].color = { { 0.0f, 0.0f, 0.2f, 0.0f } };
      clearValues[1].depthStencil = { 1.0f, 0 };

      VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
      renderPassBeginInfo.renderPass = renderPass;
      renderPassBeginInfo.renderArea.extent.width = width;
      renderPassBeginInfo.renderArea.extent.height = height;
      renderPassBeginInfo.clearValueCount = 2;
      renderPassBeginInfo.pClearValues = clearValues;
      renderPassBeginInfo.framebuffer = frameBuffers[currentImageIndex];

      vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

      VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
      vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

      VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
      vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

      // Non-instanced static objects
      vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentBuffer].staticObjects, 0, nullptr);
      vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.starfield);
      // Starfield (backdrop)
      vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
      // Planet
      vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.planet);
      models.planet.draw(cmdBuffer);

      // Instanced rocks
      VkDeviceSize offsets[1] = { 0 };
      vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentBuffer].instancedRocks, 0, NULL);
      vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.instancedRocks);
      // Binding point 0 : Mesh vertex buffer
      vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &models.rock.vertices.buffer, offsets);
      // Binding point 1 : Instance data buffer
      vkCmdBindVertexBuffers(cmdBuffer, 1, 1, &instanceBuffer.buffer, offsets);
      // Bind index buffer
      vkCmdBindIndexBuffer(cmdBuffer, models.rock.indices.buffer, 0, VK_INDEX_TYPE_UINT32);

      // Render instances
      vkCmdDrawIndexed(cmdBuffer, models.rock.indices.count, INSTANCE_COUNT, 0, 0, 0);

      drawUI(cmdBuffer);

      vkCmdEndRenderPass(cmdBuffer);

      VkImageMemoryBarrier imgbarrier = {};
      imgbarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      imgbarrier.pNext = 0;
      imgbarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
      imgbarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
      imgbarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
      imgbarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
      imgbarrier.srcQueueFamilyIndex = -1;
      imgbarrier.dstQueueFamilyIndex = -1;
      imgbarrier.image = swapChain.images[currentImageIndex];
      imgbarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imgbarrier.subresourceRange.baseMipLevel = 0;
      imgbarrier.subresourceRange.levelCount = -1;
      imgbarrier.subresourceRange.baseArrayLayer = 0;
      imgbarrier.subresourceRange.layerCount = -1;
      vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR, 0, 0, 0, 0, 0, 1, &imgbarrier);
    }

    vfBatchEnd(ctx, batches[vkcmdBuffer], FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batches[vkcmdBuffer], FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);
  }
}