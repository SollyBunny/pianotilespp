#include <mutex>

#include <unordered_set>

struct tsf;

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
	static constexpr size_t SAMPLE_RATE = 22050;
	static constexpr size_t CHANNELS = 2;
	static constexpr size_t SAMPLE_SIZE = CHANNELS * (SAMPLE_TYPE == SampleType::SHORT ? sizeof(short) : sizeof(float));
	~MidiPlayer();
	[[nodiscard]] tsf* loadFont(const char* filename);
	void unloadFont(tsf*& font);
	void update(void *uploadBuffer, size_t samples);
	bool play(tsf* font, int note, float vel = 1.0f);
};
