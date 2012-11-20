#!/bin/sh

mv config.guess config.guess.my
autoreconf -v -f -i -W all
rm -f config.guess
mv config.guess.my config.guess

rm -rf autom4te.cache
