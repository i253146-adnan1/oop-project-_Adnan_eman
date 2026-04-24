#pragma once

// ─── Window ────────────────────────────────────────────────
constexpr unsigned WINDOW_W = 768;
constexpr unsigned WINDOW_H = 672;
constexpr unsigned TILE_SIZE = 32;
constexpr float    FPS = 60.f;

// ─── Physics ────────────────────────────────────────────────
constexpr float GRAVITY = 900.f;   // px / s²
constexpr float JUMP_VELOCITY = -460.f;
constexpr float PLAYER_SPEED = 200.f;
constexpr float SNOWBALL_SPEED = 400.f;

// ─── Player ─────────────────────────────────────────────────
constexpr int   PLAYER_MAX_HP = 3;
constexpr float PLAYER_INV_TIME = 2.f;   // seconds of invincibility after hit
constexpr float SHOOT_COOLDOWN = 0.3f;

// ─── Enemy ──────────────────────────────────────────────────
constexpr float ENEMY_SPEED = 80.f;
constexpr float SNOWBALL_COVER_TIME = 3.f;  // seconds until snow melts

// ─── Score ──────────────────────────────────────────────────
constexpr int SCORE_SNOWBALL_HIT = 100;
constexpr int SCORE_KILL = 500;
constexpr int SCORE_BOUNCING_KILL = 1000;

// ─── Level 1 grid (24 cols × 21 rows, each tile = 32 px) ──
//  0 = air   1 = solid platform
constexpr int MAP_COLS = 24;
constexpr int MAP_ROWS = 21;