#include "framebuffer.h"
#include "v3.h"
#include <iostream>
#include "scene.h"

FrameBuffer::FrameBuffer(int u0, int v0, 
  int _w, int _h) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

  w = _w;
  h = _h;
  pix = new unsigned int[w*h];

}


void FrameBuffer::draw() {

  glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}


void FrameBuffer::Set(unsigned int bgr) {

  for (int i = 0; i < w*h; i++) {
    pix[i] = bgr;
  }

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

  pix[(h-1-v)*w+u] = color;

}

unsigned int FrameBuffer::Get(int u, int v) {

  return pix[(h-1-v)*w+u];

}

void FrameBuffer::SetSafe(int u, int v, unsigned int color) {

  if (u < 0 || u > w-1 || v < 0 || v > h-1)
    return;

  Set(u, v, color);

}


void FrameBuffer::SetChecker(int csize, unsigned int color0, 
  unsigned int color1) {


  for (int v = 0; v < h; v++) {
    int cv = v / csize;
    for (int u = 0; u < w; u++) {
      int cu = u / csize;
      if ( (cu+cv) % 2)
        Set(u, v, color1);
      else
        Set(u, v, color0);
    }
  }

}


void FrameBuffer::DrawPoint(int u, int v, int psize, unsigned int color) {

  for (int i = v - psize/2; i <= v + psize/2; i++) {
    for (int j = u - psize/2; j <= u + psize/2; j++) {
      SetSafe(j, i, color);
    }
  }

}


void FrameBuffer::DrawSegment(float u0f, float v0f, float u1f, float v1f, 
  unsigned int color) {

    float du = fabsf(u1f-u0f);
    float dv = fabsf(v1f-v0f);
    int stepsN = (du < dv) ? 1 + (int) dv : 1 + (int) du;

    V3 startingPoint(u0f, v0f, 0.0f);
    V3 endingPoint(u1f, v1f, 0.0f);
    V3 currentPoint = startingPoint;
    int segsN;
    if (stepsN == 1)
      segsN = 1;
    else
      segsN = stepsN-1;
    V3 segmentStep = (endingPoint - startingPoint) / (float) segsN;
    for (int i = 0; i < stepsN; i++) {
      int u = (int) currentPoint[0];
      int v = (int) currentPoint[1];
      SetSafe(u, v, color);
      currentPoint = currentPoint + segmentStep;
    }

}

void FrameBuffer::Convolve33(M33 kernel, FrameBuffer *&fb1) {

  fb1 = new FrameBuffer(30 + w, 30, w, h);

  for (int v = 1; v < h-1; v++) {
    for (int u = 1; u < w-1; u++) {
      V3 newColor(0.0f, 0.0f, 0.0f);
      for (int vi = -1; vi <= 1; vi++) {
        for (int ui = -1; ui <= 1; ui++) {
          V3 currColor;
          currColor.SetFromColor(Get(u+ui, v+vi));
          newColor = newColor + currColor * kernel[vi+1][ui+1];
        }
      }
      unsigned int newc = newColor.GetColor();
      fb1->Set(u, v, newc);
    }
  }

}


void FrameBuffer::Draw3DPoint(V3 pt, PPC *ppc, int psize, V3 color) {

  V3 ppt;
  if (!ppc->Project(pt, ppt))
    return;
  DrawPoint((int)ppt[0], (int)ppt[1], psize, color.GetColor());
}


void FrameBuffer::Draw3DSegment(V3 p0, V3 p1, PPC *ppc, unsigned int color) {

  V3 pp0, pp1;
  if (!ppc->Project(p0, pp0))
    return;
  if (!ppc->Project(p1, pp1))
    return;
  DrawSegment(pp0[0], pp0[1], pp1[0], pp1[1], color);  

}

