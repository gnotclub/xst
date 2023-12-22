let
  pkgs = import <nixpkgs> {};
in pkgs.mkShell {
  buildInputs =
    (with pkgs; [
      stdenv pkg-config xorg.libX11 ncurses xorg.libXft harfbuzz
    ]);
}
