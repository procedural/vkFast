// NOTE(Constantine):
// sudo dnf install glibc-devel.i686 libgcc.i686
// sudo dnf install mesa-libGL-devel.i686 mesa-libGLU-devel.i686 freeglut-devel.i686
// sudo dnf install glfw.i686 glfw-devel.i686
// gcc -m32 main.c "../REII Backport/reii.c" -I. -lGL -lGLU -lglut -lm -lglfw

// NOTE(Constantine): Depends on https://github.com/procedural/vkFast/tree/58b5e2679fbb914c348c031b7f6444aed902564a/extra/REII%20Backport

// NOTE(Constantine): Press and hold E key to move the arm.

#if defined(__linux__) && !defined(__ANDROID__)
#define __ptr32
#endif

// NOTE(Constantine): Microsoft people are all fired. https://github.com/ocornut/imgui/issues/2043
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#pragma comment(lib, "../../examples/Common/glfw-3.4.bin.WIN32/lib-vc2019/glfw3.lib")

#include "../REII Backport/reii.h"
#define GLFW_INCLUDE_NONE
#include "../../examples/Common/glfw-3.4.bin.WIN32/include/GLFW/glfw3.h"

#include <stdio.h>
#include <math.h>

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h> // For _CrtSetDbgFlag
#endif

// NOTE(Constantine): Skinning includes
#include <stdlib.h> // For malloc, free
#include <string.h> // For strcpy
#include "skinning_skeleton.h"

// NOTE(Constantine): Skinning globals
t_Bone g_Skeleton = {0};
float m_Grab_Rot_X;
float m_Grab_Rot_Y;
float m_Grab_Rot_Z;
float m_Grab_Trans_X;
float m_Grab_Trans_Y;
float m_Grab_Trans_Z;
int m_DrawGeometry = 1;
int m_DrawBoneSystem = 1;
int m_DrawOutline;
int m_Defmode_TwoBone;
float m_AnimBlend;
t_Bone *m_SelectedBone; // THE CURRENTLY PICKED BONE
t_Bone m_Mesh; // THE MESH TO DEFORM
tColoredVertex *m_DeformedMesh;

void skin_ResetBone(t_Bone *bone,t_Bone *parent)
{
  bone->p_scale.x =
  bone->p_scale.y =
  bone->p_scale.z = 1.0;
  bone->s_scale.x =
  bone->s_scale.y =
  bone->s_scale.z = 1.0;
  bone->scale.x =
  bone->scale.y =
  bone->scale.z = 1.0;

  bone->p_rot.x =
  bone->p_rot.y =
  bone->p_rot.z = 0.0;
  bone->s_rot.x =
  bone->s_rot.y =
  bone->s_rot.z = 0.0;
  bone->rot.x =
  bone->rot.y =
  bone->rot.z = 0.0;

  bone->p_trans.x =
  bone->p_trans.y =
  bone->p_trans.z = 0.0;
  bone->s_trans.x =
  bone->s_trans.y =
  bone->s_trans.z = 0.0;
  bone->trans.x =
  bone->trans.y =
  bone->trans.z = 0.0;

  bone->primChanType = CHANNEL_TYPE_NONE;
  bone->secChanType = CHANNEL_TYPE_NONE;
  bone->primFrameCount = 0;
  bone->secFrameCount = 0;
  bone->primCurFrame = 0;
  bone->secCurFrame = 0;
  bone->primChannel = NULL;
  bone->secChannel = NULL;
  bone->animBlend = 0.0;

  bone->visualCnt = 0;          // COUNT OF ATTACHED VISUAL ELEMENTS
  bone->visuals = NULL;          // POINTER TO VISUALS
  bone->childCnt = 0;            // COUNT OF ATTACHED BONE ELEMENTS
  bone->children = NULL;          // POINTER TO CHILDREN
  bone->parent = parent;
}

void skin_COGLViewLoadObjectFile(char *filename,t_Bone *bonePtr)
{
/// Local Variables ///////////////////////////////////////////////////////////
  FILE *fp = NULL;
  tObjDesc  *desc = NULL;      /* Pointer to Desc */
  tObjFrame  *frame = NULL;      /* Pointer to Desc */
  short loop2 = 0;
  char tempstr[80] = {0};
///////////////////////////////////////////////////////////////////////////////

  skin_ResetBone(bonePtr, &g_Skeleton);    // SETUP INITIAL BONE SETTINGS

  bonePtr->trans.x = 0;
  bonePtr->trans.y = 0;
  bonePtr->trans.z = 0;
  bonePtr->rot.x = 0.0;
  bonePtr->rot.y = 0.0;
  bonePtr->rot.z = 0.0;

  fp = fopen(filename,"rb");
  if (fp == NULL)
  {
    printf("ERROR: Can't open the DGF File. Exiting.");
    exit(1);
  }
  else
  {
    fread(tempstr,1,4,fp); // FDAT
    if (strncmp(tempstr,"DARW",4)!= 0)
    {
      printf("ERROR: Not a Valid DGF File. Exiting.");
      exit(1);
    }
    fread(tempstr,1,4,fp); // FDAT

    fread(bonePtr,sizeof(t_Bone),1,fp);
    fread(tempstr,1,4,fp); // FDAT
    bonePtr->desc = (tObjDesc *)malloc(sizeof(tObjDesc));
    desc = bonePtr->desc;
    fread(desc,sizeof(tObjDesc),1,fp);
    for (loop2 = 0; loop2 < desc->frameCnt; loop2++)
    {
      fread(tempstr,1,4,fp); // FDAT
      desc->frame[loop2] = (tObjFrame  *)malloc(sizeof(tObjFrame));
      frame = desc->frame[loop2];
      fread(frame,sizeof(tObjFrame),1,fp);
      frame->data = (float *)malloc(sizeof(float) * desc->dataSize * desc->pointCnt);
      fread(frame->data,sizeof(float),desc->dataSize * desc->pointCnt,fp);
    }
    bonePtr->cur_desc = 0;
    fclose(fp);
  }
}

int skin_COGLViewGetWeights(const char *filename)
{
  FILE *fp = fopen(filename,"rb");
  if (fp == NULL)
  {
    printf("ERROR: Can't open the Weights File. Exiting.");
    exit(1);
  }
  else
  {
    for (int loop = 0; loop < m_Mesh.desc->pointCnt; loop++)
    {
      fread(&g_Skeleton.children[0].CV_weight[loop],sizeof(float),1,fp);
      fread(&g_Skeleton.children[1].CV_weight[loop],sizeof(float),1,fp);
    }
    fclose(fp);
    return 1;
  }
  return 0;
}

#if defined(_WIN32)
#include <Windows.h>
#endif
#include <GL/gl.h>

#if defined(_WIN32)
  #pragma comment(lib, "glu32.lib")
  void __stdcall gluPerspective(
    GLdouble fovy,
    GLdouble aspect,
    GLdouble zNear,
    GLdouble zFar
  );
#else
  #include <GL/glu.h>
#endif

void MultVectorByMatrix(tMatrix *mat, tVector *v,tVector *result)
{
  result->x = (mat->m[0] * v->x) +
              (mat->m[4] * v->y) +
              (mat->m[8] * v->z) +
              mat->m[12];
  result->y = (mat->m[1] * v->x) +
              (mat->m[5] * v->y) +
              (mat->m[9] * v->z) +
              mat->m[13];
  result->z = (mat->m[2] * v->x) +
              (mat->m[6] * v->y) +
              (mat->m[10] * v->z) +
              mat->m[14];
}

GLvoid COGLViewDeformModel(t_Bone *rootBone)
{
/// Local Variables ///////////////////////////////////////////////////////////
  int loop;
  tVector pre,post1,post2;
  tColoredVertex *meshdata;  // IN THIS CASE THE DATA IS COLOR VERTICES
  tColoredVertex *defdata;  // IN THIS CASE THE DATA IS COLOR VERTICES
///////////////////////////////////////////////////////////////////////////////
  // GET A POINTER TO THE ACTUAL VERTEX DATA
  meshdata = (tColoredVertex *)m_Mesh.desc->frame[m_Mesh.desc->cur_frame]->data;
  defdata = m_DeformedMesh;
  for (loop = 0; loop < m_Mesh.desc->pointCnt; loop++)
  {
    pre.x = meshdata[loop].x - rootBone->children[0].trans.x;
    pre.y = meshdata[loop].y - rootBone->children[0].trans.y;
    pre.z = meshdata[loop].z - rootBone->children[0].trans.z;
    MultVectorByMatrix(&rootBone->children[0].matrix, &pre, &post1);
    pre.x = meshdata[loop].x - rootBone->children[1].trans.x;
    pre.y = meshdata[loop].y - rootBone->children[1].trans.y;
    pre.z = meshdata[loop].z - rootBone->children[1].trans.z;
    MultVectorByMatrix(&rootBone->children[1].matrix, &pre, &post2);
    defdata[loop].x = post1.x + ((post2.x - post1.x) * rootBone->children[1].CV_weight[loop]);
    defdata[loop].y = post1.y + ((post2.y - post1.y) * rootBone->children[1].CV_weight[loop]);
    defdata[loop].z = post1.z + ((post2.z - post1.z) * rootBone->children[1].CV_weight[loop]);
  }
}

GLvoid COGLViewDrawSkeleton(t_Bone *rootBone)
{
/// Local Variables ///////////////////////////////////////////////////////////
  int loop;
  t_Bone *curBone;
///////////////////////////////////////////////////////////////////////////////
  curBone = rootBone->children;
  for (loop = 0; loop < rootBone->childCnt; loop++)
  {
    glPushMatrix();

    // Set base orientation and position
    glTranslatef(curBone->trans.x, curBone->trans.y, curBone->trans.z);

    glRotatef(curBone->rot.z, 0.0f, 0.0f, 1.0f);
    glRotatef(curBone->rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(curBone->rot.x, 1.0f, 0.0f, 0.0f); 
  
    // THE SCALE IS LOCAL SO I PUSH AND POP
    glPushMatrix();
    glScalef(curBone->scale.x, curBone->scale.y, curBone->scale.z); 

    // DO I WISH TO DISPLAY ANYTHING
    if (m_DrawBoneSystem)
    {
      // DRAW THE AXIS OGL OBJECT
      #define OGL_AXIS_DLIST 1
      glCallList(OGL_AXIS_DLIST);
      // DRAW THE ACTUAL BONE STRUCTURE
      // ONLY MAKE A BONE IF THERE IS A CHILD
      if (curBone->childCnt > 0)
      {
        if (curBone == m_SelectedBone)
          glColor3f(1.0f, 1.0f, 0.0f);  // Selected bone is bright Yellow
        else
          glColor3f(0.4f, 0.4f, 0.0f);  // Selected bone is dull Yellow
        // DRAW THE BONE STRUCTURE
        glCallList(curBone->id);
      }
    }

    // GRAB THE MATRIX AT THIS POINT SO I CAN USE IT FOR THE DEFORMATION
    glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);

    glPopMatrix();  // THIS POP IS JUST FOR THE SCALE

    // CHECK IF THIS BONE HAS CHILDREN, IF SO RECURSIVE CALL
    if (curBone->childCnt > 0)
      COGLViewDrawSkeleton(curBone);

    glPopMatrix();  // THIS POPS THE WHOLE MATRIX

    curBone++;
  }
}

GLvoid COGLViewDrawModel(t_Bone *baseBone, tColoredVertex *deformedMesh)
{
/// Local Variables ///////////////////////////////////////////////////////////
  int loop;
///////////////////////////////////////////////////////////////////////////////

  if (deformedMesh != NULL)
  {
    // THIS MODEL HAS VERTEX COLORS
    glInterleavedArrays(GL_C3F_V3F,0,(GLvoid *)deformedMesh);
    glDrawArrays(GL_TRIANGLES,0,baseBone->desc->pointCnt);

    // NOW DRAW THE VERTEX MARKERS IF THEY ARE SELECTED
    glColor3f(1.0f, 0.0f, 0.0f);  // Selected Vertices are Red
    glBegin(GL_POINTS);
    for (loop = 0; loop < baseBone->desc->pointCnt; loop++)
    {
      glVertex3f(deformedMesh[loop].x,deformedMesh[loop].y,deformedMesh[loop].z);
    }
    glEnd();
  }
}

GLvoid COGLViewDrawScene()
{
/// Local Variables ///////////////////////////////////////////////////////////
  float m[16];
///////////////////////////////////////////////////////////////////////////////

  if (g_Skeleton.rot.y > 360.0f) g_Skeleton.rot.y -= 360.0f;
  if (g_Skeleton.rot.x > 360.0f) g_Skeleton.rot.x -= 360.0f;
  if (g_Skeleton.rot.z > 360.0f) g_Skeleton.rot.z -= 360.0f;
  
  glDisable(GL_DEPTH_TEST);  // TURN OFF DEPTH TEST FOR CLEAR

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);  // ENABLE DEPTH TESTING

  glPushMatrix();

  // Set root skeleton's orientation and position
  glTranslatef(g_Skeleton.trans.x, g_Skeleton.trans.y, g_Skeleton.trans.z);

  glRotatef(g_Skeleton.rot.z, 0.0f, 0.0f, 1.0f);
  glRotatef(g_Skeleton.rot.y, 0.0f, 1.0f, 0.0f);
  glRotatef(g_Skeleton.rot.x, 1.0f, 0.0f, 0.0f); 

  #if 0 // NOTE(Constantine): Disabling this draws model incorrectly, lol :D Because of: glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
  COGLViewDrawSkeleton(&g_Skeleton);
  #else
    t_Bone * rootBone = &g_Skeleton;
    t_Bone * curBone = rootBone->children;
    for (int loop = 0; loop < rootBone->childCnt; loop++) {
      glPushMatrix();
        // Set base orientation and position
        glTranslatef(curBone->trans.x, curBone->trans.y, curBone->trans.z);
        glRotatef(curBone->rot.z, 0.0f, 0.0f, 1.0f);
        glRotatef(curBone->rot.y, 0.0f, 1.0f, 0.0f);
        glRotatef(curBone->rot.x, 1.0f, 0.0f, 0.0f); 
        // THE SCALE IS LOCAL SO I PUSH AND POP
        glPushMatrix();
          glScalef(curBone->scale.x, curBone->scale.y, curBone->scale.z); 
          glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
        glPopMatrix(); // THIS POP IS JUST FOR THE SCALE

        // NOTE(Constantine): Here, it recurses on children, see: glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
        {
          // NOTE(Constantine): Hardcoded for 2 bones of our example:
          t_Bone * child = curBone->children;
          t_Bone * curBone = child; // NOTE(Constantine): Lower arm now.
          glPushMatrix();
            // Set base orientation and position
            glTranslatef(curBone->trans.x, curBone->trans.y, curBone->trans.z);
            glRotatef(curBone->rot.z, 0.0f, 0.0f, 1.0f);
            glRotatef(curBone->rot.y, 0.0f, 1.0f, 0.0f);
            glRotatef(curBone->rot.x, 1.0f, 0.0f, 0.0f); 
            // THE SCALE IS LOCAL SO I PUSH AND POP
            glPushMatrix();
              glScalef(curBone->scale.x, curBone->scale.y, curBone->scale.z); 
              glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
            glPopMatrix(); // THIS POP IS JUST FOR THE SCALE
          glPopMatrix(); // THIS POPS THE WHOLE MATRIX
        }

      glPopMatrix(); // THIS POPS THE WHOLE MATRIX
      curBone += 1;
    }
  #endif

  glPopMatrix();

  // GRAB THE MATRIX AT THIS POINT SO I CAN USE IT FOR THE DEFORMATION
  glGetFloatv(GL_MODELVIEW_MATRIX,m);

  #if 1
  if (m_DrawGeometry)
  {
    // TEST DEFORMATION FOR WEIGHTING BETWEEN TWO BONES ONLY
    COGLViewDeformModel(&g_Skeleton);

    COGLViewDrawModel(&m_Mesh,m_DeformedMesh);
  }
  #endif

  glFinish();
}

GLvoid COGLViewInitializeGL(GLsizei width, GLsizei height)
{
/// Local Variables ///////////////////////////////////////////////////////////
  GLfloat aspect;
///////////////////////////////////////////////////////////////////////////////

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  aspect = (GLfloat)width/(GLfloat)height;
  // Establish viewing volume
  gluPerspective(40.0, aspect,1, 2000);
  glMatrixMode(GL_MODELVIEW);

  // SET SOME OGL INITIAL STATES SO THEY ARE NOT DONE IN THE DRAW LOOP
  glPolygonMode(GL_FRONT,GL_LINE);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);
  glPointSize(4.0); // NICE BEEFY POINTS FOR THE VERTEX SELECTION
}

void COGLViewCreateBoneDLists(t_Bone *bone)
{
  // ONLY MAKE A BONE IF THERE IS A CHILD
  if (bone->childCnt > 0)
  {
    // CREATE THE DISPLAY LIST FOR A BONE
    glNewList(bone->id,GL_COMPILE);
      glBegin(GL_LINE_STRIP);
        glVertex3f( 0.0f,  0.4f, 0.0f);    // 0
        glVertex3f(-0.4f,  0.0f,-0.4f);    // 1
        glVertex3f( 0.4f,  0.0f,-0.4f);    // 2
        glVertex3f( 0.0f,  bone->children->trans.y, 0.0f);    // Base
        glVertex3f(-0.4f,  0.0f,-0.4f);    // 1
        glVertex3f(-0.4f,  0.0f, 0.4f);    // 4
        glVertex3f( 0.0f,  0.4f, 0.0f);    // 0
        glVertex3f( 0.4f,  0.0f,-0.4f);    // 2
        glVertex3f( 0.4f,  0.0f, 0.4f);    // 3
        glVertex3f( 0.0f,  0.4f, 0.0f);    // 0
        glVertex3f(-0.4f,  0.0f, 0.4f);    // 4
        glVertex3f( 0.0f,  bone->children->trans.y, 0.0f);    // Base
        glVertex3f( 0.4f,  0.0f, 0.4f);    // 3
        glVertex3f(-0.4f,  0.0f, 0.4f);    // 4
      glEnd();
    glEndList();
    // CHECK IF THIS BONE HAS CHILDREN, IF SO RECURSIVE CALL
    if (bone->childCnt > 0) {
      COGLViewCreateBoneDLists(bone->children);
    }
  }
}

void rotateMatrixByAxisAngle(float * matrix4x4, float x, float y, float z, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    float t = 1.0f - c;
    float len = sqrtf(x*x + y*y + z*z);
    
    // Normalize axis
    x /= len; y /= len; z /= len;

    float rot[16] = {
        t*x*x + c,   t*x*y - s*z, t*x*z + s*y, 0.0f,
        t*x*y + s*z, t*y*y + c,   t*y*z - s*x, 0.0f,
        t*x*z - s*y, t*y*z + s*x, t*z*z + c,   0.0f,
        0.0f,        0.0f,        0.0f,        1.0f
    };

    // Matrix multiplication: result = rot * matrix
    float temp[16];
    for (int i = 0; i < 4; i++) { // rows
        for (int j = 0; j < 4; j++) { // cols
            temp[i*4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                temp[i*4 + j] += rot[i*4 + k] * matrix4x4[k*4 + j];
            }
        }
    }
    memcpy(matrix4x4, temp, sizeof(float)*16);
}

int main() {
#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // NOTE(Constantine): Skinning init
  t_Bone * tempBones = NULL;
  {
    skin_ResetBone(&g_Skeleton, NULL);
    g_Skeleton.id = -1;
    strcpy(g_Skeleton.name, "Skeleton");
    g_Skeleton.p_trans.x = 2.0f;
    g_Skeleton.p_trans.y = 5.0f;
    g_Skeleton.p_trans.z = -20.0f;
    g_Skeleton.trans.x = 2.0f;
    g_Skeleton.trans.y = 5.0f;
    g_Skeleton.trans.z = -20.0f;

    // ALLOC ROOM FOR BONES
    tempBones = (t_Bone *)malloc(3 * sizeof(t_Bone));

    g_Skeleton.childCnt = 1;
    g_Skeleton.children = tempBones;

    skin_ResetBone(&tempBones[0], &g_Skeleton);    // SETUP INITIAL BONE SETTINGS
    skin_ResetBone(&tempBones[1], &g_Skeleton);    // SETUP INITIAL BONE SETTINGS
    skin_ResetBone(&tempBones[2], &g_Skeleton);    // SETUP INITIAL BONE SETTINGS

    // THIS IS THE BONE FOR THE UPPER ARM
    strcpy(tempBones[0].name, "UpperArm");
    tempBones[0].id = 100;      // GIVE IT A UNIQUE ID
    tempBones[0].childCnt = 1;
    tempBones[0].children = &tempBones[1];
    tempBones[0].trans.x = 0.0;
    tempBones[0].trans.y = 0.0;
    tempBones[0].rot.x = 0.0;
    tempBones[0].rot.y = -90.0;
    tempBones[0].rot.z = 0.0;

    // THIS IS THE BONE FOR THE LOWER ARM
    strcpy(tempBones[1].name, "LowerArm");
    tempBones[1].id = 101;      // GIVE IT A UNIQUE ID
    tempBones[1].trans.y = -4.0;
    tempBones[1].childCnt = 1;
    tempBones[1].children = &tempBones[2];

    // THIS IS THE BONE FOR THE UPPER ARM
    strcpy(tempBones[2].name, "Hand");
    tempBones[2].id = 102;      // GIVE IT A UNIQUE ID
    tempBones[2].trans.y = -5.0;
    tempBones[2].childCnt = 0;

    //g_Skeleton = NULL;
    m_SelectedBone = NULL;
    m_AnimBlend = 0.0;
    skin_COGLViewLoadObjectFile("Mesh.dgf", &m_Mesh);

    // SET UP THE WEIGHTS FOR THE BONES
    // UPPER ARM
    g_Skeleton.children[0].CV_weight = (float *)malloc(m_Mesh.desc->pointCnt * sizeof(float));
    // LOWER ARM
    g_Skeleton.children[1].CV_weight = (float *)malloc(m_Mesh.desc->pointCnt * sizeof(float));
    // HAND - JUST FOR COMPLETENESS THE INTERFACE DOESN'T SUPPORT
    // DEFORMATION OF THE HAND.  WEIGHTING DIALOG IS A PAIN IN THE
    // BUTT.  HOWEVER, IF YOU SET UP THE WEIGHTS IT WOULD WORK
    g_Skeleton.children[2].CV_weight = (float *)malloc(m_Mesh.desc->pointCnt * sizeof(float));
  
    // SET INITIAL WEIGHT VALUES
    for (int loop = 0; loop < m_Mesh.desc->pointCnt; loop++)
    {
      g_Skeleton.children[0].CV_weight[loop] = 1.0f;
      g_Skeleton.children[1].CV_weight[loop] = 0.0f;
      // HAND WEIGHTING IS ALWAYS ZERO
      g_Skeleton.children[2].CV_weight[loop] = 0.0f;
    }
  
    // PLACE TO STORE BASE MESH POINTS
    m_DeformedMesh = (tColoredVertex *)malloc(sizeof(tColoredVertex) * m_Mesh.desc->pointCnt);
    memcpy(m_DeformedMesh, m_Mesh.desc->frame[m_Mesh.desc->cur_frame]->data,sizeof(tColoredVertex) * m_Mesh.desc->pointCnt);

    // GET A DEFAULT WEIGHT SYSTEM
    skin_COGLViewGetWeights("arm.WGT");

    // CREATE THE DISPLAY LIST FOR AN AXIS WITH ARROWS POINTING IN
    // THE POSITIVE DIRECTION Red = X, Green = Y, Blue = Z
    glNewList(OGL_AXIS_DLIST, GL_COMPILE);
      glPushMatrix();
      glScalef(4.0, 4.0, 4.0);
      glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);  // X AXIS STARTS - COLOR RED
        glVertex3f(-0.2f,  0.0f, 0.0f);
        glVertex3f( 0.2f,  0.0f, 0.0f);
        glVertex3f( 0.2f,  0.0f, 0.0f);  // TOP PIECE OF ARROWHEAD
        glVertex3f( 0.15f,  0.04f, 0.0f);
        glVertex3f( 0.2f,  0.0f, 0.0f);  // BOTTOM PIECE OF ARROWHEAD
        glVertex3f( 0.15f, -0.04f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);  // Y AXIS STARTS - COLOR GREEN
        glVertex3f( 0.0f,  0.2f, 0.0f);
        glVertex3f( 0.0f, -0.2f, 0.0f);
        glVertex3f( 0.0f,  0.2f, 0.0f);  // TOP PIECE OF ARROWHEAD
        glVertex3f( 0.04f,  0.15f, 0.0f);
        glVertex3f( 0.0f,  0.2f, 0.0f);  // BOTTOM PIECE OF ARROWHEAD
        glVertex3f( -0.04f,  0.15f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);  // Z AXIS STARTS - COLOR BLUE
        glVertex3f( 0.0f,  0.0f,  0.2f);
        glVertex3f( 0.0f,  0.0f, -0.2f);
        glVertex3f( 0.0f,  0.0f, 0.2f);  // TOP PIECE OF ARROWHEAD
        glVertex3f( 0.0f,  0.04f, 0.15f);
        glVertex3f( 0.0f, 0.0f, 0.2f);  // BOTTOM PIECE OF ARROWHEAD
        glVertex3f( 0.0f, -0.04f, 0.15f);
      glEnd();
      glPopMatrix();
    glEndList();

    COGLViewCreateBoneDLists(g_Skeleton.children);
  }

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow * window = glfwCreateWindow(700, 700, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  ReiiContext   context = {0};
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

  ReiiMeshState mesh_state                                  = {0};
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

  #if 0
  ReiiHandleCommandList list = 0;
  reiiCreateCommandList(ctx, &list);
  reiiCommandListSet(ctx, list);
    reiiCommandSetViewport(ctx, list, 0, 0, 700, 700);
    reiiCommandSetScissor(ctx, list, 0, 0, 700, 700);
    reiiCommandClear(ctx, list, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f, 0.f, 0.05f, 1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, 0);
    reiiCommandMeshSet(ctx, list);
    //for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
    //  reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
    //  reiiCommandMeshPosition(ctx, list, mesh_vertices[i * 3 + 0], mesh_vertices[i * 3 + 1], mesh_vertices[i * 3 + 2], 1);
    //}
    reiiCommandMeshEnd(ctx, list);
  reiiCommandListEnd(ctx, list);
  #endif

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

  COGLViewInitializeGL(700, 700);
  #if 0 // FOR EXAMPLE #3
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  float aspect = (GLfloat)700/(GLfloat)700;
  // Establish viewing volume
  gluPerspective(60.0, aspect,1, 1000);
  glMatrixMode(GL_MODELVIEW);
  #endif

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

    pos_x += move_vec_x * 0.025;
    pos_y += move_vec_y * 0.025;
    pos_z += move_vec_z * 0.025;

    //glDisable(GL_DEPTH_TEST);
    //glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    //reiiSetProgramEnvironmentValueVertex(ctx, 0, pos_x, pos_y, pos_z, 0);
    //reiiSetProgramEnvironmentValueVertex(ctx, 1, cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y));
    //reiiSubmitCommandLists(ctx, 1, &list);

    #if 0 // This code works:
    glColor3f(1.0f, 0.0f, 0.0f);  // Selected Vertices are Red
    glBegin(GL_POINTS);
    for (int loop = 0; loop < m_Mesh.desc->pointCnt; loop++)
    {
      glVertex3f(m_DeformedMesh[loop].x,m_DeformedMesh[loop].y,m_DeformedMesh[loop].z);
    }
    glEnd();
    #endif

    #if 0 // This code works when `COGLViewDeformModel` is commented out:
    //COGLViewDeformModel(&g_Skeleton);
    glInterleavedArrays(GL_C3F_V3F,0,(GLvoid *)m_DeformedMesh);
    glDrawArrays(GL_TRIANGLES,0,m_Mesh.desc->pointCnt);
    #endif

    #if 0 // EXAMPLE #3
    glPushMatrix();
    glTranslatef(0, 0, -20);
    glRotatef(g_Skeleton.rot.z, 0.0f, 0.0f, 1.0f);
    glRotatef(g_Skeleton.rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(g_Skeleton.rot.x, 1.0f, 0.0f, 0.0f); 
    glInterleavedArrays(GL_C3F_V3F,0,(GLvoid *)m_DeformedMesh);
    glDrawArrays(GL_TRIANGLES,0,m_Mesh.desc->pointCnt);
    glPopMatrix();
    #endif

		//m_SelectedBone = &g_Skeleton.children[0];
		m_SelectedBone = &g_Skeleton.children[1];
		//m_SelectedBone = &g_Skeleton.children[2];
    m_SelectedBone->rot.x += mouse_disp_y * 200.f * glfwGetKey(window, GLFW_KEY_E);
    m_SelectedBone->rot.y -= mouse_disp_x * 200.f * glfwGetKey(window, GLFW_KEY_E);
    
    #if 0
    COGLViewDrawScene();
    #endif

    #if 0
    if (g_Skeleton.rot.y > 360.0f) g_Skeleton.rot.y -= 360.0f;
    if (g_Skeleton.rot.x > 360.0f) g_Skeleton.rot.x -= 360.0f;
    if (g_Skeleton.rot.z > 360.0f) g_Skeleton.rot.z -= 360.0f;
    glDisable(GL_DEPTH_TEST);  // TURN OFF DEPTH TEST FOR CLEAR
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);  // ENABLE DEPTH TESTING
    {
      t_Bone * rootBone = &g_Skeleton;
      t_Bone * curBone = rootBone->children;
      for (int loop = 0; loop < rootBone->childCnt; loop++) {
        glPushMatrix();
          // Set base orientation and position
          glTranslatef(curBone->trans.x, curBone->trans.y, curBone->trans.z);
          glRotatef(curBone->rot.z, 0.0f, 0.0f, 1.0f);
          glRotatef(curBone->rot.y, 0.0f, 1.0f, 0.0f);
          glRotatef(curBone->rot.x, 1.0f, 0.0f, 0.0f); 
          // THE SCALE IS LOCAL SO I PUSH AND POP
          glPushMatrix();
            glScalef(curBone->scale.x, curBone->scale.y, curBone->scale.z); 
            glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
          glPopMatrix(); // THIS POP IS JUST FOR THE SCALE

          // NOTE(Constantine): Here, it recurses on children, see: glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
          {
            // NOTE(Constantine): Hardcoded for 2 bones of our example:
            t_Bone * child = curBone->children;
            t_Bone * curBone = child; // NOTE(Constantine): Lower arm now.
            glPushMatrix();
              // Set base orientation and position
              glTranslatef(curBone->trans.x, curBone->trans.y, curBone->trans.z);
              glRotatef(curBone->rot.z, 0.0f, 0.0f, 1.0f);
              glRotatef(curBone->rot.y, 0.0f, 1.0f, 0.0f);
              glRotatef(curBone->rot.x, 1.0f, 0.0f, 0.0f); 
              // THE SCALE IS LOCAL SO I PUSH AND POP
              glPushMatrix();
                glScalef(curBone->scale.x, curBone->scale.y, curBone->scale.z); 
                glGetFloatv(GL_MODELVIEW_MATRIX,curBone->matrix.m);
              glPopMatrix(); // THIS POP IS JUST FOR THE SCALE
            glPopMatrix(); // THIS POPS THE WHOLE MATRIX
          }

        glPopMatrix(); // THIS POPS THE WHOLE MATRIX
        curBone += 1;
      }
    }
    COGLViewDeformModel(&g_Skeleton);
    glPushMatrix();
      glTranslatef(0, 5, -30);
      glInterleavedArrays(GL_C3F_V3F,0,(GLvoid *)m_DeformedMesh);
      glDrawArrays(GL_TRIANGLES,0,m_Mesh.desc->pointCnt);
    glPopMatrix();
    #endif

    #if 1
    glDisable(GL_DEPTH_TEST);  // TURN OFF DEPTH TEST FOR CLEAR
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);  // ENABLE DEPTH TESTING
    static int once = 0;
    if (once == 0) {
      once = 1;

      float * bone = &g_Skeleton.children[0].matrix.m[0];
      bone[0] = 0;
      bone[1] = 0;
      bone[2] = 1;
      bone[3] = 0;
      bone[4] = 0;
      bone[5] = 1;
      bone[6] = 0;
      bone[7] = 0;
      bone[8] = -1;
      bone[9] = 0;
      bone[10] = 0;
      bone[11] = 0;
      bone[12] = 0;
      bone[13] = 0;
      bone[14] = 0;
      bone[15] = 1;

      bone = &g_Skeleton.children[0].children[0].matrix.m[0];
      bone[0] = 0;
      bone[1] = 0;
      bone[2] = 1;
      bone[3] = 0;
      bone[4] = 0;
      bone[5] = 1;
      bone[6] = 0;
      bone[7] = 0;
      bone[8] = -1;
      bone[9] = 0;
      bone[10] = 0;
      bone[11] = 0;
      bone[12] = 0;
      bone[13] = -4;
      bone[14] = 0;
      bone[15] = 1;
    }
    rotateMatrixByAxisAngle(&g_Skeleton.children[0].children[0].matrix.m[0],-1,0,0, mouse_disp_y * 7.f * glfwGetKey(window, GLFW_KEY_E));
    rotateMatrixByAxisAngle(&g_Skeleton.children[0].children[0].matrix.m[0], 0,1,0, mouse_disp_x * 7.f * glfwGetKey(window, GLFW_KEY_E));
    COGLViewDeformModel(&g_Skeleton);
    glPushMatrix();
      glTranslatef(0, 5, -30); // NOTE(Constantine): Camera pos
      glInterleavedArrays(GL_C3F_V3F,0,(GLvoid *)m_DeformedMesh);
      glDrawArrays(GL_TRIANGLES,0,m_Mesh.desc->pointCnt);
    glPopMatrix();
    #endif

    glfwSwapBuffers(window);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
  }

  // Skinning deinit
  {
    free(tempBones);
    tempBones = NULL;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
