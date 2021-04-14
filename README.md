# Cloudburst
Rainy days

# TODO
* Wrapper to output keyboard/joystick-agnostic input
* Input stream
	* Store previous inputs
	* Retrieve inputs over specific buffer period number of frames
	* Helper function to call that specifies input priorities and buffer period (*e.g.*, half circles get longer input windows than quarter circles)
* addState() helper function to call when adding a state to an entity that takes care of mutual exclusivities in state
* Rollback implementation [[Pseudocode example](https://gist.github.com/rcmagic/f8d76bca32b5609e85ab156db38387e9)]
	* Store game state
	* Restore game state
	* Recalculate game state over n frames
* GUI for creating moves
	* Mouse-drawable rectangles for hitbox/hurtbox/collision
	* Display position/dims in text boxes for precise editing
	* Editable frame data and animation frame IDs/durations
	* Customizable hit vector parameters and hitstun/blockstun durations