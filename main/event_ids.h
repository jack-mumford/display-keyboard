#pragma once

#include <event_groups.h>

constexpr EventBits_t EVENT_NETWORK_GOT_IP = BIT0;
constexpr EventBits_t EVENT_NETWORK_DISCONNECTED = BIT1;
constexpr EventBits_t EVENT_SPOTIFY_GOT_AUTHORIZATION_CODE = BIT2;
constexpr EventBits_t EVENT_SPOTIFY_ACCESS_TOKEN_GOOD = BIT3;
constexpr EventBits_t EVENT_SPOTIFY_ACCESS_TOKEN_FAILURE = BIT4;
constexpr EventBits_t EVENT_SPOTIFY_ACCESS_TOKEN_EXPIRE = BIT5;
constexpr EventBits_t EVENT_KEYBOARD_EVENT = BIT6;
constexpr EventBits_t EVENT_ALL =
    BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;
