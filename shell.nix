with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "myenv";
  buildInputs = [ alsaLib rtmidi nlohmann_json ];
}

