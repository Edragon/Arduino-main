How to Use with Arduino?
======================================================================
1)install the libraries under floder "Install libraries" we provided.
  NOTE:The UTFT library provided by this program is a modified version, so be sure to back up your old library and overwrite use this new library.
2)How to USE with UTFT:
#include <UTFT.h>

if your Driver IC is ILI9486,try:
UTFT myGLCD(ILI9486,38,39,40,41)
or
UTFT myGLCD(ILI9486_8,38,39,40,41)

if your Driver IC is ILI9488,try:
UTFT myGLCD(ILI9488,38,39,40,41)
or
UTFT myGLCD(ILI9488_8,38,39,40,41)