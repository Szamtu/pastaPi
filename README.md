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

**Version 0.10.0**  
Separated libSpaghetti into: Editor, libSpaghetti and libSpaghettiUI. 
Now it is possible to build libSpaghetti and plugins without GUI.

**Changes:**  
*  Removed all gui stuff from libSpaghetti.
*  Added libSpaghettiUI that contains UI stuff like node, links, package_view, etc.
*  Moved editor specific stuff from libSpaghetti to editor.
*  Added non-ui build definitions for plugins.
*  Removed filesystem detection, now it uses only std::filesystem.
*  Changed system plugins path from lib to plugins
*  Removed build comit from editor.
*  Added changelog to editor. 

**Forked from:**  
**[Spaghetti](https://github.com/aljen/spaghetti)** - by Aljen

**License:**
MIT License
