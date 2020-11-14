{
  description = "A flake for building midimenu";

  inputs.nixpkgs.url = github:NixOS/nixpkgs/nixos-20.03;

  outputs = { self, nixpkgs }: {
    defaultPackage.aarch64-linux = 
      with import nixpkgs { system = "aarch64-linux"; };
      stdenv.mkDerivation {
        pname   = "midimenu";
        version = "0.1.0";

        src = self;  

        buildInputs = [ alsaLib rtmidi nlohmann_json ncurses ];
        makeFlags = [ "DESTDIR=$(out)" ];

        meta = with stdenv.lib; {
          description = "A menu triggering programs based on midi signals";
          license     = licenses.gpl3;
          maintainers = with maintainers; [ AberDerBart ];
        };  
      };
    defaultPackage.x86_64-linux = 
      with import nixpkgs { system = "x86_64-linux"; };
      stdenv.mkDerivation {
        pname   = "midimenu";
        version = "0.1.0";

        src = self;  

        buildInputs = [ alsaLib rtmidi nlohmann_json ncurses ];
        makeFlags = [ "DESTDIR=$(out)" ];

        meta = with stdenv.lib; {
          description = "A menu triggering programs based on midi signals";
          license     = licenses.gpl3;
          maintainers = with maintainers; [ AberDerBart ];
        };  
      };
  };
}

