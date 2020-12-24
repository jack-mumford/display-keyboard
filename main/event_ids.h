#pragma once

#include <event_groups.h>

constexpr EventBits_t EVENT_NETWORK_GOT_IP = BIT0;
constexpr EventBits_t EVENT_NETWORK_DISCONNECTED = BIT1;
constexpr EventBits_t EVENT_SPOTIFY_GOT_ONE_TIME_CODE = BIT2;
constexpr EventBits_t EVENT_SPOTIFY_ACCESS_TOKEN_GOOD = BIT3;
constexpr EventBits_t EVENT_SPOTIFY_ACCESS_TOKEN_FAILURE = BIT4;
constexpr EventBits_t EVENT_ALL = BIT0 | BIT1 | BIT2 | BIT3 | BIT4;
