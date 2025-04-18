#include "midiplayer.hpp"

#include "../external/tsf/tsf.h"

MidiPlayer::~MidiPlayer() {
	for (tsf* font : fonts)
		tsf_close(font);
	fonts.clear();
}

tsf* MidiPlayer::loadFont(const char* filename) {
	tsf* font = tsf_load_filename(filename);
	if (!font)
		return nullptr;
	tsf_set_output(font, CHANNELS == 1 ? TSF_MONO : TSF_STEREO_INTERLEAVED, SAMPLE_RATE, 0.0f);
	fonts.insert(font);
	return font;
}

void MidiPlayer::unloadFont(tsf*& font)
{
	if(fonts.erase(font) <= 0)
		return;
	tsf_close(font);
	font = nullptr;
}

void MidiPlayer::update(void *uploadBuffer, size_t samples) {
	std::scoped_lock lock(tsfMutex);
	bool first = true;
	for (tsf* font : fonts) {
		if (SAMPLE_TYPE == SampleType::SHORT)
			tsf_render_short(font, (short*)uploadBuffer, samples, first ? 0 : 1);
		else
			tsf_render_float(font, (float*)uploadBuffer, samples, first ? 0 : 1);
		first = false;
	}
}

bool MidiPlayer::play(tsf* font, int note, float vel) {
	if (!font || fonts.find(font) == fonts.end())
		return false;
	std::scoped_lock lock(tsfMutex);
	tsf_channel_midi_control(font, 0, 64, 127);
	return tsf_channel_note_on(font, 0, note, vel) > 0;
}
