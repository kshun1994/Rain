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
- [ ] Figure out how inputs are handled
	- Lock game logic to 60 fps?
	- How are a player's inputs decided for the frame? Just combine everything?
- [X] Create a full animation class
	- Takes a spritesheet and a range of sprite IDs; get texture rect pos/dim from IDs
	- Need to be able to specify number of frames each animation frame is displayed for
	- Current rudimentary class needs to be modified to take an array of durations
- [ ] Create a class to handle attacks
	- Generate hitbox/hurtbox/collision box child nodes to attach to a character 
	- Ideally the class would just read in whatever data generated by the below move creation GUI
	- Would make use of animation class; read in animation frame details from same data file

## Input class
- 

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
