#if 0
gcc -shared -fPIC -o bypass_free.so bypass_free.c -ldl
gcc -c ../../extra/REII\ Backport/reii.c
g++ -O2 -march=native main.cpp *.o /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so -lm
DRI_PRIME=0 XDG_SESSION_TYPE=x11 LD_PRELOAD=./bypass_free.so ./a.out
exit
#endif
// GALLIUM_HUD=help ./a.out
// GALLIUM_HUD="fps;primitives-generated+samples-passed" ./a.out

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_X11
#include "/home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/include/GLFW/glfw3.h"
#include "/home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/include/GLFW/glfw3native.h"

#include <math.h>

#include "../../extra/REII Backport/reii.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../../extra/REDGPU RTE/examples/Common/tiny_obj_loader.h"

#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow * window = glfwCreateWindow(700, 700, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  ReiiContext   context = {};
  ReiiContext * ctx     = &context;
  reiiCreateContext((ReiiTypeProcedureGetProcAddress)glfwGetProcAddress, &context);

  char vp_string[] =
  "!!ARBvp1.0"
  "TEMP pos;"
  "TEMP R0;"
  "ADD pos, vertex.position, -program.env[0];"
  "MUL R0.xyzw, pos.xzzx, program.env[1].xyxy;"
  "ADD pos.x, R0.x,-R0.y;"
  "ADD pos.z, R0.z, R0.w;"
  "MUL R0.xyzw, pos.yzzy, program.env[1].zwzw;"
  "ADD pos.y, R0.x,-R0.y;"
  "ADD pos.z, R0.z, R0.w;"
  "MOV result.color, vertex.color;"
  "MOV result.position.xyw, pos.xyzz;"
  "MOV result.position.z, 0.1;"
  "END";

  char fp_string[] =
  "!!ARBfp1.0"
  "MOV result.color, fragment.color;"
  "END";

  ReiiMeshState mesh_state                                  = {};
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

  float suzanne_head_mesh_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  ReiiHandleCommandList list = 0;
  reiiCreateCommandList(ctx, &list);
  reiiCommandListSet(ctx, list);
  reiiCommandSetViewport(ctx, list, 0, 0, 700, 700);
  reiiCommandSetScissor(ctx, list, 0, 0, 700, 700);
  reiiCommandClear(ctx, list, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f, 0.f, 0.05f, 1.f);
  reiiCommandMeshSetState(ctx, list, &mesh_state, 0);
  reiiCommandMeshSet(ctx, list);
  for (int i = 0, mesh_vertices_count = countof(suzanne_head_mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
    reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
    const float scale = 0.5f;
    reiiCommandMeshPosition(ctx, list,
      suzanne_head_mesh_vertices[i * 3 + 0] * scale,
      suzanne_head_mesh_vertices[i * 3 + 1] * scale,
      suzanne_head_mesh_vertices[i * 3 + 2] * scale,
      1
    );
  }
  reiiCommandMeshEnd(ctx, list);
  reiiCommandListEnd(ctx, list);

  printf("Loading lucy.obj (tested on 2.6 GB, 28 million triangles file), please wait... If you compiled main.cpp with -O2 -march=native compile flags, it should take around 10 seconds on i3 12100 CPU...\n");
  // Load lucy.obj file, originally from https://graphics.stanford.edu/data/3Dscanrep/ lucy.tar.gz, converted to a 2.6 GB lucy.obj file with Blender 5.2 LTS.
  tinyobj::ObjReader objReader = tinyobj::ObjReader();
  objReader.ParseFromFile("lucy.obj");
  assert(objReader.Valid() == true);
  const std::vector<tinyobj::real_t>    objVertices = objReader.GetAttrib().GetVertices();
  const std::vector<tinyobj::shape_t> & objShapes   = objReader.GetShapes();
  assert(objShapes.size() >= 1);
  const tinyobj::shape_t & objShape = objShapes[0];
  std::vector<uint32_t> objIndices = std::vector<uint32_t>();
  objIndices.reserve(objShape.mesh.indices.size());
  for (const tinyobj::index_t & index : objShape.mesh.indices) {
    objIndices.push_back(index.vertex_index);
  }
  printf("lucy.obj is finished loading successfully.\n");

  printf("Loading lucy.obj to GL display lists, please wait...\n");
  ReiiHandleCommandList lucy_list = 0;
  reiiCreateCommandList(ctx, &lucy_list);
  reiiCommandListSet(ctx, lucy_list);
  reiiCommandMeshSetState(ctx, lucy_list, &mesh_state, 0);
  reiiCommandMeshSet(ctx, lucy_list);
  for (int iTri = 0; iTri < 28055728; iTri += 1) {
    unsigned i0 = objIndices[iTri * 3 + 0];
    unsigned i1 = objIndices[iTri * 3 + 1];
    unsigned i2 = objIndices[iTri * 3 + 2];

    reiiCommandMeshColor(ctx, lucy_list, (iTri % 2904) * 0.00025f, 0, 0.1f, 1);
    reiiCommandMeshPosition(ctx, lucy_list,
      objVertices[i0 * 3 + 0],
      objVertices[i0 * 3 + 1],
      objVertices[i0 * 3 + 2],
      1
    );

    reiiCommandMeshColor(ctx, lucy_list, (iTri % 2904) * 0.00025f, 0, 0.1f, 1);
    reiiCommandMeshPosition(ctx, lucy_list,
      objVertices[i1 * 3 + 0],
      objVertices[i1 * 3 + 1],
      objVertices[i1 * 3 + 2],
      1
    );

    reiiCommandMeshColor(ctx, lucy_list, (iTri % 2904) * 0.00025f, 0, 0.1f, 1);
    reiiCommandMeshPosition(ctx, lucy_list,
      objVertices[i2 * 3 + 0],
      objVertices[i2 * 3 + 1],
      objVertices[i2 * 3 + 2],
      1
    );
  }
  reiiCommandMeshEnd(ctx, lucy_list);
  reiiCommandListEnd(ctx, lucy_list);
  printf("lucy.obj is finished loading to GL display lists successfully.\n");

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float pos_x = 0;
  float pos_y = 0;
  float pos_z = -2.f;
  float rot_x = 0;
  float rot_y = 0;

  glfwPollEvents();

  double start_mouse_x = 0;
  double start_mouse_y = 0;
  glfwGetCursorPos(window, &start_mouse_x, &start_mouse_y);
  double mouse_x_prev = start_mouse_x;
  double mouse_y_prev = start_mouse_y;

  for (; glfwWindowShouldClose(window) == 0;) {
    glfwPollEvents();

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    float mouse_disp_x = (float)(mouse_x_prev - mouse_x) * 0.0035f;
    float mouse_disp_y = (float)(mouse_y - mouse_y_prev) * 0.0035f;

    rot_x += mouse_disp_x;
    rot_y += mouse_disp_y;

    float move_vec_x = 0;
    float move_vec_y = 0;
    float move_vec_z = 0;

    move_vec_x += glfwGetKey(window, GLFW_KEY_D);
    move_vec_x -= glfwGetKey(window, GLFW_KEY_A);

    move_vec_y += glfwGetKey(window, GLFW_KEY_E);
    move_vec_y -= glfwGetKey(window, GLFW_KEY_Q);

    move_vec_z += glfwGetKey(window, GLFW_KEY_W);
    move_vec_z -= glfwGetKey(window, GLFW_KEY_S);

    float move_vec_len = sqrtf(move_vec_x*move_vec_x + move_vec_y*move_vec_y + move_vec_z*move_vec_z);
    if (move_vec_len != 0) {
      move_vec_x /= move_vec_len;
      move_vec_y /= move_vec_len;
      move_vec_z /= move_vec_len;
    }

    {
      float rot_cos = cosf(rot_y);
      float rot_sin = sinf(rot_y);
      float pos_y = move_vec_y;
      float pos_z = move_vec_z;
      move_vec_y = pos_y * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_y * rot_sin;
    }

    {
      float rot_cos = cosf(rot_x);
      float rot_sin = sinf(rot_x);
      float pos_x = move_vec_x;
      float pos_z = move_vec_z;
      move_vec_x = pos_x * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_x * rot_sin;
    }

    pos_x += move_vec_x * 0.1;
    pos_y += move_vec_y * 0.1;
    pos_z += move_vec_z * 0.1;

    reiiSetProgramEnvironmentValueVertex(ctx, 0, pos_x, pos_y, pos_z, 0);
    reiiSetProgramEnvironmentValueVertex(ctx, 1, cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y));
    reiiSubmitCommandLists(ctx, 1, &list);
    reiiSubmitCommandLists(ctx, 1, &lucy_list);

    glfwSwapBuffers(window);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
