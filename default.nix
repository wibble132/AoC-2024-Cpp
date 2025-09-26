{ pkgs ? import <nixpkgs> { } }:
let
  project_name = "AoC-2024";
  build_type = "Debug";
in
pkgs.stdenv.mkDerivation {
  name = project_name;
  pname = project_name;
  src = ./.;

  buildInputs = with pkgs; [
    stdenv.cc.cc.lib
    boost
    range-v3
    cmake
    ninja
  ];

  nativeBuildInputs = with pkgs; [
    gcc
    pkg-config
    tree
  ];

  configurePhase = ''
    cmake -DCMAKE_BUILD_TYPE=${build_type} \
    -DPROJECT_NAME=${project_name} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .
  '';

  buildPhase = ''
    cmake --build .
  '';

  installPhase = ''
    mkdir -p $out/bin
    mv bin/* $out/bin
    cp compile_commands.json $out/
  '';

  meta = with pkgs; {
    # Export project_name as an attribute
    project_name = project_name;
  };
}
