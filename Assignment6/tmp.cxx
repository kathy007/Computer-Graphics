// generated by Fast Light User Interface Designer (fluid) version 1.0108

#include "tmp.h"

void UserInterface::cb_TextureFile_i(Fl_File_Input*, void*) {
  loadFile_cb();
}
void UserInterface::cb_TextureFile(Fl_File_Input* o, void* v) {
  ((UserInterface*)(o->parent()))->cb_TextureFile_i(o,v);
}
UserInterface::UserInterface(int X, int Y, int W, int H, const char *L)
  : Fl_Group(X, Y, W, H, L) {
{ TextureFile = new Fl_File_Input(25, 25, 285, 34, "texture file:");
  TextureFile->callback((Fl_Callback*)cb_TextureFile);
} // Fl_File_Input* TextureFile
end();
}
