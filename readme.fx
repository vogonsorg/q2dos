Setup:
* Pick the correct glide driver (glide3x.dxe) for your 3dfx card from
  one of the directories listed below and put it in the same place as
  q2.exe and gl.dxe:
  * sst1  -> Voodoo Graphics
  * sst96 -> Voodoo Rush
  * cvg   -> Voodoo2
  * h5    -> Banshee, Voodoo3, Voodoo4 and Voodoo5
* It is recommended to use RayeR's MTRRLFBE.EXE utility
  (http://rayer.g6.cz/programm/mtrrlfbe.exe) to setup a
  Write-combining MTRR for the LFB.  This will dramatically increase
  performance on Voodoo Rush and higher cards.

Recommended System Specs:
* Pentium 2 400mhz.  Pentium 3 550mhz or higher for the best experience.
* Voodoo 3.  Older generations will work, but unknown how well they perform
  in faster computers.

Interesting parameters:
* -bpp for setting 15, 16, or 32bpp on Voodoo 4 or 5 cards.
* FX_GLIDE_NO_SPLASH=1 to disable the splash logo on Voodoo 1 and 2 cards
  on startup.
* FX_GLIDE_SWAPPENDINGCOUNT to set the number of frames to buffer.  Default
  value is 2, but can go as high as 6.  Traditionally, in Windows, higher
  values can be a slight speed advantage but can introduce input lag.  On
  the hardware I tested it on I was able to gain a few extra frames in
  timedemos (about 3-4) with 6 with no input lag and I am even able to
  use m_filter in combination with this.
* SSTH3_SLI_AA_CONFIGURATION to control SLI on Voodoo 5 5500 as well as
  Anti-aliasing on these cards.

Known Issues:
* Mesa is slower than MiniGL drivers such as the WickedGL driver or the
  OpenGL ICD from 3dfx.  These DLLs rely on special Quake engine hacks
  to speed up rendering.  They are closed source so we do not know what
  tricks were used.  However, Mesa in DOS is comparable in speeds to
  Mesa DLLs for Windows.
* Outdoor scenes are slower compared to Windows OpenGL ICD.  This issue
  also exists in Mesa.  You can verify by downloading the Windows Mesa DLLs
  from http://falconfly.de/ .
* gl_ext_multitexture is supported by the driver but appears to have an
  approximately 10fps loss on average on everything I tested it on.  It
  defaults to 0 by default.  If you would like to try it anyways set it to 1
  and do a vid_restart.
* gl_ext_pointparameters can lock some machines when particles are drawn.
  If you start a map and try to fire your gun and the game locks up,
  this is why.  It is disabled by default in Q2DOS.  But if you are using
  a custom config file imported from another OS it may secretly have this
  enabled.