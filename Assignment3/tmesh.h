#pragma once

#include "v3.h"
#include "framebuffer.h"
#include "ppc.h"

class TMesh {
public:
  bool enabled;
  V3 *verts, *normals, *cols;
  int vertsN;
  unsigned int *tris;
  int trisN;
  AABB *aabb;

  TMesh() : verts(0), vertsN(0), tris(0), trisN(0), 
    normals(0), cols(0), aabb(0), enabled(false) {};
  TMesh(V3 center, V3 dims, unsigned int color);
  TMesh(char *fileName);
  void LoadBin(char *fname);
  void RenderPoints(PPC *ppc, FrameBuffer *fb, int psize);
  void RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color);
  void SetAABB();
  void Translate(V3 tv);
  V3 GetCenter();
  void Position(V3 newCenter);
  void ScaleToNewDiagonal(float newDiagonal);
  void Scale(float scf);
  void Rotate(V3 aO, V3 aD, float thetad);
};