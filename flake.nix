{
  description = "dwm";

  inputs = {
    nixpkgs.url = "github:dkuettel/nixpkgs/stable";
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
                xorg.libX11
                xorg.libXinerama
                xorg.libXft
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
