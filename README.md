<p align="center">
  <img src="docs/pastapi_logo.png"/>
</p>

# Pasta Pi  
**Graphical multi-purpose editor / simulator**

![screen1.png](docs/screen1.png)

## Developed by:  
*  **[Szamtu](https://github.com/Szamtu)**
*  **[Gryze](https://github.com/Gryze)**

**from:**  
![escort_logo.png](docs/escort_logo.png)  
**[escort-technology.com](https://escort-technology.com/)**


**Last changes:**

Version 0.9.10
This is the first version of PastaPI - Graphical multi-purpose editor / simulator
We start here at version 0.9.10.

Changes (from oryginal Spaghetti):
New things:
*   Added support of new data types:
  *  String
  *  Matrix
  *  Point
  *  Shape
  *  Shape vector
*  Added new OpenCV elements:
  *  VideoIO - capture
  *  VideoIO - writer
  *  Video - MOG2
  *  UI - video display
  *  UI - draw contours
  *  Improc - color to gray
  *  Improc - crop image
  *  Improc - find contours
  *  Improc - median blur
  *  Improc - resize image
  *  Imcodecs - image write
*  Added new generic elements 
  *  convert (from bool/int/float/string to bool/int/float/string)
  *  concat strings
*  Moved elements from libSpaghetti to plugins
*  Added subcategories to elements panel
*  Added searchbar to elements panel
**BugFixes:**
*  Crash on delete input/output element of package - now it's non deletable
*  Crash on decrease number of inputs/outputs when they are connected - now they are automatically disconnected
*  Bad bool input color when touched with ON connection, now it's displays right state
*  Bad bool input color when disconnected ON connection,  now it's displays right state
**Removed:**
*  Some elements that we think that will be never used like: Pneumatic - tank

**Forked from:**  
**[Spaghetti](https://github.com/aljen/spaghetti)** - by Aljen

**License:**
MIT License
