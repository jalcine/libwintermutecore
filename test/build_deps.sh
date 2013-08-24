#!/usr/bin/env sh

alias cm="mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr && cd .."
alias db="make -C build all && sudo make -C build install && cd .."
# Build qcommandline
# TODO: Add this from a PPA instead
git clone git://github.com/jalcine/qcommandline && cd qcommandline
cm && db

# Build log4qt
# TODO: Add this from a PPA instead.
git clone git://gitorious.org/log4qt/log4qt.git && cd log4qt
cm && db

git clone git://github.com/zeromq/cppzmq
sudo install cppzmq/zmq.hpp /usr/include/zmq.hpp

# Build QtZeroMQ.
# TODO: Add this from a PPA.
git clone git://github.com/jalcine/QtZeroMQ
cm && db

# Install coveralls's command line tool.
sudo pip install cpp-coveralls --use-mirrors
