void buildCommandBuffer()
{
  VkCommandBuffer vkcmdBuffer = drawCmdBuffers[currentBuffer];
  VkImage presentImage = swapChain.images[currentImageIndex];
  VkImageView presentImageView = swapChain.imageViews[currentImageIndex];

  // NOTE(Constantine): Recording nothing for validation layers to chill :D
  VkCommandBufferBeginInfo vkcmdBufInfo = vks::initializers::commandBufferBeginInfo();
  VK_CHECK_RESULT(vkBeginCommandBuffer(vkcmdBuffer, &vkcmdBufInfo));
  VK_CHECK_RESULT(vkEndCommandBuffer(vkcmdBuffer));

  static std::map<VkCommandBuffer, uint64_t> batches;

  {
    #define FF __FILE__
    #define LL __LINE__

    static int once = 1;

    const int window_w = 1000;
    const int window_h = 1000;

    static gpu_handle_context_t ctx = NULL;
    if (once) {
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

      ctx = vfContextInitEx2(1, 0, &optional_parameters, &ex2_parameters, __FILE__, __LINE__);
    }

    const unsigned array65536[2] = {65536, 65536};
    static gpu_thread_t gpu_thread = NULL;
    if (once) {
      vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);
    }

    static gpu_storage_t storage_gpu_only     = {0};
    static gpu_storage_t storage_cpu_upload   = {0};
    static gpu_storage_t storage_cpu_readback = {0};
    if (once) {
      vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);
    }

    static uint64_t storage_gpu_only_mem_offset = 0;
    static uint64_t storage_cpu_upload_mem_offset = 0;
    static uint64_t storage_cpu_readback_mem_offset = 0;

    static gpu_extra_cpu_gpu_array mesh_vertex_array = {0};
    if (once) {
      mesh_vertex_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
        64/*mb*/ * 1024 * 1024,
        &storage_cpu_upload, &storage_cpu_upload_mem_offset,
        &storage_gpu_only,   &storage_gpu_only_mem_offset,
        FF, LL
      );
    }
    static gpu_extra_cpu_gpu_array instance_color_array = {0};
    if (once) {
      instance_color_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
        64/*mb*/ * 1024 * 1024,
        &storage_cpu_upload, &storage_cpu_upload_mem_offset,
        &storage_gpu_only,   &storage_gpu_only_mem_offset,
        FF, LL
      );
    }
    static gpu_extra_cpu_gpu_array instance_position_array = {0};
    if (once) {
      instance_position_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
        64/*mb*/ * 1024 * 1024,
        &storage_cpu_upload, &storage_cpu_upload_mem_offset,
        &storage_gpu_only,   &storage_gpu_only_mem_offset,
        FF, LL
      );
    }

    static RedStructDeclarationMember slots[3] = {0};
    static ReiiMeshState mesh_state = {0};
    if (once) {
      #include "C:/Users/Constantine/Desktop/vkfast/examples/21 REII Instancing/mesh.vs.h"
      #include "C:/Users/Constantine/Desktop/vkfast/examples/21 REII Instancing/mesh.fs.h"
      gpu_program_info_t vp = {0};
      vp.program_binary_bytes_count = sizeof(g_main_vs);
      vp.program_binary             = g_main_vs;
      gpu_program_info_t fp = {0};
      fp.program_binary_bytes_count = sizeof(g_main_fs);
      fp.program_binary             = g_main_fs;
      char * vp_string = NULL;
      char * fp_string = NULL;

      slots[0].slot            = 0;
      slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
      slots[0].count           = 1;
      slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
      slots[1].slot            = 1;
      slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
      slots[1].count           = 1;
      slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
      slots[2].slot            = 2;
      slots[2].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
      slots[2].count           = 1;
      slots[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
      gpu_extra_reii_mesh_state_compile_info_t mesh_state_compile_info = {0};
      mesh_state_compile_info.state_multisample_count     = RED_MULTISAMPLE_COUNT_BITFLAG_4;
      mesh_state_compile_info.output_depth_stencil_enable = 1;
      mesh_state_compile_info.output_depth_stencil_format = RED_FORMAT_DEPTH_32_FLOAT;
      mesh_state_compile_info.output_color_format         = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
      mesh_state_compile_info.variables_slot              = 3;
      mesh_state_compile_info.variables_bytes_count       = 2 * sizeof(ReiiVec4);
      mesh_state_compile_info.struct_members_count        = countof(slots);
      mesh_state_compile_info.struct_members              = slots;

      mesh_state.compileInfo                                    = mesh_state_compile_info;
      mesh_state.programVertex                                  = vp;
      mesh_state.programFragment                                = fp;
      mesh_state.codeVertex                                     = vp_string;
      mesh_state.codeFragment                                   = fp_string;
      mesh_state.rasterizationDepthClampEnable                  = 0;
      mesh_state.rasterizationCullMode                          = REII_CULL_MODE_NONE;
      mesh_state.rasterizationFrontFace                         = REII_FRONT_FACE_COUNTER_CLOCKWISE;
      mesh_state.rasterizationDepthBiasEnable                   = 0;
      mesh_state.rasterizationDepthBiasConstantFactor           = 0;
      mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
      mesh_state.multisampleEnable                              = 1;
      mesh_state.multisampleAlphaToCoverageEnable               = 0;
      mesh_state.multisampleAlphaToOneEnable                    = 0;
      mesh_state.depthTestEnable                                = 1;
      mesh_state.depthTestDepthWriteEnable                      = 1;
      mesh_state.depthTestDepthCompareOp                        = REII_COMPARE_OP_GREATER_OR_EQUAL;
      mesh_state.stencilTestEnable                              = 0;
      mesh_state.stencilTestFrontStencilTestFailOp              = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestFrontCompareOp                      = REII_COMPARE_OP_NEVER;
      mesh_state.stencilTestBackStencilTestFailOp               = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REII_STENCIL_OP_KEEP;
      mesh_state.stencilTestBackCompareOp                       = REII_COMPARE_OP_NEVER;
      mesh_state.stencilTestFrontAndBackCompareMask             = 0;
      mesh_state.stencilTestFrontAndBackWriteMask               = 0;
      mesh_state.stencilTestFrontAndBackReference               = 0;
      mesh_state.blendLogicOpEnable                             = 0;
      mesh_state.blendLogicOp                                   = REII_LOGIC_OP_CLEAR;
      mesh_state.blendConstants[0]                              = 0;
      mesh_state.blendConstants[1]                              = 0;
      mesh_state.blendConstants[2]                              = 0;
      mesh_state.blendConstants[3]                              = 0;
      mesh_state.outputColorWriteEnableR                        = 1;
      mesh_state.outputColorWriteEnableG                        = 1;
      mesh_state.outputColorWriteEnableB                        = 1;
      mesh_state.outputColorWriteEnableA                        = 1;
      mesh_state.outputColorBlendEnable                         = 0;
      mesh_state.outputColorBlendColorFactorSource              = REII_BLEND_FACTOR_ZERO;
      mesh_state.outputColorBlendColorFactorTarget              = REII_BLEND_FACTOR_ZERO;
      mesh_state.outputColorBlendColorOp                        = REII_BLEND_OP_ADD;
      mesh_state.outputColorBlendAlphaFactorSource              = REII_BLEND_FACTOR_ZERO;
      mesh_state.outputColorBlendAlphaFactorTarget              = REII_BLEND_FACTOR_ZERO;
      mesh_state.outputColorBlendAlphaOp                        = REII_BLEND_OP_ADD;
      reiiMeshStateCompile(ctx, &mesh_state);
    }

    static ReiiHandleTextureMemory outputDSTexMemory = {0};
    static ReiiHandleTexture houtputdstex = {0};
    static ReiiHandleTexture * outputdstex = &houtputdstex;
    if (once) {
      reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
      reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
      reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
      reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
      reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
      reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);
    }

    static ReiiHandleTextureMemory outputMSTexMemory = {0};
    static ReiiHandleTexture houtputmstex = {0};
    static ReiiHandleTexture * outputmstex = &houtputmstex;
    if (once) {
      reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA, (288/*mb*/ * 1024 * 1024), &outputMSTexMemory);
      reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
      reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
      reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
      reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
      reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);
    }

    static ReiiHandleUnorderedArray hmesh = {0};
    static ReiiHandleUnorderedArray * mesh = &hmesh;
    if (once) {
      float mesh_vertices[] = {
        #include "C:/Users/Constantine/Desktop/vkfast/extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
      };
      mesh->position = mesh_vertex_array;
      reiiCreateUnorderedArray(ctx, mesh);
      reiiUnorderedArraySet(ctx, mesh);
      for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
        reiiUnorderedArrayPosition(ctx, mesh, mesh_vertices[i * 3 + 0], mesh_vertices[i * 3 + 1], mesh_vertices[i * 3 + 2], 1);
      }
      reiiUnorderedArrayEnd(ctx, mesh, 1, &gpu_thread, array65536);
    }

    const int instanceCountX = 25;
    const int instanceCountY = 25;
    const int instanceCountZ = 25;
    RandomInit();

    static ReiiHandleUnorderedArray hinstanceColors = {0};
    static ReiiHandleUnorderedArray * instanceColors = &hinstanceColors;
    if (once) {
      instanceColors->texcoord[0] = instance_color_array;
      reiiCreateUnorderedArray(ctx, instanceColors);
      reiiUnorderedArraySet(ctx, instanceColors);
      for (int k = 0; k < instanceCountZ; k += 1) {
        for (int j = 0; j < instanceCountY; j += 1) {
          for (int i = 0; i < instanceCountX; i += 1) {
            reiiUnorderedArrayTexcoord(ctx, instanceColors, 0, RandomRange(0.15f, 0.45f), RandomRange(0.15f, 0.45f), RandomRange(0.25f, 1.f), 1);
          }
        }
      }
      reiiUnorderedArrayEnd(ctx, instanceColors, 1, &gpu_thread, array65536);
    }

    static ReiiHandleUnorderedArray hinstancePositions = {0};
    static ReiiHandleUnorderedArray * instancePositions = &hinstancePositions;
    if (once) {
      instancePositions->texcoord[0] = instance_position_array;
      reiiCreateUnorderedArray(ctx, instancePositions);
      reiiUnorderedArraySet(ctx, instancePositions);
      for (int k = 0; k < instanceCountZ; k += 1) {
        for (int j = 0; j < instanceCountY; j += 1) {
          for (int i = 0; i < instanceCountX; i += 1) {
            reiiUnorderedArrayTexcoord(ctx, instancePositions, 0, i * 3.f, j * 3.f, k * 3.f, RandomRange(0.05f, 0.45f));
          }
        }
      }
      reiiUnorderedArrayEnd(ctx, instancePositions, 1, &gpu_thread, array65536);
    }

    static ReiiHandleCommandList hlist = {0};
    static ReiiHandleCommandList * list = &hlist;
    static Red2Output mutable_outputs_array[3] = {0};
    if (once) {
      list->mutable_outputs_array.items    = mutable_outputs_array;
      list->mutable_outputs_array.capacity = countof(mutable_outputs_array);
    }

    static ReiiVec4   camera_pos  = {0, 0, 0};
    static ReiiVec4   camera_quat = {0, 0, 0, 1};
    const  ReiiBool32 camera_is_enabled = 0;
    if (once) {
      camera_pos.x = (instanceCountX * 3.f) / 2.f;
      camera_pos.y = (instanceCountY * 3.f) / 2.f;
      camera_pos.z = -45.f;
    }

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 2;
    bindings_info.max_storage_binds_count     = 3;
    bindings_info.max_texture_rw_binds_count  = 1;
    batches[vkcmdBuffer] = vfBatchBegin(ctx, batches[vkcmdBuffer], &bindings_info, NULL, FF, LL);
    uint64_t batch = batches[vkcmdBuffer];
    VkCommandBuffer cmdBuffer = (VkCommandBuffer)(void *)vfBatchGetRawHandle(ctx, batch, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);

    {
      VkClearValue clearValues[2]{};
      clearValues[0].color = { { 1.0f, 0.0f, 0.0f, 0.0f } };
      clearValues[1].depthStencil = { 1.0f, 0 };
      VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
      renderPassBeginInfo.renderPass = renderPass;
      renderPassBeginInfo.renderArea.extent.width = width;
      renderPassBeginInfo.renderArea.extent.height = height;
      renderPassBeginInfo.clearValueCount = 2;
      renderPassBeginInfo.pClearValues = clearValues;
      renderPassBeginInfo.framebuffer = frameBuffers[currentImageIndex];
      vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      vkCmdEndRenderPass(cmdBuffer);

      if (1) {
        // NOTE(Constantine)(08 May 2026): Stutter test.
        static float disabled_camera_ping_pong = 0.45f;
        if (camera_is_enabled == 0) {
          camera_pos.x += disabled_camera_ping_pong;
          if (camera_pos.x < -5.f || camera_pos.x > ((instanceCountX * 3.f) + 5.f)) {
            disabled_camera_ping_pong *= -1.f;
          }
        }
      }

      reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
      reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
      reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
      reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
      reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
      reiiCommandBindStorageRaw(ctx, list, 0, 1, &mesh->position.gpu);
      reiiCommandBindStorageRaw(ctx, list, 1, 1, &instanceColors->texcoord[0].gpu);
      reiiCommandBindStorageRaw(ctx, list, 2, 1, &instancePositions->texcoord[0].gpu);
      reiiCommandBindNewBindingsEnd(ctx, list);
      reiiCommandBindVariablesCopy(ctx, list, 0 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_pos);
      reiiCommandBindVariablesCopy(ctx, list, 1 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_quat);
      reiiCommandRenderTargetSet(ctx, list, outputdstex, outputmstex, outputmstex->texture);
      reiiCommandUnorderedArrayDrawInstanced(ctx, list, mesh, instanceCountX * instanceCountY * instanceCountZ);
      reiiCommandRenderTargetEnd(ctx, list);
      reiiCommandResolveMsaaColorTextureEx(ctx, list, outputmstex, (RedHandleTexture)presentImageView);

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

      vfBatchEnd(ctx, batch, FF, LL);

      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);
    }

    once = 0;
  }
}