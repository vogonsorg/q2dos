Welcome to the Quake II for MS-DOS port.
By Neozeed and [HCI]Mara'akate.


What's required?
* Pentium 1.  At least a 200MHz to play it at ~20-30fps.
* 64MB RAM.  128MB RAM for larger mods.  x1.5 MB for virtual memory.
* LFN TSR.  Use the included DOSLFN.ZIP (can be loaded high with LH DLFN.COM)

What works?
* VGA
* SVGA (LFB modes only)
* Mouse
* Keyboard
* SoundBlaster and Gravis UltraSound Family
* CD-ROM music
* OGG music
* Networking (You need a packet driver)
* Loading/unloading game DLLs in DX3 format.

What works but needs more testing?
* Modern sound cards like AC'97 or HDA
* Mouse wheel support with -mwheel.  See other misc info below.

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
  Download it at http://dk.toastednet.org/QDOS/FASTV110.ZIP.
* Gravis UltraSound Users:  If you have a "stuck" or "repeating" sound as soon
  you load Q2DOS load Ultramid -Q then Ultramid -Q -F.  If you don't have
  Ultramid for some reason, you can get the fix (with a batch included) at
  http://dk.toastednet.org/QDOS/GUSFIX.ZIP.
* Make sure you are using the included CWSDPMI.EXE in your Q2 directory or you
  may get some issues with running out of memory.
* To hear Ogg Vorbis tracks ingame, rip the Q2 CD's audio tracks (2-11) to 
  track02-track11.ogg and put them in baseq2/music.  For the mission pack audio
  tracks, rip the Ground Zero CD's audio tracks (2-11) to track12-track21.ogg.
  Don't bother ripping the tracks from the Reckoning CD, as they're a mix of
  the tracks on the Q2 and Ground Zero CDs.  Everything else will be taken care
  of internally.
* Mouse wheel code is from HoT and since mouse wheel code varies from mouse
  driver it will not be enabled by default.  We've already had reports that
  it works, but not 100% properly.  I can't really do much more for you.
  DOS was made in a time where mouse wheels didn't even exist.
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
* Gamespy SDK -- release publically in 2014.
* ggorts -- Ported 3ZB2, Action, Chaos, and DDay to Q2DOS
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
