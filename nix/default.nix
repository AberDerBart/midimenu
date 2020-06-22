{ stdenv, fetchFromGitHub, alsaLib, rtmidi, nlohmann_json }:

stdenv.mkDerivation {
  pname   = "midimenu";
  version = "0.1.0";

  src = fetchFromGitHub {
    owner  = "AberDerBart";
    repo   = "midimenu";
    rev    = "master";
    sha256 = "08j3hcvr204kk3fa4sqdrl7422z21b1vvrw4f26da6zba2pm7fvl"; 
  };

  buildInputs = [ alsaLib rtmidi nlohmann_json ];
  makeFlags = [ "DESTDIR=$(out)" ];


  meta = with stdenv.lib; {
    description = "A menu triggering programs based on midi signals";
    homepage    = https://github.com/AberDerBart/midimenu;
    license     = licenses.gpl3;
    maintainers = with maintainers; [ AberDerBart ];
    platforms   = platforms.all;
  };
}
