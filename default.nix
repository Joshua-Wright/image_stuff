# dev environment
with import <nixpkgs> {}; {
  image_stuff_env = stdenv.mkDerivation rec {
    name = "image_stuff_env";
    version = "1.1.1.1";
    src = ./.;
    buildInputs = [ stdenv gcc6 gtest cmake pkgconfig ];
    shellHook = ''
      export TERM=st-256color
    '';
  };
}