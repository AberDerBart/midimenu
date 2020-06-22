{ lib, pkgs, config, ... }:
with lib;
let
  cfg = config.services.dymoprint;
in {
  options.services.midimenu = {
    enable = mkEnableOption "midimenu service";
    port = mkOption {
      type = types.str;
      description = ''
        the midiport to use
      '';
    };
    configFile = mkOption {
      type = types.path;
      description = ''
        the config file to use
      '';
  };

  config = lib.mkIf cfg.enable {
    systemd.services.midimenu = {
      wantedBy = [ "multi-user.target" ];
      serviceConfig.ExecStart = "${pkgs.midimenu}/bin/midimenu ${cfg.port} -p ${toString cfg.configFile}";
      serviceConfig.User = "nobody";
    };
  };
}
