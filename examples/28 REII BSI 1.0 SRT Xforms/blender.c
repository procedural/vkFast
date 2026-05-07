#include <stdint.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

struct int3   { int   x, y, z; };
struct float2 { float x, y; };
struct float4 { float x, y, z, w; };

struct int3 blender_mesh_indices[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/mesh_indices.h"
};
struct float4 blender_mesh_normals[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/mesh_normals.h"
};
struct float2 blender_mesh_uvs[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/mesh_uvs.h"
};
struct float4 blender_mesh_vertices[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/mesh_vertices.h"
};
uint64_t blender_submesh_tri_begin[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/submesh_tri_begin.h"
};
uint64_t blender_submesh_tri_end[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/submesh_tri_end.h"
};
struct float4 blender_submesh_xform_scale[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/submesh_xform_scale.h"
};
struct float4 blender_submesh_xform_rotation_quaternion[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/submesh_xform_rotation_quaternion.h"
};
struct float4 blender_submesh_xform_translation[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/submesh_xform_translation.h"
};
struct float4 blender_camera_xform_scale[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/camera_xform_scale.h"
};
struct float4 blender_camera_xform_rotation_quaternion[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/camera_xform_rotation_quaternion.h"
};
struct float4 blender_camera_xform_translation[] = {
  #include "../27 REII Blender Simple Importer 1.0/BlenderMeshHeaders/06_May_2026_21h_44m_54s/camera_xform_translation.h"
};

uint64_t countof_blender_mesh_indices = countof(blender_mesh_indices);
uint64_t countof_blender_mesh_normals = countof(blender_mesh_normals);
uint64_t countof_blender_mesh_uvs = countof(blender_mesh_uvs);
uint64_t countof_blender_mesh_vertices = countof(blender_mesh_vertices);
uint64_t countof_blender_submesh_tri_begin = countof(blender_submesh_tri_begin);
uint64_t countof_blender_submesh_tri_end = countof(blender_submesh_tri_end);
uint64_t countof_blender_submesh_xform_scale = countof(blender_submesh_xform_scale);
uint64_t countof_blender_submesh_xform_rotation_quaternion = countof(blender_submesh_xform_rotation_quaternion);
uint64_t countof_blender_submesh_xform_translation = countof(blender_submesh_xform_translation);
uint64_t countof_blender_camera_xform_scale = countof(blender_camera_xform_scale);
uint64_t countof_blender_camera_xform_rotation_quaternion = countof(blender_camera_xform_rotation_quaternion);
uint64_t countof_blender_camera_xform_translation = countof(blender_camera_xform_translation);
