#include "../external/tsf/tsf.h"

#include <cstdio>
#include <mutex>

#include <unordered_set>

class MidiPlayer {
private:
	std::mutex tsfMutex;
	std::unordered_set<tsf*> fonts;

public:
	enum class SampleType {
		SHORT,
		FLOAT,
	};
	static constexpr SampleType SAMPLE_TYPE = SampleType::SHORT;
	static constexpr size_t SAMPLE_RATE = 44100;
	static constexpr size_t CHANNELS = 2;
	static constexpr size_t SAMPLE_SIZE = CHANNELS * (SAMPLE_TYPE == SampleType::SHORT ? sizeof(short) : sizeof(float));
	~MidiPlayer() {
		for (tsf* font : fonts)
			tsf_close(font);
		fonts.clear();
	}
	[[nodiscard]] tsf* loadFont(const char* filename) {
		tsf* font = tsf_load_filename(filename);
		if (!font) {
			std::perror("Failed to open sound font");
			return nullptr;
		}
		tsf_set_output(font, CHANNELS == 1 ? TSF_MONO : TSF_STEREO_INTERLEAVED, SAMPLE_RATE, 0.0f);
		fonts.insert(font);
		return font;
	}
	void unloadFont(tsf*& font)
	{
		if(fonts.erase(font) <= 0)
			return;
		tsf_close(font);
		font = nullptr;
	}
	void update(void *uploadBuffer, size_t samples) {
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
	bool play(tsf* font, int note, float vel = 1.0f, float time = -1.0f) {
		// if (fonts.find(font) == fonts.end())
		// 	return;
		// printf
		std::scoped_lock lock(tsfMutex);
		return tsf_note_on(font, 0, note, vel) > 0;
	}
};
