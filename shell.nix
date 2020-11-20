let
  pkgs = import <nixpkgs> {};
in pkgs.mkShell {
  buildInputs =
    (with pkgs; [
      stdenv pkgconfig xorg.libX11 ncurses xorg.libXft harfbuzz
    ]);
}
