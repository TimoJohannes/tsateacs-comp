// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_ClassName.ino
// (2) Find and replace "ClassName" with the name of your Applet class
// (3) Implement all of the public methods below
// (4) Add text to the help section below in SetHelp()
// (5) Add a declare line in hemisphere_config.h, which looks like this:
//     DECLARE_APPLET(id, categories, ClassName), \
// (6) Increment HEMISPHERE_AVAILABLE_APPLETS in hemisphere_config.h
// (7) Add your name and any additional copyright info to the block below

// Copyright (c) 2018, __________
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "SegmentDisplay.h"
#define CVRRY_MAX_STEP 16

const char* const Cvrry_MODES[2] = {
    "Play", "Rec"
}; 

class Cvrry : public HemisphereApplet {
public:

    const char* applet_name() { // Maximum 10 characters
        return "Cvrry";
    }

	/* Run when the Applet is selected */
    void Start() {
        segment.Init(SegmentSize::BIG_SEGMENTS);
    }

	/* Run during the interrupt service routine, 16667 times per second */
    void Controller() {
        //should the trig input 2 be remapable? maybe between reset/start sampling? for now, i will 
        //implement the reset
        bool reset = Clock(1);

        if (Clock(0) || reset){
            step++;
            if (step > end || step < start) step = start;
            if (reset){
                step = start;
                if(punch_out) punch_out = end - start;
            }
            bool rec = 0;
            signal = int2simfloat(cv[step]);
            byte next_step = step + 1;
            if (next_step > end) next_step = start;

            if(mode & 0x01){
                if (punch_out){
                    rec = 1;
                    cv[step] = In(0);
                }
            }
            if (rec){
                if (--punch_out == 0) mode = 0;
            }
        }
        if (!(mode & 0x01))
        {
            Out(0, simfloat2int(signal));
        } else {
            Out(0, In(0));

        }
        
    }

// interface shizzle
	/* Draw the screen */
    void View() {
        gfxHeader(applet_name());
        DrawInterface();
        // Add other view code as private methods
    }

	/* Called when the encoder button for this hemisphere is pressed */
    void OnButtonPress() {  
        if (cursor == 3) {
            // Check recording status
            if (mode > 0) punch_out = end - start;
            else punch_out = 0;
        }
        if (++cursor > 3) cursor = 0;
        ResetCursor();

    }


    // encoder modes
    void OnEncoderMove(int direction){
        if (cursor == 0)
        {
            int16_t fs = start;
            start = contrain(start + direction, 0, end - 1);
            if(fs != start && punch_out) punch_out -= direction;
        }
        
        if (cursor == 1)
        {
            int16_t fe = end;
            end = contrain(end + direction, start + 1, CVRRY_MAX_STEP - 1);
            if(fe != end && punch_out) punch_out += direction;
        }
        if (cursor == 2)
        {
            /* playmode, needs subfunction */
        }
        if (cursor == 3)
        {
            /* cv in 2 and tr 2 mapping, needs subfuntion */
        }
        ResetCursor();

    }

	/* Called when the encoder for this hemisphere is rotated
	 * direction 1 is clockwise
	 * direction -1 is counterclockwise
	 */
    void OnEncoderMove(int direction) {
    }
        
    /* Each applet may save up to 32 bits of data. When data is requested from
     * the manager, OnDataRequest() packs it up (see HemisphereApplet::Pack()) and
     * returns it.
     */
    uint32_t OnDataRequest() {
        uint32_t data = 0;
        Pack(data, PackLocation {0,9}, start);
        Pack(data, PackLocation {9,9}, end);
        return data;
    }

    /* When the applet is restored (from power-down state, etc.), the manager may
     * send data to the applet via OnDataReceive(). The applet should take the data
     * and unpack it (see HemisphereApplet::Unpack()) into zero or more of the applet's
     * properties.
     */
    void OnDataReceive(uint32_t data) {
        // example: unpack value at bit 0 with size of 8 bits to property_name
        // property_name = Unpack(data, PackLocation {0,8}); 

        // start and stop data sizes can be smaller, when only using 16 steps
        // but lets save the stati of cv 1 in and trig in here aswell.
        start = Unpack(data, PackLocation {0,9});
        end = Unpack(data, PackLocation {9,9});
    }

protected:
    /* Set help text. Each help section can have up to 18 characters. Be concise! */
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "1=CLK 2=VAR";
        help[HEMISPHERE_HELP_CVS]      = "1=REC 2=VAR";
        help[HEMISPHERE_HELP_OUTS]     = "1=CV OUT 2= EOC";
        help[HEMISPHERE_HELP_ENCODER]  = "VARS/LGTH/PLYMD";
        //                               "------------------" <-- Size Guide
    }
    
private:
    int cursor; // 0=Start 1=End 2=Playmode 3=IO Modes
    SegmentDisplay segment;

    int16_t cv[CVRRY_MAX_STEP];
    simfloat signal[2];

    // Transport
    int mode = 0; // 0=Playback, 1=Rec Track 1, 2=Rec Track 2, 3= Rec Tracks 1 & 2
    int16_t start = 0; // Start step number
    int16_t end = CVRRY_MAX_STEP-1; // End step number
    int16_t step = 0; // Current step
    int16_t punch_out = 0;
    
};

void DrawInterface(){
    //PlayRange end
        gfxPrint(1, 15, "Start:")
        gfxPrint(18 + pad(100, start + 1), 15, start + 1);

    //Playrange end
        gfxPrint(pad(100, end + 1), end + 1);

    //Playrange Length

    //Playmode

    //IO-Map

}


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to ClassName,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
Cvrry Cvrry_instance[2];

void Cvrry_Start(bool hemisphere) {Cvrry_instance[hemisphere].BaseStart(hemisphere);}
void Cvrry_Controller(bool hemisphere, bool forwarding) {Cvrry_instance[hemisphere].BaseController(forwarding);}
void Cvrry_View(bool hemisphere) {Cvrry_instance[hemisphere].BaseView();}
void Cvrry_OnButtonPress(bool hemisphere) {Cvrry_instance[hemisphere].OnButtonPress();}
void Cvrry_OnEncoderMove(bool hemisphere, int direction) {Cvrry_instance[hemisphere].OnEncoderMove(direction);}
void Cvrry_ToggleHelpScreen(bool hemisphere) {Cvrry_instance[hemisphere].HelpScreen();}
uint32_t Cvrry_OnDataRequest(bool hemisphere) {return Cvrry_instance[hemisphere].OnDataRequest();}
void Cvrry_OnDataReceive(bool hemisphere, uint32_t data) {Cvrry_instance[hemisphere].OnDataReceive(data);}
