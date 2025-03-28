#include "SFX.hpp"

// Helper function to create a rest between sections (helps add phrasing)
void rest(int duration) {
    sleep_for(milliseconds(duration));
}

// Function to check if sox is installed
static bool isSoxInstalled() {
    return system("which sox > /dev/null 2>&1") == 0;
}

// Function to install sox
static void installSox() {
    std::cout << "Sox is not installed. Installing now...\n";
    int result = system("sudo apt install sox -y");

    if (result != 0) {
        std::cerr << "Failed to install sox. Please install it manually.\n";
        exit(1);
    }
}

// Modified playNote function
void playNote(int frequency, int duration) {
#ifdef _WIN32
    Beep(frequency, duration);
#else
    if (!isSoxInstalled()) {
        installSox();
    }
    std::string command = "play -n synth " + std::to_string(duration / 1000.0) +
        " sine " + std::to_string(frequency) + " > /dev/null 2>&1";
    system(command.c_str());
#endif
    rest(50);
}

void GrasswalkSong() {
    thread([] {
        while (true) {
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(D4, 400); // D
            playNote(D4, 400); // D
            playNote(E4, 400); // E
            playNote(D4, 400); // D
            playNote(C4, 400); // C

            rest(200);  // Pause between phrases

            // Phrase 2: Transition section
            playNote(G4, 400); // G
            playNote(G4, 400); // G
            playNote(F4, 400); // F
            playNote(F4, 400); // F
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(D4, 400); // D
            playNote(D4, 400); // D

            rest(200);  // Pause for phrasing

            // Phrase 3: High note variation
            playNote(C5, 400); // C5
            playNote(C5, 400); // C5
            playNote(B4, 400); // B
            playNote(A4, 400); // A
            playNote(G4, 400); // G
            playNote(F4, 400); // F

            rest(200);  // Pause for phrasing

            // Phrase 4: Repetition with slight variation
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(E4, 400); // E
            playNote(D4, 400); // D
            playNote(D4, 400); // D
            playNote(E4, 400); // E
            playNote(D4, 400); // D

            // End with a long fade-out note
            playNote(E4, 800); // E (long note to signal the end)
        }
        }).detach();
}

void PlayZombieBiteSound() {
    thread([] {
        playNote(C4, 100); // Low-frequency bite
        playNote(F4, 120); // Mid-range crunch
        playNote(G4, 80);  // Tearing sound
        }).detach();
}

void PlayZombieHitSound() {
    thread([] {
        playNote(C5, 50);  // Quick high-pitch "impact"
        playNote(D4, 70);  // Slightly lower "reverberation"
        }).detach();
}
