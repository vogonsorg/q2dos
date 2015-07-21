Welcome to the Quake II for MS-DOS port.
By Neozeed, [HCI]Mara'akate, and sezero.


What's required?
* Pentium 1.  At least a 200MHz to play it at ~20-30fps.
* At least 48 MB RAM.  More for larger mods.
* LFN TSR.  Use the included DOSLFN.ZIP (can be loaded high with LH DOSLFN.COM)

What works?
* VGA (320x200 and 320x240)
* SVGA (Banked and LFB)
* Mouse
* Keyboard
* Sound Blaster and Gravis UltraSound family
* CD-ROM music
* OGG music
* Networking (You need a packet driver)
* Loading/unloading game DLLs in DXE3 format.

What works but needs more testing?
* Modern sound cards like AC'97 or HDA
* Mouse wheel support. (see info below.)
* Hor+ FOV scaling. (see info below.)

What doesn't work:
* Playing in a Win9x or NTVDM DOS box.
* TGA files from maps that were only optimized for OpenGL.
* Some models from Whale's WODX server can crash the software renderer.  I'm
  assuming they're custom player models as I tried replaying the same maps
  later and got no crashes.  Until I can spot it, be on the lookout.

Tips and other misc info:
* Run /slist2 to get a gamespy list of active servers, or use the new browser
  menu.  Run /srelist to print the previous list.  Use CTRL+C to abort.
* Don't use EMM386.  If you get weird virtual alloc failed errors, 
  it's probably related to this.
* Use FastVid for some faster linear frame buffering if you a P1 or P2-era PC.
  Download it at http://dk.toastednet.org/QDOS/FASTV110.ZIP.
* UNIRV can enable custom refresh rates.  Great if you have a large CRT monitor.
  Download it at http://dk.toastednet.org/QDOS/UNIRV142.ZIP.
* Gravis UltraSound users:  If you have a "stuck" or "repeating" sound as soon
  you load Q2DOS load Ultramid -Q then Ultramid -Q -F.  If you don't have
  Ultramid for some reason, you can get the fix (with a batch included) at
  http://dk.toastednet.org/QDOS/GUSFIX.ZIP.
  You also may need to set s_mixahead to 0.3 and use high IRQs.
* Make sure you are using the included CWSDPMI.EXE in your Q2 directory or you
  may get some issues with running out of memory.
* To hear Ogg Vorbis tracks in game, rip the Q2 CD's audio tracks (2-11) to 
  track02-track11.ogg and put them in baseq2/music.  For the mission pack audio
  tracks, rip the Ground Zero CD's audio tracks (2-11) to track12-track21.ogg.
  Don't bother ripping the tracks from the Reckoning CD, as they're a mix of
  the tracks on the Q2 and Ground Zero CDs.  Everything else will be taken care
  of internally.
* Mouse wheel:  The code is from HoT.  If you use the latest beta version of
  cutemouse, which  is 2.1b3 (ctmouse21b3.zip), the wheel should work nicely.
  We tested with the 2.0-alpha and 2.1-beta versions. We automatically detect
  and use the wheel if it is present. To disable the wheel, use the command
  line argument -nowheel.
  - Note: As of cutemouse-2.1-beta4 version (cutemouse21b4.zip), the wheel is
    not enabled by default: you must use their /O switch.
* Hor+ style field of view (FOV) scaling: Useful for widescreen resolutions.
  Configured by new cvar fov_adapt: set it to 1 and your FOV will be scaled
  automatically according to the resolution. It is enabled by default.
* MEMSTATS command will report available Physical and Virtual Memory in Mb.
  You will notice the number will never rise, this is on purpose.
  Please read the DJGPP FAQ @ http://www.delorie.com/djgpp//v2faq/faq15_2.html
  for more information on how DJGPP and CWSDPMI work.

Follow neozeed @
http://virtuallyfun.superglobalmegacorp.com/

Follow maraakate @
http://dk.toastednet.org/

Various code from
* cURL -- (http://curl.haxx.se/)
* DJGPP community -- (http://www.delorie.com/djgpp/)
* Gamespy SDK -- Released publically in 2014.
* ggorts -- Ported 3ZB2, Action, Chaos, and DDay to Q2DOS.  Banked VGA code and
            Planar mode help.
* Knightmare -- Ref_soft help.
                KMQuake II (http://www.markshan.com/knightmare/)
* Mpxplay -- (http://mpxplay.sourceforge.net/)
* Mpxplay code importing by Ruslan Starodubov (http://sound-dos.ucoz.ru/)
* Ogg Vorbis -- (https://www.xiph.org/)
* R1CH -- R1Q2 (http://old.r1ch.net/stuff/r1q2/)
* Sezero -- WATT32 and DJGPP 2.05 help.
            HoT (http://uhexen2.sourceforge.net/)
* Yamagi -- Updated game code from YamagiQ2 (http://www.yamagi.org/quake2/)
* Various DJGPP v2.05 upgrade help from RayeR (http://rayer.g6.cz/)
* Berserk, CDawg, Pat Aftermoon, Anthony Jacques, David M. Pochron, Dopefish,
  GuyP, Jitspoe, MH, MrG, NeVo, Skuller, Riot, R1ch, Psychospaz, Pooy, Sul, 
  and others.
