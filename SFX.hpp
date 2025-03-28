// SFX.hpp
#pragma once

#include "header.hpp"

// Notes frequencies (in Hz)
const int C4 = 261;
const int D4 = 294;
const int E4 = 329;
const int F4 = 349;
const int G4 = 392;
const int A4 = 440;
const int B4 = 466;
const int C5 = 523;

// Helper function to create a rest between sections (helps add phrasing)
void rest(int duration);

// Helper function to play a note with a specified frequency and duration
void playNote(int frequency, int duration);

void GrasswalkSong();

void PlayZombieBiteSound();

void PlayZombieHitSound();
