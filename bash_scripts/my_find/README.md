This is a bash script that does a subset of the bash find command. This implementation can handle the following options as described in the manual for find:
* name: handles patterns for names
* type: f and d
* print
* exec: handles {}, assumption is exec is always the last argument if present
