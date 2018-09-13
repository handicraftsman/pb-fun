#include <positronbot.hpp>

#include <map>
#include <random>
#include <string>

template<typename Iter, typename RandomGenerator>
static Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
  std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
  std::advance(start, dis(g));
  return start;
}

template<typename Iter>
static Iter select_randomly(Iter start, Iter end) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return select_randomly(start, end, gen);
}

auto cookie_handler = [] (pb::command& c, pb::event_command::ptr e) {
  static std::map<std::string, std::string> qualities {
    {"normal", ""},
    {"uncommon", "%C?LBLUEuncommon "},
    {"rare", "%C?BLUErare "},
    {"epic", "%C?PURPLEepic "},
    {"legendary", "%C?YELLOWlegendary "},
    {"holy", "%C?ORANGEholy "},
    {"hi-tech", "%C?CYANhi-tech "},
    {"quantum", "%C?LBLUEquantum "},
    {"evil", "%C?BLACKevil "},
    {"magical", "%C?PURPLEmagical "},
    {"ancient", "%C?LBLUEancient "},
    {"vampiric", "%C?REDvampiric "}
  };
  static std::map<std::string, std::string> types {
    {"normal", ""},
    {"blazing", "%C?ORANGEblazing "},
    {"hot", "%C?REDhot "},
    {"frozen", "%C?CYANfrozen "},
    {"chilling", "%C?LBLUEchilling "},
    {"shocking", "%C?YELLOWshocking "},
    {"aerial", "%C?LGREYaerial "},
    {"stone", "%C?GREYstone "},
    {"mud", "%C?BROWNmud "},
    {"void", "%C?BLACKvoid "},
    {"ghostly", "%C?WHITEghostly "},
    {"bloody", "%C?REDbloody "},
    {"nyan", "%C?REDn%C?GREENy%C?BLUEa%C?CYANn "},
    {"teleporting", "%C?CYANteleporting "},
    {"wild", "%C?BROWNwild "},
    {"alient", "%C?GREENalien "},
    {"spacious", "%C?BLACKspacious "},
    {"atomic", "%C?REDatomic "},
    {"chocolate", "%C?BROWNchocolate " }
  };
  
  std::string target;
  std::string quality;
  std::string type;

  bool has_target = false;
  bool has_quality = false;
  bool has_type = false;
  
  for (int i = 1; i < e->split.size(); ++i) {
    std::string s = e->split[i];
    if (s.substr(0,9) == "--quality") {
      quality = qualities[s.substr(10, s.size())];
      has_quality = true;
      continue;
    }
    if (s.substr(0,6) == "--type") {
      type = types[s.substr(7, s.size())];
      has_type = true;
      continue;
    }
    if (!target.empty())
      target += " ";
    target += s;
    has_target = true;
  }
  
  if (!has_quality)
    quality = select_randomly(qualities.begin(), qualities.end())->second;
  if (!has_type)
    type = select_randomly(types.begin(), types.end())->second;
  if (!has_target)
    target = e->nick;
  
  std::string rto = (e->target == e->socket->get_config().nick) ? e->nick : e->target;
  
  e->socket->stream() << pb::ircstream::ctcp(rto, "ACTION", "&Ngives &B" + target + "&N a &B" + quality + type + "&N&B%C?BROWNcookie&N");
};

auto poke_handler = [] (pb::command& c, pb::event_command::ptr e) {
  std::string target;
  if (e->split.size() == 1) {
    target = e->nick;
  } else if (e->split.size() == 2) {
    target = e->split[1];
  } else if (e->split.size() > 2) {
    target = e->split[1];
    size_t sz = e->split.size();
    for (int i = 2; i < sz; ++i) {
      target += (" " + e->split[i]);
    }
  } else {
    e->socket->stream() << pb::ircstream::nreply(e, "Invalid arguments!");
    return;
  }
  std::string rto = (e->target == e->socket->get_config().nick) ? e->nick : e->target;
  e->socket->stream() << pb::ircstream::ctcp(rto, "ACTION", "pokes " + target);
};

extern "C" {
  pb::plugin* pb_plugin;

  void pb_init(std::vector<std::pair<std::string, std::string>>& cfg) {
    
    pb::command cmd_cookie {
      .pplugin     = pb_plugin,
      .name        = "cookie",
      .usage       = "<who...> [--quality=?] [--type=?]",
      .description = "gives cookies",
      .cooldown    = 10,
      .flag        = "cookie",
      .handler     = cookie_handler
    };
    pb_plugin->register_command(cmd_cookie);
    
    pb::command cmd_poke {
      .pplugin     = pb_plugin,
      .name        = "poke",
      .usage       = "[who]",
      .description = "pokes users",
      .cooldown    = 10,
      .flag        = "poke",
      .handler     = poke_handler
    };
    pb_plugin->register_command(cmd_poke);
  }
  
  void pb_deinit() {}
}