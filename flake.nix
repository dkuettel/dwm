{
  description = "dwm";

  inputs = {
    config.url = "github:dkuettel/config/main";
    nixpkgs.follows = "config/nixpkgs";
    # nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      config,
      nixpkgs,
      flake-utils,
    }:
    let
      make =
        system:
        let
          pkgs = import nixpkgs { system = system; };
          pkg = pkgs.stdenv.mkDerivation {
            pname = "dwm";
            version = "dk";

            src = ./.;

            buildInputs = with pkgs; [
              xorg.libX11
              xorg.libXinerama
              xorg.libXft
            ];

            prePatch = ''
              sed -i "s@/usr/local@$out@" config.mk
            '';

            makeFlags = [ "CC=${pkgs.stdenv.cc.targetPrefix}cc" ];
          };
        in
        {
          packages.default = pkg;
        };
    in
    flake-utils.lib.eachDefaultSystem make;
}
