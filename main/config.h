
#include <string>

struct Config {
  struct {
    std::string ssid;  // The SSID.
    std::string key;   // The pre-shared key.
  } wifi;
  struct {
    std::string client_id;
  } spotify;
};