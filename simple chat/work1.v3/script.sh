#!/bin/bash

# Start a new tmux session with the name 'my_session'.
tmux new-session -d -s my_session

# Splits window into two vertical panes
tmux split-window -h

# Execute the command 'g++ -o server.exe server.cpp' in the left pane (index 0)
tmux send-keys -t my_session:0.0 'g++ -o server.exe server2.c' C-m

# Execute the command 'g++ -o server.exe server.cpp' in the right pane (index 1)
tmux send-keys -t my_session:0.1 'g++ -o client.exe client2.c' C-m

# Left pane (index 0)
tmux send-keys -t my_session:0.0 'tree' C-m

# Right pane (index 0)
tmux send-keys -t my_session:0.1 'tree' C-m

# Run 'server.exe' command in the left pane (index 0)
tmux send-keys -t my_session:0.0 './server.exe' C-m

# Runs the command 'client.exe' in the right pane (index 1)
tmux send-keys -t my_session:0.1 './client.exe' C-m

# Select right pane (index 1)
tmux select-pane -t my_session:0.1

# Attach the tmux session to be visible in the current terminal
tmux attach-session -t my_session

