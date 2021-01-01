
#include <string>

struct Config {
  struct {
    std::string ssid;  // The SSID.
    std::string key;   // The pre-shared key.
  } wifi;
  struct {
    std::string client_id;
    std::string client_secret;
  } spotify;
  struct {
    std::string timezone;
    std::string ntp_server;
  } time;
};