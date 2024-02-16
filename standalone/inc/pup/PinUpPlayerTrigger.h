#pragma once


// Trigger names (m_trigger), first letter means ->
// S = Solenoid
// W = Switches
// L = Lights
// G = GIs
// E = DOFs
// M = Mechs
// D = PupCap DMD Match


class PinUpPlayerTrigger {
public:
   PinUpPlayerTrigger();
   ~PinUpPlayerTrigger();

   enum PlayAction {
      // plays the file until it ends
      Normal,

      // plays the file in a Loop
      Loop,

      // meant to be used with a Looping file. If this is triggered while a Looping file is 
      // currently playing…then the SplashReset file will play to its end, and then the original 
      // Looping file will resume from its beginning (there may be a pause when the Looping file 
      // begins again). This can be handy, but you would be better using SetBG in most cases 
      // to do something similar.
      SplashReset,

      // meant to be used with a Looping file. If this is triggered while a Looping file is 
      // currently playing…then the SplashReturn file will play to its end, and then the original 
      // Looping file will resume from where it left off (there may be a pause when the Looping file 
      // begins again). This can be handy, but you would be better using SetBG in most cases 
      // to do something similar.
      SplashReturn,

      // will stop whatever file is currently playing. 
      // Priority MUST be HIGHER than the file currently playing for this to work!
      StopPlayer,

      // will stop ONLY the file specified in PlayFile (if it's playing). 
      // This has no effect on other files that are playing.
      StopFile,

      // “Set Background” will set a new default looping “Background” file. 
      // When other files are done playing, then this new SetBG file will be played in a loop. 
      // Example: This can be handy for setting a new looping “mode” video, so that new other 
      // video events during the new mode will fall back to this SetBG video. Then you can change 
      // SetBG again to the main game mode video when the mode is completed.
      SetBG,

      // used to play WAV files for Surround Sound Feedback. 
      // (You don't want these sounds playing from your front / backbox speakers). 
      // The settings for the 3D position of the sound files are set in COUNTER. The format is in X,Z,Y. 
      // Example: “-2,1,-8”. X values range from -10 (left), 0 (center), 10 (right). Z values don't ever 
      // change and stay at 1. Y values range from 0 (top), -5 (center), -10 (bottom). 
      // NOTE: This currently will only work with the DEFAULT sound card in Windows. Additional 
      // sound card / devices are not yet supported!
      PlaySSF,

      // this will ignore the trigger if the file playing has the same Priority. This is nice for 
      // events such as Jackpot videos or others that will play very often, and you don't want to 
      // have them constantly interrupting each other. “Normal” PlayAction files with the same 
      // Priority will interrupt each other no matter the Rest Seconds. Using SkipSamePri will not 
      // play the new file (with same the Priority) if the current file is still playing and allows 
      // for smoother non-interruptive action for common events.
      SkipSamePri
   };

   static PinUpPlayerTrigger* CreateFromCSVLine(string line);

   int m_id;
   int m_active;
   string m_descript;
   string m_trigger;
   int m_screenNum;
   string m_playList;
   string m_playFile;
   int m_volume;
   int m_priority;
   int m_length;
   int m_counter;
   int m_restSeconds;
   PlayAction m_loop;
   int m_defaults;
};