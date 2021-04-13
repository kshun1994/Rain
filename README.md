# Cloudburst
Rainy days

# TODO
1. Wrapper to output keyboard/joystick-agnostic input
2. Input stream
	* Store previous inputs
	* Retrieve inputs over specific buffer period number of frames
	* Helper function to call that specifies input priorities and buffer period (*e.g.*, half circles get longer input windows than quarter circles)
3. State helper function to call when adding a state to an entity that takes care of mutual exclusivities in state
4. Rollback implementation
	* Store game state
	* Restore game state
	* Recalculate game state over n frames