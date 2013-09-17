AniSimProjekt
=====================

Animation & Simulation Projekt
Partikelsimulation und co. kg.

________________________________

How To Inschtall in Eclipse CDT mit OpenGL n stuff:

+ Was du brauchst: Eclipse mit CDT, Opengl und Glut DLL Dateien installiert, GitHub for Windows (oder anderes OS)(, und git)

+ in GitHub Programm das Default Storage Directory auf Eclipse Workspace Ordner setzen
+ Dateien von GitHub klonen ( Clone in Desktop, mit GitHub Programm starten)
+ in Eclipse C++ Projekt mit Namen AniSimProjekt erstellen (Warnung: "Ordner mit gleichem Namen bereits vorhanden" Ignorieren)
+ in Eclipse Projekteinstellungen -> C/C++ Build -> Settings -> Tool Settings -> GCC C++ Compiler -> Includes
  - hinzufügen -> Workspace... -> AniSimProjekt/BasisApplication und alle anderen Ordner einzeln hinzufügen
+ MinGW C++ Linker -> Libraries 
  - hinzufügen: opengl32
                glut32
                glu32
+ Rechtsklick auf Projekt -> Build Project ( main() befindet ich in EmptyGLUTWnd.cpp )

________________________________

Vor jedem mal arbeiten -> immer erst die aktuelle Dateien vom Server ziehen ( in GitHub Programm auf synchronize )
Nach jedem mal arbeiten -> Dateien am besten per GitHub committen UND synchronisieren

________________________________

am Rande bemerkt: Git arbeitet so, dass man zuerst Änderungen anmeldet (comitted) und dann erst anwendet (pusht)
Erst nach dem Anwenden, also Pushen oder Synchronisieren, sind die Dateien hochgeladen. Oder so.

________________________________

UTF-8 Zeichenkodierung in Eclipse-Projekteigenschaften einstellen
