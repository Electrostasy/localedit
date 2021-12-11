{
  description = "Development shell flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system: {
      devShell = with nixpkgs.legacyPackages.${system}; mkShell {
        buildInputs = [
          cmake
          qt515.qtbase
          qt515.qtwayland
        ];
      };
    }
  );
}
