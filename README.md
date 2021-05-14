# Cloudburst
Rainy days

# Ideas
- Wrapper to output keyboard/joystick-agnostic input
- Input stream
	- Store previous inputs
	- Retrieve inputs over specific buffer period number of frames
	- Helper function to call that specifies input priorities and buffer period (-e.g.-, half circles get longer input windows than quarter circles)
- `addState()` helper function to call when adding a state to an entity that takes care of mutual exclusivities in state
- Rollback implementation [[Pseudocode example](https://gist.github.com/rcmagic/f8d76bca32b5609e85ab156db38387e9)]
	- Store game state
	- Restore game state
	- Recalculate game state over n frames
- Hit vectors?
	- Just use parabolic arc formula and inputs of initial angle, velocity, and object weight?

# TODO
- [ ] Figure out where Enkidu's disappeared to
- [ ] Create an in-game logger to display things in World
	- Create an Observer that monitors World entities for things to print to log
- [ ] Create a trigger class for charge inputs or expand InputTrigger to handle those too
- [ ] Expand settings state to allow joystick button binding
	- Analog stick threshold/axis binding
	- Make sure dpad inputs override analog stick inputs
- [ ] Add state-based system to Characters
- [ ] Device select state: 
	- List available input devices
	- "Move" device left or right to claim 1P/2P
- [ ] Create a class to handle attacks
	- Generate hitbox/hurtbox/collision box child nodes to attach to a character 
	- Ideally the class would just read in whatever data generated by the below move creation GUI
	- Would make use of animation class; read in animation frame details from same data file
- [ ] Fix hardcoded pixel values
	- Window resolution should be changeable and everything should scale to match

- [X] ~~Gamepad buttons~~
	- ~~Try plugging a bunch of controllers in and have the engine output whatever button's being pressed to debug log~~
- [X] ~~Include update rate in Applications::mStatisticsText~~
- [X] ~~Try creating some test trigger functions for motion inputs~~
	- ~~Observe the input stream with variable buffer, output debug log text when relevant motion is detected~~
- [X] ~~Get rid of command-style entity modification~~
	- ~~Just handle everything through input stream~~
	- ~~Entities have access to inputs over the last X frames~~
	- ~~Inputs are represented as controller state per frame, represented as bit flag sum int~~
- [X] ~~SOCD~~
	- ~~Currently possible to input opposite directions at the same time---needs to be fixed~~
- [X] ~~Input wrapper---incorporate into Player class~~
	- ~~Player class takes InputWrapper as member~~
	- ~~Device ID~~
	- ~~Keybindings~~
	- ~~Passes stream of device-agnostic input (i.e. numpad notation or something)~~
- [X] ~~Create a full animation class~~
	- ~~Takes a spritesheet and a range of sprite IDs; get texture rect pos/dim from IDs~~
	- ~~Need to be able to specify number of frames each animation frame is displayed for~~
	- ~~Current rudimentary class needs to be modified to take an array of durations~~
- [X] ~~Tick-based game logic updates~~
	- ~~Rendering rate is not tied to game logic~~
	- ~~Game logic updates at 60 fps~~
	- ~~Rendering takes snapshots of game logic; if a user drops frames then rendering will skip game logic that has already been done~~

# GUI for creating moves
- Mouse-drawable rectangles for hitbox/hurtbox/collision
- Display position/dims in text boxes for precise editing
- Editable frame data and animation frame IDs/durations
- Customizable hit vector parameters and hitstun/blockstun durations
- Onion skinning to compare changes in boxes between adjacent animation frames
- "Play" button with speed settings that allows you to play back the move animation
- Components:
	- Viewing window to render animation frames and drawn boxes
	- Text boxes and radio buttons for parameters and properties
		- Box positions and dimensions (drawable and selectable as hit/hurt/collision)
		- Extra entity placement (-e.g.-, sword trails and particles)
		- Invul (different types)
		- Character movement (to be propagated up to parent character node)
		- Inflicted damage/hitstun/blockstun/proration
		- Blocking flags (high/low/mid/air/etc.)
	- Animation frame selector (text box for spritesheet ID) and duration
	- "Propagate to identical animation frame" option to mirror data to frames with same animation frame
	- Data saves to external file to be read by game when generating character movetable
		- Automatically tag frames with hitboxes with an Active tag so other functions can differentiate between startup/active/recovery
