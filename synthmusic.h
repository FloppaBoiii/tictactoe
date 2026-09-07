#ifndef SYNTHMUSIC_H
#define SYNTHMUSIC_H

/*
    synthmusic.h - procedurally generated lofi/chillwave background loop

    Single-header, drop-in module. All symbols are `static` so this
    can simply be #included directly into main.c with no extra
    source file and no changes needed to your build task.

    Public API (unchanged from the previous version, so no edits
    needed elsewhere in main.c if you already wired it in):

        InitSynthMusic();          // call after InitAudioDevice()
        CloseSynthMusic();         // call before CloseAudioDevice()
        SetSynthVolume(float vol); // optional, 0.0 - 1.0
*/

#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define LOFI_SAMPLE_RATE     44100
#define LOFI_BPM             70
/* 8th notes at 70 BPM, 44100 Hz -> exact integer: 18900 */
#define LOFI_SAMPLES_PER_STEP ((LOFI_SAMPLE_RATE * 60) / LOFI_BPM / 2)
#define LOFI_STEPS_PER_BAR   8
#define LOFI_NUM_CHORDS      8
#define LOFI_PORTA_RATE      0.00006f   /* pad/bass glide speed between chords */
#define LOFI_LFO_FREQ        0.15f      /* slow tape-wobble rate (Hz) */
#define LOFI_LFO_DEPTH       0.0025f    /* wobble pitch depth */
#define LOFI_DELAY_SECONDS   0.42f
#define LOFI_DELAY_SAMPLES   ((int)(LOFI_SAMPLE_RATE * LOFI_DELAY_SECONDS))
#define LOFI_DELAY_FEEDBACK  0.32f
#define LOFI_DELAY_WET       0.32f
#define LOFI_LPF_ALPHA       0.32f      /* one-pole low-pass warmth */

typedef struct {
    float tones[4]; /* [root, 3rd, 5th, 7th] */
} LofiChord;

/* Fmaj7 - Am7 - Dm7 - G7 - Cmaj7 - Em7 - Fmaj7 - G7  (warm ii-V feeling, resolves back to Fmaj7 on loop) */
static LofiChord lofiChords[LOFI_NUM_CHORDS] = {
    { { 174.61f, 220.00f, 261.63f, 329.63f } }, /* Fmaj7 */
    { { 110.00f, 130.81f, 164.81f, 196.00f } }, /* Am7   */
    { { 146.83f, 174.61f, 220.00f, 261.63f } }, /* Dm7   */
    { {  98.00f, 123.47f, 146.83f, 174.61f } }, /* G7    */
    { { 130.81f, 164.81f, 196.00f, 246.94f } }, /* Cmaj7 */
    { { 164.81f, 196.00f, 246.94f, 293.66f } }, /* Em7   */
    { { 174.61f, 220.00f, 261.63f, 329.63f } }, /* Fmaj7 */
    { {  98.00f, 123.47f, 146.83f, 174.61f } }, /* G7    */
};

typedef struct {
    float phase;
    float freqCurrent;
    float freqTarget;
} SlideOsc;

static AudioStream synthStream;
static bool synthReady = false;

static SlideOsc lofiBass = { 0 };
static SlideOsc lofiPad[3] = { { 0 }, { 0 }, { 0 } };

static float lofiArpPhase = 0.0f;
static float lofiArpFreq  = 220.0f;
static float lofiArpEnv   = 0.0f;

static float lofiLfoPhase  = 0.0f;
static float lofiLpfState  = 0.0f;
static float lofiPopEnv    = 0.0f;

static float lofiDelayBuffer[LOFI_DELAY_SAMPLES] = { 0 };
static int   lofiDelayIndex = 0;

static void SynthAudioCallback(void *buffer, unsigned int frames) {
    short *out = (short *)buffer;
    static unsigned long long globalSample = 0;

    for (unsigned int i = 0; i < frames; i++) {

        unsigned long long step = globalSample / LOFI_SAMPLES_PER_STEP;
        int stepInBar = (int)(step % LOFI_STEPS_PER_BAR);
        int chordIdx  = (int)((step / LOFI_STEPS_PER_BAR) % LOFI_NUM_CHORDS);

        if (globalSample % LOFI_SAMPLES_PER_STEP == 0) {
            LofiChord c = lofiChords[chordIdx];

            if (stepInBar == 0) {
                lofiBass.freqTarget   = c.tones[0];
                lofiPad[0].freqTarget = c.tones[1];
                lofiPad[1].freqTarget = c.tones[2];
                lofiPad[2].freqTarget = c.tones[3];
            }

            /* gentle repeating arp through the chord tones, softly plucked */
            lofiArpFreq  = c.tones[stepInBar % 4];
            lofiArpPhase = 0.0f;
            lofiArpEnv   = 0.22f;
        }

        float wobble = 1.0f + LOFI_LFO_DEPTH * sinf(2.0f * PI * lofiLfoPhase);
        lofiLfoPhase += LOFI_LFO_FREQ / (float)LOFI_SAMPLE_RATE;
        if (lofiLfoPhase >= 1.0f) lofiLfoPhase -= 1.0f;

        float sample = 0.0f;

        /* soft sine sub-bass, glides smoothly between chord roots */
        lofiBass.freqCurrent += (lofiBass.freqTarget - lofiBass.freqCurrent) * LOFI_PORTA_RATE;
        lofiBass.phase += (lofiBass.freqCurrent * wobble) / (float)LOFI_SAMPLE_RATE;
        if (lofiBass.phase >= 1.0f) lofiBass.phase -= 1.0f;
        sample += sinf(2.0f * PI * lofiBass.phase) * 0.30f;

        /* warm sliding pad, 3 sine voices */
        float padSum = 0.0f;
        for (int p = 0; p < 3; p++) {
            lofiPad[p].freqCurrent += (lofiPad[p].freqTarget - lofiPad[p].freqCurrent) * LOFI_PORTA_RATE;
            lofiPad[p].phase += (lofiPad[p].freqCurrent * wobble) / (float)LOFI_SAMPLE_RATE;
            if (lofiPad[p].phase >= 1.0f) lofiPad[p].phase -= 1.0f;
            padSum += sinf(2.0f * PI * lofiPad[p].phase);
        }
        sample += (padSum / 3.0f) * 0.30f;

        /* soft triangle arp, long-ish decay, fed into a delay for space */
        float arpDry = 0.0f;
        if (lofiArpEnv > 0.001f) {
            lofiArpPhase += (lofiArpFreq * wobble) / (float)LOFI_SAMPLE_RATE;
            if (lofiArpPhase >= 1.0f) lofiArpPhase -= 1.0f;
            float tri = 4.0f * fabsf(lofiArpPhase - 0.5f) - 1.0f;
            arpDry = tri * lofiArpEnv * 0.20f;
            sample += arpDry;
            lofiArpEnv *= 0.9995f;
        }

        /* single-tap feedback delay on the arp for a spacious, dreamy echo */
        float delayed = lofiDelayBuffer[lofiDelayIndex];
        lofiDelayBuffer[lofiDelayIndex] = arpDry + delayed * LOFI_DELAY_FEEDBACK;
        sample += delayed * LOFI_DELAY_WET;
        lofiDelayIndex = (lofiDelayIndex + 1) % LOFI_DELAY_SAMPLES;

        /* faint vinyl hiss + the occasional soft crackle, for warmth not distraction */
        sample += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * 0.010f;
        if ((rand() % 200000) == 0) lofiPopEnv = 0.25f;
        if (lofiPopEnv > 0.0005f) {
            sample += (((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f) * lofiPopEnv;
            lofiPopEnv *= 0.65f;
        }

        /* one-pole low-pass: softens harsh synth edges into a mellow lofi tone */
        lofiLpfState += (sample - lofiLpfState) * LOFI_LPF_ALPHA;
        float finalSample = lofiLpfState;

        if (finalSample > 1.0f) finalSample = 1.0f;
        if (finalSample < -1.0f) finalSample = -1.0f;

        out[i] = (short)(finalSample * 24000.0f);

        globalSample++;
    }
}

static void InitSynthMusic(void) {
    synthStream = LoadAudioStream(LOFI_SAMPLE_RATE, 16, 1);
    SetAudioStreamCallback(synthStream, SynthAudioCallback);
    PlayAudioStream(synthStream);
    synthReady = true;
}

static void CloseSynthMusic(void) {
    if (synthReady) {
        UnloadAudioStream(synthStream);
        synthReady = false;
    }
}

static void SetSynthVolume(float volume) {
    if (synthReady) SetAudioStreamVolume(synthStream, volume);
}

#endif /* SYNTHMUSIC_H */