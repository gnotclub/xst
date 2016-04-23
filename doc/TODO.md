xst
---

* change aur package name.
* Implement a daemon client mode like urxvt.
* expose xresources colors through 256 with a loop and color macro.
* make startup shell Xresource value if set.
* fallback to \*setting on xresources as well.
* expose the ability to set keybinds via Xresources.
* apply the rest of the boldcolors patch (currently just exposing ability to disbable bold fonts).
* make a keybind table of existing binds?
	* take a closer look at keybinding possiblities.
* consider the scrollback patch.

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

