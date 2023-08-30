#pragma once

class CSound : public CObject
{
public:
	CSound();
	~CSound();

	void playSound(ESound EnumSound);
	void initialize(void);

	std::vector <Mix_Music*> m_Sounds {ESound::SOUND_SIZE + 1, nullptr};

};