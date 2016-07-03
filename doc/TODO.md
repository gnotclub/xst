xst
---

* expose the ability to set keybinds via Xresources.
* Implement a daemon client mode like urxvt.
* make a keybind table of existing binds?
	* take a closer look at keybinding possiblities.
* consider the scrollback patch.
* consider line spacing options.

vt emulation
------------

* double-height support

drawing
-------

* add diacritics support to xdraws()
	* switch to a suckless font drawing library
* make the font cache simpler
* add better support for brightening of the upper colors

bugs
----

* fix shift up/down (shift selection in emacs)
* remove DEC test sequence when appropriate

misc
----

`$ grep -nE 'XXX|TODO' st.c`

