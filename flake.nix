{
  description = "dwm";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    let
      make =
        system:
        let
          pkgs = import nixpkgs { system = system; };
          pkg =
            host:
            pkgs.stdenv.mkDerivation {
              pname = "dwm";
              version = "dk";

              src = ./.;

              buildInputs = with pkgs; [
                libX11
                libXinerama
                libXft
              ];

              prePatch = ''
                sed -i "s@/usr/local@$out@" config.mk
              '';

              makeFlags = [
                "CC=${pkgs.stdenv.cc.targetPrefix}cc"
                "HOSTFLAGS=-DHOST_${host}"
              ];
            };
        in
        {
          packages.default = pkg "default";
          packages.flat = pkg "flat";
        };
    in
    flake-utils.lib.eachDefaultSystem make;
}
