Setup:
* Pick the correct glide driver (glide3x.dxe) for your 3dfx card from
  one of the directories listed below and put it in the same place as
  q2.exe and gl.dxe:
  - sst1  -> Voodoo Graphics
  - sst96 -> Voodoo Rush
  - cvg   -> Voodoo2
  - h5    -> Banshee and Voodoo 3/4/5
* It is recommended to use RayeR's MTRRLFBE.EXE utility
  (http://rayer.g6.cz/programm/mtrrlfbe.exe) to setup a
  write-combining MTRR for the LFB.  This will dramatically increase
  performance on Voodoo Rush and higher cards.

Recommended System Specs:
* Pentium 2 400mhz.  Pentium 3 550mhz or higher for the best experience.
* Voodoo 3.  Older cards will work, but unknown how well they perform in
  faster computers.

Interesting parameters:
* -bpp for setting 15, 16, or 32 bpp on Voodoo 4/5 cards.
* FX_GLIDE_NO_SPLASH=1 to disable the splash logo on Voodoo 1 and 2 cards
  on startup.
* FX_GLIDE_SWAPPENDINGCOUNT to set the number of frames to buffer.  Default
  value is 2, but can go as high as 6.  Traditionally, in Windows, higher
  values can be a slight speed advantage but can introduce input lag.  On
  the hardware I tested it on I was able to gain a few extra frames in
  timedemos (about 3-4) with 6 with no input lag and I am even able to
  use m_filter in combination with this.
* SSTH3_SLI_AA_CONFIGURATION to control SLI on Voodoo 5 5500 as well as
  anti-aliasing on these cards.
  - For Voodoo 4 and 5:
    0 - SLI Disabled, AA Disabled.
    1 - SLI Disabled, AA 2x Enabled.
  - For Voodoo 5 5500:
    2 - SLI Enabled, AA Disabled (default setting).
    3 - SLI Enabled, AA 2x Enabled.
    4 - SLI Disabled, AA 4x Enabled.
  Options 5-8 are for Voodoo 5 6000 users: if you're out there, send me
  an email!
* MESA_FX_IGNORE_CMBEXT to allow Voodoo 4/5 to perform single-pass
  trilinear.  This also provides a small speed boost of 4-5 fps on average
  in my timedemo tests with bilinear.  Mesa warns some advanced (multi)texturing
  modes won't work (GL_EXT_texture_env_combine), but multitexturing is slower
  in Mesa and is recommended to be disabled (see below).

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
  approximately 10fps loss on average (more in complex scenes) on everything
  I tested it on EXCEPT in modes 1280x1024 and 1600x1200 where it really
  helps.  It defaults to 0 by default.  If you would like to try it anyways
  set it to 1 and do a vid_restart.
* gl_ext_pointparameters can lock some machines when particles are drawn,
  i.g. if you start a map and try to fire your gun and the game locks up.
  This is why it is disabled by default in Q2DOS.  But if you are using
  a custom config file imported from another OS it may secretly have this
  enabled.
* Trilinear filtering is disabled by default on Voodoo 4/5 unless you use
  MESA_FX_IGNORE_CMBEXT.
* Warehouse maps with power cubes.  Grabbing/adding a power cube to/from a
  generator uses a dynamic lighting effect to dim or turn on the lights.
  This effect drastically slows the renderer down briefly for a few seconds
  until the effect is finished.  The issue persists on Mesa 6.1 and higher
  due to the tnl code being completely overhauled.  The offending gl functions
  are in DrawGLPolyChain in gl_rsurf.c.  You can use Mesa 6.0.1 if it truly
  bothers you, but DMesaGetProcAddress will have to be re-added to the code
  (trivial to do so, do a grep for it in current Mesa code).

Other tidbits:
* 3DFX's reference hardware for benchmarking with the VSA-100 chipset
  (Voodoo 4 and 5) was a Pentium 3 866mhz.  A setup in this range provides
  the smoothest framerates and is recommended to play the game comfortably.
  See http://www.falconfly.de/downloads/3dfx-v5-PC-Reviewers-Guide.zip for
  more details.
