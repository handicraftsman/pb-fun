#include <particlepm.hpp>

using namespace PPM::Utils;

PPM_PACKAGE(p) {
  p->name("pb-fun");
  p->version(PPM::Version("v0.1.0"));
  p->description("A fun plugin for PositronBot");
  p->github(PPM::GitHub("handicraftsman", "pb-fun"));
  p->license(PPM::License("MIT", "https://github.com/handicraftsman/pb-fun/blob/master/LICENSE.txt"));

  PPM::GitHubPtr 
    pb_d    = p->github_repo("handicraftsman", "positronbot"),
    pdi_d   = p->github_repo("handicraftsman", "particledi"),
    pini_d  = p->github_repo("handicraftsman", "particleini"),
    guosh_d = p->github_repo("handicraftsman", "guosh");

  std::vector<std::string> src {
    "main.cpp"
  };

  std::string flags = flagcat({
    ("-L" + PPM::dist_dir),

    "-lpositronbot",
    "-lparticledi",
    "-lparticleini",
    "-lguosh",

    "-I.",
    ("-I" + pb_d->dir()),
    ("-I" + pdi_d->dir()),
    ("-I" + pini_d->dir()),
    ("-I" + guosh_d->dir() + "/src")
  });

  PPM::TargetPtr plugin = p->library("pb-fun");
  plugin->cpp("gnu++17");
  plugin->cpp_files(src);
  plugin->cpp_flags(flags);
}