#include "tmesh.h"

#include <fstream>
#include <iostream>

TMesh::TMesh(V3 center, V3 dims, unsigned int color) {

  enabled = true;
  vertsN = 8;
  verts = new V3[vertsN];
  cols = new V3[vertsN];
  normals = 0;

  int vi = 0;
  verts[vi++] = center + V3(-dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(-dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, -dims[1]/2.0f, +dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, +dims[1]/2.0f, +dims[2]/2.0f);

  verts[vi++] = center + V3(-dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(-dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, -dims[1]/2.0f, -dims[2]/2.0f);
  verts[vi++] = center + V3(+dims[0]/2.0f, +dims[1]/2.0f, -dims[2]/2.0f);

  for (int vi = 0; vi < vertsN; vi++) {
    cols[vi].SetFromColor(color);
  }

  trisN = 12;
  tris = new unsigned int[trisN * 3];
  int tri = 0;

  tris[3*tri+0] = 0;
  tris[3*tri+1] = 1;
  tris[3*tri+2] = 2;
  tri++;
  tris[3*tri+0] = 2;
  tris[3*tri+1] = 3;
  tris[3*tri+2] = 0;
  tri++;
  tris[3*tri+0] = 3;
  tris[3*tri+1] = 2;
  tris[3*tri+2] = 6;
  tri++;
  tris[3*tri+0] = 6;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 3;
  tri++;
  tris[3*tri+0] = 4;
  tris[3*tri+1] = 0;
  tris[3*tri+2] = 3;
  tri++;
  tris[3*tri+0] = 3;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 4;
  tri++;

  tris[3*tri+0] = 5;
  tris[3*tri+1] = 6;
  tris[3*tri+2] = 2;
  tri++;
  tris[3*tri+0] = 2;
  tris[3*tri+1] = 1;
  tris[3*tri+2] = 5;
  tri++;

  tris[3*tri+0] = 4;
  tris[3*tri+1] = 7;
  tris[3*tri+2] = 6;
  tri++;
  tris[3*tri+0] = 6;
  tris[3*tri+1] = 5;
  tris[3*tri+2] = 4;
  tri++;

  tris[3*tri+0] = 4;
  tris[3*tri+1] = 5;
  tris[3*tri+2] = 1;
  tri++;
  tris[3*tri+0] = 1;
  tris[3*tri+1] = 0;
  tris[3*tri+2] = 4;
  tri++;

}

// constructor: load from file
TMesh::TMesh(char *fileName) {
	ifstream ifs(fileName, ios::binary);
	if (ifs.fail()) {
		cerr << "Cannot Open File" << endl;
		return;;
	}
	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new V3[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (cols)
		delete cols;
	cols = 0;
	if (yn == 'y') {
		cols = new V3[vertsN];
	}
	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new V3[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float *tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN * 2];
	}

	ifs.read((char*)verts, vertsN * 3 * sizeof(float)); // load verts

	if (cols) {
		ifs.read((char*)cols, vertsN * 3 * sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int));
	if (tris)
		delete tris;
	tris = new unsigned int[trisN * 3];
	ifs.read((char*)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fileName << endl;
	cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

	SetAABB();
}


void TMesh::RenderPoints(PPC *ppc, FrameBuffer *fb, int psize) {

  for (int vi = 0; vi < vertsN; vi++) {
    fb->Draw3DPoint(verts[vi], ppc, psize, cols[vi]);
  }

}

void TMesh::RenderWireframe(PPC *ppc, FrameBuffer *fb, unsigned int color) {


  for (int tri = 0; tri < trisN; tri++) {
    int vinds[3];
    vinds[0] = tris[tri*3+0];
    vinds[1] = tris[tri*3+1];
    vinds[2] = tris[tri*3+2];
    for (int vi = 0; vi < 3; vi++) {
      fb->Draw3DSegment(verts[vinds[vi]], verts[vinds[(vi+1)%3]], ppc,
        color);
    }
  }
}

void TMesh::LoadBin(char *fname) {

  ifstream ifs(fname, ios::binary);
  if (ifs.fail()) {
    cerr << "INFO: cannot open file: " << fname << endl;
    return;
  }

  ifs.read((char*)&vertsN, sizeof(int));
  char yn;
  ifs.read(&yn, 1); // always xyz
  if (yn != 'y') {
    cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
    return;
  }
  if (verts)
    delete verts;
  verts = new V3[vertsN];

  ifs.read(&yn, 1); // cols 3 floats
  if (cols)
    delete cols;
  cols = 0;
  if (yn == 'y') {
    cols = new V3[vertsN];
  }
  ifs.read(&yn, 1); // normals 3 floats
  if (normals)
    delete normals;
  normals = 0;
  if (yn == 'y') {
    normals = new V3[vertsN];
  }

  ifs.read(&yn, 1); // texture coordinates 2 floats
  float *tcs = 0; // don't have texture coordinates for now
  if (tcs)
    delete tcs;
  tcs = 0;
  if (yn == 'y') {
    tcs = new float[vertsN*2];
  }

  ifs.read((char*)verts, vertsN*3*sizeof(float)); // load verts

  if (cols) {
    ifs.read((char*)cols, vertsN*3*sizeof(float)); // load cols
  }

  if (normals)
    ifs.read((char*)normals, vertsN*3*sizeof(float)); // load normals

  if (tcs)
    ifs.read((char*)tcs, vertsN*2*sizeof(float)); // load texture coordinates

  ifs.read((char*)&trisN, sizeof(int));
  if (tris)
    delete tris;
  tris = new unsigned int[trisN*3];
  ifs.read((char*)tris, trisN*3*sizeof(unsigned int)); // read tiangles

  ifs.close();

  cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
  cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

  SetAABB();

  enabled = true;

}

void TMesh::SetAABB() {

  if (aabb) {
    delete aabb;
  }
  
  aabb = new AABB(verts[0]);

  for (int vi = 0; vi < vertsN; vi++)
    aabb->AddPoint(verts[vi]);

  cerr << "TMesh AABB: " << aabb->corners[0] << endl << aabb->corners[1] << endl;

}

void TMesh::Translate(V3 tv) {

  for (int vi = 0; vi < vertsN; vi++)
    verts[vi] = verts[vi] + tv;
  SetAABB();
}

V3 TMesh::GetCenter() {

  V3 ret = (aabb->corners[0] + aabb->corners[1])/2.0f;
  return ret;

}

void TMesh::Position(V3 newCenter) {

  V3 oldCenter = GetCenter();
  Translate(newCenter-oldCenter);
  SetAABB();

}

void TMesh::ScaleToNewDiagonal(float newDiagonal) {

  float oldDiagonal = (aabb->corners[1] - aabb->corners[0]).Length();
  float sf = newDiagonal / oldDiagonal;
  float Length();
  V3 oldCenter = GetCenter();
  Position(V3(0.0f, 0.0f, 0.0f));
  Scale(sf);
  Position(oldCenter);
  SetAABB();

}

void TMesh::Scale(float scf) {

  for (int vi = 0; vi < vertsN; vi++) {
    verts[vi] = verts[vi] * scf;
  }

  SetAABB();

}

void TMesh::Rotate(V3 aO, V3 aD, float thetad){
	for (int vi = 0; vi < vertsN; vi++){
		verts[vi] = verts[vi].RotatePointAboutArbitraryAxis(aO, aD, thetad);
	}
}
