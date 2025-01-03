{
  description = "dwm";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-24.11";
  };

  outputs = { self, nixpkgs }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      pkg = pkgs.stdenv.mkDerivation rec {
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
      packages.x86_64-linux.default = pkg;
    };
}
